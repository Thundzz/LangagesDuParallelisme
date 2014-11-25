#include <stdlib.h>
#include <stdio.h>

#include <omp.h>

int main()
{
	#pragma omp parallel
	{
		printf("Hello World I am thread %d\n",omp_get_thread_num());
	}
	return 0;
}