#ifndef SHAPE_H
#define SHAPE_H

#include <bits/stdc++.h>
#include <QRect>
#include <QColor>
using namespace std;

class Shape
{
protected:
    int coordinates[4];
    string name;
    int type;
    QColor color;
    int getLengthBetweenPoints()const;
    bool isDrawn;

public:
    virtual double getArea() const{}
    virtual int getDiameter() const{}
    virtual double getPerimeter() const{}
    virtual bool operator < (const Shape other) const{}
    virtual QRect getPhysicalShape() const{}

//    Shape(string name,int arr[4],QRect physical);
    Shape(string name,int arr[4],QColor color);
    Shape();
    int *getCoordinates();
    string getName() const;
    void setName(string name);
    QRect getphysicalShape() const ;
    int getType();
    void setColor(QColor color);
    QString getColor() const;

    bool getIsDrawn() const;
    void setIsDrawn(bool newIsDrawn);


    int getPerimeter2(int type);

};

#endif // SHAPE_H
