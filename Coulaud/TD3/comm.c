#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main() {
	int myrank, newrank; 
	MPI_Init( NULL, NULL ); 
	MPI_Comm_rank( MPI_COMM_WORLD, &myrank ); 

	MPI_Comm comm, interComm1, interComm2;
	int color = myrank % 3;
	MPI_Comm_split( MPI_COMM_WORLD, myrank % 3, 1,  &comm);

	if(color ==1 )
	{
		MPI_Intercomm_create(comm, 0, MPI_COMM_WORLD, 0 , 69, &interComm1 );
		MPI_Intercomm_create(comm, 0, MPI_COMM_WORLD, 2, 69, &interComm2 ) ;
	}
	if(color ==1 || color ==0)
	{
		MPI_Comm_rank( interComm1, &newrank); 
		printf("I am process number %d in COMM_WORLD\nAnd number %d in interComm1\n\n", myrank, newrank);
	}
	MPI_Finalize();
	return 0;
} 
