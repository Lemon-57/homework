#include <iostream>
using std::cout;
using std::endl;

int number = 1;
namespace wd
{
    int number = 10;
    namespace wh
    {
        int number = 100;
        void display()
        {
            cout << "wd::wh::display()" << endl;
        }
    } // end of namespace wh     
    void display(int number)
    {
        cout << "形参number = " << number << endl;
        cout << "wd命名空间中的number = " << wd::number << endl;
        cout << "wh命名空间中的number = " << wd::wh::number << endl;
    }
} // end of namespace wd

int main(void)
{
    // using namespace wd;
    // using wd::wh::display;
    // display(5);
    wd::wh::display();
    return 0;
}
