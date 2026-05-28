#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1024
#define NUM_THREADS 16
#define TILE 64   //2, 4, 8, 16, 32, 64

int A[N][N], B[N][N], C[N][N];

int main() {
    srand(10);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
            C[i][j] = 0;
        }
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

        for (int ii = 0; ii < N; ii += TILE) {
                for (int jj = 0; jj < N; jj += TILE) {

                        for (int kk = 0; kk < N; kk += TILE) {

                                for (int i = ii; i < ii + TILE && i < N; i++) {
                                        for (int j = jj; j < jj + TILE && j < N; j++) {

                                                int sum = C[i][j];

                                                for (int k = kk; k < kk + TILE && k < N; k++) {
                                                        sum += A[i][k] * B[k][j];
                                                }

                                                C[i][j] = sum;
                                        }
                                }

                        }
                }
        }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                     (end.tv_nsec - start.tv_nsec) / 1000000.0;

    printf("Tiles: %d, Time: %.4f milliseconds\n", TILE, time_ms);

    FILE *fp = fopen("matrixMP.txt", "w");
    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(fp, "%d ", C[i][j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    return 0;
}