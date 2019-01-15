#include "serialport.h"

/** 打开串口
 *  @param:    portNo 串口编号,默认值为1,即COM1,注意,尽量不要大于9
 *  @return
 */
SerialPort::SerialPort(int portNo) {

    const char* DeviceName[4] = {"", "/dev/ttyUSB0", "/dev/ttyUSB1", "/dev/ttyUSB2"};

     /* WARNING :  终端设备默认会设置为控制终端，因此open(O_NOCTTY不作为控制终端)
     * Terminals'll default to be set as Control Terminals
     */
     /*打开串口*/
    fd=open(DeviceName[portNo], O_RDWR|O_NONBLOCK|O_NOCTTY|O_NDELAY);

    if (fd == -1)
    {
        perror("Can't Open Serial Port\n");
    }
    else
        printf("Open Serial Port %s Successful\n", DeviceName[portNo]);

    /*改为阻塞模式*/
    if (fcntl(fd, F_SETFL, 0) < 0)
        printf("fcntl failed!\n");
    else
        printf("fcntl=%d\n", fcntl(fd, F_SETFL, 0));

    /*测试是否为终端设备*/
    if (isatty(STDIN_FILENO) == 0)
        printf("standard input is not a terminal device\n");
    else
        printf("isatty success!\n");

}

SerialPort::~SerialPort(void)
{
    if (!close(fd))
        printf("Close Serial Port Successful\n");
}
/** 初始化串口函数
* @Default
*  @param:  int baud   波特率,默认为115200
* @Default
*  @param:  char parity 是否进行奇偶校验,'Y'表示需要奇偶校验,'N'表示不需要奇偶校验
* @Default
*  @param:  int databits 数据位的个数,默认值为8个数据位
*
*  @return: bool  初始化是否成功
*  @note:   在使用其他本类提供的函数前,请先调用本函数进行串口的初始化
*　　　　　   函数提供了一些常用的串口参数设置
*           本串口类析构时会自动关闭串口,无需额外执行关闭串口
*/
bool SerialPort::initPort(int baud )
{
    struct termios newstate, option;
    /*保存测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息*/
    if (tcgetattr(fd, &option) != 0)
    {
        perror("SetupSerial");
        printf("tcgetattr( fd,&option) -> %d\n",tcgetattr(fd, &option));
        return 0;
    }
    /*串口设置*/

    //本地连线, 取消控制功能 | 开始接收
    newstate.c_cflag |= CLOCAL | CREAD;
    //设置字符大小
    newstate.c_cflag &= ~CSIZE;
    //设置停止位1
    newstate.c_cflag &= ~CSTOPB;
    //设置数据位8位
    newstate.c_cflag |= CS8;
    //设置无奇偶校验位，N
    newstate.c_cflag &= ~PARENB;

    /*阻塞模式的设置*/
    option.c_cc[VTIME]=0;
    option.c_cc[VMIN]=1;

    /*设置发送波特率*/
    cfsetospeed(&newstate, baud);

    /*激活新配置*/
    if ((tcsetattr(fd, TCSANOW, &option)) != 0)
    {
        perror("Com Set Error\n");
        return 0;
    }
    printf("Com Set done!\n");
    return 1;
}

void SerialPort::RMSerialWrite(int x,int y,int SendDataFlag)
{
    switch (SendDataFlag)
    {
    case 0:
    {
        sprintf(g_buf,"%s%03d%s%03d","S",x,",",y);
        std::cout<<std::endl<<g_buf<<std::endl;
        write(fd,g_buf,sizeof(g_buf));
        sleepUS(10);
    }
        break;
    case 1:
    {
        sprintf(g_buf,"%s","N1000000");
        std::cout<<std::endl<<g_buf<<std::endl;
        write(fd,g_buf,sizeof(g_buf));
        sleepUS(1);
    }
        break;
    case 2:
    {
        sprintf(g_buf,"%s","N2000000");
        std::cout<<std::endl<<g_buf<<std::endl;
        write(fd,g_buf,sizeof(g_buf));
        sleepUS(1);
    }
        break;
    case 3:
    {
        sprintf(g_buf,"%s","N3000000");
        std::cout<<std::endl<<g_buf<<std::endl;
        write(fd,g_buf,sizeof(g_buf));
        sleepUS(1);
    }
        break;
    default:
        break;
    }
}

void SerialPort::sleepUS(unsigned int secs)
{
    struct timeval tval;
    tval.tv_sec=secs/1000;
    tval.tv_usec=(secs*1000)%1000000;
    select(0,NULL,NULL,NULL,&tval);
}
