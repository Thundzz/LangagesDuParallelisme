#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

#define MAX_TESTING_LEN 100

void test_word(char * word, char * alphabet)
{
	int alphabet_size = strlen(alphabet);

	int * table = alloc_table(alphabet);
	char * rev_table =alloc_rev_table(alphabet);
	encoding_table(alphabet, table, rev_table);

	char * w1 = word;
	int w1enc = encode(table, w1, alphabet_size);
	char w1dec[MAX_TESTING_LEN];
	decode(rev_table, w1enc ,alphabet_size, w1dec, strlen(w1));
	char * color = ANSI_COLOR_RED;
	if(!strcmp(w1, w1dec))
	{
		color = ANSI_COLOR_GREEN;
	}
	fprintf(stderr, "%sEncoded %s into %d which was decoded into : %s\n"
					ANSI_COLOR_RESET,
					color, w1, w1enc, w1dec );

	free(table);
	free(rev_table);
}

int main()
{

	test_word("d", "abcd");
	test_word("aaaa", "abcd");
	test_word("abcd", "abcd");
	test_word("a", "abcd");
	test_word("b", "abcd");
	test_word("c", "abcd");
	test_word("d", "abcd");
	test_word("aa", "abcd");
	test_word("ab", "abcd");
	test_word("ac", "abcd");
	test_word("ad", "abcd");
	test_word("ba", "abcd");
	test_word("bb", "abcd");

	return 0;
}