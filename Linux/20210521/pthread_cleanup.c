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
	pthread_cleanup_pop(1);
}
 
int main(int argc, char *argv[])
{
	int ret; 
	pthread_t pthid;
	ret = pthread_create(&pthid, NULL, threadFunc, NULL);
	THREAD_ERROR_CHECK(ret, "pthread_create");

	printf("main thread %ld\n", pthread_self());
	sleep(1);
	pthread_cancel(pthid);

	//回收子线程的资源
	pthread_join(pthid, NULL);

	printf("exit main()\n");
	return 0;
}
