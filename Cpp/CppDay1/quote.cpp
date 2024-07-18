#include <iostream>

int main()
{
    static int hot = 200;
    int &rad = hot;
    hot = hot + 100;
    std::cout << rad << std::endl;
    return 0;
}

