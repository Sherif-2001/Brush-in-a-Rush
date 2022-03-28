#ifndef STARTINGDIALOG_H
#define STARTINGDIALOG_H

#include <QDialog>
#include"mainwindow.h"

namespace Ui {
class startingDialog;
}

class startingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit startingDialog(QWidget *parent = nullptr);
    ~startingDialog();

private slots:

    void on_start_btn_clicked();

    void on_close_btn_clicked();

private:
    Ui::startingDialog *ui;
};

#endif // STARTINGDIALOG_H
