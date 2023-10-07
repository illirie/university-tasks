#include "mpi.h"
#include "stdio.h"

int main(int argc, char* argv[])
{
	int rank, ranksize, i;

	MPI_Init(&argc, &argv);

	double start_time, finish_time;

	start_time = MPI_Wtime();

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &ranksize);
	printf("Hello world from process %d from total number of %d\n", rank, ranksize);

	finish_time = MPI_Wtime() - start_time;
	printf("Time: %f\n", finish_time);

	MPI_Finalize();

	return 0;
}