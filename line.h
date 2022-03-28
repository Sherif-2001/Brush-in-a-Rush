#ifndef LINE_H
#define LINE_H

#include <shape.h>
#include "drawpanel.h"
class Line:public Shape
{
public:
    Line(string name,int arr[4],QColor color);
    double getPerimeter() const;
    bool operator <(const Line other) const;

};

#endif // LINE_H
