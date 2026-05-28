#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1024
#define BLOCK 16

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

    for (int ii = 0; ii < N; ii += BLOCK) {
        for (int jj = 0; jj < N; jj += BLOCK) {
            for (int kk = 0; kk < N; kk += BLOCK) {
                for (int i = ii; i < ii + BLOCK && i < N; i++) {
                    for (int j = jj; j < jj + BLOCK && j < N; j++) {
                        for (int k = kk; k < kk + BLOCK && k < N; k++) {
                            C[i][j] += A[i][k] * B[k][j];
                        }
                    }
                }
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                     (end.tv_nsec - start.tv_nsec) / 1000000.0;

    printf("Tiled time (BLOCK=%d): %.3f ms\n", BLOCK, time_ms);
    return 0;
}