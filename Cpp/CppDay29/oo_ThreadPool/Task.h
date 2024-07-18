#pragma once

class Task
{
public:
    Task() {}
    virtual ~Task() {}
    virtual void process() = 0;

private:

};

