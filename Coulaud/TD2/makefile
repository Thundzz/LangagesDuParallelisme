CC=mpicc
CFLAGS=-W -Wall -O2
EXEC=matrix.out
PBS=matrix.pbs
all: $(EXEC)

sub:
	qsub $(PBS)
	
%.out: %.o
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
	
clean:
	rm -rf *.o *~ *# *.out *.[oe]*

cleanr:
	rm -rf *.[oe][1-9]*
