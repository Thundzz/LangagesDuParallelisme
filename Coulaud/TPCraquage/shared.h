#ifndef SHARED_H
#define SHARED_H


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
char * alloc_rev_table();

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
void decode(char* rev_table, int encoded, int alphabet_size, char*  decoded);

#endif