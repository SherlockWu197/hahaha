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
    QByteArray tempData;
    hexToFloat(tempData);
    ui->label_RealTimeValue->setText("");
}

float CDataDisplayScreen::hexToFloat(const QByteArray& hexData)
{
    if (hexData.size() != 6) {
            qDebug() << "Invalid input data, must be 6 bytes.";
            return 0.0f;
        }

        // 解析符号位和整数部分
        bool isNegative = false;
        int integerPart = 0;
        for (int i = 0; i < 4; ++i) {
            if (hexData.at(i) == 0x2D) { // 负号
                isNegative = true;
            } else if (hexData.at(i) != 0x20) { // 数字部分
                integerPart = integerPart * 10 + (hexData.at(i) - 0x30); // 0x30 表示 ASCII 码中的 '0'
            }
        }

        // 解析小数部分
        int decimalPart = 0;
        for (int i = 4; i < 6; ++i) {
            if (hexData.at(i) != 0x2E) { // 小数点
                decimalPart = decimalPart * 10 + (hexData.at(i) - 0x30); // 0x30 表示 ASCII 码中的 '0'
            }
        }

        // 计算浮点数的值
        float result = static_cast<float>(integerPart) + static_cast<float>(decimalPart) / 1000.0f;

        // 加上符号位
        if (isNegative) {
            result = -result;
        }

        return result;
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
