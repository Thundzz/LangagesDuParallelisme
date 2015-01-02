#include "shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define EXECNAME "slave.out"

void unpack_msg(long * msg, long* start, long* end, long* length)
{
	*start = msg[0];
	*end = msg[1];
	*length = msg[2];
}

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
	MPI_Status st;
	MPI_Request req;

	int max_lgth = atoi(argv[5]);
	char * alphabet = argv[4];
	char * password = argv[6];
	int alphabet_size = strlen(alphabet);
	int *table = alloc_table(alphabet);
	char *rev_table = alloc_rev_table(alphabet);
	encoding_table(alphabet, table, rev_table);
	char * decoded = malloc(max_lgth* sizeof(char));
	long msg[MSG_SIZE];
	int finished=0;
	long first_task, last_task, length;
	while(!finished){
		MPI_Recv(msg, MSG_SIZE, MPI_LONG, 0, MPI_ANY_TAG, Comm_master, &st);
		if(st.MPI_TAG == TASK_TAG)
		{
			unpack_msg(msg, &first_task, &last_task, &length);

			for (int task = first_task; task <= last_task; ++task)
			{
				decode(rev_table, task, alphabet_size,  decoded, length);
				if(!strcmp(decoded, password))
				{
					fprintf(stderr, ANSI_COLOR_GREEN
						"I found the password !: %s\n"
						ANSI_COLOR_RESET, decoded);
				}
			}
			MPI_Isend(&msg, MSG_SIZE, MPI_LONG, 0, TASK_FINISHED_TAG,
					  Comm_master, &req);
		}
		else if(st.MPI_TAG == END_TAG)
		{
			finished = 1;
			MPI_Isend(&msg, MSG_SIZE, MPI_LONG, 0, END_TAG,
					  Comm_master, &req);
			#ifdef DEBUG
				fprintf(stderr, "Process n°%d received end"
				 				" message. Shutting down.\n", myrank);
			#endif
		}
	}
	free(decoded);
	free(table);
	free(rev_table);
	MPI_Finalize(); 
	return 0;
}