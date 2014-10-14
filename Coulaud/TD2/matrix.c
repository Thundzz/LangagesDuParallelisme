#include <stdio.h>
#include <string.h>
#include "mpi.h"

#define SIZE 10000
#define LINES 5
#define COLUMNS 5
int main() {
  int i,j;
  int m[LINES*COLUMNS];
  int message[LINES]; 
  int myrank; 
  MPI_Status status; 
  MPI_Request request;
  MPI_Init( NULL, NULL );
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank ); 
  MPI_Datatype diag;
  

  
  for(i = 0; i<LINES; i++){
    for(j=0; j<COLUMNS; j++){
      if(i==j)
        m[i*COLUMNS+j] = -1;
      else
        m[i*COLUMNS+j] = 0;
      }
    }
    
  MPI_Type_vector(LINES, 1, COLUMNS+1, MPI_INT, &diag);
  MPI_Type_commit(&diag);
  
  if (myrank == 0)/* code for process zero */ {

    MPI_Isend(m, 1, diag, 1, 99, MPI_COMM_WORLD, &request );
    MPI_Wait(&request, &status);
    
  } else if (myrank == 1) /* code for process one */ {
    MPI_Irecv(message, LINES, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);
    for (i = 0; i<LINES; i++)
      printf("%d \n",message[i]);
  } 
  MPI_Finalize();
  return 0;
} 
