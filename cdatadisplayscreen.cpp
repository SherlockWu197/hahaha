#include "cdatadisplayscreen.h"
#include "ui_cdatadisplayscreen.h"

CDataDisplayScreen::CDataDisplayScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDataDisplayScreen)
{
    ui->setupUi(this);
    setAutoFillBackground(true);

    InitView();
}

CDataDisplayScreen::~CDataDisplayScreen()
{
    delete ui;
}

void CDataDisplayScreen::InitView()
{
//    this->setStyleSheet("background-color: white;");
    QPalette palette = this->palette();
       palette.setColor(QPalette::Background, Qt::white);
       this->setPalette(palette);
}
