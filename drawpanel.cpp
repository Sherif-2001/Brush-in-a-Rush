#include "drawpanel.h"
#include "enternamedialog.h"
#include <QImage>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QtDebug>
#include <string>
#include <QTableView>




stack <QString> DrawPanel::stackUndo;
stack <QString> DrawPanel::stackRedo;
stack <QImage> DrawPanel::stackImagesUndo;
stack <QImage> DrawPanel::stackImagesRedo;

DrawPanel::DrawPanel(QWidget *parent) : QWidget(parent)
{
    start();
}


DrawPanel::~DrawPanel() {}


void DrawPanel::start()
{
    drawPanel = QImage(this->size(), QImage::Format_RGB32);
    drawPanel.fill(Qt::white);
    setColor(Qt::black);
    setFillColor(Qt::black);
    setBrushWidth(3);
    setPenStyle(Qt::SolidLine);
    setCapStyle(Qt::RoundCap);
    setJoinStyle(Qt::RoundJoin);
    setIsCircle(false);
    setIsRectangle(false);
    setIsStraightLine(false);
    setIsFilling(false);
    mousePressed = false;
    undoStatus = false;
}


bool DrawPanel::openImage()
{
    QString openImageLocation = QFileDialog::getOpenFileName(this, tr("Open image"), "", tr("PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)" ));
    if(!openImageLocation.isEmpty())
    {
        drawPanel.load(openImageLocation);
        return true;
    }
    else
    {
        return false;
    }
}


void DrawPanel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        firstPoint = event->pos();
        x1=firstPoint.x();
        y1=firstPoint.y();
        lastPoint = event->pos();
        x2=lastPoint.x();
        y2=lastPoint.y();
        isDrawing = true;
        mousePressed = true;

    }
    update();
}


void DrawPanel::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && isDrawing)
    {
        lastPoint = event->pos();
    }
    update();
}


void DrawPanel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isDrawing)
    {
        isDrawing = false;
        mousePressed = false;

        setCopyDrawing(getImage());
        image = getCopyDrawing();
        stackImagesUndo.push(image);

        if(getIsRectangle())
        {
            int arr[] = {firstPoint.x(),firstPoint.y(),lastPoint.x(),lastPoint.y()};

            bool ok;
            QString rectangleName = QInputDialog::getText(this,tr("Add Rectangle"),tr("Enter shape name:"),QLineEdit::Normal,tr(""),&ok );
            if( ok && !rectangleName.isEmpty() && !shapesMap.contains(rectangleName))
            {
                shapesMap[rectangleName] = Rectangle(rectangleName.toStdString(),arr,QRect(firstPoint,lastPoint),currentColor);
                BackShapeMap[rectangleName] = Rectangle(rectangleName.toStdString(),arr,QRect(firstPoint,lastPoint),currentColor);
                stackUndo.push(rectangleName);
                temp = rectangleName;

                qDebug() << shapesMap.keys();

            }
            else{
                setRedraw(true);
            }
        }
        else if(getIsCircle())
        {
            int arr[] = {firstPoint.x(),firstPoint.y(),lastPoint.x(),lastPoint.y()};

            bool ok;
            QString circleName = QInputDialog::getText(this,tr("Add Circle"),tr("Enter shape name:"),QLineEdit::Normal,tr(""),&ok );
            if( ok && !circleName.isEmpty() )
            {
                shapesMap[circleName] = Circle(circleName.toStdString(),arr,currentColor);
                BackShapeMap[circleName] = Circle(circleName.toStdString(),arr,currentColor);
                stackUndo.push(circleName);
                temp = circleName;

                qDebug() << shapesMap.keys();


            }
            else{
                setRedraw(true);
            }
        }
        else if(getIsStraightLine())
        {
            int arr[] = {firstPoint.x(),firstPoint.y(),lastPoint.x(),lastPoint.y()};

            bool ok;
            QString lineName = QInputDialog::getText(this,tr("Add Line"),tr("Enter shape name:"),QLineEdit::Normal,tr(""),&ok );
            if(ok && !lineName.isEmpty())
            {
                shapesMap[lineName] = Line(lineName.toStdString(),arr,currentColor);
                BackShapeMap[lineName] = Line(lineName.toStdString(),arr,currentColor);
                stackUndo.push(lineName);
                temp = lineName;

                qDebug() << shapesMap.keys();

            }
            else{
                setRedraw(true);
            }
        }
    }
    update();
}


