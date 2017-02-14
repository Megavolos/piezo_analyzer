#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    leg = new QwtLegend();
    div = new QwtScaleDiv();
    leg->setDefaultItemMode(QwtLegendData::ReadOnly);
    ui->qwtPlot->setTitle("Данные из файлов");
    ui->qwtPlot->insertLegend(leg,QwtPlot::TopLegend);
    grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->setMajorPen(QPen(Qt::black,1,Qt::DotLine));
    grid->setMinorPen(QPen(Qt::gray,1,Qt::DotLine));
    m1=new QwtPlotMarker;

    grid->attach(ui->qwtPlot);
    ui->qwtPlot->enableAxis(QwtPlot::yRight);
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom,QString::fromLocal8Bit("t, с"));
   // ui->qwtPlot->setAxisScale(QwtPlot::xBottom,-0.25,8.25);
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft,QString::fromLocal8Bit("U1, В"));
    ui->qwtPlot->setAxisTitle(QwtPlot::yRight,QString::fromLocal8Bit("U2, В"));
   // ui->qwtPlot->setAxisScale(QwtPlot::yLeft,-1.25,1.25);
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier( ui->qwtPlot->canvas());
    magnifier->setMouseButton(Qt::MidButton);
    QwtPlotPanner *d_panner = new QwtPlotPanner(ui->qwtPlot->canvas() );
    d_panner->setMouseButton( Qt::RightButton );

    curv1 = new QwtPlotCurve(QString("U1(t)"));
    curv1->setRenderHint(QwtPlotItem::RenderAntialiased);
    curv1->setPen(QPen(Qt::red));
    curv2 = new QwtPlotCurve(QString("U2(t)"));
    curv2->setRenderHint(QwtPlotItem::RenderAntialiased);
    curv2->setPen(QPen(Qt::green));
    curv2->setYAxis(QwtPlot::yRight);

    //QwtSymbol *symbol1 = new QwtSymbol();
    //symbol1->setStyle(QwtSymbol::Ellipse);
    //symbol1->setPen(QColor(Qt::black));
    //symbol1->setSize(4);
   // curv1->setSymbol(symbol1);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete leg;
    delete grid;
    delete curv1;
    delete curv2;
    delete magnifier;
    delete d_panner;
    delete m1;
}



void MainWindow::on_actionTest_triggered()
{
    QString str;
    QStringList strl;
    QString out;
    scope.fileNames = QFileDialog::getOpenFileNames(this,"Добавить файлы","/media/heavy/60CC-A3D9/b/", "Wave Form (*.wfm)");

    if (!scope.fileNames.isEmpty())
    {
        scope.readHeaders();
        for (int i=0;i<scope.fileNames.count();i++)
        {
            str=scope.fileNames.at(i);
    #ifdef Q_OS_WIN
            strl=str.split("\\");
    #endif
    #ifdef  Q_OS_LINUX
            strl=str.split("/");
    #endif
       ui->listWidget->addItem(strl.at(strl.count()-1));

       out=out+"Файл " + strl.at(strl.count()-1) + ":\n" +
               "Канал 1 вкл = " + (scope.ch1DataPresent.at(i) ? "true\n":"false\n")+
               "Канал 2 вкл = "+(scope.ch2DataPresent.at(i) ? "true\n":"false\n") +
               "Число точек = " + QString::number(scope.numberOfPoints.at(i))+"\n" +
               "Деление шкалы Х = " + QString::number(scope.timeMult.at(i)*0.000000000001) + "сек\n" +
               "Время задержки = " + QString::number(scope.delay.at(i))+"\n" +
               "Частота дискретизации = " + QString::number(scope.sampleRate.at(i))+"\n" +
               "Канал 1 делитель щупа = " + QString::number(scope.ch1ProbeDiv.at(i)) +"\n" +
               "Канал 1 масштаб по вертикали = " + QString::number(scope.ch1VerticalScale.at(i)*scope.ch1ProbeDiv.at(i)*0.000001) +"\n" +
               "Канал 1 позиция по вертикали = " + QString::number((scope.ch1VerticalPosition.at(i)/(25.0*scope.ch1ProbeDiv.at(i)))*(scope.ch1VerticalScale.at(i)*scope.ch1ProbeDiv.at(i)*0.000001)) +"\n" +
               "Канал 2 делитель щупа = " + QString::number(scope.ch2ProbeDiv.at(i)) +"\n" +
               "Канал 2 масштаб по вертикали = " + QString::number(scope.ch2VerticalScale.at(i)*scope.ch2ProbeDiv.at(i)*0.000001) +"\n" +
               "Канал 2 позиция по вертикали = " + QString::number((scope.ch2VerticalPosition.at(i)/(25.0*scope.ch2ProbeDiv.at(i)))*(scope.ch2VerticalScale.at(i)*scope.ch2ProbeDiv.at(i)*0.000001)) +"\n" + "\n"

               ;

        }
        ui->textBrowser->setText(out);


    }

}

