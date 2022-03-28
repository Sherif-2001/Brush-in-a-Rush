#ifndef CIRCLE_H
#define CIRCLE_H

#include <shape.h>

class Circle:public Shape
{
public:
    Circle(string name,int coordinates[4],QColor color);
    double getArea()const;
    double getPerimeter()const;
    bool operator <(const Circle other) const;
    int getDiameter() const;


private:
    QPoint pnt;


};

#endif // CIRCLE_H
