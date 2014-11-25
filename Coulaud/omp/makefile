CFLAGS=-g -O3 -Wall -std=c99   -fopenmp -lm

all: helloworld.out

%.out: %.o
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
clean:
	-rm -rf *.o *SYM helloworld.out