void DrawPanel::paintEvent(QPaintEvent *event)
{
    static bool wasMousePressed = false; //selects the shape.

    QRect rectArea = event->rect();
    QPainter painter(this);
    painter.drawImage(rectArea, drawPanel, rectArea); //Drawer. Allocates a place to draw in.

    if(mousePressed)
    {
        wasMousePressed = true;

        if (getIsCircle())
        {
            raduis = abs(sqrt(pow(lastPoint.x()-firstPoint.x(),2)+pow(lastPoint.y()-firstPoint.y(),2)));
            QRect circle = QRect(firstPoint, lastPoint); //determines the circle and draws it.
            QPainter circlePainter(this);
            circlePainter.setPen(QPen(currentColor,brushWidth,penStyle,capStyle,joinStyle)); //el shadda lonha mo5talef.
            circlePainter.drawEllipse(firstPoint.x(),firstPoint.y(),raduis,raduis); //for dynamic interaction.
            if(isFilling)
            {
                QBrush fillbrush(fillColor);
                QPainterPath path;
                path.addEllipse(firstPoint.x(),firstPoint.y(),raduis,raduis);
                painter.fillPath(path, fillbrush);
            }
        }
        else if (getIsRectangle())
        {
            QRect rect = QRect(firstPoint, lastPoint);
            QPainter rectanglePainter(this);
            rectanglePainter.setPen(QPen(currentColor,brushWidth,penStyle,capStyle,joinStyle));
            rectanglePainter.drawRect(rect);
            if (isFilling)
            {
                QBrush fillbrush(fillColor);
                QPainterPath path;
                path.addRoundedRect(rect,brushWidth, brushWidth);
                painter.fillPath(path,fillbrush);
            }
        }

        else if(getIsStraightLine())
        {
            QRect StraightLine = QRect(firstPoint, lastPoint); //determines the ST and draws it.
            QPainter StraightLinePainter(this);
            StraightLinePainter.setPen(QPen(currentColor,brushWidth,penStyle,capStyle,joinStyle)); //el shadda lonha mo5talef.
            StraightLinePainter.drawLine(firstPoint, lastPoint); //for dynamic interaction.
        }


        else
        {
            QPainter pencilPainter(&drawPanel);
            pencilPainter.setPen(QPen(currentColor,brushWidth,penStyle,capStyle,joinStyle));
            pencilPainter.drawLine(lastPoint, firstPoint);
            firstPoint = lastPoint;
        }

    }
    //----------------------------------------------------------------------------------------------//
    //For saving.
    else if(wasMousePressed)
    {
        QPainter painter(&drawPanel);
        painter.setPen(QPen(currentColor,brushWidth,penStyle,capStyle,joinStyle));

        if (getIsCircle())
        {
            QRect circle = QRect(firstPoint, lastPoint);
            painter.drawEllipse(x1,y1,raduis,raduis);
            if(isFilling)
            {
                QBrush fillbrush(fillColor);
                QPainterPath path;
                path.addEllipse(firstPoint.x(),firstPoint.y(),raduis,raduis);
                painter.fillPath(path, fillbrush);
            }
        }

        else if (getIsRectangle())
        {
            QRect rect = QRect(firstPoint, lastPoint);
            painter.drawRect(rect);
            if (isFilling)
            {
                QBrush fillbrush(fillColor);
                QPainterPath path;
                path.addRoundedRect(rect,brushWidth, brushWidth);
                painter.fillPath(path,fillbrush);
            }
        }

        else if(getIsStraightLine())
        {
            painter.drawLine(firstPoint, lastPoint);
        }

        else
        {
            painter.drawImage(rectArea, drawPanel, rectArea);
        }

        wasMousePressed = false;

    }
    if(getRedraw())
    {
        DrawPanel::clear();

        QPainter aPainter(&drawPanel);
        aPainter.setPen(QPen(currentColor,brushWidth,penStyle,capStyle,joinStyle));

        for(auto shap:shapesMap)
        {
            if(!shap.getIsDrawn()) continue;

            int type = shap.getType();
            int x1 = shap.getCoordinates()[0];
            int y1 = shap.getCoordinates()[1];
            int x2 = shap.getCoordinates()[2];
            int y2 = shap.getCoordinates()[3];
            if(type == 0)
            {
                aPainter.drawRect(QRect(QPoint(x1,y1),QPoint(x2,y2)));
            }
            else if (type == 1)
            {
//                painter->drawEllipse(150,150,100,100);


                int diameter = sqrt( pow((x2 - x1),2) + pow((y2 - y1),2));
                qDebug()<<diameter<<' ';
                aPainter.drawEllipse(x1,y1,diameter,diameter);
            }
            else
            {
                aPainter.drawLine(x1,y1,x2,y2);
            }

        }
        setRedraw(false);

    }
    else if(getIsUndo())
    {
        QPainter painter(&drawPanel);
        painter.setPen(QPen(currentColor,brushWidth,penStyle,capStyle,joinStyle));

        undo(&painter);
    }
    else if(getRedoStatus())
        {
            QPainter aPainter(&drawPanel);
            aPainter.setPen(QPen(currentColor,brushWidth,penStyle,capStyle,joinStyle));

            redo(&aPainter);
        }
    else if(getDeleteStatus() && shapesMap[getToBeDeleted()].getIsDrawn())
        {


            QPainter aPainter(&drawPanel);
            aPainter.setPen(QPen(currentColor,brushWidth,penStyle,capStyle,joinStyle));



            shapesMap[getToBeDeleted()].setIsDrawn(false);
            stackUndo.push(getToBeDeleted());

            DrawPanel::clear();

            for(auto shap:shapesMap)
            {
                if(!shap.getIsDrawn()) continue;

                int type = shap.getType();
                int x1 = shap.getCoordinates()[0];
                int y1 = shap.getCoordinates()[1];
                int x2 = shap.getCoordinates()[2];
                int y2 = shap.getCoordinates()[3];
                if(type == 0)
                {
                    aPainter.drawRect(QRect(QPoint(x1,y1),QPoint(x2,y2)));
                }
                else if (type == 1)
                {
    //                painter->drawEllipse(150,150,100,100);


                    int diameter = sqrt( pow((x2 - x1),2) + pow((y2 - y1),2));
                    qDebug()<<diameter<<' ';
                    aPainter.drawEllipse(x1,y1,diameter,diameter);
                }
                else
                {
                    aPainter.drawLine(x1,y1,x2,y2);
                }

            }
            qDebug() << shapesMap.keys();


        }
        setDeleteStatus(false);
    update();

}

