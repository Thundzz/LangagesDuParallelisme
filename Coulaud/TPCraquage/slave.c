#include "shared.h"
#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <omp.h>

#define EXECNAME "slave.out"

int main(int argc, char* argv[])
{
	UNUSED(argc);
	int myrank;
	MPI_Init( NULL, NULL );
	MPI_Comm Comm_master; 
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_get_parent(&Comm_master);
	MPI_Status st;
	MPI_Request req;
	
	int nb_thread = MAX(DEFAULT_THREAD_NUM, atoi(argv[3]));
	char * alphabet = argv[4];
	char * password = argv[6];
	int alphabet_size = strlen(alphabet);

	int *table = alloc_table(alphabet);
	char *rev_table = alloc_rev_table(alphabet);
	Queue* tasks = queue_init(2);
	encoding_table(alphabet, table, rev_table);
	unsigned long long msg[MSG_SIZE];
	char decoded[BUFFER_SIZE];

	int finished= false;

	omp_lock_t queuelock;
	omp_init_lock(&queuelock);
	omp_set_dynamic(0);
	omp_set_num_threads(nb_thread);
	#pragma omp parallel sections shared(finished)
	{
		/* Communication section */
		#pragma omp section
		{
			while(!finished)
			{
				int res=0;
				MPI_Iprobe(0 , MPI_ANY_TAG, Comm_master, &res, &st);
				if(res)
				{
					unsigned long long first_task, last_task, length;
					MPI_Recv(msg, MSG_SIZE, MPI_UNSIGNED_LONG_LONG, 0, 
							MPI_ANY_TAG, Comm_master, &st);
					if(st.MPI_TAG == TASK_TAG)
					{
						unpack_msg(msg, &first_task, &last_task, &length);
						Task task = {
										.first= first_task,
									 	.last=  last_task,
									 	.length= length 
									};
						omp_set_lock(&queuelock);
						queue_push(tasks, task);
	    				omp_unset_lock(&queuelock);
					}
					else if(st.MPI_TAG == END_TAG)
					{
						finished = true;
						MPI_Isend(&msg, MSG_SIZE, MPI_UNSIGNED_LONG_LONG, 0,
						 		END_TAG, Comm_master, &req);
					}
				}
			}
		}
		/* Worker section */
		#pragma omp section
		{
			MPI_Request req_end;
			MPI_Status st_end;
			unsigned long long first_task, last_task, length;
			while(!finished)
			{
				if(queue_isEmpty(tasks))
					continue;
				else
				{
					omp_set_lock(&queuelock);
					Task t = queue_pop(tasks);
					omp_unset_lock(&queuelock);
					first_task = t.first;
					last_task = t.last;
					length = t.length;
					unsigned long long cur;
					#pragma omp parallel for
					for (cur = first_task; cur <= last_task; ++cur)
					{
						if(finished) continue;
						decode(rev_table, cur, alphabet_size, decoded, length);
						if(!strcmp(decoded, password))
						{	
							fprintf(stderr, ANSI_COLOR_GREEN
								"I (Proc. %d) found the password! It is: %s\n"
								ANSI_COLOR_RESET, myrank, decoded);
							msg[0] = SUCCESS;
							msg[1] = myrank;
							finished = true;
						}
					}
					MPI_Isend(msg, MSG_SIZE, MPI_UNSIGNED_LONG_LONG, 0,
							 TASK_FINISHED_TAG, Comm_master, &req_end);
					MPI_Wait(&req_end, &st_end);
				}
			}
		}
	}
	free(table);
	free(rev_table);
	MPI_Finalize(); 
	return 0;
}