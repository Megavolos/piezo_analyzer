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
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include <QMessageBox>
#include "port.h"

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
    void setDataSource (QString source);
    void readDataFromFiles();
    void readDataFromRS232();
    void setCurvesStyle(uchar i);
    ~MainWindow();
signals:

    void savesettings(QString name, int baudrate, int DataBits, int Parity, int StopBits, int FlowControl);
    void writeData(const QByteArray &data);
private slots:





    void on_lpf1_enable_toggled(bool checked);

    void on_lpf2_enable_toggled(bool checked);

    void on_addFileButton_triggered();

    void on_draw_button_clicked();

    void on_draw_button2_clicked();

    void Print(QString data);

    void on_savePortSettingsButton_clicked();



    void on_filesRadioButton_toggled(bool checked);

    void on_RS232radioButton_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    QwtLegend *leg, *leg1, *leg2;                     //легенда для графика
    QwtPlotMagnifier *magnifier;        //зумер
    QwtPlotMagnifier *zoom_x, *zoom_x1,*zoom_x2;
    QwtPlotMagnifier *zoom_y, *zoom_y1, *zoom_y2;
    QwtPlotPanner *d_panner,*d_panner1,*d_panner2;            //перемещатель
    QwtPlotGrid *grid,*grid1,*grid2;                  //сетка
   // QwtPlotCurve *curv1,*curv2;         //две кривые
    QwtPlotMarker *m1;                  //маркер- горизонтальная линия в 0.
    QVector<QwtPlotCurve*> curves;
    Port *PortNew;
    QSerialPortInfo serialPortInfo;
    QList<QSerialPortInfo> serialPorts;
    QString dataSource;
    QMessageBox msgBox;
    uchar emptyItems;
    bool portopened;
};


#endif // MAINWINDOW_H
