#ifndef POWDER_SIM_H
#define POWDER_SIM_H

// FOR POWDER
int cursorX = WIDTH / 2;
int cursorY = HEIGHT / 2;
bool cursorVisible = true;
unsigned long lastBlinkTime = 0;
const int blinkInterval = 100;
bool dropping = 0;

void updatePowder();

#endif