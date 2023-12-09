/**
 * Integrantes:
 *
 * Eduardo de Oliveira Motta
 * Gabriel Henrique Silva
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int iternum = 50;
int n = 25;
int rank, size;

float getCell(float **grid, int i, int j) {
  return grid[(i + n) % n][(j + n) % n];
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

int numberOfCells(float **grid, int start_line, int stop_line) {
  int cells = 0;
  for (int i = start_line; i < stop_line; i++) {
    for (int j = 0; j < n; j++) {
      if (grid[i][j] > 0.0f) {
        cells++;
      }
    }
  }
  if (rank == 0) {
    for (int k = 1; k < size; k++) {
      int res;
      MPI_Recv(&res, 1, MPI_INT, k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      cells += res;
    }
  } else {
    MPI_Send(&cells, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  return cells;
}

void clearStdout() { printf("\033[H\033[J"); }

void printGrid(float **grid, int i, int start_line, int stop_line) {
  printf("%d começou a imprimir\n", rank);
  fflush(stdout);
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) {
    printf("\n");
    fflush(stdout);
  }
  for (int k = 0; k < size; k++) {
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank != k) {
      continue;
    }
    if (stop_line >= n) {
      stop_line = n - 1;
    }
    for (int i = start_line; i < stop_line; i++) {
      printf("|");
      for (int j = 0; j < n; j++) {
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
    fflush(stdout);
  }
}

void addGlider(float **grid, int x, int y) {
  if (x < 0 || x >= n - 2 || y < 0 || y >= n - 2) {
    return;
  }
  grid[x][y + 1] = 1.0f;
  grid[x + 1][y + 2] = 1.0f;
  grid[x + 2][y] = 1.0f;
  grid[x + 2][y + 1] = 1.0f;
  grid[x + 2][y + 2] = 1.0f;
}

void addRPentomino(float **grid, int x, int y) {
  if (x < 0 || x >= n - 2 || y < 0 || y >= n - 2) {
    return;
  }
  grid[x][y + 1] = 1.0f;
  grid[x][y + 2] = 1.0f;
  grid[x + 1][y] = 1.0f;
  grid[x + 1][y + 1] = 1.0f;
  grid[x + 2][y + 1] = 1.0f;
}

int getResult(void (*addPatterns)(float **grid), int start_line, int stop_line,
              int iternum) {

  float **grid = (float **)malloc(sizeof(float *) * n);
  float **newgrid = (float **)malloc(sizeof(float *) * n);

  for (int i = 0; i < n; i++) {
    grid[i] = (float *)malloc(sizeof(float) * n);
    newgrid[i] = (float *)malloc(sizeof(float) * n);

    for (int j = 0; j < n; j++) {
      grid[i][j] = 0.0f;
      newgrid[i][j] = 0.0f;
    }
  }

  if (rank == 0) {
    printf("Adicionando patterns...\n");
  }

  addPatterns(grid);

  if (rank == 0) {
    printf("Patterns adicionados!\n");
  }

  struct timeval start, end;
  gettimeofday(&start, NULL);
  int i;

  for (i = 0; i < iternum; i++) {
    for (int i = start_line; i < stop_line; i++) {
      for (int j = 0; j < n; j++) {
        iterate(grid, newgrid, i, j);
      }
    }

    swap(&grid, &newgrid);

    int tag = 0;
    float *send_buf = grid[stop_line - 1];
    float *recv_buf = grid[(start_line - 1 + n) % n];
    // printf("%d: Enviando linha %d para o rank %d\n", rank, stop_line - 1,
    //        (rank + 1) % size);
    // for (int i = 0; i < n; i++) {
    //   printf("%f ", send_buf[i]);
    // }
    // printf("\n");

    int res = MPI_Sendrecv(send_buf, n, MPI_FLOAT, (rank + 1) % size, tag,
                           recv_buf, n, MPI_FLOAT, (rank - 1 + size) % size,
                           tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    if (res == MPI_SUCCESS) {
      // printf("%d: Recebendo linha %d do rank %d\n", rank,
      //        (start_line - 1 + n) % n, (rank - 1 + size) % size);

      // for (int i = 0; i < n; i++) {
      //   printf("%f ", recv_buf[i]);
      // }
      // printf("\n");
    }

    float *send_buf2 = grid[start_line];
    float *recv_buf2 = grid[(stop_line) % n];
    // printf("%d: Enviando linha %d para o rank %d\n", rank, start_line,
    //        (rank - 1 + size) % size);

    // for (int i = 0; i < n; i++) {
    //   printf("%f ", send_buf2[i]);
    // }
    // printf("\n");

    res = MPI_Sendrecv(send_buf2, n, MPI_FLOAT, (rank - 1 + size) % size, tag,
                       recv_buf2, n, MPI_FLOAT, (rank + 1) % size, tag,
                       MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    if (res == MPI_SUCCESS) {
      // printf("%d: Recebendo linha %d do rank %d\n", rank, (stop_line) % n,
      //        (rank + 1) % size);
      // for (int i = 0; i < n; i++) {
      //   printf("%f ", recv_buf2[i]);
      // }
      // printf("\n");
    }
  }

  // MPI_Barrier(MPI_COMM_WORLD);

  gettimeofday(&end, NULL);
  printf("Tempo de execução: %ld microssegundos\n",
         ((end.tv_sec * 1000000 + end.tv_usec) -
          (start.tv_sec * 1000000 + start.tv_usec)));

  fflush(stdout);

  MPI_Barrier(MPI_COMM_WORLD);

  printGrid(grid, i, start_line, stop_line);

  int cells = numberOfCells(grid, start_line, stop_line);

  for (int i = 0; i < n; i++) {
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
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  printf("Processo %d de %d\n", rank, size);
  if (rank == 0) {
    printf("Integrantes:\n");
    printf("Eduardo de Oliveira Motta\n");
    printf("Gabriel Henrique Silva\n");
  }
  if (argc != 3) {
    printf("Uso: ./rainbow <iteracoes> <tamanho>\n");
    return 1;
  }
  iternum = atoi(argv[1]);
  n = atoi(argv[2]);
  printf("Iterações: %d\n", iternum);
  printf("Tamanho: %d\n", n);

  int start_line = rank * n / size;
  int stop_line = (rank + 1) * n / size;
  printf("Processo %d: linhas %d a %d\n", rank, start_line, stop_line);

  int result1 = getResult(testOne, start_line, stop_line, iternum);

  if (rank == 0) {
    printf("Celulas vivas: %d\n", result1);
  }

  MPI_Finalize();
  return 0;
}
