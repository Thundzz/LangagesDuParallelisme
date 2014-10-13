#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>



#define MAX 100
#define NBREPEAT 100
#define NBBENCHMARKS 10
#define FAC 2


void quick_sort (int *a, int n) {
  if (n < 2)
    return;
  int p = a[n / 2];
  int *l = a;
  int *r = a + n - 1;
  while (l <= r) {
    if (*l < p) {
      l++;
    }
    else if (*r > p) {
      r--;
    }
    else {
            int t = *l;
            *l = *r;
            *r = t;
            l++;
            r--;
    }
  }
  quick_sort(a, r - a + 1);
  quick_sort(l, a + n - l);
}

void print_array(int size, int * a)
{
  int i;
  for(i=0; i < size; i++ ){
    printf("%d ", a[i]);
  }
}

void gen_and_sort(int size)
{
  int i;
  int * a = (int *) malloc(size*sizeof(int));
  for (i = 0; i< size; i++)
  {
    a[i] = rand();
  }
  quick_sort(a, size);
  // print_array(size, a);
  free(a);
}


void write_file(FILE* file, double * time)
{
  int i;
  int curr_size =1;
  for(i = 0; i < NBBENCHMARKS; i++)
  {
    curr_size *= FAC;
    fprintf(file, "%d %g\n", curr_size, time[i]);
  }
}

int main (void) {
  int  i, j;
  struct timeval debut, fin;
  double elapsed[NBBENCHMARKS];
  srand(time(NULL));
  FILE * output = fopen("output.dat", "w");
  int curr_size = 1;
  for(i =0;i < NBBENCHMARKS; i ++)
  {
    curr_size *= FAC;
    gettimeofday(&debut,0);
    for( j=0; j < NBREPEAT; j++)
    {
      gen_and_sort(curr_size);
    }
    gettimeofday(&fin, 0);

    elapsed[i] += (fin.tv_sec-debut.tv_sec)*1000000 + fin.tv_usec-debut.tv_usec;
  }
  write_file(output, elapsed);
  return 0;
}
 
