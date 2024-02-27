#include <omp.h>
#include <stdio.h>

double* consistent(double* a, double* b, int n){
	double* sum = (double*)calloc(sizeof(double), n);
	for(int i = 0; i < n; ++i){
		sum[i] = a[i] + b[i];
	}
	return sum;
}

int main(int argc, char* argv[]) {
	int n = 450000;
	double st_time;
	double* a = (double*)calloc(sizeof(double), n);
	double* b = (double*)calloc(sizeof(double), n);
	double* sum_vec = NULL;
	double* referenced_vec = NULL;

	for (int i = 0; i < n; ++i){
		a[i] = 1;
		b[i] = 1;
	}

	st_time = omp_get_wtime();
	referenced_vec = consistent(a, b, n);
	printf("\nTime of work consistent: %f", omp_get_wtime() - st_time);

	free(a);
	free(b);
	free(referenced_vec);

	return 0;
}