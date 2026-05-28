#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 1024

#define NUM_THREADS 16

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
        }
        //printf("A sample is %d, B sample is %d, C sample is %d\n", A[i][1], B[i][1], C[i][1]);
        count++;
}
printf("%d is our count", count);

clock_gettime(CLOCK_MONOTONIC, &start);
omp_set_num_threads(NUM_THREADS);
#pragma omp parallel
        {
            int ID = omp_get_thread_num();
            int nthrds = omp_get_num_threads();
            printf("This is thread %d\n", ID);

for(int i = ID * 64; i < (ID + 1) * 64; i++) {
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < N; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
             //printf("C[%d][1] is %d\n", i, C[i][1]);
        }
}
    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                     (end.tv_nsec - start.tv_nsec) / 1000000.0;

    printf("Baseline time: %.3f ms\n", time_ms);
    for(int i = 0; i < 16; i++) {
        printf("Baseline output: %d \n", C[i][300]);
    }


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