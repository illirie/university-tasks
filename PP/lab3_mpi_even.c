#include "mpi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define Q 1

void sum(double* a, double* b, double* res, int n){
	for (int i = 0; i < n; ++i){
		for (int j = 0; j < Q; ++j){
			res[i] = a[i] + b[i];
		}
	}
}

void test(double* vec1, double* vec2, int n){
	int flag = 1;
	for (int i = 0; i < n && flag; ++i){
		flag = vec1[i] == vec1[i];
	}
	if (flag){
		printf("\nTrue");
	}
	else{
		printf("\nFalse");
	}
}

int main(int argc, char* argv[])
{
	int ProcNum, ProcRank;
	int N = 4500000;
	double* a = NULL;
	double* b = NULL;
	double* c = NULL;
	double* real_sum = NULL;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	if (ProcRank == 0)
	{
		a = (double*)calloc(sizeof(double), N);
		b = (double*)calloc(sizeof(double), N);
		c = (double*)calloc(sizeof(double), N);
		real_sum = (double*)calloc(sizeof(double), N);
		for (int i = 0; i < N; ++i){
			a[i] = 1;
			b[i] = 1;
            real_sum[i] = a[i] + b[i];
		}
    }
	double st_time;
	MPI_Status status;
	int offset = N / ProcNum;
	double* a_loc = (double*)calloc(sizeof(double), offset);
	double* b_loc = (double*)calloc(sizeof(double), offset);
	double* c_loc = (double*)calloc(sizeof(double), offset);
	MPI_Scatter(a, offset, MPI_DOUBLE, a_loc, offset, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatter(b, offset, MPI_DOUBLE, b_loc, offset, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	if (ProcRank == 0) {
		free(a);
		free(b);
		st_time = MPI_Wtime();
	}
	sum(a_loc, b_loc, c_loc, offset);
	MPI_Gather(c_loc, offset, MPI_DOUBLE, c, offset, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	if (ProcRank == 0)
	{
		printf("\nTime of work is %f\n", MPI_Wtime() - st_time);
		test(c, real_sum, N);
        printf("\nRank: %d", ProcNum);
		free(real_sum);
		free(c);
	}
	free(a_loc);
	free(b_loc);
	free(c_loc);
	MPI_Finalize();

	return 0;
}
