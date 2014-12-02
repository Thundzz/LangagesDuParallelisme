#include <stdlib.h>
#include <stdio.h>

#include <omp.h>

int main()
{

	int num, me;
	#pragma omp parallel
	{
		me = omp_get_thread_num();
		num = omp_get_num_threads();
		printf("Hello World I am thread %d\n", me);
	if(me ==0 )
		printf("I'm the first thread and there are %d other ones.\n", num);
	
	}
	return 0;
}