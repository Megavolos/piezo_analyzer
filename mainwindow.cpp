#include "mainwindow.h"
#include "ui_mainwindow.h"
void MainWindow::setDataSource(QString source)
{
    dataSource=source;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->actionStartRS232->setEnabled(false);
    ui->actionStopRS232->setEnabled(false);
    //создание интерфейса MainWindow и всяких кнопок
    setDataSource(ui->filesRadioButton->text());
    portopened = false;
    startRecieved = 0;
    leg = new QwtLegend();                                      //легенда
    leg->setDefaultItemMode(QwtLegendData::ReadOnly);           //нельзя редактировать легенду
    leg1 = new QwtLegend();                                      //легенда
    leg1->setDefaultItemMode(QwtLegendData::ReadOnly);
    leg2 = new QwtLegend();                                      //легенда
    leg2->setDefaultItemMode(QwtLegendData::ReadOnly);
    ui->qwtPlot->setTitle("Данные из файлов");                  //далее интиуитивно понятно
    ui->qwtPlot->insertLegend(leg,QwtPlot::TopLegend);
    ui->qwtPlot->setCanvasBackground(Qt::black);
    ui->qwtPlot_data1->setTitle("Данные из файлов");                  //далее интиуитивно понятно
    ui->qwtPlot_data1->insertLegend(leg1,QwtPlot::TopLegend);
    ui->qwtPlot_data1->setCanvasBackground(Qt::black);
   // ui->qwtPlot_data2->setTitle("Данные из файлов");                  //далее интиуитивно понятно
   // ui->qwtPlot_data2->insertLegend(leg2,QwtPlot::TopLegend);
    ui->qwtPlot_data2->setCanvasBackground(Qt::black);
    grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->setMajorPen(QPen(Qt::white,1,Qt::DotLine));
    grid->setMinorPen(QPen(Qt::gray,1,Qt::DotLine));

    grid1 = new QwtPlotGrid;
    grid1->enableXMin(true);
    grid1->setMajorPen(QPen(Qt::white,1,Qt::DotLine));
    grid1->setMinorPen(QPen(Qt::gray,1,Qt::DotLine));

    grid2 = new QwtPlotGrid;
    grid2->enableXMin(true);
    grid2->setMajorPen(QPen(Qt::white,1,Qt::DotLine));
    grid2->setMinorPen(QPen(Qt::gray,1,Qt::DotLine));

    grid->attach(ui->qwtPlot);
    grid1->attach(ui->qwtPlot_data1);
    grid2->attach(ui->qwtPlot_data2);

    ui->qwtPlot->enableAxis(QwtPlot::yRight);
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom,QString::fromLocal8Bit("t, с"));
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft,QString::fromLocal8Bit("U1, В"));
    ui->qwtPlot->setAxisTitle(QwtPlot::yRight,QString::fromLocal8Bit("U2, В"));

    //ui->qwtPlot_data1->enableAxis(QwtPlot::yRight);
  //  ui->qwtPlot_data1->setAxisTitle(QwtPlot::xBottom,QString::fromLocal8Bit("t, с"));
    ui->qwtPlot_data1->setAxisTitle(QwtPlot::yLeft,QString::fromLocal8Bit("U1, В"));
  //  ui->qwtPlot_data1->setAxisTitle(QwtPlot::yRight,QString::fromLocal8Bit("U2, В"));

    //ui->qwtPlot_data2->enableAxis(QwtPlot::yRight);
    ui->qwtPlot_data2->setAxisTitle(QwtPlot::xBottom,QString::fromLocal8Bit("t, с"));
    //ui->qwtPlot_data2->setAxisTitle(QwtPlot::yLeft,QString::fromLocal8Bit("U1, В"));
   // ui->qwtPlot_data2->setAxisTitle(QwtPlot::yRight,QString::fromLocal8Bit("U2, В"));

    QwtPlotMagnifier *zoom_x = new QwtPlotMagnifier( ui->qwtPlot->canvas());
    QwtPlotMagnifier *zoom_y = new QwtPlotMagnifier( ui->qwtPlot->canvas() );

    QwtPlotMagnifier *zoom_x1 = new QwtPlotMagnifier( ui->qwtPlot_data1->canvas());
    QwtPlotMagnifier *zoom_y1 = new QwtPlotMagnifier( ui->qwtPlot_data1->canvas() );

    QwtPlotMagnifier *zoom_x2 = new QwtPlotMagnifier( ui->qwtPlot_data2->canvas());
    QwtPlotMagnifier *zoom_y2 = new QwtPlotMagnifier( ui->qwtPlot_data2->canvas() );
   // QwtPlotMagnifier *zoom_y2 = new QwtPlotMagnifier( ui->qwtPlot->canvas() );
    zoom_x->setMouseButton(Qt::MidButton);
    zoom_x->setWheelModifiers(Qt::ShiftModifier);
    zoom_x->setAxisEnabled(QwtPlot::xBottom, true);
    zoom_x->setAxisEnabled(QwtPlot::yRight ,false);
    zoom_x->setAxisEnabled(QwtPlot::yLeft ,false);
    zoom_y->setMouseButton(Qt::MidButton);
    zoom_y->setWheelModifiers(Qt::ControlModifier);
    zoom_y->setAxisEnabled(QwtPlot::xBottom,false);
    zoom_y->setAxisEnabled(QwtPlot::yRight,true);
    zoom_y->setAxisEnabled(QwtPlot::yLeft,true);

    zoom_x1->setMouseButton(Qt::MidButton);
    zoom_x1->setWheelModifiers(Qt::ShiftModifier);
    zoom_x1->setAxisEnabled(QwtPlot::xBottom, true);
    zoom_x1->setAxisEnabled(QwtPlot::yRight ,false);
    zoom_x1->setAxisEnabled(QwtPlot::yLeft ,false);
    zoom_y1->setMouseButton(Qt::MidButton);
    zoom_y1->setWheelModifiers(Qt::ControlModifier);
    zoom_y1->setAxisEnabled(QwtPlot::xBottom,false);
    zoom_y1->setAxisEnabled(QwtPlot::yRight,true);
    zoom_y1->setAxisEnabled(QwtPlot::yLeft,true);

    zoom_x2->setMouseButton(Qt::MidButton);
    zoom_x2->setWheelModifiers(Qt::ShiftModifier);
    zoom_x2->setAxisEnabled(QwtPlot::xBottom, true);
    zoom_x2->setAxisEnabled(QwtPlot::yRight ,false);
    zoom_x2->setAxisEnabled(QwtPlot::yLeft ,false);
    zoom_y2->setMouseButton(Qt::MidButton);
    zoom_y2->setWheelModifiers(Qt::ControlModifier);
    zoom_y2->setAxisEnabled(QwtPlot::xBottom,false);
    zoom_y2->setAxisEnabled(QwtPlot::yRight,true);
    zoom_y2->setAxisEnabled(QwtPlot::yLeft,true);

    QwtPlotPanner *d_panner = new QwtPlotPanner(ui->qwtPlot->canvas() );
    QwtPlotPanner *d_panner1 = new QwtPlotPanner(ui->qwtPlot_data1->canvas() );
    QwtPlotPanner *d_panner2 = new QwtPlotPanner(ui->qwtPlot_data2->canvas() );

    d_panner->setMouseButton( Qt::RightButton );
    d_panner1->setMouseButton( Qt::RightButton );
    d_panner2->setMouseButton( Qt::RightButton );




    m1=new QwtPlotMarker;
    m1->setLinePen(QPen(Qt::black));
    m1->setLineStyle(QwtPlotMarker::HLine);
    m1->setValue(0,0);
    m1->attach(ui->qwtPlot);

    serialPorts=serialPortInfo.availablePorts();
    for (const QSerialPortInfo &info: serialPorts )
    {
        ui->portsBox->addItem(info.portName());
    }
    for (const qint32 &bauds: QSerialPortInfo::standardBaudRates() )
    {
        ui->speedBox->addItem(QString::number(bauds));
    }
    QThread *thread_New = new QThread;//Создаем поток для порта платы
    Port *PortNew = new Port();//Создаем обьект по классу
    PortNew->moveToThread(thread_New);//помешаем класс  в поток
    PortNew->thisPort.moveToThread(thread_New);//Помещаем сам порт в поток
    connect(PortNew, SIGNAL(error_(QString)), this, SLOT(Print(QString)));//Лог ошибок
    connect(thread_New, SIGNAL(started()), PortNew, SLOT(process_Port()));//Переназначения метода run
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(quit()));//Переназначение метода выход
    connect(thread_New, SIGNAL(finished()), PortNew, SLOT(deleteLater()));//Удалить к чертям поток
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(deleteLater()));//Удалить к чертям поток
    connect(this,SIGNAL(savesettings(QString,int,int,int,int,int)),PortNew,SLOT(Write_Settings_Port(QString,int,int,int,int,int)));//Слот - ввод настроек!
    connect(ui->actionStartRS232, SIGNAL(triggered(bool)),PortNew,SLOT(ConnectPort()));
    connect(ui->actionStopRS232, SIGNAL(triggered(bool)),PortNew,SLOT(DisconnectPort()));
    connect(PortNew, SIGNAL(outPort(QByteArray)), this, SLOT(Print(QByteArray)));//Лог ошибок
    connect(this,SIGNAL(writeData(QByteArray)),PortNew,SLOT(WriteToPort(QByteArray)));
    thread_New->start();


}
void MainWindow::processSamples()
{
    
    curves.at(0)->setYAxis(QwtPlot::yRight);
    curves.at(0)->setSamples(xData,ch0);
    curves.at(0)->attach(ui->qwtPlot);
    ui->qwtPlot->setAxisAutoScale(QwtPlot::yRight,true);
    ch0.clear();
    ch1.clear();
    ch2.clear();
    ch3.clear();
    ui->qwtPlot->replot();
}

