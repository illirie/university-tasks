#include <omp.h>
#define Q 18
#define N 4500000
#include "stdio.h"

double a [N];

int main(int argc, char* argv[]) {
	omp_set_num_threads(15);
	int i, j, u;
	double  sum;
	srand(1);
	for (j = 0; j < N; j++)
	{
		 a[j] = rand() % 10 + 1;
	}
	double st_time, end_time;
	st_time = omp_get_wtime();
	sum = 0;
for (u = 0; u < Q; u++) {
	sum = 0;
#pragma omp parallel for 
	for (i = 0; i < N; i++) {
#pragma omp critical
		{
			sum += a[i];
		}
	}
}
	end_time = omp_get_wtime();
	end_time = end_time - st_time;
	printf("\nTotal Sum = %10.2f", sum);
	printf("\nTIME OF WORK IS %f ", end_time);
	return 0;
}