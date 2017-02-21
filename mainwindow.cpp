#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);                                          //создание интерфейса MainWindow и всяких кнопок

    leg = new QwtLegend();                                      //легенда
    leg->setDefaultItemMode(QwtLegendData::ReadOnly);           //нельзя редактировать легенду
    ui->qwtPlot->setTitle("Данные из файлов");                  //далее интиуитивно понятно
    ui->qwtPlot->insertLegend(leg,QwtPlot::TopLegend);
    ui->qwtPlot->setCanvasBackground(Qt::black);
    grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->setMajorPen(QPen(Qt::white,1,Qt::DotLine));
    grid->setMinorPen(QPen(Qt::gray,1,Qt::DotLine));

    grid->attach(ui->qwtPlot);
    ui->qwtPlot->enableAxis(QwtPlot::yRight);
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom,QString::fromLocal8Bit("t, с"));
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft,QString::fromLocal8Bit("U1, В"));
    ui->qwtPlot->setAxisTitle(QwtPlot::yRight,QString::fromLocal8Bit("U2, В"));

    QwtPlotMagnifier *zoom_x = new QwtPlotMagnifier( ui->qwtPlot->canvas());
    QwtPlotMagnifier *zoom_y1 = new QwtPlotMagnifier( ui->qwtPlot->canvas() );
   // QwtPlotMagnifier *zoom_y2 = new QwtPlotMagnifier( ui->qwtPlot->canvas() );
    zoom_x->setMouseButton(Qt::MidButton);
    zoom_x->setWheelModifiers(Qt::ShiftModifier);
    zoom_x->setAxisEnabled(QwtPlot::xBottom, true);
    zoom_x->setAxisEnabled(QwtPlot::yRight ,false);
    zoom_x->setAxisEnabled(QwtPlot::yLeft ,false);
    zoom_y1->setMouseButton(Qt::MidButton);
    zoom_y1->setWheelModifiers(Qt::ControlModifier);
    zoom_y1->setAxisEnabled(QwtPlot::xBottom,false);
    zoom_y1->setAxisEnabled(QwtPlot::yRight,true);
    zoom_y1->setAxisEnabled(QwtPlot::yLeft,true);

    QwtPlotPanner *d_panner = new QwtPlotPanner(ui->qwtPlot->canvas() );
    d_panner->setMouseButton( Qt::RightButton );

    curv1 = new QwtPlotCurve(QString("U1(t)"));                 //создание кривых
    curv1->setRenderHint(QwtPlotItem::RenderAntialiased);
    curv1->setPen(QPen(Qt::red));
    curv2 = new QwtPlotCurve(QString("U2(t)"));
    curv2->setRenderHint(QwtPlotItem::RenderAntialiased);
    curv2->setPen(QPen(Qt::green));
    curv2->setYAxis(QwtPlot::yRight);

    m1=new QwtPlotMarker;



}

MainWindow::~MainWindow()
{


    delete ui;

    delete leg;
    delete grid;
    delete curv1;
    delete curv2;
    //delete zoom_x;
    //delete zoom_y1;
    //delete magnifier;
    delete m1;

}





void MainWindow::on_lpf1_enable_toggled(bool checked)
{
    if (checked)
    {
        ui->coeff1->setEnabled(true);
    }
    else
    {
        ui->coeff1->setEnabled(false);
    }
}

void MainWindow::on_lpf2_enable_toggled(bool checked)
{
    if (checked)
    {
        ui->coeff2->setEnabled(true);
    }
    else
    {
        ui->coeff2->setEnabled(false);
    }
}

void MainWindow::on_addFileButton_triggered()
{
    QString str;
    QStringList strl;
    QString out;
    scope.fileNames = QFileDialog::getOpenFileNames(this,"Добавить файлы","/media/heavy/60CC-A3D9/b/", "Wave Form (*.wfm)"); //открываем файловый диалог и имена файлов загоняем в scope.filenames

    if (!scope.fileNames.isEmpty())     //если список не пуст
    {
        scope.readHeaders();            //читаем заголовки
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
               "Канал 1 вкл = " +                    (scope.ch1DataPresent.at(i) ? "true\n":"false\n")+
               "Канал 2 вкл = " +                    (scope.ch2DataPresent.at(i) ? "true\n":"false\n") +
               "Число точек = " +                    QString::number(scope.numberOfPoints.at(i))+"\n" +
               "Деление шкалы Х = " +                QString::number(scope.timeMult.at(i)) + "сек\n" +
               "Время задержки = " +                 QString::number(scope.delay.at(i))+"\n" +
               "Частота дискретизации = " +          QString::number(scope.sampleRate.at(i))+"\n" +
               "Канал 1 делитель щупа = " +          QString::number(scope.ch1ProbeDiv.at(i)) +"\n" +
               "Канал 1 масштаб по вертикали = " +   QString::number(scope.ch1VerticalScale.at(i)) +"\n" +
               "Канал 1 позиция по вертикали = " +   QString::number(scope.ch1VerticalPosition.at(i)) +"\n" +
               "Канал 2 делитель щупа = " +          QString::number(scope.ch2ProbeDiv.at(i)) +"\n" +
               "Канал 2 масштаб по вертикали = " +   QString::number(scope.ch2VerticalScale.at(i)) +"\n" +
               "Канал 2 позиция по вертикали = " +   QString::number(scope.ch2VerticalPosition.at(i)) +"\n" + "\n";
            curves.append(new QwtPlotCurve(QString::number(i)) );
            curves.append(new QwtPlotCurve(QString::number(i)) );
            curves.at(2*i)->setRenderHint(QwtPlotItem::RenderAntialiased);

            curves.at(2*i+1)->setRenderHint(QwtPlotItem::RenderAntialiased);

            switch (i) {
            case 0:
                curves.at(0)->setPen(QPen(Qt::red,2,Qt::DashLine));
                curves.at(1)->setPen(QPen(Qt::red,1,Qt::SolidLine));
                break;
            case 1:
                curves.at(2)->setPen(QPen(Qt::blue,2,Qt::DashLine));
                curves.at(3)->setPen(QPen(Qt::blue,1,Qt::SolidLine));
                break;
            case 2:
                curves.at(4)->setPen(QPen(Qt::yellow,Qt::DashLine));
                curves.at(5)->setPen(QPen(Qt::yellow,1,Qt::SolidLine));
                break;
            case 3:
                curves.at(6)->setPen(QPen(Qt::white,2,Qt::DashLine));
                curves.at(7)->setPen(QPen(Qt::white,1,Qt::SolidLine));
                break;
            case 4:
                curves.at(8)->setPen(QPen(Qt::green,2,Qt::DashLine));
                curves.at(9)->setPen(QPen(Qt::green,1,Qt::SolidLine));
                break;
            case 5:
                curves.at(10)->setPen(QPen(Qt::magenta,2,Qt::DashLine));
                curves.at(11)->setPen(QPen(Qt::magenta,1,Qt::SolidLine));
                break;
            }

        }
        ui->textBrowser->setText(out);


    }

}

