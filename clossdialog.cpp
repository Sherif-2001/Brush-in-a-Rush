#include "clossdialog.h"
#include "ui_clossdialog.h"
#include <QCloseEvent>

ClossDialog::ClossDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClossDialog)
{
    ui->setupUi(this);
}

ClossDialog::~ClossDialog()
{
    delete ui;
}


void ClossDialog::on_Back_btn_clicked()
{

}


void ClossDialog::on_close_btn_clicked()
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

