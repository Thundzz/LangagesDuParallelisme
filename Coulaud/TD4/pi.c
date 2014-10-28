#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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

	double surface =  0;
	for ( i = 0; i < nIntervals; ++i)
	{

		const double x= x0  +  i* invNIntervals;
		surface += invNIntervals* f(x + invNIntervals*0.5);
	}

	printf("Pi %.16f, Error is %e\n", surface, fabs(surface-PI25DT));
	return 0;
}