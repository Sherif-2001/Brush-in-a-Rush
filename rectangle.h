#ifndef RECTANGLE_H
#define RECTANGLE_H

#include<shape.h>

class Rectangle:public Shape
{
private:
    QRect physicalShape;
public:
    QRect getPhysicalShape() const;
    Rectangle(string name,int coordinates[4],QRect physicalShape,QColor color);
    double getPerimeter()const;
    double getArea()const;
    bool operator <(const Rectangle other) const;
};

#endif // RECTANGLE_H
