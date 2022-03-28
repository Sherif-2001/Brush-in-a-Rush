#include "line.h"


Line::Line(string name, int arr[4],QColor color):Shape(name,arr,color)
{
    this->type = 2;
}

double Line::getPerimeter() const
{
    return getLengthBetweenPoints();
}

bool Line::operator <(const Line other) const
{
    return (getPerimeter() < other.getPerimeter());
}

