#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <string.h>
#include <mpi.h>
#include "shared.h"

#define EXECNAME "./master"
#define DEFAULT_PROC_NUM 2
#define SLICE 1024

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

	int nb_proc = atoi(argv[1]);
	if(nb_proc <= 1)
	{
		fprintf(stderr, "Proc num of 1 is an invalid value. Defaulting to: %d\n",
			DEFAULT_PROC_NUM);
		nb_proc = DEFAULT_PROC_NUM;
	}
	int max_lgth = atoi(argv[4]);
	char * alphabet = argv[3];
	int a_size = strlen(alphabet);
	int nb_slaves = nb_proc -1;
	int debut= 0, fin= 0;
	long msg[MSG_SIZE];

	if(!is_valid(alphabet)){
		fprintf(stderr, "Error: Alphabet is invalid. "
						"(Encountered multiple times the same character)\n");
		exit(EXIT_FAILURE);	
	}

	int myrank;
	MPI_Status st;
	MPI_Request req;
	MPI_Comm Comm_slaves;
	MPI_Init( NULL, NULL );
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_spawn("slave.out", argv, nb_slaves, MPI_INFO_NULL,
				  0, MPI_COMM_SELF, &Comm_slaves,MPI_ERRCODES_IGNORE);

	/* For words of increasing length */
	for (int length = 1; length < max_lgth; ++length)
	{	
		int last_task= pow(a_size, length);
		int workers = 0;
		/*Send a slice to each process */
		for (int proc = 0; proc < nb_proc-1; ++proc)
		{
			debut= fin+1;
			fin += SLICE-1;
			fin = MIN(fin, last_task-1);
			pack_msg(msg, debut, fin);
			#ifdef DEBUG
				fprintf(stderr, "Envoi de %ld-%ld à: %d\n", msg[0], msg[1], proc);
			#endif
			MPI_Isend(&msg, MSG_SIZE, MPI_LONG, proc, TASK_TAG,
					  Comm_slaves, &req);
			workers ++;
			if (fin == last_task-1) break;
		}
		/* Wait until all the workers have processing this length */
		for (int proc = 0; proc < workers; ++proc)
		{
			MPI_Recv(msg, MSG_SIZE, MPI_LONG, proc, TASK_FINISHED_TAG,
			 Comm_slaves, &st);
		}
	}
	/* Tell all slave processes that it's the end. */
	for (int proc = 0; proc < nb_proc-1; ++proc)
	{
		MPI_Isend(&msg, MSG_SIZE, MPI_LONG, proc, END_TAG,
					  Comm_slaves, &req);
	}
	/* Make sure that all slaves have answered with an end tag.*/
	for (int proc = 0; proc < nb_proc-1; ++proc)
	{
		MPI_Recv(msg, MSG_SIZE, MPI_LONG, proc, END_TAG, Comm_slaves, &st);
	}
	MPI_Finalize();
	return 0;
}