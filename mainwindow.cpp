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

    // 隐藏水平和垂直表头
    //    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    model = new QStandardItemModel;
    model->setColumnCount(5);

    ui->tableView->setModel(model);
    ui->tableView->setSelectionMode(QAbstractItemView::NoSelection);

    model->setHeaderData(0, Qt::Horizontal, QString(tr("NO.")));
    model->setHeaderData(1, Qt::Horizontal, QString(tr("Time")));
    model->setHeaderData(2, Qt::Horizontal, QString(tr("Value")));
    model->setHeaderData(3, Qt::Horizontal, QString(tr("Unit")));
    model->setHeaderData(4, Qt::Horizontal, QString(tr("Basis")));

    //给列设定宽度
    ui->tableView->setColumnWidth(0, 80);
    ui->tableView->setColumnWidth(1, 200);
    ui->tableView->setColumnWidth(2, 130);
    ui->tableView->setColumnWidth(3, 100);
    ui->tableView->setColumnWidth(4, 140);

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
    connect(m_pDatadisplayScreen, SIGNAL(signalSendMsg(const STliveDataMsg&)), this, SLOT(slotReceiveMsg(const STliveDataMsg&)));

    /*手动发送数据，模拟程序对数据的处理*/
    connect(ui->btn_SendData, &QPushButton::clicked, this, &MainWindow::sendDataBySelf);
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
    qDebug() << "handleResultData" ;

    /*对返回的数据格式进行校验*/
    if(!resultData.isEmpty() && data.startsWith(QByteArray::fromHex("AABB")))
    {
        qDebug() << "entre verify sentence";
        /*下位机设备ID*/
        if(resultData.at(3) == MSG_TYPE_DEVICE_ID)
        {
            qDebug() << "you have enterd request device ID Mode";
        }

        /*实时数据*/
        if(resultData.at(3) == MSG_TYPE_LIVE_DATA)
        {
            qDebug() << "you have entred realTime Mode";
            handleRealTimeData(resultData);
        }

        /*已存储的数据*/
        if(resultData.at(3) == MSG_TYPE_STORE_DATA)
        {
            qDebug() << "you have entred request store data mode";
        }

        /*握手结果*/
        if(static_cast<quint8>(resultData.at(3)) == MSG_TYPE_SHAKE_HAND)
        {
            qDebug() << "you have enter request resulte of handshake";
        }
    }
    else
    {
        qDebug() << "error,check your data form whether is right";
    }
}

void MainWindow::handleRealTimeData(const QByteArray& data)
{
    QByteArray realTimeData = data;

    m_pDatadisplayScreen->disPlay(data);
}

void MainWindow::sendDataBySelf()
{
    QString tempString = ui->lineEdit->text();

    QByteArray byteArray = QByteArray::fromHex(tempString.toUtf8());

    /*计算校验和*/
    QString checkSumString = QString::number(calculateChecksum(byteArray),16);

    /*将数据包的校验部分给粘合一起*/
    byteArray.append(checkSumString);

    handleResultData(byteArray);
}

quint16 MainWindow::calculateChecksum(const QByteArray &data)
{
    quint16 checksum = 0;
    for (int i = 0; i < data.size(); i++) {
        checksum += static_cast<quint8>(data.at(i));
        qDebug() << "data.at(" << i << "):" << data.at(i) << " size is:" << static_cast<quint8>(data.at(i));
    }

    qDebug() << "the end of calculate checkSum is:" << checksum;
    return checksum;
}

void MainWindow::slotRefreshSerial()
{
    initView();

    slotClickCloseSerialBtn();
}

void MainWindow::slotReceiveData()
{
    QByteArray data = m_pQSerialPort->readAll();

    QString str = data;

    /*将蓝牙接收的数据重新转换为十六进制的数*/
    QByteArray transfromData = QByteArray::fromHex(str.toUtf8());
    //ToDo
    qDebug() << "receive data byte form:" << data;

    qDebug() << "receive data string form:" << str;
    handleResultData(transfromData);
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
        connect(m_pQSerialPort, &QSerialPort::readyRead, this, &MainWindow::slotReceiveData);

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

void MainWindow::slotReceiveMsg(const STliveDataMsg &liveDataMessage)
{
    static int itemCount = 1; // 计数器变量
    QDateTime currentTime = QDateTime::currentDateTime();

    qDebug() << "slotReceiveLiveDataMessageToTable" << liveDataMessage.display;

    QList<QStandardItem *> rowData1;
    rowData1 << new QStandardItem(QString::number(itemCount));  //No.
    rowData1 << new QStandardItem(currentTime.toString());  //Time
    rowData1 << new QStandardItem(QString::number(liveDataMessage.display));
    rowData1 << new QStandardItem(liveDataMessage.unit);
    rowData1 << new QStandardItem(liveDataMessage.function);

    for (int i = 0; i < rowData1.size(); ++i)
    {
        /*遍历item设置文本居中*/
        rowData1.at(i)->setTextAlignment(Qt::AlignCenter);
    }

    model->appendRow(rowData1);

    itemCount++;
}