void MainWindow::Print(QByteArray data)
{
    bool ok;
    if (portopened&&startRecieved<4)
    {
        for (int i=0; i<data.size();i++)
        {
            if (data.at(i)==0x11 || data.at(i)==0x12 || data.at(i)==0x13 || data.at(i)==0x14)
            {
                if (data.at(i)==0x12)
                {
                    uchar w;
                    w=1;
                }
                startRecieved++;
                data.remove(i,1);
                i--;
                if (startRecieved==4) break;
            }

        }

    }
    if (startRecieved==4)
    {
    test++;
    if (test==4)
    {
        test=4;
    }
        for (int i=0; i<data.size();i++)
        {
            switch (channelSwitch)  {
            case 0:
                ch0.append(data.at(i));
                receivedBytes++;
                break;
            case 1:
                ch1.append(data.at(i));
                break;
            case 2:
                ch2.append(data.at(i));
                break;
            case 3:
                ch3.append(data.at(i));
                if (receivedBytes==ui->widthLineEdit->text().toInt(&ok)) //считан последний байт пакета + количество полученных байт равно заданному в поле ввода
                {
                    processSamples();
                }
                break;
            }
            channelSwitch++;

            if (channelSwitch==4) channelSwitch=0;
        }

    }
   // ui->consol->textCursor().insertText(data+'\r'); // Вывод текста в консоль
   // ui->consol->moveCursor(QTextCursor::End);//Scroll
}
MainWindow::~MainWindow()
{


    delete ui;

    delete leg;
    delete grid;
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
    emptyItems=0;
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



        }
        ui->textBrowser->setText(out);


    }

}
void MainWindow::setCurvesStyle(uchar i)
{
    switch (i) {
    case 0:
        curves.at(0)->setPen(QPen(Qt::red,2,Qt::DashLine));
        curves.at(1)->setPen(QPen(Qt::darkRed ,2,Qt::SolidLine));
        break;
    case 1:
        curves.at(2)->setPen(QPen(Qt::blue,2,Qt::DashLine));
        curves.at(3)->setPen(QPen(Qt::darkBlue,2,Qt::SolidLine));
        break;
    case 2:
        curves.at(4)->setPen(QPen(Qt::yellow,2,Qt::DashLine));
        curves.at(5)->setPen(QPen(Qt::darkYellow,1,Qt::SolidLine));
        break;
    case 3:
        curves.at(6)->setPen(QPen(Qt::white,2,Qt::DashLine));
        curves.at(7)->setPen(QPen(Qt::lightGray,2,Qt::SolidLine));
        break;
    case 4:
        curves.at(8)->setPen(QPen(Qt::green,2,Qt::DashLine));
        curves.at(9)->setPen(QPen(Qt::darkGreen ,2,Qt::SolidLine));
        break;
    case 5:
        curves.at(10)->setPen(QPen(Qt::magenta,2,Qt::DashLine));
        curves.at(11)->setPen(QPen(Qt::darkMagenta,2,Qt::SolidLine));
        break;
    }
}

