#include <stdio.h>
#include <string.h>
#include "mpi.h"

#define SIZE 10000

int main() {
  MPI_Status status; 
  MPI_Init( NULL, NULL ); 
  MPI_Comm intercomm;

  int work;
	MPI_Comm_get_parent(&intercomm);

	MPI_Recv(&work, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, intercomm, &status);
	int result = work /10;
	MPI_Send(&result, 1 ,MPI_INT, 0 , 99, intercomm);


  MPI_Finalize();
  return 0;
}