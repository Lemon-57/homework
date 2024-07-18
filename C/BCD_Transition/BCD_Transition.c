// BCD转换
// 函数输入类型为十进制的INT、DINT、BYTE、WORD、DWORD、USINT、UINT、UDINT.输出类型BYTE/WORD/DWORD.
// 由于对有符号数，需使用四位来表示符号，剩余的位表示有效的0~9数字。对于无符号输入数，转换后全部表示有效数字
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

// INT: 有符号整型，2个字节
typedef int16_t INT;
// DINT: 有符号双整型，4个字节
typedef int32_t DINT;
// BYTE: 有符号整数, 1个字节
typedef uint8_t BYTE;
// WORD: 无符号整数, 2个字节
typedef uint16_t WORD;
// DWORD: 无符号整数, 4个字节
typedef uint32_t DWORD;
// USINT：无符号整数, 1个字节
typedef uint8_t USINT;
// UINT: 无符号整型，2个字节
typedef uint16_t UINT;
// UDINT: 无符号双整型，4个字节
typedef uint32_t UDINT;

void print_binary(uint32_t val, int bits);
WORD INT_TO_BCD(INT val);
DINT DINT_TO_BCD(DWORD val);
BYTE BYTE_TO_BCD(BYTE dec);
UINT WORD_TO_BCD(WORD dec);
DWORD DWORD_TO_BCD(DWORD dec);
BYTE USINT_TO_BCD(USINT dec);
WORD UINT_TO_BCD(UINT dec);
DWORD UDINT_TO_BCD(UDINT dec);

void test_INT_TO_BCD()
{
    // 测试正数
    assert(INT_TO_BCD(123) == 0x0123);
    assert(INT_TO_BCD(999) == 0x0999);

    // 测试负数
    assert(INT_TO_BCD(-123) == 0x1123);
    assert(INT_TO_BCD(-999) == 0x1999);

    // 测试零
    assert(INT_TO_BCD(0) == 0x0000);

    // 测试超过999的数字
    assert(INT_TO_BCD(1000) == 0x0000); // 限制了最大的数字为999，所以1000应该被转换为0
}

void test_DINT_TO_BCD()
{
    // 测试正数
    assert(DINT_TO_BCD(123) == 0x0000000123);
    assert(DINT_TO_BCD(999) == 0x0000000999);

    // 测试负数
    assert(DINT_TO_BCD(-123) == 0x1000000123);
    assert(DINT_TO_BCD(-999) == 0x1000000999);

    // 测试零
    assert(DINT_TO_BCD(0) == 0x0000000000);

    // 测试超过9999999的数字
    assert(DINT_TO_BCD(10000000) == 0x0000000000); // 限制了最大的数字为9999999，所以10000000应该被转换为0
}

void test_BYTE_TO_BCD()
{
    // 测试正数
    assert(BYTE_TO_BCD(12) == 0x12);
    assert(BYTE_TO_BCD(99) == 0x99);

    // 测试零
    assert(BYTE_TO_BCD(0) == 0x00);

    // 测试超过99的数字
    assert(BYTE_TO_BCD(100) == 0x00); // 限制了最大的数字为99，所以100应该被转换为0
}

void test_WORD_TO_BCD()
{
    // 测试正数
    assert(WORD_TO_BCD(123) == 0x0123);
    assert(WORD_TO_BCD(9999) == 0x9999);

    // 测试零
    assert(WORD_TO_BCD(0) == 0x0000);

    // 测试超过9999的数字
    assert(WORD_TO_BCD(10000) == 0x0000); // 限制了最大的数字为9999，所以10000应该被转换为0
}

void test_DWORD_TO_BCD()
{
    // 测试正数
    assert(DWORD_TO_BCD(123) == 0x0000000123);
    assert(DWORD_TO_BCD(99999999) == 0x99999999);

    // 测试零
    assert(DWORD_TO_BCD(0) == 0x0000000000);

    // 测试超过99999999的数字
    assert(DWORD_TO_BCD(100000000) == 0x0000000000); // 限制了最大的数字为99999999，所以100000000应该被转换为0
}

void test_USINT_TO_BCD()
{
    // 测试正数
    assert(USINT_TO_BCD(12) == 0x12);
    assert(USINT_TO_BCD(99) == 0x99);

    // 测试零
    assert(USINT_TO_BCD(0) == 0x00);

    // 测试超过99的数字
    assert(USINT_TO_BCD(100) == 0x00); // 限制了最大的数字为99，所以100应该被转换为0
}

