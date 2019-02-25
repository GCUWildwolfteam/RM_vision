#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include "serialport.h"

using namespace cv;
using namespace std;

Mat frame;      //原图
Mat hsv_img;    //HSV图
Mat bin_img;    //二值图
Mat dst_img;    //输出图

int main()
{
    SerialPort port;
    if(serialisopen == 1)
    {
        port.serialSet(1);
        //initPort();//串口初始化函数
    }

    VideoCapture capture(1);
    while(1)
    {
        capture >> frame;

        cvtColor(frame,hsv_img,COLOR_BGR2HSV);
        inRange(hsv_img,Scalar(0,60,60),Scalar(10,255,255),bin_img);  //红色
//        threshold(bin_img,bin_img,20,255,THRESH_BINARY);
//        GaussianBlur(bin_img,bin_img,Size(1,1),0,0,BORDER_DEFAULT);
        medianBlur(bin_img,bin_img,5);

        Point up = Point(320,0);                          //定义显示窗口上中点
        Point down = Point(320,480);                      //定义显示窗口下中点
        line(frame, up,down,Scalar(0,255,255),4,8,0);     //连中线

        vector<vector<Point>>contours;
        vector<Vec4i>hierarchy;
        findContours(bin_img,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE,Point(0,0));

        vector<RotatedRect>minRects(contours.size());

        for (int i = 0; i < contours.size(); ++i)
        {
            minRects[i] = minAreaRect(Mat(contours[i]));
            {
                Point2f rectPoints[4];
                minRects[i].points(rectPoints);

                double x0= rectPoints[0].x;
                double y0= rectPoints[0].y;
                double x1= rectPoints[1].x;
                double y1= rectPoints[1].y;
                double x2= rectPoints[2].x;
                double y2= rectPoints[2].y;
                double x3= rectPoints[3].x;
                double y3= rectPoints[3].y;                //定义返回的四个坐标点               

                double width= sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
                double height= sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));    //定义长宽
                double area = width * height;                            //面积
                double w=0,h=0;
                if(width>height)
                {
                    h=width;
                    w=height;
                }
                else
                {
                    h=height;
                    w=width;
                }
                double i = fabs(h/w) ;                     // 高宽比

                double rect_x1 = (x0 + x2)/2;                 //轮廓在X轴上的中点
                double rect_y1 = (y0 + y2)/2;                 //轮廓在Y轴上的中点

                Point p0 = Point(rect_x1, rect_y1);
                double distance_1 = sqrt((rect_x1 -320)*(rect_x1 -320));  //轮廓矩中点到中线的距离
                double distance_2 = sqrt((rect_x1 - x0)*(rect_x1 - x0)+(rect_y1 - y0)*(rect_y1 - y0));
                                                                         //角点到轮廓矩中点的距离
                double xx= rect_x1 - 320;       //判断砖头在中线哪侧 xx>0为右侧，xx<0为左侧

                if(i >2.800 && i< 3.800 && area >1000 )
                {
                     for (int j = 0; j < 4; j++)
                    {
                        if(distance_1 <distance_2 * 3.5 && xx+15 >0)
                       {
                            line(frame, rectPoints[j], rectPoints[(j + 1) % 4], Scalar(0,255,0), 4, 8, 0);//--
                            circle(frame, p0,15,Scalar( 0,255, 0),2,8,0);

                             cout<<"h/w"<<i<<endl;
                            cout<<p0<<endl<<endl;
                            port.RMSerialWrite(p0.x,p0.y,2);

                       }

                    }

//                     cout<<area <<endl;
                }
            }
        }
        imshow("笑果",frame);
     imshow("2",bin_img);
        char (key)=(char) waitKey(30);
        if (key==27)break;
    }
   return 0;
}
