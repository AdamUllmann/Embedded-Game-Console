#include "rain_game.h"

void initializeRain() {
  shipX = WIDTH / 2;
  shipY = HEIGHT - 1;
  bulletX = -1;
  bulletY = -1;
  gameOver = false;
  score = 0;
 
  for (int i = 0; i < maxRains; i++) {
    rainX[i] = random(WIDTH);
    rainY[i] = random(HEIGHT / 2);
    rainActive[i] = true;
  }
}
 
void updateRain() {
  int xValue = analogRead(joystickXPin);
 
  if (xValue < 200 && shipX > 0) shipX--;
  else if (xValue > 600 && shipX < WIDTH - 1) shipX++;
 
  if (bulletY >= 0) {
    bulletY--;
  }
 
  if (bulletY < 0) {
    bulletX = -1;
  }
 
  if (!(analogRead(joystickSWPin) > 0) && bulletY < 0) {
    bulletX = shipX;
    bulletY = shipY - 1;
  }
 
  for (int i = 0; i < maxRains; i++) {
    if (rainActive[i]) {
      rainY[i]++;
      if (rainY[i] >= HEIGHT) {
        rainX[i] = random(WIDTH);
        rainY[i] = 0;
        score++;
      }
 
      if (rainX[i] == shipX && rainY[i] == shipY) {
        gameOver = true;
      }
 
      if (bulletX == rainX[i] && bulletY == rainY[i]) {
        rainActive[i] = false;
        bulletY = -1;
      }
    } else {
      rainX[i] = random(WIDTH);
      rainY[i] = 0;
      rainActive[i] = true;
    }
  }
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      gameGrid[x][y] = 0;
    }
  }
  gameGrid[shipX][shipY] = 1;
 
  if (bulletY >= 0) {
    gameGrid[bulletX][bulletY] = 1;
  }
 
  for (int i = 0; i < maxRains; i++) {
    if (rainActive[i]) {
      gameGrid[rainX[i]][rainY[i]] = 1;
    }
  }
}

void runRain() {
  if (gameOver == 0) {
    updateRain();
    updateLCD();
    delay(20);
    lcd.setCursor(9, 0);
    lcd.print("Hi:");
    lcd.print(highScoreRain);
    lcd.setCursor(6, 1);
    lcd.print("Score:");
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
    lcd.print("Score");
    lcd.print(score);
    if (score > highScoreRain) {
      highScoreRain = score;
      EEPROM.update(highScoreAddressRain, highScoreRain);
    }
  
    lcd.setCursor(9, 1);
    lcd.print("Hi");
    lcd.print(highScoreRain);
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