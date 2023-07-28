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
    /*初始化界面*/
    void InitView();

    /*初始化数据*/
    void InitData();

    /*初始化信号和槽*/
    void InitConnect();

    /*刷新串口*/
    void RefreshSerial();

private slots:
    /*点击刷新串口*/
    void SlotRefreshSerial();

    /*点击打开串口按钮*/
    void SlotClickConnectSerialBtn();

    /*点击关闭串口按钮*/
    void SlotClickCloseSerialBtn();

private:
    Ui::MainWindow *ui;
    QSerialPort* m_pQSerialPort;
    QString m_serialportName;   //用于保存串口名

};
#endif // MAINWINDOW_H
