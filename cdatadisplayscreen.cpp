#include "cdatadisplayscreen.h"
#include "ui_cdatadisplayscreen.h"

CDataDisplayScreen::CDataDisplayScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDataDisplayScreen)
{
    ui->setupUi(this);
    setAutoFillBackground(true);

    InitData();

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

void CDataDisplayScreen::InitData()
{
    m_fNumer = 0;
    m_fAvg = 0;
    m_fMin = 0;
    m_fMax = 0;

    m_bFe = false;

}

void CDataDisplayScreen::disPlayData(const QByteArray& data)
{
    /*display部分数据*/
    QByteArray displayData = data.mid(4, 6);
    qDebug() << "hex: " << displayData << "to normal: " << hexToNormal(displayData);
    ui->label_RealTimeValue->setText(hexToNormal(displayData));  //显示数值

    /*单位码数据，本协议只有um单位无其它单位,因此单位默认显示，不做处理*/
    QByteArray unitData = data.mid(10,3);

    /*Function数据*/
    QByteArray functionData = data.mid(13,1);

    qDebug() << "the functionData is:" << functionData;
    if(functionData.at(0) == '\x46')
    {
        ui->label_Fe->setText("Fe");
    }
    else
    {
        ui->label_Fe->setText("N");
    }

    /*模式部分数据*/
    QByteArray modeData = data.mid(14,1);

    /*status状态部分数据*/
    QByteArray statusData = data.mid(15,1);

    /*check校验码*/
    QByteArray verifyData = data.mid(16,2);
}

QString CDataDisplayScreen::hexToNormal(const QByteArray& hexData)
{
    if (hexData.size() != 6) {
        qDebug() << "Invalid input data, must be 6 bytes.your size is" << hexData.size();
        return "error";
    }

    /*存储屏幕显示的数据内容*/
    QString normal;

    /*存储数据转换为十进制浮点数的值*/
    float decimalFloat;

    for (int i = 0; i < hexData.size(); ++i)
    {
        char ch = hexData.at(i);

        qDebug() << "Byte at index" << i << "is" << ch;

        if(hexData.at(i) == 0x20 || hexData.at(i) == '\x2D' || hexData.at(i) == '\x2E')
        {
            normal += ch;

            qDebug() << "end of data string is:" << normal;
        }

        else if((hexData.at(i) >= 0x00) && (hexData.at(i) <= 0x09))
        {
            int decimal = hexData.at(i);
            qDebug() << "Byte at index decimal" << i << "is" << decimal;
            QString decimalStr = QString::number(decimal);
            normal += decimalStr;
        }

        /*当接收的单字节数据十进制不属于正" ",负"-",空位" ",以及0~9时进行报错*/
        else
        {
           qDebug() << "your data is error!";
        }
    }

    decimalFloat = normal.toFloat();

    /*当数据在1250以及-500的位置溢出时,显示OL*/
    if(decimalFloat < -500)
    {
        qDebug() << "the value " << decimalFloat << "is lower than -500um";

        normal = "    OL";
    }

    if(decimalFloat > 1250)
    {
        qDebug() << "the value " << decimalFloat << "is higher than 1250um";

        normal = "    OL";
    }

    qDebug() << "the end of data string is:" << normal;
    qDebug() << "the end of data value is:" << decimalFloat;
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