void MainWindow::readDataFromFiles()
{
    double Ts=0;
    bool ok;
    QDataStream stream;
    uchar filesCount=scope.fileNames.count();
    setCurvesStyle(filesCount-1);
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
}

void MainWindow::readDataFromRS232()
{
    QByteArray d;
    d.append('3');
    if (!portopened)
    {
        portopened=true;
        emit writeData(d);
        ui->draw_button->setText("Стоп");
    }
    else
    {
         portopened=false;
         d[0]='0';
         emit writeData(d);
         ui->draw_button->setText("Старт");
    }
    double Ts=0;
    bool ok;
    QVector<QVector<double>> yData;
    curves.append(new QwtPlotCurve("MEMS1_rs232"));
    curves.append(new QwtPlotCurve("PIEZO1_rs232"));
    curves.append(new QwtPlotCurve("MEMS2_rs232"));
    curves.append(new QwtPlotCurve("PIEZO2_rs232"));
    curves.at(0)->setRenderHint(QwtPlotItem::RenderAntialiased);
    curves.at(1)->setRenderHint(QwtPlotItem::RenderAntialiased);
    curves.at(2)->setRenderHint(QwtPlotItem::RenderAntialiased);
    curves.at(3)->setRenderHint(QwtPlotItem::RenderAntialiased);
    curves.at(0)->attach(ui->qwtPlot);
    curves.at(1)->attach(ui->qwtPlot);
    curves.at(2)->attach(ui->qwtPlot);
    curves.at(3)->attach(ui->qwtPlot);
    setCurvesStyle(0);
    setCurvesStyle(1);
    xData.clear();
    xData.resize(ui->widthLineEdit->text().toInt());
    for (int i=0;i<ui->widthLineEdit->text().toInt();i++)
    {
        xData[i]=i;
    }
}


