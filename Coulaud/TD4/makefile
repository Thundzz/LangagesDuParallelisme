CC=mpicc
CFLAGS=-W -Wall -O2
EXEC=pi_mpi.out
PBS=pi_mpi.pbs
all: $(EXEC)

send:
	scp * formation:LangagesDuParallelisme/TD4
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
