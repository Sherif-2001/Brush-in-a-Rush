#include "startingdialog.h"
#include "ui_startingdialog.h"
#include "mainwindow.h"

startingDialog::startingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::startingDialog)
{
    ui->setupUi(this);
}

startingDialog::~startingDialog()
{
    delete ui;
}

void startingDialog::on_start_btn_clicked()
{
    this ->close();
    MainWindow *mainWindow= new MainWindow();
    mainWindow->show();
}

void startingDialog::on_close_btn_clicked()
{
   this->close();
}

