CFLAGS=-g -O3 -Wall -std=c99   -fopenmp -lm
CFLAGS2=-g -O3 -Wall -std=c99 -lm

all: helloworld.out pi_par.out Quicksort.out Quicksort_seq.out pi_par_old.out

Quicksort_seq.out: Quicksort.c
	$(CC) -o $@ $< $(CFLAGS2) $(LDFLAGS)

%.out: %.o
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
clean:
	-rm -rf *.o *SYM helloworld.out