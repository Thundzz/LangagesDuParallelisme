#include "shared.h"

#include <stdio.h>
#include <mpi.h>

int main()
{
	int myrank, result, work;
	MPI_Init( NULL, NULL );
	MPI_Status status;
	MPI_Comm Comm_master; 
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_get_parent(&Comm_master);

	MPI_Recv(&result, 1, MPI_INT, 1, MPI_ANY_TAG, Comm_master, &status);
	MPI_Send(&work, 1 ,MPI_INT, 0, 99, Comm_master);



	MPI_Finalize(); 
	return 0;
}