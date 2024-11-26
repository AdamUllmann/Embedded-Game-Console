#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

extern byte nextGrid[WIDTH][HEIGHT];

void initializeGrid();
int countAliveNeighbors(int x, int y);
void updateGrid();
void runGameOfLife();

#endif