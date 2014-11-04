#include <stdio.h>
#include <string.h>
#include "mpi.h"

#define SIZE 10000
int main() {
  int size; 
  int myrank; 
  MPI_Datatype bloc, newbloc; 
  MPI_Init( NULL, NULL ); 
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank); 
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int mybloc[4];
  for (int i = 0; i < 4; ++i)
  {
  	mybloc[i] =0;
  }
  int matrix[16];
  if (myrank == 0)
  {
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        matrix[i+4*j]= j+1 + 10*(i+1); 
      }
    }
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        printf("%d ", matrix[i+4*j] );
      }
      printf("\n");
    }
  }
  MPI_Type_vector(2, 2, 4, MPI_INT, &bloc);
  MPI_Type_commit(&bloc);

  MPI_Aint bloclb, blocextent;
  MPI_Type_get_extent(bloc, &bloclb, &blocextent);
  MPI_Type_create_resized(bloc, bloclb, 4*sizeof(MPI_INT), &newbloc);
  MPI_Type_commit(&newbloc);

  MPI_Scatter(&matrix, 1, newbloc, &mybloc, 4, MPI_INT, 0, MPI_COMM_WORLD);
  
  for (int i = 0; i < 2; ++i)
  {
    for (int j = 0; j < 2; ++j)
    {
      printf("myrank=%d - %d\n", myrank, mybloc[i+2*j] );
    }
  }
  MPI_Finalize();
  return 0;
}