void DrawPanel::resizeEvent(QResizeEvent *event)
{
    if (width() != drawPanel.width() || height() != drawPanel.height())
    {
        int nWidth, nHeight;

        if (width() > drawPanel.width())
            nWidth = width();
        if (width() <= drawPanel.width())
            nWidth = drawPanel.width();
        if (height() > drawPanel.height())
            nHeight = height();
        if (height() <= drawPanel.height())
            nHeight = drawPanel.height();

        resize(nWidth,nHeight);
    }
}


QImage DrawPanel::getImage()
{
    return drawPanel;
}

void DrawPanel::setImage(QImage image)
{
    drawPanel = image;
}

void DrawPanel::resize(int w, int h)
{
    QPixmap newImage(QSize(w, h));
    newImage.fill(Qt::white);
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), drawPanel);
    setImage(newImage.toImage());
    update();
}

void DrawPanel::clear()
{
    drawPanel.fill(Qt::white);
    update();
}

QColor DrawPanel::getPrevColor() const
{
    return prevColor;
}

void DrawPanel::setPrevColor(const QColor &value)
{
    prevColor = value;
}

Qt::PenStyle DrawPanel::getPenStyle() const
{
    return penStyle;
}

void DrawPanel::setPenStyle(const Qt::PenStyle &value)
{
    penStyle = value;
}

