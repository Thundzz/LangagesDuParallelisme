#include <stdio.h>
#include <string.h>
#include "mpi.h"

#define SIZE 10000
#define LINES 5
#define COLUMNS 5

int main() {
  int i,j;
  int m[LINES*COLUMNS];
  int message[LINES*COLUMNS]; 
  int myrank; 
  MPI_Status status; 
  MPI_Request request;
  MPI_Init( NULL, NULL );
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank ); 
  
  MPI_Datatype diag;
  MPI_Datatype trsup;
  int trisup = 2;
  for(i = 0; i<LINES; i++){
    for(j=0; j<COLUMNS; j++){
      if(i==j)
        m[i*COLUMNS+j] = 1;
      else if(j>i)
        m[i*COLUMNS+j] = trisup++;
      else
        m[i*COLUMNS+j] = 0;
      printf("%d ", m[i*COLUMNS+j]);
    }
    printf("\n");
  }
    
  int size = 0;
  int a_of_b[LINES];
  int a_of_d[LINES];
  for (i = 0; i<LINES; i++){
    a_of_b[i] = LINES - i - 1; 
    size += a_of_b[i];
    a_of_d[i] = i*LINES + i +1;
  }
  

  MPI_Type_indexed(LINES, a_of_b, a_of_d,  MPI_INT, &trsup);
  MPI_Type_vector(LINES, 1, COLUMNS+1, MPI_INT, &diag);
  MPI_Type_commit(&diag);  
  MPI_Type_commit(&trsup);
  
  if (myrank == 0)/* code for process zero */ {
    MPI_Isend(m, 1, trsup, 1, 99, MPI_COMM_WORLD, &request );
    MPI_Wait(&request, &status);
    
  } else if (myrank == 1) /* code for process one */ {
    MPI_Irecv(message, size, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);
    for (i = 0; i<size; i++)
      printf("%d \n",message[i]);
  } 
  MPI_Finalize();
  return 0;
} 
