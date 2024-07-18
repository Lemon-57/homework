#include <func.h>

#define QueSize 10

typedef struct shareResource
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int front;
    int rear;
    int data[QueSize];
} block_que_t;

void resource_init(block_que_t *pres)
{
    int ret;
    ret = pthread_mutex_init(&pres->mutex, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_init");

    ret = pthread_cond_init(&pres->cond, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_cond_init");

    pres->front = pres->rear = 0;
}

void resource_destory(block_que_t *pres)
{
    int ret;
    ret = pthread_mutex_destroy(&pres->mutex);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_destroy")

    ret = pthread_cond_destroy(&pres->cond);
    THREAD_ERROR_CHECK(ret, "pthread_cond_destroy");
}

int isfull(block_que_t *pres)
{
    if ((pres->rear + 1) % QueSize == pres->front)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int isempty(block_que_t *pres)
{
    if (pres->rear == pres->front)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void push(block_que_t *pres, int num)
{
    if (!isfull(pres))
    {
        pres->data[pres->rear++] = num;
        pres->rear %= QueSize;
    }
    else
    {
        printf("que is full");
    }
}

void pop(block_que_t *pres)
{
    if (!isempty(pres))
    {
        pres->front++;
        pres->front %= QueSize;
    }
    else
    {
        printf("que is empty");
    }
}

void *producer(void *arg)
{
    sleep(1);
    block_que_t *p = (block_que_t *)arg;
    srand(time(NULL));
    int ret;
    int res = 10;
    int number;
    do
    {
        ret = pthread_mutex_lock(&p->mutex);
        THREAD_ERROR_CHECK(ret, "pthread_mutex_lock");

        if (isfull(p))
        {
            pthread_cond_wait(&p->cond, &p->mutex);
        }
        number = rand() % 10;
        push(p, number);
        printf("product number:%d\n", number);

        pthread_cond_broadcast(&p->cond);
        THREAD_ERROR_CHECK(ret, "pthread_cond_broadcast");

        pthread_mutex_unlock(&p->mutex);
        THREAD_ERROR_CHECK(ret, "pthread_mutex_unlock");
    } while (--res);
    pthread_exit(NULL);
}

void *consumer(void *arg)
{
    block_que_t *p = (block_que_t *)arg;
    int ret;
    int res = 20;
    int number;
    int consumtotal = 0;
    do
    {
        pthread_mutex_lock(&p->mutex);
        THREAD_ERROR_CHECK(ret, "pthread_mutex_lock");
        if (isempty(p))
        {
            pthread_cond_wait(&p->cond, &p->mutex);
        }
        number = p->data[p->front];
        pop(p);
        consumtotal++;
        printf("consum number:%d\n", number);

        pthread_cond_broadcast(&p->cond);
        THREAD_ERROR_CHECK(ret, "pthread_cond_broadcast");

        pthread_mutex_unlock(&p->mutex);
        THREAD_ERROR_CHECK(ret, "pthread_mutex_unlock");
    } while (--res);
    printf("consumtotal:%d\n", consumtotal);
    pthread_exit(NULL);
}

int main()
{
    int ret;
    block_que_t sharedRes;
    resource_init(&sharedRes);

    pthread_t pthidproduct1, pthidproduct2, pthidconsum;
    ret = pthread_create(&pthidproduct1, NULL, producer, &sharedRes);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    ret = pthread_create(&pthidproduct2, NULL, producer, &sharedRes);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    ret = pthread_create(&pthidconsum, NULL, consumer, &sharedRes);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    ret = pthread_join(pthidproduct1, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_join");

    ret = pthread_join(pthidproduct2, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_join");

    ret = pthread_join(pthidconsum, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_join");

    resource_destory(&sharedRes);
    return 0;
}
