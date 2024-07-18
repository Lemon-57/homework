#include <nlohmann/json.hpp>
using Json = nlohmann::json;
#include <iostream>
#include <string>
int main(){
    // Json js;
    // js["stringkey"] = "stringvalue";
    // js["intkey"] = 1000;
    // std::cout << js.dump() << std::endl;
    char buf[] = "{\"intkey\":1000,\"stringkey\":\"stringvalue\"}";
    Json js = Json::parse(buf);
    std::cout << js["intkey"] << std::endl;
}