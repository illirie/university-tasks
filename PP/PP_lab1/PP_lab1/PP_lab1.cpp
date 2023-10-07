//#include <omp.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <iostream>
//
//
//int main(int argc, char* argv[])
//{
//	omp_set_num_threads(5);
//	int nThreads, threadNum;
//	double start_time = omp_get_wtime();
//
//#pragma omp parallel private(nThreads, threadNum)
//	{
//		nThreads = omp_get_num_threads();
//		threadNum = omp_get_thread_num();
//		printf("Hello world from process = %d total number of process: %d\n", threadNum, nThreads);
//	}
//
//	double finish_time = omp_get_wtime();
//	double result_time = finish_time - start_time;
//	std::cout << "Time: " << result_time;
//	return 0;
//}

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <ctime>


int main(int argc, char* argv[])
{
	unsigned int start_time = clock();
	for (int u = 0; u < 5; u++) {
		std::cout << "Hello world from process " << u << " of 15\n";
	}
	unsigned int end_time = clock(); // конечное время
	unsigned int search_time = end_time - start_time; // искомое время
	std::cout << (double)search_time / CLOCKS_PER_SEC;
	return 0;
}
