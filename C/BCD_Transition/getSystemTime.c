#include <time.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    bool ENO;
    struct tm OUT;
    uint16_t OUT_Y;
    uint8_t OUT_M;
    uint8_t OUT_D;
    uint8_t OUT_H;
    uint8_t OUT_MI;
    uint8_t OUT_S;
    uint32_t OUT_NS;
    uint32_t Status;
} GetClockOutput;

GetClockOutput GetClock(bool EN)
{
    GetClockOutput output = {0};

    if (EN)
    {
        struct timespec ts;
        struct tm *timeinfo;

        if (clock_gettime(CLOCK_REALTIME, &ts) == 0)
        {
            timeinfo = localtime(&ts.tv_sec);

            if (timeinfo != NULL)
            {
                output.ENO = true;
                output.OUT = *timeinfo;
                output.OUT_Y = timeinfo->tm_year + 1900;
                output.OUT_M = timeinfo->tm_mon + 1;
                output.OUT_D = timeinfo->tm_mday;
                output.OUT_H = timeinfo->tm_hour;
                output.OUT_MI = timeinfo->tm_min;
                output.OUT_S = timeinfo->tm_sec;
                output.OUT_NS = ts.tv_nsec; // 纳秒级别的时间精度
                output.Status = 0x0000;     // 读取正常
            }
            else
            {
                output.Status = 0x0001; // 时间读取异常
            }
        }
        else
        {
            output.Status = 0x0002; // clock_gettime调用失败
        }
    }

    return output;
}


int main(int argc, char *argv[])
{
    GetClockOutput output = GetClock(true);

    if (output.ENO)
    {
        printf("Current time: %d-%02d-%02d %02d:%02d:%02d.%09d\n", output.OUT_Y, output.OUT_M, output.OUT_D, output.OUT_H, output.OUT_MI, output.OUT_S, output.OUT_NS);
    }
    else
    {
        printf("GetClock failed\n");
    }

    return 0;
}