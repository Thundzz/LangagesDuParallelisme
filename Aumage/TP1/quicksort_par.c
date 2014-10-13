#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>


#define MAX 100
#define NBREPEAT 100
#define NBBENCHMARKS 10
#define LVLLIM 2
#define MAX_THREAD_NUM 10000
#define FAC 2

int thread_count = 0;

static pthread_mutex_t mutex_stock = PTHREAD_MUTEX_INITIALIZER;

struct args {
  int* a;
  int n;
  int level;
};

void args_init(struct args * args, int *a , int n, int level)
{
  args->a = a;
  args->n = n;
  args->level = level;
}

void quick_sort (struct args * args);

void incr_thread_count()
{
      pthread_mutex_lock(&mutex_stock);
      thread_count ++;
      pthread_mutex_unlock(&mutex_stock);
}

void decr_thread_count()
{
      pthread_mutex_lock(&mutex_stock);
      thread_count --;
      pthread_mutex_unlock(&mutex_stock);
}

void * thread_f(void * params)
{
  struct args * args = (struct args *) params;
  quick_sort(args);
  return 0;
}
void quick_sort (struct args * args) {
  int *a  = args->a;
  int n = args->n, level = args->level;
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
  struct args args1;
  struct args args2;
  args_init(&args1, a, r -a + 1 , level +1 );
  args_init(&args2, l, a + n - l, level +1 );
  if( thread_count < MAX_THREAD_NUM && level <LVLLIM ){
    pthread_t t1;
    pthread_t t2;
    pthread_create(&t1, NULL, thread_f, (void *) &args1);
    incr_thread_count();
    pthread_create(&t2, NULL, thread_f, (void *) &args2);
    incr_thread_count();
    pthread_join(t1, NULL);
    decr_thread_count();
    pthread_join(t2, NULL);
    decr_thread_count();
  }
  else
  {
    quick_sort(&args1);
    quick_sort(&args2);
  }

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
    a[i] = rand() % 10000;
  }
  struct args arguments;
  arguments.a = a;
  arguments.n = size;
  arguments.level = 0;
  quick_sort(&arguments);
  //print_array(size, a);
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
  FILE * output = fopen("output_par_max.dat", "w");
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
