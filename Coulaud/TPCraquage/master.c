#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <string.h>
#include <mpi.h>
#include "shared.h"

#define EXECNAME "./master"
#define DEFAULT_PROC_NUM 2
#define MIN_SLICE 1024

void pack_msg(unsigned long long *msg, unsigned long long debut, unsigned long long fin, unsigned long long length)
{
	msg[0]= debut;
	msg[1]= fin;
	msg[2]= length;
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
	unsigned max_lgth = atoi(argv[4]);
	char * alphabet = argv[3];
	int a_size = strlen(alphabet);
	int nb_slaves = nb_proc -1;
	unsigned long long msg[MSG_SIZE];

	if(!is_valid(alphabet)){
		fprintf(stderr, "Error: Alphabet is invalid. "
						"(Encountered multiple times the same character)\n");
		exit(EXIT_FAILURE);	
	}

	int myrank, found=0;
	MPI_Status st;
	MPI_Request req;
	MPI_Comm Comm_slaves;
	MPI_Init( NULL, NULL );
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_spawn("slave.out", argv, nb_slaves, MPI_INFO_NULL,
				  0, MPI_COMM_SELF, &Comm_slaves,MPI_ERRCODES_IGNORE);
	int * nb_tasks = (int*) malloc(nb_proc *sizeof(int));
	/* For words of increasing length */
	for (unsigned int length = 1; length <= max_lgth; length++)
	{	
		nb_tasks = memset(nb_tasks, 0,nb_proc *sizeof(int));
		unsigned long long last_task= pow(a_size, length)-1;
		unsigned long long debut= 0, fin = 0;
		int proc = 0, workers = 0;
		long slice = last_task / nb_proc;
		slice = MAX(slice, MIN_SLICE);

		/*Send a slice to each process */
		while(true)
		{
			debut= fin;
			fin += slice-1;
			fin = MIN(fin, last_task);
			pack_msg(msg, debut, fin, (unsigned long long) length);
			#ifdef DEBUG
				fprintf(stderr, "Envoi de %ld-%ld (taille %ld) "
								"à: %d\n", msg[0], msg[1], msg[2], proc);
			#endif
			MPI_Isend(&msg, MSG_SIZE, MPI_UNSIGNED_LONG_LONG, proc, TASK_TAG,
					  Comm_slaves, &req);

			workers= MIN(workers +1, nb_proc-1);
			nb_tasks[proc] += 1;
			proc = (proc +1) % (nb_proc-1);
			if (fin == last_task) break;
		}
		/* Wait until all the workers have finished processing this length */
		for (int proc = 0; proc < workers; ++proc)
		{
			for (int i = 0; i < nb_tasks[proc]; ++i)
			{
				MPI_Recv(msg, MSG_SIZE, MPI_UNSIGNED_LONG_LONG, proc, TASK_FINISHED_TAG,
				 Comm_slaves, &st);
				if(msg[0] == SUCCESS)
				{
					fprintf(stderr, ANSI_COLOR_GREEN
						"Process %d found the password !\n"
						ANSI_COLOR_RESET, myrank);
					found = 1;
				}
			}
		}
		if(found) break;
	}
	/* Tell all slave processes that it's the end. */
	for (int proc = 0; proc < nb_proc-1; ++proc)
	{
		MPI_Isend(&msg, MSG_SIZE, MPI_UNSIGNED_LONG_LONG, proc, END_TAG,
					  Comm_slaves, &req);
	}
	/* Make sure that all slaves have answered with an end tag.*/
	for (int proc = 0; proc < nb_proc-1; ++proc)
	{
		MPI_Recv(msg, MSG_SIZE, MPI_UNSIGNED_LONG_LONG, proc, END_TAG, Comm_slaves, &st);
	}
	if(!found)
	{
		printf(ANSI_COLOR_RED"Could not find the password.\n"ANSI_COLOR_RESET);
	}
	MPI_Finalize();
	return 0;
}