// LED controller for the light in a bookcase.
// Herman Kopinga herman@kopinga.nl for 212 Fahrenheit 212f.nl

#include <TimerOne.h>

// Pin definitions.
#define LEDOUT 14
#define STATUSLED 11
#define LIGHTSENSOR A1
#define MULTIPLIER A2

// Globals
unsigned long nextBlink;

void setup() {
  // Speed is ignored by Teensy.
  Serial.begin(115200);
  
  pinMode(A0, INPUT);
  pinMode(LIGHTSENSOR, INPUT);
  pinMode(MULTIPLIER, INPUT);
  pinMode(A3, INPUT);
  pinMode(STATUSLED, OUTPUT);
  pinMode(LEDOUT, OUTPUT);
  
  Timer1.initialize(128);
  Timer1.pwm(LEDOUT, 0);
}

void loop() {
  // Blink internal LED: working.
  if (millis() > nextBlink) {
    digitalWrite(STATUSLED, !digitalRead(STATUSLED));
    nextBlink = millis() + 1000;
  }
  // If sound detected, restart countdown and fade in.
  
  // Measure light intensity, map to output power (through adjustment pot).
  int light = analogRead(LIGHTSENSOR);
  Serial.println(light);
  Timer1.setPwmDuty(LEDOUT, map(light, 0, 1024, 255, 0)*analogRead(MULTIPLIER)/1024);
}

