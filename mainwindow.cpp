#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // This connects the signal from the solver class to the slot here which just show a massage box when any thing wrong occur because of the entered function
    QObject::connect(&solver,&EquationSolver::errorMsg,this,&MainWindow::showErrorMsg);
    ui->setupUi(this);

    // Intialaize the grid and add a graph
    ui->grid->setBackground(QBrush(QColor("#FFFFFF")));
    ui->grid->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->grid->yAxis->setLabel("Y(X)");
    ui->grid->xAxis->setLabel("X");
    graph = ui->grid->addGraph();

    // Validators for the line edits
    ui->from_lineEdit->setValidator(new QDoubleValidator(-99999.0,99999.0,3));
    ui->to_lineEdit->setValidator(new QDoubleValidator(-99999.0,99999.0,3));
    ui->step_lineEdit->setValidator(new QDoubleValidator(-99999.0,99999.0,3));

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_graphBtn_clicked()
{
    double from = ui->from_lineEdit->text().toDouble();
    double to = ui->to_lineEdit->text().toDouble();
    double step = ui->step_lineEdit->text().toDouble();


    QVector<double> y = solver.calcFunc(ui->func_TE->toPlainText().toLocal8Bit().data(),from,to,step);

    // If any error occurr y vector will be empty then we will exit the function
    if(y.isEmpty())
        return;

    // Make X axis vector
    QVector<double> x;
    while(from <= to)
    {
        x.append(from);
        from += step;
    }

    // size of the x and y must be the same, otherwise there is an error
    if(y.length() != x.length()) return;

    // Graph the final output
    graph->setData(x,y);
    ui->grid->replot(QCustomPlot::rpQueuedReplot);
    graph->rescaleAxes();

}

void MainWindow::showErrorMsg(QString msg){
    QMessageBox Msgbox;
    Msgbox.setText(msg);
    Msgbox.exec();
}

// This just change background of the grid when the "Dark" check box is toggeled
void MainWindow::on_checkBox_stateChanged(int arg1)
{
    arg1==2? ui->grid->setBackground(QBrush(QColor("#0B2739"))) : ui->grid->setBackground(QBrush(QColor("#FFFFFF")));
    ui->grid->repaint();

}

