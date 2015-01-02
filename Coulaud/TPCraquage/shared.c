#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "shared.h"

int * alloc_table(char * alphabet)
{
	char m = alphabet[0];
	int size = strlen(alphabet);
	for (int i = 0; i < size; ++i)
	{
		m = MAX(m, alphabet[i]);
	}
	int * table = (int*) malloc((1+m)*sizeof(int));
	memset((void*) table, -1, (1+m)*sizeof(int));
	return table;
}

char * alloc_rev_table(char * alphabet)
{
	int size = strlen(alphabet);
	char* rev_table= (char*) malloc((size+1)*sizeof(char));
	return rev_table;
}

void print_rev_table(char * rev_table, int alphabet_size)
{
	fprintf(stderr, "rev_table\n");
	for (int i = 0; i < alphabet_size+1; ++i)
	{
		fprintf(stderr, "%d \n", rev_table[i]);
	}
}


int encoding_table(char *alphabet, int* table, char* rev_table)
{
	int size = strlen(alphabet);
	for (int i = 0; i < size; ++i)
	{
		int cur = (int) alphabet[i];
		if(table[cur] == -1)
		{
			table[cur] = i;
			rev_table[i] = (char) cur; 
		}
		else return 1;
	}
	return 0;
}
int is_valid(char *alphabet)
{
	int * table = alloc_table(alphabet);
	char * rev_table =alloc_rev_table(alphabet);
	int valid = (encoding_table(alphabet, table,rev_table) == 0);
	free(table);
	free(rev_table);
	return(valid);
}



int encode(int * table, char *  word, int alphabet_size){
	int size = strlen(word);
	int code = 0;
	for (int i = 0; i < size; ++i)
	{
		int current= (int) word[i];
		code +=  table[current]* pow(alphabet_size, i);
	}
	return code;
}

void decode(char* rev_table, int encoded, int alphabet_size, char*  decoded, int length){
	int q, r;
	int cur = encoded;
	int i = 0;
	while(cur != 0)
	{
		q = cur/ alphabet_size;
		r = cur % alphabet_size;
		decoded[i]= rev_table[r];
		cur = q;
		i++;
	}
	for (int j = 0; j < length-i; ++i)
	{
		decoded[i]= rev_table[0];
	}
	decoded[i]= '\0';
}