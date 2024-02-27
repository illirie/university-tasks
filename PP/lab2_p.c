#define NMAX 4500000
#define Q 18
#include <stdio.h>
#include <time.h>

double a [NMAX];
int main(int argc, char* argv[])
{
        double sum = 0;
        int i, j, q;
        srand();
	for (j = 0; j < NMAX; j++)
	{
		a[j] = rand() % 10 + 1;
	}
        clock_t start = clock();
        for (i=0; i<NMAX; ++i)
        {
                double subsum = 0;
                for (q = 0; q < Q; q++) {
                        subsum += a[i];
                }
                sum += subsum / Q;
        }
        clock_t end = clock();
        printf("\nTotal Sum = %10.2f\n",sum);
        printf("time %f\n", ((double) (end - start)) / CLOCKS_PER_SEC);
        return 0;
}