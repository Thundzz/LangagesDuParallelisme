#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <string.h>
#include <mpi.h>
#include "shared.h"

#define EXECNAME "./master"
#define SLICE 1024

#define TASK_TAG 101

void pack_msg(long *msg, long debut, long fin)
{
	msg[0]= debut;
	msg[1]= fin;
}

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
	int a_size = strlen(alphabet);
	int nb_slaves = nb_proc -1;
	int debut= 0, fin= 0;
	long msg[2];

	if(! is_valid(alphabet)){
		fprintf(stderr, "Alphabet is invalid. (several times the same character)\n");
		exit(EXIT_FAILURE);	
	}

	int myrank;
	MPI_Request req;
	MPI_Comm slaves;
	MPI_Init( NULL, NULL );
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_spawn("slave.out", argv, nb_slaves,
				 MPI_INFO_NULL, 0, MPI_COMM_SELF, &slaves,MPI_ERRCODES_IGNORE);


	for (int length = 0; length < max_lgth; ++length)
	{	
		int last_task= pow(a_size, length);
		for (int proc = 0; proc < nb_proc; ++proc)
		{
			debut= fin;
			fin += SLICE-1;
			fin = MIN(fin, last_task);
			pack_msg(msg, debut, fin);
			#ifdef DEBUG
				fprintf(stderr, "Envoi de %ld-%ld à: %d\n", msg[0], msg[1], proc);
			#endif
			MPI_Isend(&msg, 2, MPI_LONG, proc, TASK_TAG,
					  MPI_COMM_WORLD, &req);
			if (fin == last_task) break;
		}
	}

	MPI_Finalize();
	return 0;
}