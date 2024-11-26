#ifndef FLAPPY_BIRD_H
#define FLAPPY_BIRD_H

// FOR FLAPPY BIRD
int birdX = 2;
int birdY = HEIGHT / 2;
int pipeX = WIDTH - 1;
int pipeGapY = HEIGHT / 2;
const int highScoreAddressFlappyBird = 0; // EEPROM address to store the high score
int highScoreFlappyBird = 0;

// FLAPPY BIRD FUNCTIONS
void initializeFlappyBird();
void updateFlappyBird();
void runFlappyBird();

#endif