Qt::PenCapStyle DrawPanel::getCapStyle() const
{
    return capStyle;
}

void DrawPanel::setCapStyle(const Qt::PenCapStyle &value)
{
    capStyle = value;
}

Qt::PenJoinStyle DrawPanel::getJoinStyle() const
{
    return joinStyle;
}

void DrawPanel::setJoinStyle(const Qt::PenJoinStyle &value)
{
    joinStyle = value;
}

bool DrawPanel::getIsRectangle() const
{
    return isRectangle;
}

void DrawPanel::setIsRectangle(bool value)
{
    isRectangle = value;
}

bool DrawPanel::getIsCircle() const
{
    return isCircle;
}

void DrawPanel::setIsCircle(bool value)
{
    isCircle = value;
}

bool DrawPanel::getIsStraightLine() const
{
    return isStraightLine;
}

void DrawPanel::setIsStraightLine(bool value)
{
    isStraightLine = value;
}

bool DrawPanel::getIsFilling() const
{
    return isFilling;
}

void DrawPanel::setIsFilling(bool value)
{
    isFilling = value;
}



QColor DrawPanel::getFillColor() const
{
    return fillColor;
}

void DrawPanel::setFillColor(const QColor &value)
{
    fillColor = value;
}

QImage DrawPanel::getCopyDrawing() const
{
    return copyDrawing;
}

void DrawPanel::setCopyDrawing(const QImage &value)
{
    copyDrawing = value;
}

void DrawPanel::setColor(QColor setColor)
{
    currentColor = setColor;
}

void DrawPanel::setBrushWidth(int setBrushWidth)
{
    brushWidth = setBrushWidth;
}
bool DrawPanel::getIsUndo()
{
    return undoStatus;
}

void DrawPanel::setIsUndo(bool status)
{
    undoStatus = status;
}

bool DrawPanel::getDeleteStatus()
{
    return this->deleteStatus;
}

void DrawPanel::setDeleteStatus(bool deleteStatus)
{
    this->deleteStatus = deleteStatus;
}

const QString &DrawPanel::getToBeDeleted() const
{
    return toBeDeleted;
}

void DrawPanel::setToBeDeleted(const QString &newToBeDeleted)
{
    toBeDeleted = newToBeDeleted;
}

bool DrawPanel::getRedoStatus() const
{
    return redoStatus;
}

void DrawPanel::setRedoStatus(bool newRedoStatus)
{
    redoStatus = newRedoStatus;
}

bool DrawPanel::getRedraw() const
{
    return redraw;
}

void DrawPanel::setRedraw(bool newRedraw)
{
    redraw = newRedraw;
}

QColor DrawPanel::getColor()
{
    return currentColor;
}

