#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "equationsolver.h"
#include "Plotter/qcustomplot.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    EquationSolver solver;
    QCPGraph * graph;

    ~MainWindow();

private slots:

    void on_graphBtn_clicked();
    void showErrorMsg(QString);

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
