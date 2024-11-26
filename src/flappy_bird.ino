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

void runFlappyBird() {
  if (gameOver == 0) {
      updateFlappyBird();
      delay(100);
      updateLCD();
      lcd.setCursor(11, 0);
      lcd.print("Score");
      lcd.setCursor(11, 1);
      lcd.print(score);
      if (cleared != 0) {
        cleared = 0;
      }
  }
  else {
    if (cleared != 1) {
      lcd.clear();
      cleared = 1;
    }
    lcd.setCursor(0, 0);
    lcd.print("Game Over");
    lcd.setCursor(0, 1);
    lcd.print("Score:");
    lcd.print(score);
    if (score > highScoreFlappyBird) {
      highScoreFlappyBird = score;
      EEPROM.update(highScoreAddressFlappyBird, highScoreFlappyBird);
    }
  
    lcd.setCursor(8, 1);
    lcd.print("High:");
    lcd.print(highScoreFlappyBird);
    if (!analogRead(joystickSWPin) > 0) {
      gameOver = 0;
      lcd.clear();
      cleared = 1;
      updateDisplay();
      score = 0;
    }
    delay(50);
  }
}