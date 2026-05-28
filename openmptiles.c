#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 1024
#define NUM_THREADS 32
#define TILE 64   //2, 4, 8, 16, 32, 64

int A[N][N], B[N][N], C[N][N];

int main() {
    struct timespec start, end;

    srand(10);
    int count = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
            C[i][j] = 0;
            count++;
        }
    }

    printf("%d is our count\n", count);

    clock_gettime(CLOCK_MONOTONIC, &start);

    omp_set_num_threads(NUM_THREADS);

#pragma omp parallel
{
    int ID = omp_get_thread_num();
    int nthrds = omp_get_num_threads();

    int tiles_per_dim = N / TILE;
    int start = ID * (tiles_per_dim / nthrds);
    int end   = (ID + 1) * (tiles_per_dim / nthrds);

    for (int ii = start * TILE; ii < end * TILE; ii += TILE) {
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
}
    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                     (end.tv_nsec - start.tv_nsec) / 1000000.0;

    printf("Threads: %d, Time: %.4f milliseconds\n", NUM_THREADS, time_ms);

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