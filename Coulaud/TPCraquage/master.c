#include "shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mpi.h>
#include <sys/time.h>

#define EXECNAME "./master"

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
		fprintf(stderr, "1 process is an invalid value. Defaulting to: %d\n",
			DEFAULT_PROC_NUM);
		nb_proc = DEFAULT_PROC_NUM;

	}
	int nb_thread = atoi(argv[2]);
	if(nb_thread <= 1)
	{
		fprintf(stderr, "1 thread is an invalid value. Defaulting to: %d\n",
			DEFAULT_THREAD_NUM);
		nb_thread = DEFAULT_THREAD_NUM;
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
    struct timeval tvalBefore, tvalAfter; 
  	gettimeofday (&tvalBefore, NULL);
	int myrank, found=false;
	unsigned long long total=0;
	MPI_Status st;
	MPI_Request req;
	MPI_Comm Comm_slaves;
	MPI_Init( NULL, NULL );
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_spawn("./slave.out", argv, nb_slaves, MPI_INFO_NULL,
				  0, MPI_COMM_SELF, &Comm_slaves,MPI_ERRCODES_IGNORE);
	int * nb_tasks = (int*) malloc(nb_proc *sizeof(int));
	/* For words of increasing length */
	int archimede = nb_slaves +1;
	for (unsigned int length = 1; length <= max_lgth; length++)
	{	
		nb_tasks = memset(nb_tasks, 0,nb_proc *sizeof(int));
		unsigned long long last_task= pow(a_size, length)-1;
		unsigned long long debut= 0, fin = 0;
		int proc = 0, workers = 0;
		unsigned long slice = MAX((last_task+2*nb_slaves)/nb_slaves,
								 (unsigned) MIN_SLICE);
		total+= last_task;
		/*Send a slice to each process */
		while(true)
		{
			debut= fin;
			fin += slice-1;
			fin = MIN(fin, last_task);
			pack_msg(msg, debut, fin, (unsigned long long) length);
			MPI_Isend(&msg, MSG_SIZE, MPI_UNSIGNED_LONG_LONG, proc, TASK_TAG,
					  Comm_slaves, &req);

			workers= MIN(workers +1, nb_slaves);
			nb_tasks[proc] += 1;
			proc = (proc +1) % (nb_proc-1);
			if (fin == last_task) break;
		}
		/* Wait until all the workers have finished processing this length */
		for (int proc = 0; proc < workers; ++proc)
		{
			for (int i = 0; i < nb_tasks[proc]; ++i)
			{
				MPI_Recv(msg, MSG_SIZE, MPI_UNSIGNED_LONG_LONG,
				 MPI_ANY_SOURCE, TASK_FINISHED_TAG, Comm_slaves, &st);
				if(msg[0] == SUCCESS)
				{
					archimede = msg[1];
					fprintf(stderr, ANSI_COLOR_GREEN
						"Process %d found the password !\n"
						ANSI_COLOR_RESET, archimede);
					found = true;
				}
				if(found) break;
			}
			if(found) break;	
		}
		if(found) break;
	}
	/* Tell all slave processes that it's the end. */
	for (int proc = 0; proc < nb_proc-1; ++proc)
	{
		if(proc == archimede) continue;
		MPI_Isend(&msg, MSG_SIZE, MPI_UNSIGNED_LONG_LONG, proc, END_TAG,
					  Comm_slaves, &req);
	}
	/* Make sure that all slaves have answered with an end tag.*/
	for (int proc = 0; proc < nb_proc-1; ++proc)
	{
		if(proc == archimede) continue;
		MPI_Recv(msg, MSG_SIZE, MPI_UNSIGNED_LONG_LONG, proc, END_TAG,
					 Comm_slaves, &st);
	}

	if(!found)
	{
		printf(ANSI_COLOR_RED
			   "Could not find the password.\n"
			   ANSI_COLOR_RESET);
	}
  	gettimeofday (&tvalAfter, NULL);
	long int timeus= ((tvalAfter.tv_sec - tvalBefore.tv_sec)*1000000L 
					+tvalAfter.tv_usec) - tvalBefore.tv_usec;
	double times = ((double)timeus)/1000000.0;
	double wps = (double)total/times;
    printf("%lld words processed in %g seconds (%g w/s)\n", total, times, wps);

#ifdef BENCH
	char tname[30], wpsname[30];
	sprintf(tname, "time_%d_%d_%d.dat", nb_proc,nb_thread, max_lgth);
	sprintf(wpsname, "wps_%d_%d_%d.dat", nb_proc,nb_thread, max_lgth);
	FILE* f = fopen(tname, "w");
	if(f != NULL)
		fprintf(f, "%ld", timeus);
	fclose(f);
	FILE* f2 = fopen(wpsname, "w");
	if(f2 != NULL)
		fprintf(f2, "%g", wps);
	fclose(f2);
#endif

	MPI_Finalize();
	return 0;
}