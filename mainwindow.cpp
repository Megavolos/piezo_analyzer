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
    scope.readHeaders();
}
