#include <stdio.h>
#include <string.h>
#include "mpi.h"

#define SIZE 10000
int main( int argc, char **argv ) {
  int i;
  int message[SIZE]; 
  int myrank; 
  MPI_Status status; 
  int count;
  for(i=0; i<SIZE; i++){
    message[i] = i;
  }
  MPI_Init( NULL, NULL ); 
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank ); 
  for(i=0; i<SIZE; i++){
    if (myrank == 0)/* code for process zero */ { 
      MPI_Send(message, i, MPI_INT, 1, 99, MPI_COMM_WORLD);
      MPI_Recv(message, i, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
      printf("%d \n", i);
      
    } else if (myrank == 1) /* code for process one */ {
      MPI_Send(message, i, MPI_INT, 0, 99, MPI_COMM_WORLD);
      MPI_Recv(message, i, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
    } 
  }
  MPI_Finalize();
} 
