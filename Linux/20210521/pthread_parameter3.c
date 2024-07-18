 ///
 /// @file    pthread_creaet.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-05-20 15:15:04
 ///
 
#include <func.h>

void * threadFunc(void * arg) //void * arg = (void*)number;
{	//一个子线程是拥有自己的独立的栈空间
	printf("sub thread is running\n");
	printf("sub thread id:%ld\n", pthread_self());

	int *pn = (int*)arg;//在子线程的栈空间
	printf("sub thread pn: %p\n", pn);
	printf("sub thread *pn: %d\n", *pn);

	return NULL;
}
 
int main(int argc, char *argv[])
{
	pthread_t pthid;
	int number = 100;
	int ret = pthread_create(&pthid, NULL, 
			threadFunc, (void*)&number);
	THREAD_ERROR_CHECK(ret, "pthread_create");
	printf("sub thread id:%ld\n", pthid);

	number = 1000;//main线程的栈空间
	printf("main thread number: %d\n", number);
	printf("main thread &number: %p\n", &number);
	pthread_join(pthid, NULL);
	return 0;
}
