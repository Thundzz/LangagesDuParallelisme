
 #include <stdio.h>
#include <string.h>
#include "mpi.h"

#define SIZE 10000
int main() {
  int size; 
  int myrank; 
  MPI_Comm  grid2D, Comm1D; 
  MPI_Init( NULL, NULL ); 
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank); 
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  

  int dims[2];
  int periods[2];
  int coords[2];
  int subdivision[2];
  int reorder;
  int rang2D;
  int V[3];
  int W;

 	dims[0] = 3 ; dims[1] = 4 ; periods[0] = 0 ; periods[1] = 1 ; reorder = 1 ;
 	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &grid2D);

	MPI_Cart_coords(grid2D, myrank, 2, coords);
	MPI_Cart_rank(grid2D, coords, &rang2D);

 	if (coords[0] == 1) {
  	for(int i=0 ; i<3 ; ++i) {
  		V[i] = (int)(myrank) ; 
  	} 
	} 

	subdivision[0] = 1;
	subdivision[1] = 0; 

 MPI_Cart_sub (grid2D,subdivision,&Comm1D) ; 

	MPI_Scatter (V,1, MPI_INT ,&W,1, MPI_INT ,1,Comm1D); 
	printf("Rang %2d Coordonnées (%d,%d) : W %d.\n",myrank,coords[0],coords[1],W); 

  MPI_Finalize();
  return 0;
}