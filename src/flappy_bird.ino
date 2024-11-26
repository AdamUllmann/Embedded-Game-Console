#include "flappy_bird.h"

// FLAPPY BIRD FUNCTIONS
void initializeFlappyBird() {
  birdX = 2;
  birdY = HEIGHT / 2;
  pipeX = WIDTH - 1;
  pipeGapY = HEIGHT / 2;
  score = 0;
  gameOver = false;
  lcd.clear();
}
 
void updateFlappyBird() {
  int joystickY = analogRead(joystickYPin);
  int threshold = 512;
 
  if (joystickY < threshold) {
    birdY--;
  } else if (joystickY > threshold) {
    birdY++;
  }
 
  if (birdY < 0) {
    birdY = 0;
  } else if (birdY >= HEIGHT) {
    birdY = HEIGHT - 1;
  }
 
  pipeX--;
 
  if (pipeX < 0) {
    pipeX = WIDTH - 1;
    pipeGapY = random(1, HEIGHT - 3);
    if (!(birdX == pipeX && (birdY < pipeGapY || birdY > pipeGapY + 2))) {
      score++;
    }
  }
 
  memset(gameGrid, 0, sizeof(gameGrid));
 
  if (birdY >= 0 && birdY < HEIGHT) {
    gameGrid[birdX][birdY] = 1;
  }
 
  for (int y = 0; y < HEIGHT; y++) {
    if (y < pipeGapY || y > pipeGapY + 2) {
      if (pipeX >= 0 && pipeX < WIDTH) {
        gameGrid[pipeX][y] = 1;
      }
    }
  }
 
  if (birdX == pipeX && (birdY < pipeGapY || birdY > pipeGapY + 2)) {
    gameOver = true;
    pipeX = WIDTH - 1;
  }
}