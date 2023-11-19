/**
 * Integrantes:
 *
 * Eduardo de Oliveira Motta
 * Gabriel Henrique Silva
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define ITERNUM 50
#define N 2048
#define USEC 1000000
#define SLEEP_TIME 0.001

float getCell(float **grid, int i, int j) {
  return grid[(i + N) % N][(j + N) % N];
}

int getNeighbors(float **grid, int i, int j) {
  int neighbors = 0;
  if (getCell(grid, i - 1, j - 1) > 0.0f) {
    neighbors++;
  }
  if (getCell(grid, i - 1, j) > 0.0f) {
    neighbors++;
  }
  if (getCell(grid, i - 1, j + 1) > 0.0f) {
    neighbors++;
  }
  if (getCell(grid, i, j - 1) > 0.0f) {
    neighbors++;
  }
  if (getCell(grid, i, j + 1) > 0.0f) {
    neighbors++;
  }
  if (getCell(grid, i + 1, j - 1) > 0.0f) {
    neighbors++;
  }
  if (getCell(grid, i + 1, j) > 0.0f) {
    neighbors++;
  }
  if (getCell(grid, i + 1, j + 1) > 0.0f) {
    neighbors++;
  }
  return neighbors;
}

void swap(float ***grid, float ***newgrid) {
  float **temp = *grid;
  *grid = *newgrid;
  *newgrid = temp;
}

float average(float **grid, int i, int j) {
  float sum = 0.0f;
  sum += getCell(grid, i - 1, j - 1);
  sum += getCell(grid, i - 1, j);
  sum += getCell(grid, i - 1, j + 1);
  sum += getCell(grid, i, j - 1);
  sum += getCell(grid, i, j + 1);
  sum += getCell(grid, i + 1, j - 1);
  sum += getCell(grid, i + 1, j);
  sum += getCell(grid, i + 1, j + 1);
  return sum / 8.0f;
}

void iterate(float **grid, float **newgrid, int i, int j) {

  int neighbors = getNeighbors(grid, i, j);
  if (grid[i][j] > 0.0f) {
    if (neighbors < 2) {
      newgrid[i][j] = 0.0f;
    } else if (neighbors == 2 || neighbors == 3) {
      newgrid[i][j] = 1.0f;
    } else if (neighbors > 3) {
      newgrid[i][j] = 0.0f;
    }
  } else {
    if (neighbors == 3) {
      float newvalue = average(grid, i, j);
      if (newvalue > 0.0f)
        newgrid[i][j] = newvalue;
      else {
        newgrid[i][j] = 0.1f;
      }
    } else {
      newgrid[i][j] = 0.0f;
    }
  }
}

int numberOfCells(float **grid) {
  int cells = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (grid[i][j] > 0.0f) {
        cells++;
      }
    }
  }
  return cells;
}

void clearStdout() { printf("\033[H\033[J"); }

void printGrid(float **grid, int i, int stop) {
  printf("Número de Células: %d \n", numberOfCells(grid));
  for (int i = 0; i < stop; i++) {
    printf("|");
    for (int j = 0; j < stop; j++) {
      if (grid[i][j] == 0.0f) {
        printf("\033[1;30m██\033[0m");
      } else if (grid[i][j] < 0.2f) {
        printf("\033[1;31m██\033[0m");
      } else if (grid[i][j] < 0.4f) {
        printf("\033[1;33m██\033[0m");
      } else if (grid[i][j] < 0.6f) {
        printf("\033[1;32m██\033[0m");
      } else if (grid[i][j] < 0.8f) {
        printf("\033[1;36m██\033[0m");
      } else {
        printf("\033[1;34m██\033[0m");
      }
    }
    printf("|\n");
  }
}

void addGlider(float **grid, int x, int y) {
  grid[x][y + 1] = 1.0f;
  grid[x + 1][y + 2] = 1.0f;
  grid[x + 2][y] = 1.0f;
  grid[x + 2][y + 1] = 1.0f;
  grid[x + 2][y + 2] = 1.0f;
}

void addRPentomino(float **grid, int x, int y) {
  grid[x][y + 1] = 1.0f;
  grid[x][y + 2] = 1.0f;
  grid[x + 1][y] = 1.0f;
  grid[x + 1][y + 1] = 1.0f;
  grid[x + 2][y + 1] = 1.0f;
}

int getResult(void (*addPatterns)(float **grid)) {

  float **grid = (float **)malloc(sizeof(float *) * N);
  float **newgrid = (float **)malloc(sizeof(float *) * N);

  for (int i = 0; i < N; i++) {
    grid[i] = (float *)malloc(sizeof(float) * N);
    newgrid[i] = (float *)malloc(sizeof(float) * N);

    for (int j = 0; j < N; j++) {
      grid[i][j] = 0.0f;
      newgrid[i][j] = 0.0f;
    }
  }

  addPatterns(grid);

  // printGrid(grid, 0);

  struct timeval start, end;
  gettimeofday(&start, NULL);

  for (int i = 0; i < ITERNUM; i++) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        iterate(grid, newgrid, i, j);
      }
    }

    if (i < 6) {
      printGrid(grid, 0, 50);
    }
    if (i % 100 == 0) {
      printf("Iteração: %d\n", i);
    }
    swap(&grid, &newgrid);
    // usleep(USEC * SLEEP_TIME);
  }

  gettimeofday(&end, NULL);
  printf("Tempo de execução: %ld microssegundos\n",
         ((end.tv_sec * 1000000 + end.tv_usec) -
          (start.tv_sec * 1000000 + start.tv_usec)));

  // printGrid(grid, 2000);

  int cells = numberOfCells(grid);

  printf("Número de Células: %d \n", cells);

  for (int i = 0; i < N; i++) {
    free(grid[i]);
    free(newgrid[i]);
  }
  free(grid);
  free(newgrid);

  return cells;
}

void testOne(float **grid) {
  addGlider(grid, 1, 1);
  addRPentomino(grid, 10, 30);
}

int main(int argc, char *argv[]) {
  int result1 = getResult(testOne);

  printf("Número final de células: %d\n", result1);

  return 0;
}
