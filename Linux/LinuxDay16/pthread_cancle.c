#include <func.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <error.h>
#include <stdlib.h>

int main(void)
{
    DIR *dir;
    struct dirent *ptr;
    char *dirname = "/test/rewinddir_1";
    mkdir(dirname, 0777);
    dir = opendir("/test/rewinddir_1");
    while ((ptr = readdir(dir)) != NULL)
    {
        printf("d_name: %s\n", ptr->d_name);
    }
    rewinddir(dir);
    printf("readdir again !\n");
    while ((ptr = readdir(dir)) != NULL)
    {
        printf("d_name: %s\n", ptr->d_name);
    }
    closedir(dir);

    // The calendar time 1635386400 is converted into data and time is Thu  2021.10.28 02:00:00.
    // const time_t *restrict timer;
    // time_t cal_time = 1635386400;
    // timer = &cal_time;
    // struct tm *restrict result = (struct tm *)malloc(80);
    // struct tm *ret;

    // ret = localtime_r(timer, result);
    // printf("%d年%d月%d日%d时%d分%d秒\n", result->tm_year + 1900, result->tm_mon + 1, result->tm_mday, result->tm_hour, result->tm_min, result->tm_sec);
    // puts("\n");

    // i = mktime(p); //将struct tm结构体反转，变为自1970年到现在经过的秒数
    // printf("将struct tm反转为1970年到现在经过的秒数\n");
    // printf("i=%ld\n", i);

    // time_t cal_time = 1635386400;
    // const time_t *clock;
    // clock = &cal_time;

    // char *ret, *buf = (char *)malloc(26);
    // char *string_time_i = "Thu Oct 28 00:00:00 2021";
    // char *string_time_j = "Thu Oct 28 00:00:00 2021";

    // // The time of Thu 00:00:00 2021.10.28 from UTC time is 51 years 9 months 3 weeks and 6 days,a total of 18928 days that is 1635379200 seconds (Thu 00:00:00 2021.10.28 ).
    // int i = 0, j = 0;
    // ret = ctime_r(clock, buf);
    // printf("buf = %s", buf);
    // time_t Nowt = 0;
    // time_t i = 0;
    // char *buf = NULL;
    // char Buf[50];
    // struct tm *p; // struct tm变量
    // char outstr[200];

    // i = time(&Nowt); //自1970来到现在一共过去了几秒
    // printf("1970到目前经历的秒数为:\n");
    // printf("i=%ld\n", i);
    // puts("\n");

    // printf("1970到目前经历的秒数转成成字符串格式显示:\n");
    // buf = ctime(&Nowt); //得到一个可视化时间字符串
    // printf("buf=%s", buf);
    // ctime_r(&Nowt, Buf); //可重入的版本，指定输出的位置，而不是内核随机位置输出
    // printf("Buf=%s", Buf);
    // puts("\n");

    // p = localtime(&Nowt); //获取本地时区的时间
    // printf("1970到目前经历的秒数通过localtime转换成struct tm结构体时间:\n");
    // printf("%d年%d月%d日%d时%d分%d秒\n", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec); // 1900年为基准，月份是0-11月
    // localtime_r(&Nowt, p);                                                                                                //可重入的版本，指定输出的位置，而不是内核随机位置输出
    // printf("%d年%d月%d日%d时%d分%d秒\n", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
    // puts("\n");

    // p = gmtime(&Nowt);
    // printf("1970到目前经历的秒数通过gmtime转换成struct tm结构体时间：\n");
    // printf("%d年%d月%d日%d时%d分%d秒\n", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec); //格林尼治时间值与北京时间小时相差8小时。
    // gmtime_r(&Nowt, p);                                                                                                   //可重入的版本，指定输出的位置，而不是内核随机位置输出
    // printf("%d年%d月%d日%d时%d分%d秒\n", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
    // puts("\n");

    // buf = asctime(p); //将struct tm时间结构体转换成字符串格式输出。
    // printf("将struct tm结构体时间转换成字符串格式时间显示：\n");
    // printf("Buf=%s", buf);
    // asctime_r(p, Buf); //可重入的版本，指定输出的位置，而不是内核随机位置输出
    // printf("Buf=%s", Buf);
    // puts("\n");

    // strftime(outstr, sizeof(outstr), "%F %j", p); //选择输出什么格式  %F 年-月-日  %j 十进制表示的每年的第几天
    // printf("将struct tm转换成特定格式的时间规则样式显示：\n");
    // printf("Result string is \"%s\"\n", outstr);
    // puts("\n");

    // i = mktime(p); //将struct tm结构体反转，变为自1970年到现在经过的秒数
    // printf("将struct tm反转为1970年到现在经过的秒数\n");
    // printf("i=%ld\n", i);

    return 0;
}
