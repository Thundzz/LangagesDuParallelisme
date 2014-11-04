




 #include <stdio.h>
#include <string.h>
#include "mpi.h"

#define SIZE 10000
int main() {
  int size; 
  int myrank; 
  MPI_Comm  grid2D, Comm1D; 
  MPI_Init( NULL, NULL ); 

	MPI_Spawn(!, slave, !); 
	for (i=0; i < size; ++i) 
		MPI_Send(work, !,i, !); 
	for (i=0; i < size; ++i) 
		MPI_Recv(presults, !); 
	calc_and_display_result(!) 

  MPI_Finalize();
  return 0;
}