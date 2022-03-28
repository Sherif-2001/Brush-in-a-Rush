#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
#include <QScrollArea>
#include <QCloseEvent>
#include "clossdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      drawpanel(new DrawPanel(this))
{
    ui->setupUi(this);
    setCentralWidget(drawpanel);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete drawpanel;
}

int MainWindow::openDialog()
{
    QMessageBox dialog(QMessageBox::Question, tr("PaintQT"), tr("Do you want to save changes?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, this);
    dialog.setButtonText(QMessageBox::Yes, tr("Yes"));
    dialog.setButtonText(QMessageBox::No, tr("No"));
    dialog.setButtonText(QMessageBox::Cancel, tr("Cancel"));
    dialog.setDefaultButton(QMessageBox::Yes);

    return dialog.exec();
}

void MainWindow::on_actionPen_color_triggered()
{
    QColor customColor = QColorDialog::getColor(Qt::white, this, QString("Pick a color"), QColorDialog::ShowAlphaChannel);
    drawpanel->setColor(customColor);
}

void MainWindow::on_actionSave_triggered()
{
    QImage saveDrawing = drawpanel->getImage();
    QString filePath = QFileDialog::getSaveFileName(this, "Save Image", "", "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)");
    saveDrawing.save(filePath);
}

void MainWindow::on_actionPen_size_triggered()
{
    bool correct = false;

    int size = QInputDialog::getInt(this, "Set pen size", "Pen size", 1, 1, 50, 1, &correct);

    if (correct)
    {
        drawpanel->setBrushWidth(size);
    }
}

void MainWindow::on_actionEraser_triggered()
{
    if(ui->actionEraser->isChecked() == true)
    {
       drawpanel->setPrevColor(drawpanel->getColor());
       drawpanel->setColor(Qt::white);
       drawpanel->setBrushWidth(50);

       drawpanel->setIsCircle(false);
       drawpanel->setIsRectangle(false);
       drawpanel->setIsStraightLine(false);

       ui->actionRectangle->setChecked(false);
       ui->actionCircle->setChecked(false);
       ui->actionStraightLine->setChecked(false);
       ui->actionFill_with_color->setChecked(false);

    }
    if(ui->actionEraser->isChecked() == false)
    {
        drawpanel->setColor(drawpanel->getPrevColor());
        drawpanel->setBrushWidth(3);
    }
}

void MainWindow::on_actionRectangle_triggered()
{
    if(ui->actionRectangle->isChecked() == true)
    {
        ui->actionCircle->setChecked(false);
        ui->actionStraightLine->setChecked(false);
        ui->actionEraser->setChecked(false);

        drawpanel->setIsRectangle(true);
        drawpanel->setIsCircle(false);
        drawpanel->setIsStraightLine(false);
    }
    if(ui->actionRectangle->isChecked() == false)
    {
        drawpanel->setIsRectangle(false);
    }
}

void MainWindow::on_actionCircle_triggered()
{
    if(ui->actionCircle->isChecked() == true)
    {
        ui->actionRectangle->setChecked(false);
        ui->actionStraightLine->setChecked(false);
        ui->actionEraser->setChecked(false);


        drawpanel->setIsCircle(true);
        drawpanel->setIsRectangle(false);
        drawpanel->setIsStraightLine(false);
    }
    if(ui->actionCircle->isChecked() == false)
    {
        drawpanel->setIsCircle(false);
    }
}

void MainWindow::on_actionStraightLine_triggered()
{
    if(ui->actionStraightLine->isChecked() == true)
    {
        ui->actionRectangle->setChecked(false);
        ui->actionCircle->setChecked(false);
        ui->actionEraser->setChecked(false);


        drawpanel->setIsCircle(false);
        drawpanel->setIsRectangle(false);
        drawpanel->setIsStraightLine(true);
    }
    if(ui->actionStraightLine->isChecked() == false)
    {
        drawpanel->setIsStraightLine(false);
    }
}

void MainWindow::on_actionOpen_triggered()
{
    int dialog = openDialog();
    if(dialog == QMessageBox::Yes)
    {
       on_actionSave_triggered();
       drawpanel->openImage();
    }
    else if(dialog == QMessageBox::No)
    {
        drawpanel->openImage();
    }
    else if(dialog == QMessageBox::Cancel)
    {
        return;
    }
}

void MainWindow::on_actionNew_triggered()
{
    int dialog = openDialog();
    if(dialog == QMessageBox::Yes)
    {
       on_actionSave_triggered();
       drawpanel->start();
       update();
    }
    else if(dialog == QMessageBox::No)
    {
        drawpanel->start();
        update();
    }
    else if(dialog == QMessageBox::Cancel)
    {
        return;
    }
}

void MainWindow::on_actionClose_triggered()
{
    int dialog = openDialog();
    if(dialog == QMessageBox::Yes)
    {
       on_actionSave_triggered();
       QApplication::quit();
    }
    else if(dialog == QMessageBox::No)
    {
        QApplication::quit();
    }
    else if(dialog == QMessageBox::Cancel)
    {
        return;
    }
}

void MainWindow::on_actionSearch_triggered(){

    drawpanel->search();
}

void MainWindow::on_actionSort_triggered(){

    drawpanel->shapesSort();
}

void MainWindow::on_actionDelete_triggered(){

        bool ok;
        QString ShapesToDelete = QInputDialog::getItem(this,tr("Delete"),tr("Shapes: "),this->drawpanel->shapesMap.keys(),0,true,&ok);

        if(ok)
        {
            this->drawpanel->setToBeDeleted(ShapesToDelete);
    //     qDebug() << shapesMap.keys();
         QMessageBox::information(this,tr("Delete"),tr("Shape is Deleted!!"));
        }
        else return;
        this->drawpanel->setDeleteStatus(true);
    }

void MainWindow::on_actionClear_triggered(){

    drawpanel->clear();
    drawpanel->shapesMap.clear();
    drawpanel->BackShapeMap.clear();
    while(!drawpanel->stackUndo.empty()){
    drawpanel->stackUndo.pop();
    }
    while(!drawpanel->stackRedo.empty()){
    drawpanel->stackRedo.pop();
    }
}

void MainWindow::on_actionFill_with_color_triggered()
{
    if(ui->actionFill_with_color->isChecked() == true)
    {
        drawpanel->setIsFilling(true);
        ui->actionEraser->setChecked(false);

    }
    else if(ui->actionFill_with_color->isChecked() == false)
    {
        drawpanel->setIsFilling(false);
    }
}

void MainWindow::on_actionFill_color_triggered()
{
    QColor customColor = QColorDialog::getColor(Qt::white, this, QString("Pick a fill color"), QColorDialog::ShowAlphaChannel);
    drawpanel->setFillColor(customColor);
}

void MainWindow::on_actionCut_triggered()
{
    drawpanel->setCopyDrawing(drawpanel->getImage());
    drawpanel->clear();
    ui->actionPaste->setEnabled(true);
}

void MainWindow::on_actionPaste_triggered()
{
    drawpanel->setImage(drawpanel->getCopyDrawing());
    QPainter painter;
    painter.drawImage(0,0, drawpanel->getImage());
    ui->actionPaste->setEnabled(false);
}

void MainWindow::on_actionUndo_triggered()
{
    this->drawpanel->setIsUndo(true);
}

void MainWindow::on_actionRedo_triggered()
{
    this->drawpanel->setRedoStatus(true);
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    reply = QMessageBox::question(this, "Exit", "Are you sure...??",QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
        QApplication::quit();
      } else {
        event->ignore();
      }
    }


void MainWindow::on_actionClick_triggered()
{
    drawpanel->Click();
}

