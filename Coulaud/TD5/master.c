#include <stdio.h>
#include <string.h>
#include "mpi.h"

#define SIZE 10000
int main() {
  int size =2 ; 
  int myrank; 
  MPI_Status status; 
  MPI_Comm all;           /* intercommunicateur */ 
  int work = 420;
  MPI_Init( NULL, NULL ); 
  int result;
	MPI_Comm_spawn("slave.out", MPI_ARGV_NULL, size,
						 MPI_INFO_NULL, 0, MPI_COMM_SELF, &all, MPI_ERRCODES_IGNORE);
	// TODO :Client/server

	for (int i=0; i < size; ++i){
		MPI_Send(&work, 1 ,MPI_INT, i, 99, all);
	} 
	for (int i=0; i < size; ++i){
		MPI_Recv(&result, 1, MPI_INT, i, MPI_ANY_TAG, all, &status);
		printf("resultat : %d\n", result);
	}

  MPI_Finalize();
  return 0;
}