#include <memory>
#include "MyRedis.h"

using std::unique_ptr;

void test(){
    unique_ptr<MyRedis> pRedis(new MyRedis());
    if(!pRedis->connect("127.0.0.1", 6379)){
        std::cerr << "connect error!" << endl;
        return;
    }
    pRedis->set("name", "manman");
    cout << "Get the name is " << pRedis->get("name") << endl;
}


int main()
{
    test();
    return 0;
}

