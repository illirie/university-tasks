#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define IDX2C(i,j,ld) (((j)*(ld))+(i)) 
#define SIZE 5600 // 1400, 2800, 5600

void cpu_mmul(const float* A, const float* B, float* C, int m_lenght) {
    for (int i = 0; i < m_lenght; ++i) {
        for (int j = 0; j < m_lenght; ++j) {
            C[IDX2C(i, j, m_lenght)] = 0.0;
            for (int r = 0; r < m_lenght; ++r) {
                C[IDX2C(i, j, m_lenght)] += A[IDX2C(i, r, m_lenght)] * B[IDX2C(r, j, m_lenght)];
            }
        }
    }
}


void print_matrix(float* matrix, int m_lenght) {
    printf("\n");
    for (int i = 0; i < m_lenght; ++i) {
        for (int j = 0; j < m_lenght; ++j) {
            printf("%f ", matrix[j * m_lenght + i]);
        }
        printf("\n");
    }
    printf("\n");
}


int main() {
    int m_lenght = SIZE, n2b = m_lenght * m_lenght * sizeof(float);
    float* h_A = (float*)malloc(n2b);
    float* h_B = (float*)malloc(n2b);
    float* h_C = (float*)malloc(n2b);

    double start_time, end_time, timeofwork;
    srand(time(0));

    start_time = clock();
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < m_lenght * m_lenght; j++) {
            h_A[j] = (float)rand() / RAND_MAX;
            h_B[j] = (float)rand() / RAND_MAX;
        }

        cpu_mmul(h_A, h_B, h_C, m_lenght);
    }
    end_time = clock();
    timeofwork = end_time - start_time;
    printf("Sequence. SIZE: %d\n", SIZE);
    printf("TIME OF WORK IS: %.9f\n\n", timeofwork / CLOCKS_PER_SEC / 12);

    printf("A =\n");
    print_matrix(h_A, 5);
    printf("B =\n");
    print_matrix(h_B, 5);

    printf("C =\n");
    print_matrix(h_C, 5);

    free(h_A);
    free(h_B);
    free(h_C);
    return 0;
}
