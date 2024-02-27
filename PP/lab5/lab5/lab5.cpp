#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <complex>

#define NMAX 700 // 700, 2100, 6300

void printMatrix(std::complex<double>* matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%f + %fi ", matrix[j + i * rows].real(), matrix[j + i * rows].imag());
        }
        printf("\n");
    }
    printf("\n");
}

void transpose(std::complex<double>* matrix, int nrCols) {
    std::complex<double> t;
    for (int i = 0; i < nrCols; ++i) {
        for (int j = i; j < nrCols; ++j) {
            t = matrix[j + i * nrCols];
            matrix[j + i * nrCols] = matrix[i + j * nrCols];
            matrix[i + j * nrCols] = t;
        }
    }
}
void multiply(std::complex<double>* A, std::complex<double>* B, std::complex<double>* C, int N)
{
    int i, j, k;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            C[i + j * N] = 0;
            for (k = 0; k < N; k++)
                C[i + j * N] += A[i + k * N] * B[k + j * N];
        }
    }
}

int main(int argc, char* argv[]) {
    int s = NMAX * NMAX;
    std::complex<double>* a = new std::complex<double>[s], * c = new std::complex<double>[s], * b = new std::complex<double>[s];
    double st_time, end_time = 0;
    srand(clock());
    st_time = clock();
    for (int j = 0; j < s; ++j) {
        a[j] = std::complex<double>((rand() + 0.0) / RAND_MAX, (rand() + 0.0) / RAND_MAX);
        b[j] = std::complex<double>((rand() + 0.0) / RAND_MAX, (rand() + 0.0) / RAND_MAX);
    }
    transpose(b, NMAX);
    multiply(a, b, c, NMAX);
    end_time = clock() - st_time;
    printf("\nTIME OF WORK IS: %f ", end_time / CLOCKS_PER_SEC);
    printf("\nC = \n");
    printMatrix(c, 5, 5);
    delete[] a, b, c;
    return 0;
}
