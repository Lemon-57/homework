#pragma once

class NonCopyable
{
public:
protected:
    NonCopyable() {}
    ~NonCopyable() {}

    NonCopyable(const NonCopyable &rhs) = delete;
    NonCopyable &operator = (const NonCopyable &rhs) = delete; 
private:

};