void MainWindow::on_draw_button_clicked()
{


    if (dataSource==ui->filesRadioButton->text())
    {
        if (emptyItems==1)
        {
            msgBox.setText("Необходимо снова добавить файлы");
            if(msgBox.exec()==0x400) return;

        }
        readDataFromFiles();
    }
    else
    {
        readDataFromRS232();
    }



    ui->qwtPlot->replot();

}

void MainWindow::on_draw_button2_clicked()
{
    if (emptyItems==1)
    {
        msgBox.setText("Необходимо снова добавить файлы");
        if(msgBox.exec()==0x400) return;

    }
    QDataStream stream;
    bool ok;

    uchar filesCount=scope.fileNames.count();

    double Ts=0;
    stream.setByteOrder(QDataStream::LittleEndian);
    for (int i=0;i<filesCount;i++)
    {


        switch (i) {
        case 0:
            curves.at(0)->setPen(QPen(Qt::red,2,Qt::SolidLine));
            curves.at(1)->setPen(QPen(Qt::red ,2,Qt::SolidLine));
            break;
        case 1:
            curves.at(2)->setPen(QPen(Qt::blue,2,Qt::SolidLine));
            curves.at(3)->setPen(QPen(Qt::blue,2,Qt::SolidLine));
            break;
        case 2:
            curves.at(4)->setPen(QPen(Qt::yellow,2,Qt::SolidLine));
            curves.at(5)->setPen(QPen(Qt::yellow,1,Qt::SolidLine));
            break;
        case 3:
            curves.at(6)->setPen(QPen(Qt::white,2,Qt::SolidLine));
            curves.at(7)->setPen(QPen(Qt::white,2,Qt::SolidLine));
            break;
        case 4:
            curves.at(8)->setPen(QPen(Qt::green,2,Qt::SolidLine));
            curves.at(9)->setPen(QPen(Qt::green ,2,Qt::SolidLine));
            break;
        case 5:
            curves.at(10)->setPen(QPen(Qt::magenta,2,Qt::SolidLine));
            curves.at(11)->setPen(QPen(Qt::magenta,2,Qt::SolidLine));
            break;
        }
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
            curves.at(2*i)->attach(ui->qwtPlot_data1);
            curves.at(2*i+1)->setYAxis(QwtPlot::yLeft);
            curves.at(2*i+1)->setSamples(xData,yData[2*i+1]);
            curves.at(2*i+1)->attach(ui->qwtPlot_data2);
            ui->qwtPlot_data1->setAxisAutoScale(QwtPlot::yLeft,true);
            ui->qwtPlot_data2->setAxisAutoScale(QwtPlot::yLeft,true);
           // ui->qwtPlot_data1->axisWidget()
            file.close();
        }
    }

    ui->qwtPlot_data1->replot();
    ui->qwtPlot_data2->replot();


}


