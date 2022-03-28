#include "shape.h"
#include "drawpanel.h"


int Shape::getLengthBetweenPoints() const
{
    int x1 = coordinates[0];
    int y1 = coordinates[1];
    int x2 = coordinates[2];
    int y2 = coordinates[3];
    return sqrt( pow((x2 - x1),2) + pow((y2 - y1),2));
}

int Shape::getPerimeter2(int type){
    if(type == 0){
        return(2 * abs(coordinates[0] - coordinates[2]) + 2 * abs(coordinates[1] - coordinates[3]));
    }
    else if(type == 1){
        return( M_PI * getLengthBetweenPoints());
    }
    else{
        return getLengthBetweenPoints();
    }
}

Shape::Shape(string name, int arr[],QColor color)
{
    this->name = name;
    for(int i=0;i<4;i++)
        coordinates[i] = arr[i];
    this->color=color;
    this->isDrawn=true;
}



Shape::Shape()
{
    name = "";
    for(int i=0;i<4;i++)
        coordinates[i] = 0;
    this->type = 0;

}

int *Shape::getCoordinates()
{
    return coordinates;
}

string Shape::getName() const
{
    return name;
}

void Shape::setName(string name)
{
    this->name = name;
}


int Shape::getType()
{
    return type;
}

void Shape::setColor(QColor color)
{
    this->color=color;
}


QString Shape::getColor() const
{
    //return QString(color.rgb());
    return color.name();

}
bool Shape::getIsDrawn() const
{
    return isDrawn;
}

void Shape::setIsDrawn(bool newIsDrawn)
{
    isDrawn = newIsDrawn;
}
