#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <mpi.h>
#include "shared.h"


#define EXECNAME "./master"

int main(int argc, char *argv[])
{
	if (argc != 6){
		fprintf(stderr, "Expecting syntax: "EXECNAME
			" nb_proc nb_thread alphabet max_lgth password\n");
		exit(EXIT_FAILURE);
	}

	int nb_proc = MAX(1, atoi(argv[1]));
	int max_lgth = atoi(argv[4]);
	char * alphabet = argv[3];
	int alphabet_size = strlen(alphabet);


	int myrank;
	int nb_slaves = nb_proc -1;
	MPI_Comm slaves;
	MPI_Init( NULL, NULL );
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	MPI_Comm_spawn("slave.out", argv, nb_slaves,
				 MPI_INFO_NULL, 0, MPI_COMM_SELF, &slaves, MPI_ERRCODES_IGNORE);

	for (int length = 0; length < max_lgth; ++length)
	{

	}

	MPI_Finalize();
	return 0;
}