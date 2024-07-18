#include <iostream>
#include"Line.hpp"

using namespace std;

class Line::LineImpl
{
public:
    LineImpl(int x1, int y1, int x2, int y2);    
    void printLineImpl() const;
private:
    class Point
    {
    public:
        Point(int x = 0, int y = 0)
        :_x(x),_y(y)
        {

        }
    void print() const;
    private:
        int _x;
        int _y;
    };
    Point _pt1;
    Point _pt2;
};

Line::LineImpl::LineImpl(int x1, int y1, int x2, int y2)
:_pt1(x1, y1)
,_pt2(x2, y2)
{
    cout << "Line::LineImpl::LineImpl" << endl;
}

void Line::LineImpl::printLineImpl() const
{
    _pt1.print();
    cout << endl;
    _pt2.print();
    cout << endl;
}

void Line::LineImpl::Point::print() const{
    cout << "_x: " << _x << "_y: " << _y << endl;
}

Line::Line(int x1, int y1, int x2, int y2)
: _pimpl(new LineImpl(x1, y1, x2, y2))    
{
    cout << "Line::Line" << endl;
}

Line::~Line()
{
    delete _pimpl;
}

void Line::printfLine() const
{
    _pimpl->printLineImpl();
}

int main()
{
    Line line(1, 2, 3, 4);
    line.printfLine();
    
    return 0;
}

