#ifndef RAIN_GAME_H
#define RAIN_GAME_H

// FOR RAIN
int shipX = WIDTH / 2;
int shipY = HEIGHT - 1;
int bulletX = -1;
int bulletY = -1;
const int maxRains = 5;
int rainX[maxRains];
int rainY[maxRains];
bool rainActive[maxRains];
const int highScoreAddressRain = 4; // EEPROM address to store the high score
int highScoreRain = 0;

void initializeRain();
void updateRain();
void runRain();

#endif