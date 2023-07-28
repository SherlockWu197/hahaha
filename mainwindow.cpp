#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    InitView();
    InitData();
    InitConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitView()
{
    /*初始化按钮状态为失能状态*/
    ui->btn_OpenSerial->setEnabled(false);
    ui->btn_CloseSerial->setEnabled(false);

    ui->tabWidget->setTabText(0, QString(tr("Table")));
    ui->tabWidget->setTabText(1, QString(tr("Histogram")));
    ui->tabWidget->setTabText(2, QString(tr("Chart")));
}

void MainWindow::InitData()
{
    m_pQSerialPort = new QSerialPort();

    RefreshSerial();
}

void MainWindow::InitConnect()
{
    //connect(  this,&MainWindow::mainwidgetstr,da,&Data::show_data);
    connect(ui->btn_refreshSerial,&QPushButton::clicked, this, &MainWindow::SlotRefreshSerial);
    connect(ui->btn_OpenSerial,&QPushButton::clicked, this, &MainWindow::SlotClickConnectSerialBtn);
    connect(ui->btn_CloseSerial,&QPushButton::clicked, this, &MainWindow::SlotClickCloseSerialBtn);
}

void MainWindow::RefreshSerial()
{
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

void MainWindow::SlotRefreshSerial()
{
    ui->comboBox_Serial->clear();
    InitView();
    RefreshSerial();
}

void MainWindow::SlotClickConnectSerialBtn()
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
    }

    else
    {
        connect(m_pQSerialPort, &QSerialPort::readyRead, [&]() {
            QByteArray data = m_pQSerialPort->readAll();
            qDebug() << "Received data:" << data;
            ui->textBrowser->setText(data);
        });

        /*连接上串口后更新按钮使能状态*/
        ui->btn_OpenSerial->setEnabled(false);
        ui->btn_CloseSerial->setEnabled(true);
    }
}

void MainWindow::SlotClickCloseSerialBtn()
{
    m_pQSerialPort->clear();
    m_pQSerialPort->close();
    m_pQSerialPort->deleteLater();
    ui->btn_OpenSerial->setEnabled(true);
    ui->btn_CloseSerial->setEnabled(false);
}

