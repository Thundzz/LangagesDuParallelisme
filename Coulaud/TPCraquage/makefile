DEBUG= #-DDEBUG
CC=mpicc
CFLAGS=-W -Wall -O2 -std=c99 -lm -fopenmp
EXEC=master.out slave.out
TEST= test_shared.out
OBJS=shared.o queue.o
PBS=scatter.pbs

all: $(EXEC) $(TEST)

send:
	scp * formation:LangagesDuParallelisme/TPCraquage
sub:
	qsub $(PBS)
	
load:
	module add compiler/gcc/4.9.0 mpi/openmpi/1.8.1 

%.out: %.o $(OBJS)
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(DEBUG)
	

clean:
	rm -rf *.o *~ *# *.out *.[oe]* *.[oe][1-9]*
