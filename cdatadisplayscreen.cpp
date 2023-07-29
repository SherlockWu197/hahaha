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

CDataDisplayScreen *CDataDisplayScreen::GetInstance()
{
    return this;
}

void CDataDisplayScreen::InitView()
{

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