void test_UINT_TO_BCD()
{
    // 测试正数
    assert(UINT_TO_BCD(123) == 0x0123);
    assert(UINT_TO_BCD(9999) == 0x9999);

    // 测试零
    assert(UINT_TO_BCD(0) == 0x0000);

    // 测试超过9999的数字
    assert(UINT_TO_BCD(10000) == 0x0000); // 限制了最大的数字为9999，所以10000应该被转换为0
}

void test_UDINT_TO_BCD()
{
    // 测试正数
    assert(UDINT_TO_BCD(123) == 0x0000000123);
    assert(UDINT_TO_BCD(99999999) == 0x99999999);

    // 测试零
    assert(UDINT_TO_BCD(0) == 0x0000000000);

    // 测试超过99999999的数字
    assert(UDINT_TO_BCD(100000000) == 0x0000000000); // 限制了最大的数字为99999999，所以100000000应该被转换为0
}



int main()
{
    test_INT_TO_BCD();

    return 0;
}

// uint8_t转换为二进制字符串
void print_binary(uint32_t val, int bits)
{
    for (int i = bits - 1; i >= 0; --i)
    {
        putchar((val & (1 << i)) ? '1' : '0');
    }
    putchar('\n');
}

// -999 ~ 999
// 将INT转换为BCD码(8421)
// 将int16_t转换为BCD码(8421) 返回类型为uint16_t
// WORD INT_TO_BCD(INT val)
WORD INT_TO_BCD(INT val)
{
    uint16_t bcd = 0;
    uint16_t sign = 0;
    if (val < 0)
    {
        sign = 1;   // 设置符号位为1表示负数
        val = -val; // 转换为正数
    }
    for (int i = 0; val != 0 && i < 3; ++i, val /= 10) // 限制最大的数字为999
    {
        uint16_t digit = val % 10;
        bcd |= digit << (i * 4);
    }
    bcd |= sign << 12; // 将符号位添加到最高的四位
    return bcd;
}

// -9999999 ~ 9999999
// 将DINT转换为BCD码(8421)返回类型为DWORD
// 将uint32_t转换为BCD码(8421)，再转换为int32_t返回，考虑符号位
// DWORD DINT_TO_BCD(DINT val)
DINT DINT_TO_BCD(DWORD val)
{
    uint32_t bcd = 0;
    uint32_t sign = 0;
    if (val < 0)
    {
        sign = 1;   // 设置符号位为1表示负数
        val = -val; // 转换为正数
    }
    for (int i = 0; val != 0 && i < 7; ++i, val /= 10) 
    {
        uint32_t digit = val % 10;
        bcd |= digit << (i * 4);
    }
    bcd |= sign << 28; // 将符号位添加到最高的四位
    return bcd;
}

// 0 ~ 99
// 将uint8_t转换为BCD码(8421) 返回类型为uint8_t
BYTE BYTE_TO_BCD(BYTE dec)
{
    uint8_t bcd = 0;
    for (int i = 0; dec != 0 && i < 2; ++i, dec /= 10) 
    {
        uint8_t digit = dec % 10;
        bcd |= digit << (i * 4);
    }
    return bcd;
}

// 0 ~ 9999
// 将uint16_t转换为BCD码(8421) 返回类型为uint16_t
UINT WORD_TO_BCD(WORD dec)
{
    uint16_t bcd = 0;
    for (int i = 0; dec != 0; ++i, dec /= 10)
    {
        uint16_t digit = dec % 10;
        bcd |= digit << (i * 4);
    }
    return bcd;
}

// 0 ~ 99999999
// 将uint32_t转换为BCD码(8421) 返回类型为uint32_t
DWORD DWORD_TO_BCD(DWORD dec)
{
    uint32_t bcd = 0;
    for (int i = 0; dec != 0; ++i, dec /= 10)
    {
        uint32_t digit = dec % 10;
        bcd |= digit << (i * 4);
    }
    return bcd;
}

// 0 ~ 99
BYTE USINT_TO_BCD(USINT dec)
{
    uint8_t bcd = 0;
    for (int i = 0; dec != 0; ++i, dec /= 10)
    {
        uint8_t digit = dec % 10;
        bcd |= digit << (i * 4);
    }
    return bcd;
}

// 0 ~ 9999
WORD UINT_TO_BCD(UINT dec)
{
    uint16_t bcd = 0;
    for (int i = 0; dec != 0; ++i, dec /= 10)
    {
        uint16_t digit = dec % 10;
        bcd |= digit << (i * 4);
    }
    return bcd;
}

// 0 ~ 99999999
DWORD UDINT_TO_BCD(UDINT dec)
{
    uint32_t bcd = 0;
    for (int i = 0; dec != 0; ++i, dec /= 10)
    {
        uint32_t digit = dec % 10;
        bcd |= digit << (i * 4);
    }
    return bcd;
}
