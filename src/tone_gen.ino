#include "tone_gen.h"

void initializeToneGen() {
  pinMode(buzzerPin, OUTPUT);
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void updateToneGen() {
  int pitchMod = analogRead(joystickXPin);
  float pitchFactor = map(pitchMod, 0, 1023, 50, 200) / 100.0;
 
  bool buttonPressed = false;
 
  for (int i = 0; i < numButtons; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      buttonPressed = true;
      int modulatedTone = baseTones[i] * pitchFactor;

      if (currentTone != modulatedTone) {
        tone(buzzerPin, modulatedTone);
        currentTone = modulatedTone;
      }
      break;
    }
  }
 
  if (!buttonPressed && currentTone != -1) {
    noTone(buzzerPin);
    currentTone = -1;
  }
}