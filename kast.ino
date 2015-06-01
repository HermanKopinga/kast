// LED controller for the light in a bookcase.
// Herman Kopinga herman@kopinga.nl for 212 Fahrenheit 212f.nl

#include <TimerOne.h>
#include <Bounce.h>
#include "LEDFader.h"

// Pin definitions.
#define LEDOUT 4       // White
#define STATUSLED 11   // Orange
#define LIGHTSENSOR A1
#define MULTIPLIER A2
#define SENSITIVITY A3
#define SOUND A0
#define SOUNDLED 15    // Yellow
#define MANUALMODE 0
#define OVERRIDE 16
#define POWERSENSE A8

// Globals
int blinkBreak = 1000;
int ledLevel = 0;
unsigned long previousBlink = 0;
unsigned long previousLevelCheck = 0;
unsigned long soundTime = 0;
//unsigned long silentTime = 900000; // 15 minutes
unsigned long silentTime = 24000; // 15 minutes
unsigned long fadeTime = 9000;
unsigned long levelCheckBreak = 12000;
LEDFader led = LEDFader(4);
bool manualMode = 0;
bool voltageGood = 0;
bool powerOverride = 0;
Bounce overrideButton = Bounce(OVERRIDE, 20);   
// For the averaging of the sound sensor
const int numReadings = 5;
int readings[numReadings];      // the readings from the analog input
int avgIndex = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

void setup() {
  // Speed is ignored by Teensy.
  Serial.begin(115200);

  pinMode(SOUND, INPUT);
  pinMode(LIGHTSENSOR, INPUT);
  pinMode(MULTIPLIER, INPUT);
  pinMode(SENSITIVITY, INPUT);
  pinMode(POWERSENSE, INPUT);
  pinMode(STATUSLED, OUTPUT);
  pinMode(LEDOUT, OUTPUT);
  pinMode(SOUNDLED, OUTPUT);
  pinMode(MANUALMODE, INPUT_PULLUP);
  pinMode(OVERRIDE, INPUT_PULLUP);

  // Timer1 library, 10 BIT PWM! (http://www.pjrc.com/teensy/td_libs_TimerOne.html)
  Timer1.initialize(512);
  Timer1.pwm(LEDOUT, ledLevel);
}

void loop() {
   unsigned long currentMillis = millis();
   
  // Blink internal LED: working.
  // Also: do power check.
  if ((unsigned long)(currentMillis - previousBlink) >= blinkBreak) {
    previousBlink = currentMillis;
    Serial.print(soundTime);
    Serial.print(" ");
    Serial.print(currentMillis);
    Serial.print(" ");
    Serial.println(ledLevel);
    digitalWrite(STATUSLED, !digitalRead(STATUSLED));

    // Auto shut off if supply voltage is below ~11.8v.
    // Measured using 4.7k/10k voltage divider on A8.
    if (analogRead(POWERSENSE) > 770 || powerOverride) {
      blinkBreak = 1000;
      voltageGood = 1;
    } 
    else {
      blinkBreak = 50;
      voltageGood = 0;
    }

    // Handle the two buttons:
    // Manual mode button, potmeter sets intensity level, no auto adjust.
    manualMode = !digitalRead(MANUALMODE);
    // Power override button, allows usage if power appears low.
    overrideButton.update();
    if (overrideButton.fallingEdge()) {
      powerOverride = !powerOverride;
    }
  }

  if (!voltageGood) {
    // Voltage is too low.
    ledLevel = 0;
    Timer1.setPwmDuty(LEDOUT, ledLevel);
  }
  else {
    // Voltage is good.
    if (manualMode) {
      ledLevel = analogRead(MULTIPLIER);
      Timer1.setPwmDuty(LEDOUT, ledLevel);
    }
    else {    
      // Detect sound.
      // If sound detected, restart countdown and fade in.
      total = total - readings[avgIndex];         
      // read from the sensor:  
      readings[avgIndex] = analogRead(SOUND); 
      // add the reading to the total:u
      total= total + readings[avgIndex];       
      // advance to the next position in the array:  
      avgIndex = avgIndex + 1;                    
      // if we're at the end of the array...
      if (avgIndex >= numReadings) {      
        // ...wrap around to the beginning: 
        avgIndex = 0;              
      }    
      // calculate the average:
      int soundLevel = total / numReadings;         
      
      int sensitivityLevel = analogRead(SENSITIVITY);
      if (soundLevel < sensitivityLevel) {
        digitalWrite(SOUNDLED, 1);
        soundTime = currentMillis;
        if (ledLevel == 0) {
          // This make the autofade fade in.
          ledLevel = 1;
          Serial.print("Sound: ");
          Serial.println(soundLevel);
        }
      }
      else {
        digitalWrite(SOUNDLED, 0);
      }
      
      if (led.is_fading()) {
        led.update();        
      } 
      else if (ledLevel > 0) {
        if ((unsigned long)(currentMillis - soundTime) >= silentTime) {
          // Todo: fadeout 15 sec
          ledLevel = 0;
          led.fade(ledLevel, fadeTime);
          Serial.println("Timeout");
        }
        else if ((unsigned long)(currentMillis - previousLevelCheck) >= levelCheckBreak) {
          // Automatic brightness correction every minute.
          previousLevelCheck = currentMillis;
          // Measure light intensity, map to output power (through adjustment pot).
          int lightLevel = analogRead(LIGHTSENSOR);
          // Todo: fadein 15 sec
          ledLevel = map(lightLevel, 0, 1023, 1023, 0);
          Serial.print("LL: ");
          Serial.println(lightLevel);
          led.fade(ledLevel, fadeTime);         
          led.update();                  
        }    
      }
    }
  }  
}

