#include <iostream>
#include <string>
using namespace std;

class User 
{
    public: User(const string &name) : _name(name) , _score(0) 
{

}
void consume(float delta) 
{ 
    cout << "User::consume()" << endl; 
    _score += delta; cout << ">> " << _name << " consume " << delta << endl; 
} 
    protected: string _name; 
    float _score; 
    };
class VipUser : public User 
{
    public: VipUser(const string &name) 
    : User(name) 
    ,_discount(1) 
    {
        
    }

int main()
{
    std::cout << "Hello world" << std::endl;
    return 0;
}

