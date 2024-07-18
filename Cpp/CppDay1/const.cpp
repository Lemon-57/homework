#include <iostream>
#include "func.h"
#include <string>
using namespace std;

int main()
{
    int number1 = 10;
    int number2 = 20;
    const int *p1 = &number1;
    std::cout << "p1:"<< p1 << std::endl;
    //*p1 = 100;
    p1 = &number2;
    
    std::cout << "p1:"<< p1 << std::endl;
    int const *p2 = &number1;
    char *string = "111111";
    itoa(1, string, 1);
    int * const p3 = &number1;
    *p3 = 100;
    //p3 = &number2;
    std::cout << "*p3"<< *p3  << std::endl;
    return 0;
}

