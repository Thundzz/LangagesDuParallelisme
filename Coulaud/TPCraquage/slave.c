#include "shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define EXECNAME "slave.out"

int main(int argc, char* argv[])
{
	#ifdef DEBUG
		if (argc != 7){
			fprintf(stderr, "Expecting syntax: "EXECNAME
				" nb_proc nb_thread alphabet max_lgth password\n");
			for (int i = 0; i < 7; ++i)
			{
				fprintf(stderr, "Argument %i: %s\n",i, argv[i]);
			}
			exit(EXIT_FAILURE);
		}
	#endif
	int myrank;
	MPI_Init( NULL, NULL );
	MPI_Comm Comm_master; 
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_get_parent(&Comm_master);

	int max_lgth = atoi(argv[5]);
	char * alphabet = argv[4];
	char * password = argv[6];
	int alphabet_size = strlen(alphabet);

	int *table = alloc_table(alphabet);
	char *rev_table = alloc_rev_table(alphabet);
	encoding_table(alphabet, table, rev_table);
	char * decoded = malloc(max_lgth* sizeof(char));

	int first_task = 0;
	int last_task = 5;
	for (int task = first_task; task <= last_task; ++task)
	{
		decode(rev_table, task, alphabet_size,  decoded);
		if(!strcmp(decoded, password))
		{
			// Found !!!
		}
	}
	free(decoded);
	free(table);
	free(rev_table);
	MPI_Finalize(); 
	return 0;
}