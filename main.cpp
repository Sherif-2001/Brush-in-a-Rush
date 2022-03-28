#include "mainwindow.h"
#include "startingdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    startingDialog w ;
    w.show();
    return a.exec();
}
