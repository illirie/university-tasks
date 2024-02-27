#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <complex.h>

#define IDX2C(i,j,ld) (((j)*(ld))+(i)) 
#define NMAX 700 // 700, 2100, 6300
#define ITERATIONS 1

double complex* transpose(const double complex* A, int matrixSize, int n2b) {
    double complex* new_array = (double complex*)malloc(n2b);
    for (int i = 0; i < matrixSize; ++i)
    {
        for (int j = 0; j < matrixSize; ++j)
        {
            // Index in the original matrix.
            int index1 = i * matrixSize + j;

            // Index in the transpose matrix.
            int index2 = j * matrixSize + i;

            new_array[index2] = A[index1];
        }
    }

    return new_array;
}



void cpu_mmul(const double complex* A, const double complex* B, double complex* C, int matrixSize, int n2b) {
    double complex* B_T = transpose(B, matrixSize, n2b);
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            C[IDX2C(i, j, matrixSize)] = 0.0 + 0.0 * I;
            for (int r = 0; r < matrixSize; ++r) {
                C[IDX2C(i, j, matrixSize)] += A[IDX2C(i, r, matrixSize)] * B_T[IDX2C(r, j, matrixSize)];
            }
        }
    }
}



void print_matrix(double complex* matrix, int matrixSize) {
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            printf("%f + i*%f", creal(matrix[j * matrixSize + i]), cimag(matrix[j * matrixSize + i]));
        }
        printf("\n");
    }
    printf("\n");
}


int main() {
    // Allocate 3 arrays on CPU
    // for simplicity we are going to use square arrays
    srand(time(0));
    int matrixSize = NMAX, n2b = matrixSize * matrixSize * sizeof(double complex);
    double complex* h_A = (double complex*)malloc(n2b);
    double complex* h_B = (double complex*)malloc(n2b);
    double complex* h_C = (double complex*)malloc(n2b);

    double start_time, end_time;

    start_time = clock();
    for (int i = 0; i < ITERATIONS; ++i) {
        for (int j = 0; j < matrixSize * matrixSize; j++) {
            h_A[j] = (double)rand() / RAND_MAX + I * ((double)rand() / RAND_MAX);
            h_B[j] = (double)rand() / RAND_MAX + I * ((double)rand() / RAND_MAX);
        }

        // Multiply A and B
        cpu_mmul(h_A, h_B, h_C, matrixSize, n2b);
    }
    end_time = clock();

    printf("Consistent time of work is: %.9f\n\n", (end_time - start_time) / CLOCKS_PER_SEC / ITERATIONS);
    printf("NMAX: ", NMAX);
    printf("A =\n");
    print_matrix(h_A, 3);
    printf("B =\n");
    print_matrix(h_B, 3);

    printf("C =\n");
    print_matrix(h_C, 3);

    // Free CPU memory
    free(h_A);
    free(h_B);
    free(h_C);
    return 0;
}
