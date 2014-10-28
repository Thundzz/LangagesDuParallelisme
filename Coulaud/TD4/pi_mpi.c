#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

double f(double x)
{
	return 4.0/(1+x*x);
}

int main(int argc, char ** argv)
{ 
	/*Initializing MPI */
	int rank, size; 
	MPI_Init( NULL, NULL ); 
	MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
	MPI_Comm_size(MPI_COMM_WORLD, &size);


	double PI25DT = 3.141592653589793238462643;
	const int nIntervals = (argc >=2) ? atoi(argv[1]): 100;
	const double invNIntervals = 1.0 / (double) nIntervals;
	double x = 0, mypi, pi;
	int i;



	int bloc = (nIntervals-1)/ size ; 
	int istart = 1+ rank*bloc ; 
	int iend = istart + bloc -1 ; 
	if (rank == size-1) 
		iend = nIntervals; 


	printf("I am process number %d\nI'm in charge of start=%d to end=%d\n\n", rank, istart, iend);
	double sum = 0;
	for (i = istart; i <= iend; ++i) { 
		x = invNIntervals * ((double) i - 0.5); 
		sum += f(x); 
	} 

	mypi = invNIntervals * sum; 
	MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
	

	if (rank ==0)
		printf("Pi %.16f, Error is %e\n", pi, fabs(pi-PI25DT));

  	MPI_Finalize();
	return 0;
}