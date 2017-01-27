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

void MainWindow::on_pushButton_clicked()
{
    unsigned int data;
    QByteArray temp;
    scope.fileNames = QFileDialog::getOpenFileNames(this,"Добавить файлы","/media/heavy/60CC-A3D9/", "Wave Form (*.wfm)");
    ui->listWidget->addItems(scope.fileNames);
    QFile file(scope.fileNames.at(0));
    if(file.open(QIODevice::ReadOnly))
    {
        file.seek(headers::ch1DataPresentOffset);

        ui->textBrowser->setText((file.read(1).at(0)==1) ? "true\n": "false\n");

        file.seek(headers::numberOfPointsOffset);
        temp = file.read(4);
        for (unsigned int i=4; i>0; i--)
        {
            data|=temp.at(i);
            data=data<<8;
        }

        //ui->textBrowser->setText();
        file.close();
    }
}