void DrawPanel::undo(QPainter *painter)
{
    if (!stackUndo.empty()){

        QString shape = stackUndo.top();
        stackRedo.push(shape);
        stackUndo.pop();

        shapesMap[shape].setIsDrawn(!shapesMap[shape].getIsDrawn());

        if(!shapesMap[shape].getIsDrawn())
        {
            DrawPanel::clear();
            for(auto shap:shapesMap)
            {
                if(!shap.getIsDrawn()) continue;

                int type = shap.getType();
                int x1 = shap.getCoordinates()[0];
                int y1 = shap.getCoordinates()[1];
                int x2 = shap.getCoordinates()[2];
                int y2 = shap.getCoordinates()[3];
                if(type == 0)
                {
                    painter->drawRect(QRect(QPoint(x1,y1),QPoint(x2,y2)));
                }
                else if (type == 1)
                {
    //                painter->drawEllipse(150,150,100,100);


                    int diameter = sqrt( pow((x2 - x1),2) + pow((y2 - y1),2));
                    qDebug()<<diameter<<' ';
                    painter->drawEllipse(x1,y1,diameter,diameter);
                }
                else
                {
                    painter->drawLine(x1,y1,x2,y2);
                }

            }
        }
        else
        {
            auto shap = shapesMap[shape];

            int x1 = shap.getCoordinates()[0];
            int y1 = shap.getCoordinates()[1];
            int x2 = shap.getCoordinates()[2];
            int y2 = shap.getCoordinates()[3];

            int type = shap.getType();

            if(type == 0)
            {
                painter->drawRect(QRect(QPoint(x1,y1),QPoint(x2,y2)));
            }
            else if (type == 1)
            {
                int diameter = sqrt( pow((x2 - x1),2) + pow((y2 - y1),2));
                qDebug()<<diameter<<' ';
                painter->drawEllipse(x1,y1,diameter,diameter);
            }
            else
            {
                painter->drawLine(x1,y1,x2,y2);
            }
        }

//        stackRedo.push(stackUndo.top());
        qDebug() << shapesMap.keys();

    }
    else DrawPanel::clear();
    setIsUndo(false);
}
void DrawPanel::redo(QPainter *painter)
{
    if (!stackRedo.empty()){

        QString shape = stackRedo.top();
        stackUndo.push(shape);
        stackRedo.pop();

        shapesMap[shape].setIsDrawn(!shapesMap[shape].getIsDrawn());

        if(!shapesMap[shape].getIsDrawn())
        {
            DrawPanel::clear();
            for(auto shap:shapesMap)
            {
                if(!shap.getIsDrawn()) continue;

                int type = shap.getType();
                int x1 = shap.getCoordinates()[0];
                int y1 = shap.getCoordinates()[1];
                int x2 = shap.getCoordinates()[2];
                int y2 = shap.getCoordinates()[3];
                if(type == 0)
                {
                    painter->drawRect(QRect(QPoint(x1,y1),QPoint(x2,y2)));
                }
                else if (type == 1)
                {
    //                painter->drawEllipse(150,150,100,100);


                    int diameter = sqrt( pow((x2 - x1),2) + pow((y2 - y1),2));
                    qDebug()<<diameter<<' ';
                    painter->drawEllipse(x1,y1,diameter,diameter);
                }
                else
                {
                    painter->drawLine(x1,y1,x2,y2);
                }

            }
        }
        else
        {
            auto shap = shapesMap[shape];

            int x1 = shap.getCoordinates()[0];
            int y1 = shap.getCoordinates()[1];
            int x2 = shap.getCoordinates()[2];
            int y2 = shap.getCoordinates()[3];

            int type = shap.getType();

            if(type == 0)
            {
                painter->drawRect(QRect(QPoint(x1,y1),QPoint(x2,y2)));
            }
            else if (type == 1)
            {
                int diameter = sqrt( pow((x2 - x1),2) + pow((y2 - y1),2));
                qDebug()<<diameter<<' ';
                painter->drawEllipse(x1,y1,diameter,diameter);
            }
            else
            {
                painter->drawLine(x1,y1,x2,y2);
            }
        }

//        stackRedo.push(stackUndo.top());
        qDebug() << shapesMap.keys();

    }
    setRedoStatus(false);
}


void DrawPanel::search(){
    bool ok;
    QString shapeName = QInputDialog::getText(this,tr("Search"),tr("Enter shape name:"),QLineEdit::Normal,tr(""),&ok );
    if( ok && !shapeName.isEmpty())
    {
        if(shapesMap.contains(shapeName) && shapesMap[shapeName].getIsDrawn()){

           // QMessageBox::information(this,tr("Search"),tr("Found!!"));
            QMessageBox msgBox;

            QString message="Name: " + shapeName + "\nPerimeter: " + QString::number(shapesMap[shapeName].getPerimeter2(shapesMap[shapeName].getType())) + "\nColor: "+ QString(DrawPanel::getColor().name()) +"\nCoordinates: " +
                   +"\n("+ QString::number(shapesMap[shapeName].getCoordinates()[0]) + " , "+ QString::number(shapesMap[shapeName].getCoordinates()[1]) +")\n(" + QString::number(shapesMap[shapeName].getCoordinates()[2]) +" , "+ QString::number(shapesMap[shapeName].getCoordinates()[3]) +")\n";
            msgBox.setWindowTitle("Search");
            msgBox.setText("Found!!");
            msgBox.setDetailedText(message);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.addButton(QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::No);
            if(msgBox.exec() == QMessageBox::Yes){
              // do something
            }else {
              // do something else
            }
        }
        else{
            QMessageBox::information(this,tr("Search"),tr("Not Found!!"));
        }
    }
}

