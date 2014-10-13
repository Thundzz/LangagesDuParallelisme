#include <stdio.h>
#include <string.h>
#include "mpi.h"

#define SIZE 10000
int main( int argc, char **argv ) {
  int i;
  int message = 100, recu; 
  int myrank; 
  MPI_Status status; 
  int count;
  MPI_Init( NULL, NULL ); 
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank ); 
  message += myrank;
  if (myrank == 0)/* code for process zero */ { 
    MPI_Send(&message, 1, MPI_INT, 1, 99, MPI_COMM_WORLD);
    MPI_Recv(&recu, 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
    printf("%d reçu, je suis: %d\n", recu, myrank);      
  } else if (myrank == 1) /* code for process one */ {
    MPI_Recv(&recu, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
    MPI_Send(&message, 1, MPI_INT, 0, 99, MPI_COMM_WORLD);
    printf("%d reçu, je suis: %d\n", recu, myrank); 
  }
  MPI_Finalize();
} 
