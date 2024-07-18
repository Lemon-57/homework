#include <stdio.h>
#include <func.h>
#include <tgmath.h>
#include <math.h>

#define NUM 2 + 3

int main()
{
    printf("atan2(0, 0) : %e\n", atan2(0, 0));
    // CU_ASSERT_DOUBLE_EQUAL(atan2(0, 0), pi/2, 0.0001);

    printf("atan2(-pi/2, 0) : %e\n", atan2(-M_PI / 2, 0));
    // CU_ASSERT_DOUBLE_EQUAL(atan2(-pi/2, 0), -pi/2, 0.0001);

    printf("atan2(pi/2, pi/2) : %e\n", atan2(M_PI / 2, M_PI / 2));
    printf("atan2(-pi/2, pi/2) : %e\n", atan2(-M_PI / 2, M_PI / 2));
    printf("atan2(0, -pi/2) : %e\n", atan2(0, -M_PI / 2));
    printf("atan2(pi/2, -pi/2) : %e\n", atan2(M_PI / 2, -M_PI / 2));
    return 0;
}
