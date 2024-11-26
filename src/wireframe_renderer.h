#ifndef WIREFRAME_RENDERER_H
#define WIREFRAME_RENDERER_H

// FOR CUBE
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

void rotateCube();
void renderCube();
void drawLine(int x0, int y0, int x1, int y1);

#endif