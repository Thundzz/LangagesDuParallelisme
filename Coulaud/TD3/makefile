CC=mpicc
CFLAGS=-W -Wall -O2
EXEC=struct.out
PBS=struct.pbs
all: $(EXEC)

send:
	scp * formation:LangagesDuParallelisme/TD3
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
