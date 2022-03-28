#ifndef CLOSSDIALOG_H
#define CLOSSDIALOG_H

#include <QDialog>

namespace Ui {
class ClossDialog;
}

class ClossDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClossDialog(QWidget *parent = nullptr);
    ~ClossDialog();

private slots:



    void on_Back_btn_clicked();

    void on_close_btn_clicked();

private:
    Ui::ClossDialog *ui;
};

#endif // CLOSSDIALOG_H
