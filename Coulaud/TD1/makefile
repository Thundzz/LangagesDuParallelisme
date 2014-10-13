CC=gcc
CFLAGS=-W -Wall -O2
LDFLAGS= -pthread
EXEC=

all: $(EXEC)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
	
clean:
	rm -rf *.o *~ *# *.out
