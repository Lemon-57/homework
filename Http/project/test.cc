#include "hashValue.h"
#include "unixHeader.h"
int main(){
    HashValue hashValue("tmp/1.txt");
    fprintf(stderr,"hashvalue = %s\n", hashValue.getsha1sum().c_str());
}