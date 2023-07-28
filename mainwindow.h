#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSerialPortInfo>
#include <QSerialPort>
#define NUM_ONE 0x01

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void InitView();
    void InitData();
    void InitConnect();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
