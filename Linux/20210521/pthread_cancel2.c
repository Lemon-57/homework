 ///
 /// @file    pthread_cancel.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-05-21 10:59:42
 ///
 
#include<func.h>

void * threadFunc(void* arg)
{
	int * pnumber = (int*)malloc(sizeof(int));
	*pnumber = 10;
	while(1) {
		printf("sub thread is alive\n");
	}
}
 
int main(int argc, char *argv[])
{
	int ret; 
	pthread_t pthid;
	ret = pthread_create(&pthid, NULL, threadFunc, NULL);
	THREAD_ERROR_CHECK(ret, "pthread_create");

	printf("main thread %ld\n", pthread_self());
	sleep(1);
	ret = pthread_cancel(pthid);

	//回收子线程的资源
	pthread_join(pthid, NULL);

	printf("exit main()\n");
	return 0;
}
