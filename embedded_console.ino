#include <LiquidCrystal.h>
#include <EEPROM.h>
 
// =================== GLOBAL VARIABLES ===================
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int score = 0;
bool gameOver = false;
// BASIC 4X2 CHARACTER SCREEN STUFF
const int CHAR_WIDTH = 5;   // number of x pixels in a character
const int CHAR_HEIGHT = 8;  // number of y pixels in a character
const int LCD_WIDTH = 4;    // number of characters wide
const int LCD_HEIGHT = 2;   // number of characters tall
const int modeButtonPin = 13;
int displayMode = 0;       // 3D Cube
const int WIDTH = CHAR_WIDTH * LCD_WIDTH;       // number of total x pixels in 4x2 character screen
const int HEIGHT = CHAR_HEIGHT * LCD_HEIGHT;      // number of total y pixels in 4x2 character screen
 
// FOR CONWAY'S GAME OF LIFE
byte gameGrid[WIDTH][HEIGHT];
byte nextGrid[WIDTH][HEIGHT];
 
// FOR TONE GEN
const int buttonPins[] = {6, 7, 8, 9};
const int numButtons = sizeof(buttonPins) / sizeof(buttonPins[0]);
const int buzzerPin = 10;
const int joystickXPin = A5;
 
const int baseTones[] = {262, 294, 330, 349};
int currentTone = -1;
 
// FOR CUBE
const int joystickYPin = A4;
const int numVertices = 8;
float vertices[numVertices][3] = {                      // the cube
  {-2, -2, -2}, {2, -2, -2}, {2, 2, -2}, {-2, 2, -2},
  {-2, -2, 2}, {2, -2, 2}, {2, 2, 2}, {-2, 2, 2}
};
 
const int numEdges = 12;
int edges[numEdges][2] = {
  {0, 1}, {1, 2}, {2, 3}, {3, 0},
  {4, 5}, {5, 6}, {6, 7}, {7, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}
};
 
// controls
float angleX = 0;                           // rotation positions
float angleY = 0;
float angleZ = 0;
 
const float deadZone = 50;
const float maxRotationSpeed = 0.0002;
 
// FOR FLAPPY BIRD
int birdX = 2;
int birdY = HEIGHT / 2;
int pipeX = WIDTH - 1;
int pipeGapY = HEIGHT / 2;
const int highScoreAddressFlappyBird = 0; // EEPROM address to store the high score
int highScoreFlappyBird = 0;
 
 
// Wolfenstein Minigame Variables
const int mapWidth = 8;
const int mapHeight = 15;
const char map1[mapWidth * mapHeight] = {
  '1', '1', '1', '1', '1', '1', '1', '1',
  '1', '0', '0', '0', '0', '0', '0', '1',
  '1', '0', '0', '0', '0', '0', '0', '1',
  '1', '0', '0', '1', '1', '0', '0', '1',
  '1', '0', '0', '1', '0', '0', '0', '1',
  '1', '0', '0', '1', '0', '0', '0', '1',
  '1', '0', '0', '0', '0', '0', '0', '1',
  '1', '0', '0', '0', '0', '0', '0', '1',
  '1', '0', '0', '0', '0', '0', '0', '1',
  '1', '0', '0', '0', '0', '0', '0', '1',
  '1', '0', '0', '0', '0', '0', '0', '1',
  '1', '0', '0', '0', '0', '0', '0', '1',
  '1', '0', '0', '0', '0', '0', '0', '1',
  '1', '0', '0', '0', '0', '0', '0', '1',
  '1', '1', '1', '1', '1', '1', '1', '1'
};
 
float playerX = 2;
float playerY = 2;
float playerAngle = 0.0;
 
// FOR RAIN
int shipX = WIDTH / 2;
int shipY = HEIGHT - 1;
int bulletX = -1;
int bulletY = -1;
const int maxRains = 5;
int rainX[maxRains];
int rainY[maxRains];
bool rainActive[maxRains];
const int joystickSWPin = A3;
const int highScoreAddressRain = 4; // EEPROM address to store the high score
int highScoreRain = 0;
 
