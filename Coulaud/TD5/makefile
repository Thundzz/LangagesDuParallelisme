CC=mpicc
CFLAGS=-W -Wall -O2 -std=c99
EXEC=comm1d.out
PBS=scatter.pbs
all: $(EXEC)

send:
	scp * formation:LangagesDuParallelisme/TD5
sub:
	qsub $(PBS)
	
load:
	module add compiler/gcc/4.9.0 mpi/openmpi/1.8.1 

%.out: %.o
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
	

clean:
	rm -rf *.o *~ *# *.out *.[oe]*

cleanr:
	rm -rf *.[oe][1-9]*
