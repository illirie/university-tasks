#define NMAX 4500000
#include <stdio.h>
#include <time.h>

double a [NMAX];
int main(int argc, char* argv[])
{
        double sum = 0;
        int i;
        for (i=0; i < NMAX; ++i)
        {
                a[i] = (double)(i % 2);
        }

        clock_t start = clock();
        for (i=0; i<NMAX; ++i)
        {
                sum += a[i];
        }
        clock_t end = clock();
        printf("\nTotal Sum = %10.2f\n",sum);
        printf("time %f\n", ((double) (end - start)) / CLOCKS_PER_SEC);
        return 0;
}