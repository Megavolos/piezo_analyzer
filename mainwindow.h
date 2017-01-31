#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include "headers.h"
#include <qwt_plot_canvas.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    Scope scope;
    QVector<quint8> yData;
    QVector<double> xData;

    ~MainWindow();

private slots:

    void on_actionTest_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QwtLegend *leg;
    QwtPlotGrid *grid;
    QwtPlotCurve *curv1,*curv2;

};


#endif // MAINWINDOW_H