// FOR POWDER
int cursorX = WIDTH / 2;
int cursorY = HEIGHT / 2;
bool cursorVisible = true;
unsigned long lastBlinkTime = 0;
const int blinkInterval = 100;
bool dropping = 0;
 
 
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
 
void setup() {
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  pinMode(modeButtonPin, INPUT_PULLUP);
  initializeGrid();
  updateDisplay();
  initializeRain();
  highScoreFlappyBird = EEPROM.read(highScoreAddressFlappyBird);
  highScoreRain = EEPROM.read(highScoreAddressRain);
 
  // FOR TONE GEN
  pinMode(buzzerPin, OUTPUT);
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}
 
void printText(const char* text, int row) {
  lcd.setCursor(4, row);
  lcd.print(text);
}
 
void initializeGrid() {
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      gameGrid[x][y] = random(2);
    }
  }
}
 
// CONWAY'S GAME OF LIFE
void updateGrid() {
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      int aliveNeighbors = countAliveNeighbors(x, y);
      if (gameGrid[x][y] == 1) {
        nextGrid[x][y] = (aliveNeighbors == 2 || aliveNeighbors == 3) ? 1 : 0;  // survive
      } else {
        nextGrid[x][y] = (aliveNeighbors == 3) ? 1 : 0;                         // birth
      }
    }
  }
  memcpy(gameGrid, nextGrid, sizeof(gameGrid));
}
 
int countAliveNeighbors(int x, int y) {
  int count = 0;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (i == 0 && j == 0) {
        continue;
      }
      int neighbor_x = (x + i + WIDTH) % WIDTH;
      int neighbor_y = (y + j + HEIGHT) % HEIGHT;
      if (gameGrid[neighbor_x][neighbor_y] == 1) {
        count += 1;
      }
    }
  }
  return count;
}
 
void createCustomCharacter(byte index, byte data[]) {
  lcd.createChar(index, data);
}
 
void updateLCD() {    // draw the 4x2 character screen from pixel grid
  const int maxCustomChars = 8;
 
  for (int charY = 0; charY < LCD_HEIGHT; charY++) {
    for (int charX = 0; charX < LCD_WIDTH; charX++) {
      byte charData[8] = {0};
 
      for (int y = 0; y < CHAR_HEIGHT; y++) {
        for (int x = 0; x < CHAR_WIDTH; x++) {
          int pixelX = charX * CHAR_WIDTH + x;
          int pixelY = charY * CHAR_HEIGHT + y;
 
          if (pixelX < WIDTH && pixelY < HEIGHT) {
            if (gameGrid[pixelX][pixelY]) {         // directly draw from game grid to save memory.
                                                    // (switching display modes to game of life or other grid-dependant modes can create interesting results)
              charData[y] |= (1 << (4 - x));
            }
          }
        }
      }
 
      byte charIndex = (charY * LCD_WIDTH + charX) % maxCustomChars;
      createCustomCharacter(charIndex, charData);
      lcd.setCursor(charX, charY);
      lcd.write(charIndex);
    }
  }
}
 
void updateDisplay() {
  lcd.clear();
  if (displayMode == 1) {
    printText("Conway's", 0);
    printText("Game of Life", 1);
  }
  else if (displayMode == 2) {
    initializeFlappyBird();
    printText("Flappy", 0);
    printText("Square", 1);
  }
  else if (displayMode == 3) {
    printText("Wolfenstein", 0);
    printText("Raycasting", 1);
  }
  else if (displayMode == 4) {
    initializeRain();
    printText("Rain", 0);
    printText("", 1);
  }
  else if (displayMode == 5) {
    printText("Powder", 0);
    printText("", 1);
  }
  else {
    printText("3D", 0);
    printText("CUBE", 1);
    renderCube();
  }
}
 
