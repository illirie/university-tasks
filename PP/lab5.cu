#include <stdlib.h>
#include <stdio.h>
#include <curand.h>
#include <iostream>
#include <ctime>
#include "cublas_v2.h"

using namespace std;

// Fill the array A(nr_rows_A, nr_cols_A) with random numbers on GPU
void GPU_fill_rand(cuDoubleComplex* A, int nr_rows_A, int nr_cols_A) {
    // Create a pseudo-random number generator
    curandGenerator_t prng;
    curandCreateGenerator(&prng, CURAND_RNG_PSEUDO_DEFAULT);
    // Set the seed for the random number generator using the system clock
    curandSetPseudoRandomGeneratorSeed(prng, (unsigned long long) clock());

    // Fill the array with random numbers on the device
    curandGenerateUniformDouble(prng, (double*)A, nr_rows_A * nr_cols_A * 2);
}

/// Multiply the arrays A and B on GPU and save the result in C
// C(m,n) = A(m,k) * B(k,n)
void gpu_blas_mmul(const cuDoubleComplex* A, const cuDoubleComplex* B, cuDoubleComplex* C, const int m, const int k, const int n) {
    int lda = m, ldb = k, ldc = m;
    const cuDoubleComplex alf = make_cuDoubleComplex(1.0, 0.0);
    const cuDoubleComplex bet = make_cuDoubleComplex(0.0, 0.0);
    const cuDoubleComplex* alpha = &alf;
    const cuDoubleComplex* beta = &bet;

    // CUBLAS is starting here
    cublasHandle_t handle;
    cublasStatus_t status = cublasCreate(&handle);
    // Do the actual multiplication
    cublasZgemm(handle,
                CUBLAS_OP_T,
                CUBLAS_OP_T,
                m,
                n,
                k,
                alpha,
                A,
                lda,
                B,
                ldb,
                beta,
                C,
                ldc);
    // Destroy the handle
    cublasDestroy(handle);
}

void print_matrix(cuDoubleComplex* matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%f + %fi ", matrix[i + j * rows].x, matrix[i + j * rows].y);
        }
        printf("\n");
    }
    printf("\n");
}

void fill_matrix(cuDoubleComplex* matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i + j * rows] = make_cuDoubleComplex((rand() + 0.0) / RAND_MAX, (rand() + 0.0) / RAND_MAX);
        }
    }
}

int main() {
    int nr_rows_A, nr_cols_A, nr_rows_B, nr_cols_B, nr_rows_C, nr_cols_C;
    nr_rows_A = nr_cols_A = nr_rows_B = nr_cols_B = nr_rows_C = nr_cols_C = 700; // 700, 2100, 6300
    cuDoubleComplex* h_A = (cuDoubleComplex*)malloc(nr_rows_A * nr_cols_A * sizeof(cuDoubleComplex));
    cuDoubleComplex* h_B = (cuDoubleComplex*)malloc(nr_rows_B * nr_cols_B * sizeof(cuDoubleComplex));
    cuDoubleComplex* h_C = (cuDoubleComplex*)malloc(nr_rows_C * nr_cols_C * sizeof(cuDoubleComplex));

    // create pointers into the GPU
    cuDoubleComplex* d_A;
    cuDoubleComplex* d_B;
    cuDoubleComplex* d_C;

    // allocate memory in the GPU
    cudaMalloc(&d_A, nr_rows_A * nr_cols_A * sizeof(cuDoubleComplex));
    cudaMalloc(&d_B, nr_rows_B * nr_cols_B * sizeof(cuDoubleComplex));
    cudaMalloc(&d_C, nr_rows_C * nr_cols_C * sizeof(cuDoubleComplex));

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    float gpuTime = 0.0f;
    srand(time(0));
    cudaEventRecord(start, 0);

    
    // GPU_fill_rand(d_A, nr_rows_A, nr_cols_A);
    // GPU_fill_rand(d_B, nr_rows_B, nr_cols_B);
    
    fill_matrix(h_A, nr_rows_A, nr_cols_A);
    fill_matrix(h_B, nr_rows_B, nr_cols_B);
    cudaMemcpy(d_A, h_A, nr_rows_A * nr_cols_A * sizeof(cuDoubleComplex), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, nr_rows_B * nr_cols_B * sizeof(cuDoubleComplex), cudaMemcpyHostToDevice);
    gpu_blas_mmul(d_A, d_B, d_C, nr_rows_A, nr_cols_A, nr_cols_B);
    // cudaMemcpy(h_A, d_A, nr_rows_A * nr_cols_A * sizeof(cuDoubleComplex), cudaMemcpyDeviceToHost);
    // cudaMemcpy(h_B, d_B, nr_rows_B * nr_cols_B * sizeof(cuDoubleComplex), cudaMemcpyDeviceToHost);
    cudaMemcpy(h_C, d_C, nr_rows_C * nr_cols_C * sizeof(cuDoubleComplex), cudaMemcpyDeviceToHost);

    cudaEventRecord(stop, 0);
    cudaEventElapsedTime(&gpuTime, start, stop);

    printf("C = \n");
    print_matrix(h_C, 5, 5);
    printf("CUDA TIME OF WORK IS: %.9f\n\n", gpuTime / 1000);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    free(h_A);
    free(h_B);
    free(h_C);
    return 0;
}
