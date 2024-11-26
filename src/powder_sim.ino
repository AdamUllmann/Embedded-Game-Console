#include "powder_sim.h"

void updatePowder() {
  static int prevCursorX = cursorX;
  static int prevCursorY = cursorY;
  int xValue = analogRead(joystickXPin);
  int yValue = analogRead(joystickYPin);
  if (xValue < 200 && cursorX > 0) cursorX--;
  else if (xValue > 600 && cursorX < WIDTH - 1) cursorX++;
  if (yValue < 200 && cursorY > 0) cursorY--;
  else if (yValue > 600 && cursorY < HEIGHT - 1) cursorY++;
 
  if (gameGrid[prevCursorX][prevCursorY] == 3) {
      gameGrid[prevCursorX][prevCursorY] = 0;
  }
  if (!(analogRead(joystickSWPin) > 0)) {
      if (gameGrid[cursorX][cursorY] == 0 || gameGrid[cursorX][cursorY] == 3) {
          gameGrid[cursorX][cursorY] = 1;
      }
  }
 
  for (int x = 0; x < WIDTH; x++) {
      for (int y = HEIGHT - 2; y >= 0; y--) {
          if (gameGrid[x][y] == 1) {
              if (gameGrid[x][y + 1] == 0) {  // fall down if possible
                  gameGrid[x][y + 1] = 1;
                  gameGrid[x][y] = 0;
              } else if (x > 0 && gameGrid[x - 1][y + 1] == 0) {  // fall left if possible
                  gameGrid[x - 1][y + 1] = 1;
                  gameGrid[x][y] = 0;
              } else if (x < WIDTH - 1 && gameGrid[x + 1][y + 1] == 0) {  // fall right if possible
                  gameGrid[x + 1][y + 1] = 1;
                  gameGrid[x][y] = 0;
              }
          }
      }
  }
  // blinking cursor logic
  unsigned long currentTime = millis();
  if (currentTime - lastBlinkTime > blinkInterval) {
      cursorVisible = !cursorVisible;
      lastBlinkTime = currentTime;
  }
  if (cursorVisible) {
      gameGrid[cursorX][cursorY] = 3;
  }
  prevCursorX = cursorX;
  prevCursorY = cursorY;
}