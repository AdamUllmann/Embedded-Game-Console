#include "wireframe_renderer.h"

void initializeWireframe() {
  vertices = cubeVertices;
  numVertices = numCubeVertices;
  edges = cubeEdges;
  numEdges = numCubeEdges;
}

void rotateWireframe() {
  int xValue = analogRead(joystickXPin);
  int yValue = analogRead(joystickYPin);
 
  int xMapped = map(xValue, 0, 1023, -512, 512);
  int yMapped = map(yValue, 0, 1023, -512, 512);
 
  angleY = (xMapped) * maxRotationSpeed;
  angleX = (yMapped) * maxRotationSpeed;
 
  float cosX = cos(angleX);
  float sinX = sin(angleX);
  float cosY = cos(angleY);
  float sinY = sin(angleY);
  float cosZ = cos(angleZ);
  float sinZ = sin(angleZ);
 
  for (int i = 0; i < numVertices; i++) {
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
 
void renderWireframe() {
  memset(gameGrid, 0, sizeof(gameGrid));
 
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
 
  for (int i = 0; i < numEdges; i++) {
    int x0 = projectedVertices[edges[i][0]][0];
    int y0 = projectedVertices[edges[i][0]][1];
    int x1 = projectedVertices[edges[i][1]][0];
    int y1 = projectedVertices[edges[i][1]][1];
 
    drawLine(x0, y0, x1, y1);
  }
 
  updateLCD();
}
 
void drawLine(int x0, int y0, int x1, int y1) {
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

void runWireframe() {
  if (!analogRead(joystickSWPin) > 0) {
    switchShape();
    delay(200); // debounce
  }
  rotateWireframe();
  renderWireframe();
  delay(16);
}

void switchShape() {
  static bool isCube = true;
  if (isCube) {
    vertices = pyramidVertices;
    numVertices = numPyramidVertices;
    edges = pyramidEdges;
    numEdges = numPyramidEdges;
  } else {
    vertices = cubeVertices;
    numVertices = numCubeVertices;
    edges = cubeEdges;
    numEdges = numCubeEdges;
  }
  isCube = !isCube;
}