#ifndef SHARED_H
#define SHARED_H

#define MSG_SIZE  3

#define NOT_FOUND 404
#define SUCCESS 200

#define TASK_TAG 101
#define TASK_FINISHED_TAG 102
#define END_TAG 103

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

/* Allocates the memory for the encoding table
 */
int * alloc_table(char * alphabet);


/* Checks for the validity of the passed alphabet.
 */
int is_valid(char * alphabet);

/* Allocates the memory for the reverse encoding
 * table
 */
char * alloc_rev_table(char* alphabet);

/* Allocates and computes an encoding
 * table for the passed alphabet
 */
int encoding_table(char *alphabet, int *table, char *rev_table);

/* Encodes a word using the table passed as argument.
 * The returned int is the encoded value of the word.
 */
int encode(int* table, char*  word, int alphabet_size);


/* Decodes the integer contained in the variable encoded.
 * The decoded word is written at the address pointed by
 * the variable named decoded.
 */
void decode(char* rev_table, int encoded, int alphabet_size, char*  decoded, int length);

#endif