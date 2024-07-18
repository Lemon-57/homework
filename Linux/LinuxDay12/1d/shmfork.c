#include <func.h>

//int flag[2]={0};提供两个进程的状态
//int turn = 0;//表示哪个进程应该进入 0代表进程1有权限 1代表进程2有权限
int main()
{
    int shmid = shmget(1000,4096,0666|IPC_CREAT);
    ERROR_CHECK(shmid,-1,"shmget");
    int *p = (int*)shmat(shmid,NULL,0);
    ERROR_CHECK(p,(int*)-1,"shmget");
    p[0] = 0;
    p[1] = 0;//flag[0]
    p[2] = 0;//flag[1]
    p[3] = 0;//turn
    pid_t pid = fork();
    if(pid == 0)
    {
        while(1)
        {
            p[1] = 1;//进程1想使用临界区
            while(p[2])//进程2也想用
            {
                if(p[3] == 1)//如果进程2想使用，查看进程2是否有进入权限
                {
                    p[1] = 0;//如果进程2有权限，进程1取消使用请求
                    while(p[3] == 1);//进程2使用完后
                    p[1] = 1;//进程1发出请求
                }
            }
            //临界区
            for(int i = 0;i < 10000000; ++i)
            {
                ++p[0];
            }
            p[1] = 0;//进程1用完了
            p[3] = 1;//将权限交给进程1
            exit(0);
        }
    }
    else{
        while(1)
        {
            p[2] = 1;//进程2想使用进程
            while(p[1] == 1)//检查进程1也想用
            {
                if(p[3] == 0)//检查进程1是否有使用权限
                {
                    p[2] = 0;//如果进程1有权限，进程2取消使用
                    while(p[3] == 0);//进程1使用完后
                    p[2] = 1;//进程
                }
            }
            for(int i = 0;i < 10000000; ++i)
            {        
                ++p[0];       
            }
        p[3] = 0;//访问完全将权限给进程1
        p[2] = 0;//进程2使用完毕
        wait(NULL);
        printf("total = %d\n", p[0]);
        exit(0);
        }
    }
    return 0;
}

