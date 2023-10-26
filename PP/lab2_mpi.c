#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define NMAX 4500000

int main(int argc=5, char* argv[])
{
    double TotalSum, ProcSum = 0.0;
    double a [NMAX];
    int ProcRank, ProcNum;
    MPI_Status Status;

    double st_time, end_time;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD,&ProcRank);

    size_t i;

    if (ProcRank==0)
    {
        //заполнение массива
        size_t i;
        for (i=0; i < NMAX; ++i)
        {
            a[i] = (double)(i % 2);
        }
    }

    st_time = MPI_Wtime();

    int k = NMAX / ProcNum;
    int i1 = k * ProcRank;
    int i2 = k * ( ProcRank + 1 );

    if ( ProcRank == ProcNum-1 ) i2 = NMAX;
    for ( i = i1; i < i2; i++ )
        ProcSum = ProcSum + a[i];

    if ( ProcRank == 0 )
    {
        TotalSum = ProcSum;
        for ( i = 1; i < ProcNum; i++ )
        {
            MPI_Recv(&ProcSum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD,&Status);
            TotalSum = TotalSum + ProcSum;
        }
    }
    else
        MPI_Send(&ProcSum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    end_time = MPI_Wtime();
    end_time = end_time - st_time;

    if ( ProcRank == 0 )
    {
        printf("\nTotal Sum = %10.2f",TotalSum);
        printf("\nTIME OF WORK IS %f ", end_time);
    }

    MPI_Finalize();
    return 0;
}