#include "rectangle.h"
#include "drawpanel.h"


Rectangle::Rectangle(string name, int coordinates[],QRect physicalShape,QColor color):Shape(name,coordinates,color)
{
    this->type = 0;
    this->physicalShape = physicalShape;
}


QRect Rectangle::getPhysicalShape() const{
    return this->physicalShape ;
}

double Rectangle::getPerimeter() const
{
    return(2 * abs(coordinates[0] - coordinates[1]) + 2 * abs(coordinates[2] - coordinates[3]));
}

double Rectangle::getArea() const
{
    return(abs(coordinates[0] - coordinates[1]) * abs(coordinates[2] - coordinates[3]));
}

bool Rectangle::operator <(const Rectangle other) const
{
    return (getPerimeter() < other.getPerimeter());
}
