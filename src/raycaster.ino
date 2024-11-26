#include "raycaster.h"

void renderWolfenstein() {
    int xValue = analogRead(joystickXPin);
    int yValue = analogRead(joystickYPin);
    float moveStep = 0.16;
    float rotStep = 0.08;
 
    if (xValue < 200) {
      playerAngle -= rotStep;
    } else if (xValue > 600) {
      playerAngle += rotStep;
    }
 
    float moveX = cos(playerAngle) * moveStep;
    float moveY = sin(playerAngle) * moveStep;
 
    if (yValue < 200) {
    int newX = (int)(playerX - moveX);
    int newY = (int)(playerY - moveY);
      playerX += moveX;
      playerY += moveY;
  } else if (yValue > 600) {
    int newX = (int)(playerX + moveX);
    int newY = (int)(playerY + moveY);
      playerX -= moveX;
      playerY -= moveY;
  }
 
  for (int x = 0; x < WIDTH; x++) {
    float rayAngle = (playerAngle - 0.4) + (x / (float)WIDTH) * 0.8;
    float distanceToWall = 3;
    bool hitWall = false;
 
    float eyeX = cos(rayAngle);
    float eyeY = sin(rayAngle);
 
    while (!hitWall && distanceToWall < 16.0) {
      distanceToWall += 0.1;
 
      int testX = (int)(playerX + eyeX * distanceToWall);
      int testY = (int)(playerY + eyeY * distanceToWall);
 
      if (testX < 0 || testX >= mapWidth || testY < 0 || testY >= mapHeight) {
        hitWall = true;
        distanceToWall = 16.0;
      } else {
        if (map1[testY * mapWidth + testX] == '1') {
          hitWall = true;
        }
      }
    }
 
    int ceiling = (HEIGHT / 2.0) - HEIGHT / distanceToWall;
    int floor = HEIGHT - ceiling;
 
    for (int y = 0; y < HEIGHT; y++) {
      if (y <= ceiling) {
        gameGrid[x][y] = 0;
      } else if (y > ceiling && y <= floor) {
        gameGrid[x][y] = 1; // walls
      } else {
        gameGrid[x][y] = 0;
      }
    }
  }
 
  updateLCD();
}