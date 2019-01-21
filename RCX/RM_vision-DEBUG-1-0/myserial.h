
#ifndef MYSERIAL_H
#define MYSERIAL_H

#include "configure.h"
#include "contourfeature.h"

int fd;
char * buf = new char[8];//分配内存空间

void sleepUS(unsigned int secs)
{
    struct timeval tval;
    tval.tv_sec=secs/1000;
    tval.tv_usec=(secs*1000)%1000000;
    select(0,NULL,NULL,NULL,&tval);
}

void serialSet()
{
    //串口部分
    struct termios options, newstate;
    int x=2,y=1;
    sprintf(buf, "%s%d%d%s", "S", x, y, "E");
    //sprintf(buf2,"%s%d%s%d", "Y", ":", y);
    fd=open("/dev/ttyUSB0", O_RDWR|O_NONBLOCK|O_NOCTTY|O_NDELAY);    //打开串口
    if(fd==-1)
          printf("can not open the COM!\n");
    else
          printf("open COM ok!\n");
    /*判断是否是终端设备
    if(isatty(STDIN_FILENO) == 0)
      printf("不是终端设备\n");
    else
      printf("是终端设备\n");
    */
   if(fcntl(fd, F_SETFL, 0) <0 ) //改为阻塞模式
      printf("fcntl failed\n");
    else
      printf("fcntl=%d\n", fcntl(fd, F_SETFL, 0));

    tcgetattr(fd, &options);
    //设置波特率
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    //获取波特率
    tcgetattr(fd, &newstate);
//    baud_rate_i=cfgetispeed(&newstate);
//    baud_rate_o=cfgetospeed(&newstate);
    //串口设置
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;//设置无奇偶校验位，N
    options.c_cflag &= ~CSTOPB; //设置停止位1
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8; //设置数据位
    options.c_cc[VTIME]=0;//阻塞模式的设置
    options.c_cc[VMIN]=1;
    //激活新配置
    tcsetattr(fd, TCSANOW, &options);
    //输出波特率
    //printf("输入波特率为%d，输出波特率为%d\n" , baud_rate_i, baud_rate_o);
   //配置其他选项
//    SerialPortSettings.c_cflag &= ~CRTSCTS;     //关闭基于硬件的流量控制
//    SerialPortSettings.c_cflag |= CREAD | CLOCAL;   //打开串口（CREAD）的接收器
//    SerialPortSettings.c_iflag &= ~(IXON|IXOFF|IXANY);  //关闭基于软件的流量控制（XON / XOFF）
//    SerialPortSettings.c_iflag &= ~(ICANON|ECHO|ECHOE|ISIG);    //设置操作模式
}

void RMSerialWrite(int x,int y,int SendDataFlag)
{
    switch (SendDataFlag)
    {
    case 0:
    {
        sprintf(buf,"%s%03d%s%03d","S",x,",",y);
        std::cout<<std::endl<<buf<<std::endl;
        write(fd,buf,sizeof(buf));
        sleepUS(10);
    }
        break;
    case 1:
    {
        sprintf(buf,"%s","N1000000");
        std::cout<<std::endl<<buf<<std::endl;
        write(fd,buf,sizeof(buf));
        sleepUS(1);
    }
        break;
    case 2:
    {
        sprintf(buf,"%s","N2000000");
        std::cout<<std::endl<<buf<<std::endl;
        write(fd,buf,sizeof(buf));
        sleepUS(1);
    }
        break;
    case 3:
    {
        sprintf(buf,"%s","N3000000");
        std::cout<<std::endl<<buf<<std::endl;
        write(fd,buf,sizeof(buf));
        sleepUS(1);
    }
        break;
    case 4:
    {
        sprintf(buf,"%s%03d%s%03d","S",x,",",y);
        std::cout<<std::endl<<buf<<std::endl;
        write(fd,buf,sizeof(buf));
        sleepUS(10);
    }
        break;
    default:
        break;
    }
}

#endif // MYSERIAL_H