void DrawPanel::Click(){
    bool ok;
    QString message = QInputDialog::getText(this,tr("Message"),tr(""),QLineEdit::Normal,tr("Give me a bonus please!!!!"),&ok);
}

bool DrawPanel::cmp(QPair<string, Shape> a, QPair<string, Shape> b)
{
   return a.second.getPerimeter2(a.second.getType()) < b.second.getPerimeter2(b.second.getType());
}

void DrawPanel:: shapesSort(){

    sortedVec.clear();
    for(auto it : shapesMap){
        sortedVec.emplace_back(it.getName() , it);
    }

  sort(sortedVec.begin(),sortedVec.end(),cmp );



    //SORTLIST AS;
    QTableView *view = new QTableView;
    view->setSortingEnabled(true);
    view->setFixedSize(450,300);

    QStandardItemModel *mod = new QStandardItemModel;
    QStandardItem *it = new QStandardItem(QObject::tr("Shape"));
    mod->setHorizontalHeaderItem(0,it);
    QStandardItem *it1 = new QStandardItem(QObject::tr("Name"));
    mod->setHorizontalHeaderItem(1,it1);
    QStandardItem *it2 = new QStandardItem(QObject::tr("Perimeter"));
    mod->setHorizontalHeaderItem(2,it2);
    QStandardItem *it3 = new QStandardItem(QObject::tr("Coordinates"));
    mod->setHorizontalHeaderItem(3,it3);
    QStandardItem *it4 = new QStandardItem(QObject::tr("Color"));
    mod->setHorizontalHeaderItem(4,it4);

    int row=0,cnt=1;
     for(auto i: qAsConst(sortedVec)){
         int col=0;
         if(!i.second.getIsDrawn()) continue;
         if(i.second.getType()==0){
             QStandardItem *it00 = new QStandardItem(QObject::tr("Rectangle"));
             mod->setItem(row,it00);
            }
         else if(i.second.getType()==1){
             QStandardItem *it01 = new QStandardItem(QObject::tr("Circle"));
             mod->setItem(row,it01);
         }
         else{
             QStandardItem *it02 = new QStandardItem(QObject::tr("Line"));
             mod->setItem(row,it02);
         }
//name
        string nameofShape=i.second.getName() ;
        const char * n = nameofShape.c_str();
        QStandardItem *it0 = new QStandardItem(QObject::tr(n));
        mod->setItem(row , col+1, it0);
//perimeter
        string s = std::to_string(i.second.getPerimeter2(i.second.getType()));
        const char* p = s.c_str() ;
        QStandardItem *it1 = new QStandardItem(QObject::tr(p));
        mod->setItem(row , col+2, it1);

//coordinates
        string coor = "("+ std::to_string(i.second.getCoordinates()[0]) + ","+std::to_string(i.second.getCoordinates()[1]) +") , (" + std::to_string(i.second.getCoordinates()[2]) +","+ std::to_string(i.second.getCoordinates()[3]) +")";
        const char* c = coor.c_str() ;
        QStandardItem *it2 = new QStandardItem(QObject::tr(c));
        mod->setItem(row , col+3, it2);


//color

       QString color=i.second.getColor();
       //const char* cl = color.c_str();
       QStandardItem *it3 = new QStandardItem();
       it3->setText(color);
       mod->setItem(row , col+4, it3);

       row++;cnt++;

    }
    view->setModel(mod);
    view->resizeColumnsToContents();
    view->show();
  }

