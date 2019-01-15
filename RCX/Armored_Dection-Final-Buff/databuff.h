#ifndef DATABUFF_H
#define DATABUFF_H

#include "configure.h"

#define bufftime 15
int x_buff;
int y_buff;

bool iscentralBUffer(Mat src,Point point)
{
    bool is = false;
    Point center;
    center.x = src.cols/2;
    center.y = src.rows/2;
    Point left_focus = Point(center.x-53,center.y);
    Point right_focus = Point(center.x+53,center.y);
    float PF1 = centerDistance(point,left_focus);
    float PF2 = centerDistance(point,right_focus);
    if((PF1 + PF2) < 160)
    {
        is = true;
    }

    return is;
}

int missingFlag(Mat src,int X_Widht)
{
    int flag;
    Point center;
    center.x = src.cols/2;
    center.y = src.rows/2;
    int Xc = center.x;
    //int Yc = center.y;
    int Xp = X_Widht;
    //int Yp = Y_height;

    if(Xp < (Xc - 480/8))
    {
        flag = 1;
    }
    else if (Xc - 480/8 <= Xp && Xp <= Xc + 480/8)
    {
        flag = 2;
    }
    else if (Xp > (Xc + 480/8))
    {
        flag = 1;
    }
    return flag;
}

int bufferReturn(Mat src,int buff,int value,int cnt,int flag)
{
    int send;
    int max;
    if(flag == 0)
    {
        max = src.cols/2;
    }
    else
    {
        max = src.rows/2;
    }
    if(buff < max)
    {
        send = buff + value/(bufftime-cnt);
    }
    else
    {
        send = buff - value/(bufftime-cnt);
    }

    return send;
}

#endif // DATABUFF_H
