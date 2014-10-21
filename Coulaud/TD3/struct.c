#include <stdio.h>
#include <string.h>
#include "mpi.h"

typedef struct { 
 double pos[3]; 
 double v[3]; 
 double f[3];
 double mass; 
 int globalID; 
 char name[4];
} Particle; 

int main() {
  int myrank; 
  MPI_Status status; 
  MPI_Init( NULL, NULL ); 
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank ); 

  MPI_Datatype type[6] = { MPI_DOUBLE /*, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_INT, MPI_CHAR */}; 
  MPI_Datatype Particletype;
  int blocklen[6] = { 3 /*, 3, 3, 1, 1, 4 */}; 
  MPI_Aint i1,i2 ; 
  MPI_Aint disp[6];
  Particle p;
  Particle recvd;
  p.pos[0] = 420;
  p.pos[1] = 2.0;
  p.pos[2] = 1.0;

  MPI_Get_address(&p, &i1); 
  MPI_Get_address(&p.pos[0], &i2); disp[0] = i2 - i1 ; 
/*
  MPI_Get_address(&p.v[0], &i2); disp[1] = i2 - i1 ; 
  MPI_Get_address(&p.f[0], &i2); disp[2] = i2-i1 ; 
  MPI_Get_address(&p.mass, &i2); disp[3] = i2-i1;
  MPI_Get_address(&p.globalID, &i2); disp[4] = i2-i1;
  MPI_Get_address(&p.name[0], &i2); disp[5] = i2-i1;
*/
  MPI_Type_struct(1, blocklen, disp, type, &Particletype); 
  MPI_Type_commit(&Particletype);

  if (myrank == 0)/* code for process zero */ { 
	fprintf(stderr, "Before send\n");
    MPI_Send(&p, 1, Particletype, 1, 99, MPI_COMM_WORLD);
    fprintf(stderr, "After send\n");
  } else if (myrank == 1) /* code for process one */ {
	fprintf(stderr, "Before recv\n");
  	MPI_Recv(&recvd, 1, Particletype, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  	fprintf(stderr, "After recv\n");	
    printf("%g \n",recvd.pos[0]);
    printf("%g \n",recvd.pos[1]);
    printf("%g \n",recvd.pos[2]);
  } 
  MPI_Type_free(&Particletype);	
  MPI_Finalize();
  return 0;
} 
