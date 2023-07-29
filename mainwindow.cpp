#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initView();
    initData();
    initConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initView()
{
    /*初始化按钮状态为失能状态*/
    ui->btn_OpenSerial->setEnabled(false);
    ui->btn_CloseSerial->setEnabled(false);

    ui->tabWidget->setTabText(0, QString(tr("Table")));
    ui->tabWidget->setTabText(1, QString(tr("Histogram")));
    ui->tabWidget->setTabText(2, QString(tr("Chart")));

}

void MainWindow::initData()
{
    m_pQSerialPort = new QSerialPort();
    m_pDatadisplayScreen = new CDataDisplayScreen(ui->textBrowser);

    header.append(0xAA); // 消息头高字节
    header.append(0xBB); // 消息头低字节

    refreshSerial();
}

void MainWindow::initConnect()
{
    connect(ui->btn_refreshSerial,&QPushButton::clicked, this, &MainWindow::slotRefreshSerial);
    connect(ui->btn_OpenSerial,&QPushButton::clicked, this, &MainWindow::slotClickConnectSerialBtn);
    connect(ui->btn_CloseSerial,&QPushButton::clicked, this, &MainWindow::slotClickCloseSerialBtn);
    connect(ui->btn_RequestID, &QPushButton::clicked, this, &MainWindow::requestDeviceID);
//    connect(ui->btn_RealTimeMeasure, &QPushButton::clicked, this, &MainWindow::requestRealTimeData);
    connect(ui->btn_SendData, &QPushButton::clicked, this, [=](){
        QByteArray temByteArray;
        QString tempString = ui->lineEdit->text();
        temByteArray = tempString.toUtf8();
        qDebug() << tempString;
        qDebug() << temByteArray;

    });
}

void MainWindow::refreshSerial()
{
    ui->btn_OpenSerial->setEnabled(false);
    ui->btn_CloseSerial->setEnabled(false);

    /*当combox为非空时，清除里面的item*/
    if(ui->comboBox_Serial->count() != 0)
    {
        ui->comboBox_Serial->clear();
    }

    /*对可用的串口进行查询*/
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        //测试串口是否可用
        QSerialPort qserial(info);

        /*如果当前串口为空闲*/
        if(qserial.open(QIODevice::ReadWrite))
        {
            ui->comboBox_Serial->addItem(info.portName());
            ui->btn_OpenSerial->setEnabled(true);
            qserial.close();
        }

        else
        {
            ui->comboBox_Serial->addItem(info.portName()+"(被占用)");
        }
    }
}

void MainWindow::requestDeviceID()
{
    // UT343D协议消息：设备ID请求
    QByteArray deviceIdRequest;
    deviceIdRequest.append(0x31); // 消息类型
    deviceIdRequest.append(0x30); // 上位机请求设备ID
    deviceIdRequest.append(0x01); // 校验码1
    deviceIdRequest.append(0xCA); // 校验码2

    // UT343D协议消息长度
    QByteArray msgLength;
    msgLength.append(deviceIdRequest.length());

    // 发送设备ID请求消息到串口
    QByteArray requestPacket = header + msgLength + deviceIdRequest;

    qDebug() << requestPacket;
    qDebug() << requestPacket.toHex();
    qDebug() << deviceIdRequest.length();

    int bytesWritten = m_pQSerialPort->write(requestPacket);

    if (bytesWritten == -1)
    {
        qWarning() << "向串口写入数据失败：" << m_pQSerialPort->errorString();
    }
    else
    {
        qDebug() << "已发送设备ID请求。";
    }

    /*读取串口的响应数据*/
    QByteArray responseData;

    while(m_pQSerialPort->waitForReadyRead(3000))
    {
        responseData.append(m_pQSerialPort->readAll());
    }

    /*解析响应数据*/
    if (!responseData.isEmpty())
    {
        // TODO：根据UT343D协议处理接收到的数据
        qDebug() << "收到的数据：" << responseData.toHex();
    }
    else
    {
        qWarning() << "未从串口接收到响应数据。";
    }

}

void MainWindow::handleResultData(const QByteArray &data)
{
    QByteArray resultData = data;

    qDebug() << "your result data is:" << data/*.toHex()*/;

    /*解析响应数据*/
    if(!resultData.isEmpty())
    {
        /*下位机设备ID*/
        if(resultData.at(2) == 0x00)
        {

        }

        /*实时数据*/
        if(resultData.at(2) == 0x01)
        {
            qDebug() << "you have entred realTime Mode";
        }

        /*已存储的数据*/
        if(resultData.at(2) == 0x05)
        {

        }

        /*握手结果*/
        if(resultData.at(2) == 0xFF)
        {

        }
    }

}

void MainWindow::handleRealTimeData(const QByteArray& data)
{
//    LiveDataMessage* liveDataMessage = (LiveDataMessage*)data.constData();
}

void MainWindow::slotRefreshSerial()
{
    initView();

    slotClickCloseSerialBtn();

}

void MainWindow::slotClickConnectSerialBtn()
{
    m_pQSerialPort = new QSerialPort(ui->comboBox_Serial->currentText(),this);//打开串口

    m_pQSerialPort->setBaudRate(QSerialPort::Baud9600); // 设置波特率9600
    m_pQSerialPort->setDataBits(QSerialPort::Data8); // 设置8数据位
    m_pQSerialPort->setParity(QSerialPort::NoParity); // 设置无校验位
    m_pQSerialPort->setStopBits(QSerialPort::OneStop); // 设置1停止位
    m_pQSerialPort->setFlowControl(QSerialPort::NoFlowControl); // 设置无流控制

    if(!m_pQSerialPort->open(QIODevice::ReadWrite))
    {
        qWarning() << "Failed to open serial port:" << m_pQSerialPort->errorString();

        QMessageBox::information(nullptr, QString(tr("connect serial")), QString(tr("fail to connect the serial!Please check your state.")));
    }

    else
    {
        connect(m_pQSerialPort, &QSerialPort::readyRead, [&]() {
            QByteArray data = m_pQSerialPort->readAll();
           //ToDo
            handleResultData(data);
        });

        /*连接上串口后更新按钮使能状态*/
        QMessageBox::information(nullptr, QString(tr("connect serial")), QString(tr("you have connected the serial!")));
        ui->btn_OpenSerial->setEnabled(false);
        ui->btn_CloseSerial->setEnabled(true);
    }

}

void MainWindow::slotClickCloseSerialBtn()
{
    /*断开串口连接*/
    if(m_pQSerialPort != nullptr)
    {
        m_pQSerialPort->clear();
        m_pQSerialPort->close();
        m_pQSerialPort->deleteLater();
        m_pQSerialPort = nullptr;
    }

    refreshSerial();
}

