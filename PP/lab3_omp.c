#include <omp.h>
#include "stdio.h"

#define Q 18

double* omp_static(double* a, double* b, int n){
	double* sum = (double*)calloc(sizeof(double), n);
	#pragma omp parallel for shared(a, b, s um) schedule(static, 100)
	for(int i = 0; i < n; ++i){
		for(int j = 0; j < Q; ++j){
			sum[i] = a[i] + b[i];
		}
	}
	return sum;
}

double* omp_dynamic(double* a, double* b, int n){
	double* sum = (double*)calloc(sizeof(double), n);
	#pragma omp parallel for shared(a, b, sum) schedule(dynamic, 100)
	for(int i = 0; i < n; ++i){
		for(int j = 0; j < Q; ++j){
			sum[i] = a[i] + b[i];
		}
	}
	return sum;
}


double* omp_guided(double* a, double* b, int n){
	double* sum = (double*)calloc(sizeof(double), n);
	#pragma omp parallel for shared(a, b, sum) schedule(guided, 100)
	for(int i = 0; i < n; ++i){
		for(int j = 0; j < Q; ++j){
			sum[i] = a[i] + b[i];
		}
	}
	return sum;
}

double* consistent(double* a, double* b, int n){
	double* sum = (double*)calloc(sizeof(double), n);
	for(int i = 0; i < n; ++i){
		for(int j = 0; j < Q; ++j){
			sum[i] = a[i] + b[i];
		}	
	}
	return sum;
}

void test(double* a, double* b, int n){
	int flag = 1;
	for (int i = 0; i < n; ++i){
		flag = a[i] == b[i];
	}
	if (flag){
		printf("\nTrue");
	}
	else{
		printf("\nFalse");
	}
}


int main(int argc, char* argv[]) {
	omp_set_num_threads(15);
	int n = 4500000;
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

	st_time = omp_get_wtime();
	sum_vec = omp_static(a, b, n);
	test(sum_vec, referenced_vec, n);
	free(sum_vec);
	printf("\nTime of work static: %f ", omp_get_wtime() - st_time);

	st_time = omp_get_wtime();
	sum_vec = omp_dynamic(a, b, n);
	test(sum_vec, referenced_vec, n);
	free(sum_vec);
	printf("\nTime of work dynamic: %f ", omp_get_wtime() - st_time);

	st_time = omp_get_wtime();
	sum_vec = omp_guided(a, b, n);
	test(sum_vec, referenced_vec, n);
	free(sum_vec);
	printf("\nTime of work guided: %f ", omp_get_wtime() - st_time);

	free(a);
	free(b);
	free(referenced_vec);

	return 0;
}
