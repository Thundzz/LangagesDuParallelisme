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

	int nb_proc = atoi(argv[1]);
	int nb_thread = atoi(argv[2]);
	int max_lgth = atoi(argv[4]);
	char * alphabet = argv[3];
	char * password = argv[5];

	int alphabet_size = strlen(alphabet);

	int *table = alloc_table(alphabet);
	char *rev_table = alloc_rev_table(alphabet);

	encoding_table(alphabet, table, rev_table);
	#ifdef DEBUG
		int e = encode(table, password, alphabet_size);
		char d[100];
		fprintf(stderr, "Encodage  de : %s, %d \n", password, e);
		decode(rev_table, e, alphabet_size, d);
	#endif

	int myrank, result, work =42;
	int nb_slaves = nb_proc -1;
	MPI_Status status; 
	MPI_Comm slaves;
	MPI_Init( NULL, NULL );
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	MPI_Comm_spawn("slave.out", argv, nb_slaves,
				 MPI_INFO_NULL, 0, MPI_COMM_SELF, &slaves, MPI_ERRCODES_IGNORE);

	for (int i=0; i < nb_slaves; ++i){
		work = work +1;
		MPI_Send(&work, 1 ,MPI_INT, i, 99, slaves);
	} 
	for (int i=0; i < nb_slaves; ++i){
		MPI_Recv(&result, 1, MPI_INT, i, MPI_ANY_TAG, slaves, &status);
		printf("resultat : %d\n", result);
	}

	free(table);
	free(rev_table);
	MPI_Finalize();
	return 0;
}