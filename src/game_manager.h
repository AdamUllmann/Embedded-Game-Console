#ifndef CORE_H
#define CORE_H

// =================== GLOBAL VARIABLES ===================
int score = 0;              // we have these here to save program storage space
bool gameOver = false;

enum DisplayMode {
  CUBE = 0,
  GAME_OF_LIFE = 1,
  FLAPPY_BIRD = 2,
  WOLFENSTEIN = 3,
  RAIN = 4,
  POWDER = 5
};

DisplayMode displayMode = 0;       // switches between gamemodes. 0 is the wireframe rasterizer

byte gameGrid[WIDTH][HEIGHT]; // The 2D pixel interface for writing to the LCD screen

void updateDisplay();

#endif