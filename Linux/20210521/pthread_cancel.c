#include <func.h>

void * threadFunc(void* arg)
{
	//子线程这里没有cancel点
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
	usleep(1);
	ret = pthread_cancel(pthid);
	if(0 != ret){
		printf("******1*************************\n");
	}

	//回收子线程的资源
	pthread_join(pthid, NULL);

	printf("exit main()\n");
	return 0;
}
