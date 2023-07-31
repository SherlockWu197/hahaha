#ifndef GLOBAL_H
#define GLOBAL_H


#include <QString>

const quint8 HEADER_HIGHTBYTE = 0xAA; // 消息头高字节
const quint8 HEADER_LOWBYTE = 0xBB; // 消息头低字节
const quint8 MSG_TYPE_DEVICE_ID = 0x00; // 消息类型：下位机设备ID
const quint8 MSG_TYPE_LIVE_DATA = 0x01; // 消息类型：实时数据
const quint8 MSG_TYPE_STORE_DATA = 0x05; // 消息类型，存储的数据
const quint8 MSG_TYPE_SHAKE_HAND = 0xFF; // 消息类型，握手

// 实时数据消息结构体
struct STliveDataMsg {
    float display;  // 显示值
    QString unit;   // 单位
    QString function;  // 功能码
    quint8 mode;    // 模式
    quint8 status;  // 状态
    bool bIsOverload;  //溢出标志位
};


#endif // GLOBAL_H
