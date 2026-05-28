#include <iostream>
#include <cuda_runtime.h>
#include <chrono>
#include <cstdlib>

#define N 1024

// edit if needed
#define TILE_SIZE 16

// edit if needed 
#define NUM_RUNS 100

__global__ void matmul_tiled(float* A, float* B,float* C,int n)
{
    __shared__ float tileA[TILE_SIZE][TILE_SIZE];
    __shared__ float tileB[TILE_SIZE][TILE_SIZE];

    int row = blockIdx.y * blockDim.y + threadIdx.y;

    int col = blockIdx.x * blockDim.x + threadIdx.x;

    float sum = 0.0f;

    for (int t = 0; t < n; t += TILE_SIZE)
    {
        if (row < n && t + threadIdx.x < n)
            tileA[threadIdx.y][threadIdx.x] = A[row * n + t + threadIdx.x];
        else
            tileA[threadIdx.y][threadIdx.x] = 0.0f;

        if (col < n && t + threadIdx.y < n)
            tileB[threadIdx.y][threadIdx.x] = B[(t + threadIdx.y) * n + col];
        else
            tileB[threadIdx.y][threadIdx.x] = 0.0f;

        __syncthreads();

        for (int k = 0; k < TILE_SIZE; k++)
        {
            sum += tileA[threadIdx.y][k] * tileB[k][threadIdx.x];
        }

        __syncthreads();
    }

    if (row < n && col < n)
        C[row * n + col] = sum;
}

int main()
{
    srand(10);

    size_t size = N * N * sizeof(float);

    float* h_A = (float*)malloc(size);

    float* h_B = (float*)malloc(size);

    float* h_C = (float*)malloc(size);

    for (int i = 0; i < N * N; i++)
    {
        h_A[i] =
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

        h_B[i] =
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }

    float* d_A;
    float* d_B;
    float* d_C;

    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);

    cudaMemcpy(d_A,h_A,size,cudaMemcpyHostToDevice);

    cudaMemcpy(d_B,h_B,size,cudaMemcpyHostToDevice);

    dim3 block(TILE_SIZE,TILE_SIZE);

    dim3 grid(
        (N + TILE_SIZE - 1) / TILE_SIZE,
        (N + TILE_SIZE - 1) / TILE_SIZE);

    double total_time = 0.0;

    for (int run = 0; run < NUM_RUNS; run++)
    {
        auto start = std::chrono::high_resolution_clock::now();

        matmul_tiled<<<grid, block>>>(d_A,d_B,d_C,N);

        cudaDeviceSynchronize();

        auto end = std::chrono::high_resolution_clock::now();

        total_time += std::chrono::duration<double>(end - start).count();
    }

    double avg_time = total_time / NUM_RUNS;

    cudaMemcpy(h_C,d_C,size,cudaMemcpyDeviceToHost);

    double checksum = 0.0;

    for (int i = 0; i < N * N; i++)
        checksum += h_C[i];

    std::cout
        << "Average execution time: "
        << avg_time
        << " seconds\n";

    std::cout
        << "Tile size: "
        << TILE_SIZE
        << "x"
        << TILE_SIZE
        << std::endl;

    std::cout
        << "Checksum: "
        << checksum
        << std::endl;

    std::cout
        << "C[0] = "
        << h_C[0]
        << std::endl;

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    free(h_A);
    free(h_B);
    free(h_C);

    return 0;
}