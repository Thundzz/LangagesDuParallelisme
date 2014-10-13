CC=gcc
CFLAGS=-W -Wall -O2
LDFLAGS= -pthread
EXEC=quicksort.out

all: $(EXEC)

quicksort.out: quicksort.c
	$(CC) -o $@ -c $< $(CFLAGS)
par.out: quicksort_par.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)


%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
	
clean:
	rm -rf *.o *~ *# *.out
