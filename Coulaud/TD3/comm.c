#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main() {
	int myrank, newrank, message; 
	MPI_Init( NULL, NULL ); 
	MPI_Comm_rank( MPI_COMM_WORLD, &myrank ); 

	if(myrank ==0)
	{
		message =100;
	}

	MPI_Comm comm, interComm1, interComm2;
	int color = myrank % 3;
	MPI_Comm_split( MPI_COMM_WORLD, myrank % 3, 1,  &comm);

	if(color== 0)
	{
		MPI_Intercomm_create(comm, 0, MPI_COMM_WORLD, 1 , 42, &interComm1 );
	}
	if(color== 1)
	{
		MPI_Intercomm_create(comm, 0, MPI_COMM_WORLD, 0 , 42, &interComm1 );
		MPI_Intercomm_create(comm, 0, MPI_COMM_WORLD, 2, 69, &interComm2 ) ;
	}
	if(color ==2)
	{
		MPI_Intercomm_create(comm, 0, MPI_COMM_WORLD, 1, 69, &interComm2 ) ;
	}
	if(color ==1 || color ==0)
	{
		MPI_Comm_rank( interComm1, &newrank); 
		//printf("I am process number %d in COMM_WORLD\nAnd number %d in interComm1\n\n", myrank, newrank);
	}

	if(color== 0)
	{
		MPI_Bcast(&message, 1, MPI_INT, 0, comm);
		if (myrank == 0)
			MPI_Bcast(&message, 1, MPI_INT, MPI_ROOT, interComm1);
		else
			MPI_Bcast(&message, 1, MPI_INT, MPI_PROC_NULL, interComm1);
	}

	if(color == 1)
	{
		MPI_Bcast( &message, 1, MPI_INT,0, interComm1) ;
		if (myrank == 1)
			MPI_Bcast(&message, 1, MPI_INT, MPI_ROOT, interComm2);
		else
			MPI_Bcast(&message, 1, MPI_INT, MPI_PROC_NULL, interComm2);
	}

	if(color== 2)
	{
		MPI_Bcast( &message, 1, MPI_INT,0, interComm2) ;
	}
	//MPI_Barrier(MPI_COMM_WORLD);

	printf("I am process number %d in COMM_WORLD\nThis is the message %d \n\n", myrank, message);


	MPI_Finalize();
	return 0;
} 
