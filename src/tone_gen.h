#ifndef TONE_GEN_H
#define TONE_GEN_H

// FOR TONE GEN
const int numButtons = sizeof(buttonPins) / sizeof(buttonPins[0]);
const int baseTones[] = {262, 294, 330, 349};
int currentTone = -1;

void initializeToneGen();
void updateToneGen();

#endif