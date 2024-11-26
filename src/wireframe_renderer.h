#ifndef WIREFRAME_RENDERER_H
#define WIREFRAME_RENDERER_H

// FOR CUBE
const int numCubeVertices = 8;
float cubeVertices[numCubeVertices][3] = {
  {-2, -2, -2}, {2, -2, -2}, {2, 2, -2}, {-2, 2, -2},
  {-2, -2, 2}, {2, -2, 2}, {2, 2, 2}, {-2, 2, 2}
};
 
const int numCubeEdges = 12;
int cubeEdges[numCubeEdges][2] = {
  {0, 1}, {1, 2}, {2, 3}, {3, 0},
  {4, 5}, {5, 6}, {6, 7}, {7, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

// FOR PYRAMID
const int numPyramidVertices = 5;
float pyramidVertices[numPyramidVertices][3] = {
  {0, 0, 2}, {-2, -2, -2}, {2, -2, -2}, {2, 2, -2}, {-2, 2, -2}
};

const int numPyramidEdges = 8;
int pyramidEdges[numPyramidEdges][2] = {
  {0, 1}, {0, 2}, {0, 3}, {0, 4},
  {1, 2}, {2, 3}, {3, 4}, {4, 1}
};

float (*vertices)[3];
int numVertices;
int (*edges)[2];
int numEdges;

// Controls
float angleX = 0;
float angleY = 0;
float angleZ = 0;

const float deadZone = 50;
const float maxRotationSpeed = 0.0002;

void initializeWireframe();
void rotateWireframe();
void renderWireframe();
void drawLine(int x0, int y0, int x1, int y1);
void runWireframe();
void switchShape();

#endif