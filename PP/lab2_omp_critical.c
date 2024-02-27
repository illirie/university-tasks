#include <omp.h>
#define Q 18
#define CHUNK 100
#define N 4500000
#include "stdio.h"

double a [N];

int main(int argc, char* argv[]) {
	omp_set_num_threads(15);
	int i, j, q;
	double  sum, subsum;
	srand(1);
	for (j = 0; j < N; j++)
	{
		 a[j] = rand() % 10 + 1;
	}
	double st_time, end_time;
	st_time = omp_get_wtime();
	sum = 0;
#pragma omp parallel for shared(a) private(i, q, subsum)
	for (i = 0; i < N; i++) {
		subsum = 0;
		for (q = 0; q < Q; q++) {
			subsum += a[i];
		}
#pragma omp critical
		{
			sum += subsum / Q;
		}
	}
	end_time = omp_get_wtime();
	end_time = end_time - st_time;
	printf("\nTotal Sum = %10.2f", sum);
	printf("\nTIME OF WORK IS %f ", end_time);
	return 0;
}