void rotateCube() {
  int xValue = analogRead(joystickXPin);
  int yValue = analogRead(joystickYPin);
 
  int xMapped = map(xValue, 0, 1023, -512, 512);  // remap joystick values from 0-1023 to -512 to 512
  int yMapped = map(yValue, 0, 1023, -512, 512);  // this is so that our resting position is 0.
 
  angleY = (xMapped) * maxRotationSpeed;    // rotation
  angleX = (yMapped) * maxRotationSpeed;
 
  float cosX = cos(angleX);
  float sinX = sin(angleX);
  float cosY = cos(angleY);
  float sinY = sin(angleY);
  float cosZ = cos(angleZ);
  float sinZ = sin(angleZ);
 
  for (int i = 0; i < numVertices; i++) {   // transform the vertices
    float x = vertices[i][0];
    float y = vertices[i][1];
    float z = vertices[i][2];
 
    float tempX = x * cosY - z * sinY;
    float tempZ = x * sinY + z * cosY;
    x = tempX;
    z = tempZ;
 
    float tempY = y * cosX - z * sinX;
    z = y * sinX + z * cosX;
    y = tempY;
 
    tempX = x * cosZ - y * sinZ;
    tempY = x * sinZ + y * cosZ;
    x = tempX;
    y = tempY;
 
    vertices[i][0] = x;
    vertices[i][1] = y;
    vertices[i][2] = z;
  }
}
 
void renderCube() {
  memset(gameGrid, 0, sizeof(gameGrid));
 
  // Project vertices
  int projectedVertices[numVertices][2];
  for (int i = 0; i < numVertices; i++) {
    float x = vertices[i][0];
    float y = vertices[i][1];
    float z = vertices[i][2];
 
    int px = (x / (z + 5) * 10) + WIDTH / 2;
    int py = (y / (z + 5) * 10) + HEIGHT / 2;
 
    projectedVertices[i][0] = px;
    projectedVertices[i][1] = py;
  }
 
  // Draw edges
  for (int i = 0; i < numEdges; i++) {
    int x0 = projectedVertices[edges[i][0]][0];
    int y0 = projectedVertices[edges[i][0]][1];
    int x1 = projectedVertices[edges[i][1]][0];
    int y1 = projectedVertices[edges[i][1]][1];
 
    drawLine(x0, y0, x1, y1);
  }
 
  updateLCD();
}
 
void drawLine(int x0, int y0, int x1, int y1) {   // Bresenham's Line Algorithm
                                                  // modified from https://gist.github.com/bert/1085538
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  int sx = x0 < x1 ? 1 : -1;
  int sy = y0 < y1 ? 1 : -1;
  int err = dx - dy;
 
  while (true) {
    if (x0 >= 0 && x0 < WIDTH && y0 >= 0 && y0 < HEIGHT) {
      gameGrid[x0][y0] = 1;
    }
    if (x0 == x1 && y0 == y1) break;
    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }
}
 
bool cleared = 0;
 
void loop() {
  static bool lastModeButtonState = HIGH;
  bool modeButtonState = digitalRead(modeButtonPin);
 
  if (lastModeButtonState == HIGH && modeButtonState == LOW) {
    displayMode = (displayMode + 1) % 6;
    updateDisplay();
  }
  lastModeButtonState = modeButtonState;
 
  if (displayMode == 1) {
      updateGrid();
      updateLCD();
      delay(30);
    }
    else if (displayMode == 2) {
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
    else if (displayMode == 3) {
        renderWolfenstein();
        delay(20);
    }
    else if (displayMode == 4) {
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
    else if (displayMode == 5) {
        updatePowder();
        updateLCD();
    }
    else {
      rotateCube();
      renderCube();
      delay(20);
    }
 
  // FOR TONE GEN
  int pitchMod = analogRead(joystickXPin);
  float pitchFactor = map(pitchMod, 0, 1023, 50, 200) / 100.0;
 
  bool buttonPressed = false;
 
  for (int i = 0; i < numButtons; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      buttonPressed = true;
      int modulatedTone = baseTones[i] * pitchFactor;
 
      if (currentTone != modulatedTone) {
        tone(buzzerPin, modulatedTone);
        currentTone = modulatedTone;
      }
      break;
    }
  }
 
  if (!buttonPressed && currentTone != -1) {
    noTone(buzzerPin);
    currentTone = -1;
  }
}
 
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