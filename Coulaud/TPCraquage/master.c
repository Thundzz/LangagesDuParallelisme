#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <math.h>

#define EXECNAME "./master"
 #define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

int* encoding_table(char *alphabet)
{
	int size = strlen(alphabet);
	char m = alphabet[0];
	for (int i = 0; i < size; ++i)
	{
		m = MAX(m, alphabet[i]);
	}
	int * table = (int *) malloc((1+m)*sizeof(int));
	memset((void*) table, -1, (1+m)*sizeof(int));
	for (int i = 0; i < size; ++i)
	{
		if(table[alphabet[i]] == -1)
		{
			table[alphabet[i]] = i+1;
		}
		else
		{
			fprintf(stderr, "Alphabet is invalid. (several times the same character)\n");
			return 0;
		}
	}
	return table;
}

int encode(int * table, char *  word, int alphabet_size){
	int size = strlen(word);
	int code = 0;
	for (int i = 0; i < size; ++i)
	{
		code +=  table[word[i]]* pow(alphabet_size, i);
	}
	return code;
}

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

	int* table = encoding_table(alphabet);

	fprintf(stderr, " Encodage  de : %s, %d \n", password, encode(table, password, alphabet_size) );
	int size =2 ; 
	int myrank; 
	MPI_Status status; 
	MPI_Comm all;
	int work = 420;
	MPI_Init( NULL, NULL ); 
	int result;
	/*MPI_Comm_spawn("slave.out", MPI_ARGV_NULL, size,
				 MPI_INFO_NULL, 0, MPI_COMM_SELF, &all, MPI_ERRCODES_IGNORE);
	for (int i=0; i < size; ++i){
		MPI_Send(&work, 1 ,MPI_INT, i, 99, all);
	} 
	for (int i=0; i < size; ++i){
		MPI_Recv(&result, 1, MPI_INT, i, MPI_ANY_TAG, all, &status);
		printf("resultat : %d\n", result);
	}*/
	free(table);
	MPI_Finalize();
	return 0;
}