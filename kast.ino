// LED controller for the light in a bookcase.
// Herman Kopinga herman@kopinga.nl for 212 Fahrenheit 212f.nl

// Pin definitions.
#define LEDOUT 12
#define STATUSLED 11

// Globals
unsigned long nextBlink;

void setup() {
  // Speed is ignored by Teensy.
  Serial.begin(115200);
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(STATUSLED, OUTPUT);
  pinMode(LEDOUT, OUTPUT);
}

void loop() {
  // Blink internal LED: working.
  if (millis() > nextBlink) {
    digitalWrite(STATUSLED, !digitalRead(STATUSLED));
    nextBlink = millis() + 1000;
  }
  // If sound detected, restart countdown and fade in.
  
  // Measure light intensity, map to output power (through adjustment pot).
  analogWrite(LEDOUT, 15);
}

