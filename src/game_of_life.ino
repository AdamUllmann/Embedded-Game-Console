byte nextGrid[WIDTH][HEIGHT];

void initializeGrid() {
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      gameGrid[x][y] = random(2);
    }
  }
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