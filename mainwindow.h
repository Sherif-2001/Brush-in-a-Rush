#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <drawpanel.h>
#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int openDialog();

private:
    Ui::MainWindow *ui;
    DrawPanel *drawpanel;
    QMessageBox::StandardButton reply;

private slots:
    void on_actionPen_color_triggered();
    void on_actionSave_triggered();
    void on_actionPen_size_triggered();
    void on_actionEraser_triggered();

    void on_actionRectangle_triggered();
    void on_actionCircle_triggered();
    void on_actionStraightLine_triggered();
    void on_actionOpen_triggered();
    void on_actionNew_triggered();
    void on_actionClose_triggered();
    void on_actionSearch_triggered();
    void on_actionDelete_triggered();
    void on_actionClear_triggered();
    void on_actionSort_triggered();
    void on_actionFill_with_color_triggered();
    void on_actionFill_color_triggered();
    void on_actionCut_triggered();
    void on_actionPaste_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void closeEvent (QCloseEvent *event);
    void on_actionClick_triggered();
};
#endif // MAINWINDOW_H
