#include <stdio.h>
#include <string.h>
#include "mpi.h"

#define SIZE 10000
int main() {
  int message = 0, recu, size; 
  int myrank; 
  MPI_Status status; 
  MPI_Init( NULL, NULL ); 
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank); 
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (myrank == 0 && message ==0)/* code for process zero */ { 
    MPI_Send(&message,1, MPI_INT, myrank+1, 99, MPI_COMM_WORLD);
    MPI_Recv(&recu, 1, MPI_INT, size-1, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
    printf("La boucle est finie.\n");      
  } else /* code for the others */ {
    MPI_Recv(&recu, 1, MPI_INT, myrank -1 , MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
    message = recu +1;
    MPI_Send(&message, 1, MPI_INT, (myrank +1) % size, 99, MPI_COMM_WORLD);
    printf("%d reçu, je suis: %d\n", recu, myrank); 
  }
  MPI_Finalize();
  return 0;
} 
