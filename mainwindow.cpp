#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    InitData();
    InitView();
    InitConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitView()
{
    QApplication::setApplicationDisplayName("UT343D V1.02");

}

void MainWindow::InitData()
{

}

void MainWindow::InitConnect()
{

}

