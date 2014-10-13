#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)  {
  int myrank, size;

  MPI_Init(&argc,&argv); 
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

  /* printf("Hello world, je suis le processus %d.\n",myrank); */
  printf("Hello world, je suis le processus %d parmi %d.\n",myrank,size);
  
  MPI_Finalize();

}
