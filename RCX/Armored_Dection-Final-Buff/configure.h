#ifndef CONFIGURE_H
#define CONFIGURE_H

#include "CameraApi.h"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <math.h>
#include <iostream>
#include <fcntl.h>  //文件控制定义
#include <termios.h>   //POSIX终端控制定义
#include <unistd.h>    //UNIX标准定义
#include <errno.h>     //ERROR数字定义
#include <sys/select.h>

using namespace std;
using namespace cv;

#define capture_defult 0    //相机的默认值
#define armor_color 1       //装甲颜色 red=0  blue=1
#define serialisopen 1      //是否启用串口
#define forecastisopen 0    //是否启用预测
#define cameramode 0        //选用相机 工业相机=0 普通相机=1

#endif // CONFIGURE_H
