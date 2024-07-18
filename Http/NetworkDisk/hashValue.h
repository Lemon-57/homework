#include <string>
#ifndef _HASHVALUE_H_
#define _HASHVALUE_H_

using std::string;

class HashValue{
public:
    HashValue(string filepath);
    string getsha1sum() const{
        return _sha1sum;
    }
private:
    string _sha1sum;
    string _filepath;
};

#endif