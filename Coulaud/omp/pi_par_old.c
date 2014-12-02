#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double f(double x)
{
	return 4.0/(1+x*x);
}

int main(int argc, char ** argv)
{ 
	double PI25DT = 3.141592653589793238462643;
	const int nIntervals = (argc >=2) ? atoi(argv[1]): 100;
	const double invNIntervals = 1.0 / (double) nIntervals;
	double x0 = 0;
	int i;
	int nbThreads;
	#pragma omp parallel
	{
 		nbThreads= omp_get_num_threads();
	}
	double *sum = malloc( nbThreads * sizeof(double));
	double surface =  0;
	#pragma omp parallel for schedule(static) private(i)
	for ( i = 0; i < nIntervals; ++i)
	{
		int me = omp_get_thread_num();
		const double x= x0  +  i* invNIntervals;
		sum[me] += invNIntervals* f(x + invNIntervals*0.5);
	}


	for (int i = 0; i < nbThreads; ++i)
	{
		surface += sum[i];
	}

	printf("Pi %.16f, Error is %e\n", surface, fabs(surface-PI25DT));
	return 0;
}

