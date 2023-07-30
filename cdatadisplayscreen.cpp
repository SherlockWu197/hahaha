#include "cdatadisplayscreen.h"
#include "ui_cdatadisplayscreen.h"

CDataDisplayScreen::CDataDisplayScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDataDisplayScreen)
{
    ui->setupUi(this);
    setAutoFillBackground(true);

//    InitData();

    InitView();
}

CDataDisplayScreen::~CDataDisplayScreen()
{
    delete ui;
}

void CDataDisplayScreen::disPlay(const QByteArray& data)
{
    disPlayData(data);
}

void CDataDisplayScreen::InitView()
{
    ui->label_num->setText(QString(tr("Num:0")));
    ui->label_avg->setText(QString(tr("Avg:0")));
    ui->label_min->setText(QString(tr("Min:0")));
    ui->label_max->setText(QString(tr("Max:0")));
    ui->label_Fe->setText(QString(tr("Fe")));
    ui->label_um->setText(QString(tr("μm")));
    ui->label_RealTimeValue->setText(QString(tr("00000")));
}

void CDataDisplayScreen::disPlayData(const QByteArray& data)
{
    QByteArray tempData = data.mid(4, 6);
//    hexToFloat(tempData);
    qDebug() << "hex: " << tempData << "to normal: " << hexToNormal(tempData);
    ui->label_RealTimeValue->setText(hexToNormal(tempData));
}

QString CDataDisplayScreen::hexToNormal(const QByteArray& hexData)
{
    if (hexData.size() != 6) {
        qDebug() << "Invalid input data, must be 6 bytes.";
        return "error";
    }

    /*存储正常显示的数据*/
    QString normal;

    /*存储转换后不带空格的数值*/
    QString noneSpaceNormal;

    /*存储单个字节的数据*/
//    char singleByte;

    /*数据是负数标志位*/
    bool isNegative = false;

    QByteArray byte;
    for (int i = 0; i < hexData.size(); ++i)
    {
        char ch = hexData.at(i);

        qDebug() << "Byte at index" << i << "is" << ch;
        if(hexData.at(i) != 0x20 && hexData.at(i) != 0x2D && hexData.at(i) != 0x2E)
        {
//            int decimalValue = QString(ch).toInt(nullptr, 16);
            int decimal = hexData.at(i);
            qDebug() << "Byte at index decimal" << i << "is" << decimal;
            QString decimalStr = QString::number(decimal);
        }
    }

    qDebug() << "normal initial is:" << normal << "line" << __LINE__;

//    for(int i = 0; i< 6; i++)
//    {
//        if(hexData.at(i) == 0x20)
//        {
//            normal += " ";
//        }

//        else if(hexData.at(i) == 0x2D)
//        {
//            normal += "-";
//            noneSpaceNormal += "-";
//        }

//        else if(hexData.at(i) == 0x2E)
//        {
//            normal += ".";
//            noneSpaceNormal += ".";
//        }

//        else
//        {
//            singleByte = hexData.at(i);

//            qDebug() << "single is:" << hexData.at(i) << "line" <<  __LINE__;

//            normal += hexData.at(i);
//        }
//    }

    return normal;
}

void CDataDisplayScreen::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // 绘制控件的背景颜色
    painter.fillRect(rect(), Qt::white);

    // 绘制自定义控件的边框为灰黑
    QPen borderPen("#828790");
    borderPen.setWidth(1);
    painter.setPen(borderPen);
    painter.drawRect(rect().adjusted(0, 0, -1, -1));

    QWidget::paintEvent(event);
}
