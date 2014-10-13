CC=mpicc
CFLAGS=-W -Wall -O2
EXEC=

all: $(EXEC)

%.out: %.o
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
	
clean:
	rm -rf *.o *~ *# *.out
