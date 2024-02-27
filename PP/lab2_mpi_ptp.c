#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char* argv[])
{
	double TotalSum, ProcSum = 0.0;
	int ProcRank, ProcNum, N = 4500000, i, j, Q = 18;
	double x [N];
	MPI_Status Status;

	double st_time, end_time;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	if (ProcRank == 0)
	{
		for (i = 0; i < N; i++) {
			x[i] = rand() % 10 + 1;
		}
	}
	
	double y [N/ProcNum];
	MPI_Scatter(x, N / ProcNum, MPI_DOUBLE, y, N / ProcNum, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	st_time = MPI_Wtime();

	int k = N / ProcNum;
	for (i = 0; i < k; i++) {
		for (j = 0; j < Q; j++) {
			ProcSum += y[i];
		}
	}
	ProcSum /= Q;

	if (ProcRank == 0)
	{
		TotalSum = ProcSum;
		for (i = 1; i < ProcNum; i++)
		{
			MPI_Recv(&ProcSum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &Status);
			TotalSum = TotalSum + ProcSum;
		}
	}
	else
		MPI_Send(&ProcSum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	end_time = MPI_Wtime();
	end_time = end_time - st_time;

	if (ProcRank == 0)
	{
        printf("\nRank: %d", ProcNum);
		printf("\nTotal Sum = %10.2f", TotalSum);
		printf("\nTIME OF WORK IS %f ", end_time);
	}

	MPI_Finalize();
	return 0;
}
