 ///
 /// @file    pthread_cancel.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-05-21 10:59:42
 ///
 
#include <func.h>

void free1(void * p)
{
	free(p);
	printf("free p1\n");
}


void * threadFunc(void* arg)
{
	int * pnumber = (int*)malloc(sizeof(int));
	pthread_cleanup_push(free1, pnumber);
	*pnumber = 10;
	printf("sub thread is alive\n");
	/* return NULL; */
	pthread_exit((void*)1);

	pthread_cleanup_pop(1);
}
 
int main(int argc, char *argv[])
{
	int ret; 
	pthread_t pthid;
	ret = pthread_create(&pthid, NULL, threadFunc, NULL);
	THREAD_ERROR_CHECK(ret, "pthread_create");

	printf("main thread %ld\n", pthread_self());

	//回收子线程的资源
	pthread_join(pthid, NULL);

	printf("exit main()\n");
	return 0;
}
