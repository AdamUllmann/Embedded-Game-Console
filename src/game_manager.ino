#include "game_manager.h"

void updateDisplay() {
  lcd.clear();
  if (displayMode == GAME_OF_LIFE) {
    printText("Conway's", 0);
    printText("Game of Life", 1);
  }
  else if (displayMode == FLAPPY_BIRD) {
    initializeFlappyBird();
    printText("Flappy", 0);
    printText("Square", 1);
  }
  else if (displayMode == WOLFENSTEIN) {
    printText("Wolfenstein", 0);
    printText("Raycasting", 1);
  }
  else if (displayMode == RAIN) {
    initializeRain();
    printText("Rain", 0);
    printText("", 1);
  }
  else if (displayMode == POWDER) {
    printText("Powder", 0);
    printText("", 1);
  }
  else {
    printText("3D", 0);
    printText("Wireframe", 1);
  }
}

void runGame(DisplayMode displayMode) {
  if (displayMode == GAME_OF_LIFE) runGameOfLife();
  else if (displayMode == FLAPPY_BIRD) runFlappyBird();
  else if (displayMode == WOLFENSTEIN) runWolfenstein();
  else if (displayMode == RAIN) runRain();
  else if (displayMode == POWDER) runPowder();
  else runWireframe();
}