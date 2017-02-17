#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>                  //класс главного окна
#include <QFile>
#include <QDataStream>
#include <QFileDialog>                  //класс диалогового окна "открыть файл"
#include "headers.h"
#include <qwt_plot_canvas.h>            //дальше грузятся необходимые либы библиотеки QWT для построения графиков
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_scale_div.h>
#include <qwt_plot_marker.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    Scope scope;

    QVector<double> xData;      //значения по оси x



    ~MainWindow();

private slots:

    void on_actionTest_triggered();     //слот, обрабатывающий нажатие кнопки "добавить файл"

    void on_pushButton_clicked();       //слот, обрабатывающий нажатие кнопки "нарисовать"

    void on_lpf1_enable_toggled(bool checked);

    void on_lpf2_enable_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    QwtLegend *leg;                     //легенда для графика
    QwtPlotMagnifier *magnifier;        //зумер
    QwtPlotMagnifier *zoom_x;
    QwtPlotMagnifier *zoom_y1;
    QwtPlotPanner *d_panner;            //перемещатель
    QwtPlotGrid *grid;                  //сетка
    QwtPlotCurve *curv1,*curv2;         //две кривые
    QwtPlotMarker *m1;                  //маркер- горизонтальная линия в 0.

};


#endif // MAINWINDOW_H
