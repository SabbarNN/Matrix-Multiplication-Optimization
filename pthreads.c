#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define N 1024
#define NUM_THREADS 2


int A[N][N], B[N][N], C[N][N];

typedef struct { int start, end; } ThreadArgs;

void *matmul(void *arg) {
    ThreadArgs *t = (ThreadArgs *)arg;
    for (int i = t->start; i < t->end; i++)
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
    return NULL;
}

int main() {
    srand(10);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }

    pthread_t threads[NUM_THREADS];
    ThreadArgs args[NUM_THREADS];
    int chunk = N / NUM_THREADS;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int t = 0; t < NUM_THREADS; t++) {
        args[t].start = t * chunk;
        args[t].end = (t == NUM_THREADS - 1) ? N : args[t].start + chunk;
        pthread_create(&threads[t], NULL, matmul, &args[t]);
    }
    for (int t = 0; t < NUM_THREADS; t++)
        pthread_join(threads[t], NULL);
    
    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = (end.tv_sec - start.tv_sec) +
                     (end.tv_nsec - start.tv_nsec) / 1e9;
    
    long long checksum = 0;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
                checksum += C[i][j];
        }
    }
    printf("Checksum: %lld\n", checksum);   

    printf("Threads: %d, Time: %.4f seconds\n", NUM_THREADS, elapsed, checksum);
    return 0;
}
