// LED controller for the light in a bookcase.
// Herman Kopinga herman@kopinga.nl for 212 Fahrenheit 212f.nl

#include <TimerOne.h>
#include <Bounce.h>

// Pin definitions.
#define LEDOUT 4
#define STATUSLED 11
#define LIGHTSENSOR A1
#define MULTIPLIER A2
#define SENSITIVITY A3
#define SOUND A0
#define SOUNDLED 17
#define MANUALMODE 0
#define OVERRIDE 16

// Globals
unsigned long nextBlink;
bool manualMode = 0;
bool powerGood = 0;
bool powerOverride = 0;
Bounce overrideButton = Bounce(OVERRIDE, 20);   

void setup() {
  // Speed is ignored by Teensy.
  Serial.begin(115200);

  pinMode(SOUND, INPUT);
  pinMode(LIGHTSENSOR, INPUT);
  pinMode(MULTIPLIER, INPUT);
  pinMode(A3, INPUT);
  pinMode(A8, INPUT);
  pinMode(STATUSLED, OUTPUT);
  pinMode(LEDOUT, OUTPUT);
  pinMode(SOUNDLED, OUTPUT);
  pinMode(MANUALMODE, INPUT_PULLUP);
  pinMode(OVERRIDE, INPUT_PULLUP);

  // Timer1 library, 10 BIT PWM! (http://www.pjrc.com/teensy/td_libs_TimerOne.html)
  Timer1.initialize(512);
  Timer1.pwm(LEDOUT, 0);
}

void loop() {
  // Blink internal LED: working.
  // Also: do power check.
  if (millis() > nextBlink) {
    digitalWrite(STATUSLED, !digitalRead(STATUSLED));
    nextBlink = millis() + 1000;

    // Auto shut off if supply voltage is below ~11.8v.
    // Measured using 4.7k/10k voltage divider on A8.
    if (analogRead(A8) > 770 || powerOverride) {
      powerGood = 1;
    } 
    else {
      nextBlink = millis() + 50;
      powerGood = 0;
    }

    // Handle the two buttons:
    manualMode = !digitalRead(MANUALMODE);
    overrideButton.update();
    if (overrideButton.fallingEdge()) {
      powerOverride = !powerOverride;
    }
  }

  if (powerGood) {
    if (manualMode) {
      Timer1.setPwmDuty(LEDOUT, analogRead(MULTIPLIER));
    }
    else {    
      // If sound detected, restart countdown and fade in.
      int soundLevel = analogRead(SOUND);
      int sensitivityLevel = analogRead(SENSITIVITY);
      //    Serial.print(sensitivityLevel);
      //    Serial.print(" ");
      //    Serial.println(soundLevel);
      if (soundLevel > sensitivityLevel) {
        digitalWrite(SOUNDLED, 1);
        delay(1);
        digitalWrite(SOUNDLED, 0);
      }

      // Measure light intensity, map to output power (through adjustment pot).
      int lightLevel = analogRead(LIGHTSENSOR);
      Timer1.setPwmDuty(LEDOUT, map(lightLevel, 0, 1023, 1023, 1));
    }
  }  
  else {
    digitalWrite(LEDOUT, 0);
  }
}



