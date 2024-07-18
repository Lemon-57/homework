#pragma once

class Line
{
public:
    Line(int, int, int, int);
    ~Line();
    void printfLine() const;
private:
    class LineImpl;
    LineImpl * _pimpl;
};

