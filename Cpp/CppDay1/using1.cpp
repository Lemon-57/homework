#include <iostream>

using std::cin;
using std::cout;
using std::endl;

void f2(int &x, int &y)
{
    int z = x;
    x = y;
    y = z;
}
void f3(int *x, int *y)
{
    int z = *x;
    *x = *y;
    *y = z;
}

int foo(int x, int y)
{
    if (x <= 0 || y <= 0)
    {
        return 1;
    }
    return 3 * foo(x - 1, y / 2);
}

int func(int x)
{
    int countx = 0;
    while (x)
    {
        countx++;
        x = x & (x - 1);
    }
    return countx;
}

int main()
{
    int x = 10, y = 26;
    cout << "x, y = " << x << "," << y << endl; // 10, 26
    f2(x, y);
    cout << "x, y = " << x << "," << y << endl; // 26, 10
    f3(&x, &y);
    cout << "x, y = " << x << "," << y << endl; // 10, 26
    x++;
    y--;
    f2(y, x);
    cout << "x, y = " << x << "," << y << endl; // 25, 11
    //
    //    cout << foo(3, 5)<< endl;//
    //
    //    cin >> x;//x = 5
    //    if(x++ > 5)//x = 5 没进if 但是++了 x = 6
    //    {
    //        cout << x << endl;
    //    }
    //    else
    //    {
    //        cout << x-- << endl;//x = 6 输出完 x = 5
    //    }
    //    cout << x <<endl;
    //
    //    int a[5] = {1, 2, 3, 4, 5};
    //    int *ptr = (int *)(&a + 1);//2
    //    printf("%d, %d\n", *(a + 1), *(ptr - 1));//2, 5
    //    int x = 9999;
    //    int countx = func(x);
    //    cout<<"countx:"<<countx<<endl;
    //    return 0;
    // for (int i = 0; -i < 20; i--)
    // {
    //     cout << "hello" << endl;
    // }
}
