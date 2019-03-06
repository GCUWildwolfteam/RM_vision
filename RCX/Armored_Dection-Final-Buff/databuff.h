#ifndef DATABUFF_H
#define DATABUFF_H

#include "configure.h"

#define change_th_x 0
#define change_th_y 0
#define bufftime 20
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

int missingFlag(Mat src,Point point)
{
    int flag;
    Point center;
    center.x = src.cols/2;
    center.y = src.rows/2;
    Point left_focus = Point(center.x-53,center.y);
    Point right_focus = Point(center.x+53,center.y);
    float PF1 = centerDistance(point,left_focus);
    float PF2 = centerDistance(point,right_focus);
    if((PF1 + PF2) < 160)
    {
        flag = 2;
    }
    else
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

Point ispointChangemuch(Point last_point, Point new_point)
{
    int x1, y1;
    int x2, y2;
    x1 = last_point.x;   y1 = last_point.y;
    x2 = new_point.x;    y2 = new_point.y;
    if (abs(x1 - x2) < change_th_x && abs(y1 - y2) < change_th_y)
    {
        return new_point;
    }
}
#endif // DATABUFF_H
