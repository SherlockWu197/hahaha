#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QByteArray>
#include <QMessageBox>
#include <cdatadisplayscreen.h>

const quint8 HEADER_HIGHTBYTE = 0xAA; // 消息头高字节
const quint8 HEADER_LOWBYTE = 0xBB; // 消息头低字节
const quint8 MSG_TYPE_DEVICE_ID = 0x00; // 消息类型：下位机设备ID
const quint8 MSG_TYPE_LIVE_DATA = 0x01; // 消息类型：实时数据
const quint8 MSG_TYPE_STORE_DATA = 0x05; // 消息类型，存储的数据
const quint8 MSG_TYPE_RESULT = 0xFF;   // 消息类型：握手结果

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
    void initView();

    /*初始化数据*/
    void initData();

    /*初始化信号和槽*/
    void initConnect();

    /*刷新串口*/
    void refreshSerial();

    /*请求设备ID号*/
    void requestDeviceID();

    /*请求下载数据*/
    void requsetDownloadData();


private slots:
    /*点击刷新串口*/
    void slotRefreshSerial();

    /*点击打开串口按钮*/
    void slotClickConnectSerialBtn();

    /*点击关闭串口按钮*/
    void slotClickCloseSerialBtn();


private:
    Ui::MainWindow *ui;
    QSerialPort* m_pQSerialPort;
    QString m_serialportName;   //用于保存串口名
    CDataDisplayScreen* m_pDatadisplayScreen;

};
#endif // MAINWINDOW_H
