#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 1024

#define NUM_THREADS 64

int A[N][N], B[N][N], C[N][N];

int main() {
struct timespec start, end;

int b, nthreads;
double sum[NUM_THREADS];

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

for(int i = ID * 16; i < (ID + 1) * 16; i++) {
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < N; k++) {
                    C[i][j] += A[i][k] * B[k][j];
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
        fprintf(fp, "\n"); // new row
    }

    fclose(fp);
    return 0;
}