#ifndef CDATADISPLAYSCREEN_H
#define CDATADISPLAYSCREEN_H

#include <QWidget>
#include <QPainter>
#include <QDebug>

//// 实时数据消息结构体
//struct LiveDataMessage {
//    quint8 headerHightByte;  // 消息头高字节
//    quint8 headerLowByte;  // 消息头低字节
//    quint8 length;  // 消息长度
//    quint8 msgType; // 消息类型
//    float display;  // 显示值
//    char unit[3];   // 单位
//    char function;  // 功能码
//    quint8 mode;    // 模式
//    quint8 status;  // 状态
//    quint16 checksum; // 校验和
//};

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

    /*根据提供的6字节十六进制数转为浮点函数*/
    float hexToFloat(const QByteArray& hexData);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    Ui::CDataDisplayScreen *ui;
};

#endif // CDATADISPLAYSCREEN_H