void MainWindow::on_pushButton_clicked()
{
    QDataStream stream;
    QVector<double> yDataDouble(scope.numberOfPoints.at(0));
    QVector<double> yDataDouble2(scope.numberOfPoints.at(0));
    uchar filesCount=scope.fileNames.count();
    float scale2 = (scope.ch2VerticalScale.at(0)*scope.ch2ProbeDiv.at(0)*0.000001);
    float pos2 = (scope.ch2VerticalPosition.at(0)/(25.0*scope.ch2ProbeDiv.at(0)))*(scope.ch2VerticalScale.at(0)*scope.ch2ProbeDiv.at(0)*0.000001);
    float scale1 = (scope.ch1VerticalScale.at(0)*scope.ch1ProbeDiv.at(0)*0.000001);
    float pos1 = (scope.ch1VerticalPosition.at(0)/(25.0*scope.ch1ProbeDiv.at(0)))*(scope.ch1VerticalScale.at(0)*scope.ch1ProbeDiv.at(0)*0.000001);
    m1->setLinePen(QPen(Qt::black));
    m1->setLineStyle(QwtPlotMarker::HLine);
    m1->setValue(0,0);
    m1->attach(ui->qwtPlot);
    double Ts=0;
    stream.setByteOrder(QDataStream::LittleEndian);
    for (int i=0;i<filesCount;i++)
    {
        QFile file(scope.fileNames.at(i));
        if(file.open(QIODevice::ReadOnly))
        {
            stream.setDevice(&file);

            xData.resize(scope.numberOfPoints.at(0));
            yData.resize(scope.numberOfPoints.at(0));
            integral.resize(scope.numberOfPoints.at(0));
            Ts=1/scope.sampleRate.at(i);
            file.seek(272+scope.numberOfPoints.at(i));


            for (unsigned int j=0; j<scope.numberOfPoints.at(0);j++)
            {
                stream>>yData[j];
                xData[j]=j*Ts;

                yDataDouble2[j]= scale2*((((float)(128-(yData[j])))/256*10)) - pos2;
                //if (j) integral[j]=integral.at(j-1)+yDataDouble.at(j);
            }

            file.seek(272);

            for (unsigned int j=0; j<scope.numberOfPoints.at(0);j++)
            {
                stream>>yData[j];
                yDataDouble[j]= scale1*((((float)(128-(yData[j])))/256*10)) - pos1;

            }
             curv2->setYAxis(QwtPlot::yLeft);
             curv2->setSamples(xData,yDataDouble2);
             curv2->attach(ui->qwtPlot);
             curv1->setYAxis(QwtPlot::yRight);
            curv1->setSamples(xData,yDataDouble);
            curv1->attach(ui->qwtPlot);
            file.close();
        }
    }

    ui->qwtPlot->replot();
}
