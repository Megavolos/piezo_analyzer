#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
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
               "Канал 1 позиция по вертикали = " + QString::number((scope.ch1VerticalPosition.at(i)/(25*scope.ch1ProbeDiv.at(i)))*(scope.ch1VerticalScale.at(i)*scope.ch1ProbeDiv.at(i)*0.000001)) +"\n" +
               "Канал 2 делитель щупа = " + QString::number(scope.ch2ProbeDiv.at(i)) +"\n" +
               "Канал 2 масштаб по вертикали = " + QString::number(scope.ch1VerticalScale.at(i)*scope.ch2ProbeDiv.at(i)*0.000001) +"\n" +
               "Канал 2 позиция по вертикали = " + QString::number((scope.ch2VerticalPosition.at(i)/(25*scope.ch2ProbeDiv.at(i)))*(scope.ch2VerticalScale.at(i)*scope.ch2ProbeDiv.at(i)*0.000001)) +"\n" + "\n"

               ;

        }
        ui->textBrowser->setText(out);

    }

}