void MainWindow::on_draw_button_clicked()
{
    QDataStream stream;
    bool ok;

    uchar filesCount=scope.fileNames.count();

    m1->setLinePen(QPen(Qt::black));
    m1->setLineStyle(QwtPlotMarker::HLine);
    m1->setValue(0,0);
    m1->attach(ui->qwtPlot);
    double Ts=0;
    stream.setByteOrder(QDataStream::LittleEndian);
    for (int i=0;i<filesCount;i++)
    {
        QFile file(scope.fileNames.at(i));
        QVector<QVector<double>> yData;       //эти векторы будут содержать данные по оси Y для построения графиков


        if(file.open(QIODevice::ReadOnly))
        {
            yData.resize(filesCount*2);
            yData[2*i].resize(scope.numberOfPoints.at(i));
            yData[2*i+1].resize(scope.numberOfPoints.at(i));
            stream.setDevice(&file);                                    //готовим поток
            xData.clear();
            xData.resize(scope.numberOfPoints.at(i));                   //готовим X

            Ts=1/scope.sampleRate.at(i);                                //расчитываем период дескритизации

            for (unsigned int j=0; j<scope.numberOfPoints.at(i); j++)
            {
                xData[j]=j*Ts;                                          //расчитываем значения по X
            }
//проход по кнопкам. можно красивей сделать проход, но сейчас болит нога, и мозги в кучу заплетаются.
            if (ui->data1_mems->isChecked())
            {
                yData[2*i]=scope.recalcSamples(i,1);                      //переводим семплы в вольты для 1 канала
                if (ui->lpf1_enable->isChecked())
                {
                     yData[2*i] = scope.filter(&yData[2*i],ui->coeff1->text().toFloat(&ok));            //добавим фильтр на канал 1
                }
            }
            if (ui->data2_mems->isChecked())
            {
                yData[2*i+1]=scope.recalcSamples(i,1);
                if (ui->lpf2_enable->isChecked())
                {
                    yData[2*i+1] = scope.filter(&yData[2*i+1],ui->coeff2->text().toFloat(&ok));            //добавим фильтр на канал 2
                }
            }
            if (ui->data1_piezo->isChecked())
            {
                yData[2*i]=scope.recalcSamples(i,2);
                if (ui->lpf1_enable->isChecked())
                {
                    yData[2*i] = scope.filter(&yData[2*i],ui->coeff1->text().toFloat(&ok));            //добавим фильтр на канал 1
                }
            }
            if (ui->data2_piezo->isChecked())
            {
                yData[2*i+1]=scope.recalcSamples(i,2);
                if (ui->lpf2_enable->isChecked())
                {
                    yData[2*i+1] = scope.filter(&yData[2*i+1],ui->coeff2->text().toFloat(&ok));            //добавим фильтр на канал 1
                }
            }
            if (ui->data1_integrate->isChecked())
            {
                yData[2*i]=scope.recalcSamples(i,2);
                if (ui->lpf1_enable->isChecked())
                {
                    yData[2*i] = scope.filter(&yData[2*i],ui->coeff1->text().toFloat(&ok));            //добавим фильтр на канал 1
                }
                yData[2*i]=scope.integrate(&yData[2*i],ui->zeroLevel1->text().toFloat());
            }
            if (ui->data2_integrate->isChecked())
            {
                yData[2*i+1]=scope.recalcSamples(i,2);
                if (ui->lpf2_enable->isChecked())
                {
                    yData[2*i+1] = scope.filter(&yData[2*i+1],ui->coeff2->text().toFloat(&ok));            //добавим фильтр на канал 2
                }
                yData[2*i+1]=scope.integrate(&yData[2*i+1],ui->zeroLevel1->text().toFloat());
            }

            curves.at(2*i)->setYAxis(QwtPlot::yLeft);
            //curv2->setYAxis(QwtPlot::yLeft);
            curves.at(2*i)->setSamples(xData,yData[2*i]);
            curves.at(2*i)->attach(ui->qwtPlot);
            curves.at(2*i+1)->setYAxis(QwtPlot::yRight);
            curves.at(2*i+1)->setSamples(xData,yData[2*i+1]);
            curves.at(2*i+1)->attach(ui->qwtPlot);
            ui->qwtPlot->setAxisAutoScale(QwtPlot::yLeft,true);
            ui->qwtPlot->setAxisAutoScale(QwtPlot::yRight,true);
            file.close();
        }
    }

    ui->qwtPlot->replot();

}
