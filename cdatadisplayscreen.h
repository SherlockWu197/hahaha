#ifndef CDATADISPLAYSCREEN_H
#define CDATADISPLAYSCREEN_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <global.h>

namespace Ui {
class CDataDisplayScreen;
}

class CDataDisplayScreen : public QWidget
{
    Q_OBJECT

public:
    explicit CDataDisplayScreen(QWidget *parent = nullptr);
    ~CDataDisplayScreen();

    /*提供给外部调用的显示数据接口函数*/
    void disPlay(const QByteArray& data);

private:
    void InitView();

    void InitData();

    void InitConnect();

    /*显示数据*/
    void disPlayData(const QByteArray& data);

    /*将提供的6字节数值十六进制转为十进制显示格式*/
    QString hexToNormal(const QByteArray& hexData);

protected:
    virtual void paintEvent(QPaintEvent *event);

signals:
    /*发送实时消息结构体*/
    void signalSendMsg(const STliveDataMsg& livedataMessage);

private:
    Ui::CDataDisplayScreen *ui;

    float m_fNumer; //当前的数值
    float m_fAvg;   //平均数值
    float m_fMin;   //最小值
    float m_fMax;  //最大值
    bool m_bFe;    //是否为铁基
};

#endif // CDATADISPLAYSCREEN_H
