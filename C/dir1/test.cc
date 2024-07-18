#include <func.h>
#include <math.h>
using namespace std;

bool IsBigEndian();

static double pi = 3.1415926535897931160E+00; /* 0x400921FB, 0x54442D18 */

int main(int argc, char *argv[])
{
    short a = 0x1234;
    char b = *(char *)&a;
    printf("%x\n", &a);
    printf("%x\n", b);

    if (0x12 == b)
    {
        return true;
    }

    return false;
}
