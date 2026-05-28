#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1024

int A[N][N], B[N][N], C[N][N];

int main() {
    srand(10);
        int count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = 7;
            B[i][j] = 8;
            C[i][j] = 0;
            count++;
        }

    }
    printf("Our count of total objects is %d\n", count);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_ms = ((end.tv_sec - start.tv_sec) * 1000.0) + ((end.tv_nsec - start.tv_nsec) / 1000000.0);

    printf("Baseline time: %.3f ms\n", time_ms);
    FILE *fp = fopen("matrixC.txt", "w");

    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(fp, "%d ", C[i][j]);
        }
        fprintf(fp, "\n"); // new row
    }

    fclose(fp);
    return 0;
}