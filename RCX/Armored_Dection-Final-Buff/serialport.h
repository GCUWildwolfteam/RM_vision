#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "configure.h"

class SerialPort
{
public:
    //串口标志量
    int fd;
    char *g_buf = new char[8];
public:
    /**              函数声明            **/
    SerialPort();

    void serialSet(int port_No);
    //自定义串口发送
    void RMSerialWrite(int x,int y,int SendDataFlag);
    //延时函数，防止发送过快
    void sleepUS(unsigned int secs);

public:
    ~SerialPort();
};

#endif // SERIALPORT_H
