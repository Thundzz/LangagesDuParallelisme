#include <stdio.h>
#include <string.h>
#include "mpi.h"

#define SIZE 10000
int main() {
  int i = 42;
  int message[SIZE]; 
  int myrank; 
  MPI_Status status; 
  MPI_Request request;
  MPI_Init( NULL, NULL ); 
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank ); 

  if (myrank == 0)/* code for process zero */ { 
    MPI_Isend(message, i, MPI_INT, 1, 99, MPI_COMM_WORLD, &request );
    MPI_Wait(&request, &status);
    
  } else if (myrank == 1) /* code for process one */ {
    MPI_Irecv(message, i, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);
    printf("%d \n",i);
  } 
  MPI_Finalize();
  return 0;
} 
