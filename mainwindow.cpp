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


    headers.fileNames = QFileDialog::getOpenFileNames(this,"Добавить файлы","/media/", "Wave Form (*.wfm)");
    ui->listWidget->addItems(headers.fileNames);
    QFile file(headers.fileNames.at(0));
    if(file.open(QIODevice::ReadOnly))
    {
        file.seek(49);

        ui->textBrowser->setText((file.read(1).at(0)==1) ? "true\n": "false\n");
        file.seek(numberOfPointsOffset);
        ui->textBrowser->setText();
        file.close();
    }
}