//void MainWindow::on_startButton_clicked()
//{

//}

void MainWindow::on_savePortSettingsButton_clicked()
{
    savesettings(ui->portsBox->currentText(), ui->speedBox->currentText().toInt(),8,0,0,0);


}


void MainWindow::on_filesRadioButton_toggled(bool checked)
{
   if (checked)
   {
       setDataSource(ui->filesRadioButton->text());
       ui->addFileButton->setEnabled(true);
       ui->draw_button->setText("Нарисовать");
       ui->actionStartRS232->setEnabled(false);
       ui->actionStopRS232->setEnabled(false);
   }
   else
   {
       setDataSource(ui->RS232radioButton->text());
       ui->actionStartRS232->setEnabled(true);
       ui->actionStopRS232->setEnabled(true);
       ui->addFileButton->setEnabled(false);
       if (portopened)
       {
           ui->draw_button->setText("Стоп");
       }
       else
       {
           ui->draw_button->setText("Старт");
       }

   }
}

void MainWindow::on_RS232radioButton_toggled(bool checked)
{
    if (checked)
    {
        ui->actionStartRS232->setEnabled(true);
        ui->actionStopRS232->setEnabled(true);
        QVector<QVector<double>> yData;
        setDataSource(ui->RS232radioButton->text());
        ui->addFileButton->setEnabled(false);
        ui->qwtPlot->detachItems(QwtPlotItem::Rtti_PlotCurve,true);
        ui->qwtPlot_data1->detachItems(QwtPlotItem::Rtti_PlotCurve,true);
        ui->qwtPlot_data2->detachItems(QwtPlotItem::Rtti_PlotCurve,true);
        curves.clear();
        ui->qwtPlot->replot();
        ui->qwtPlot_data1->replot();
        ui->qwtPlot_data2->replot();

        emptyItems=1;
        if (portopened)
        {
            ui->draw_button->setText("Стоп");
        }
        else
        {
            ui->draw_button->setText("Старт");
        }
    }
    else
    {
        ui->draw_button->setText("Нарисовать");
        ui->actionStartRS232->setEnabled(false);
        ui->actionStopRS232->setEnabled(false);
        setDataSource(ui->filesRadioButton->text());
        ui->addFileButton->setEnabled(true);
    }
}
