#include <configure.h>
#include "serialport.h"

int main()
{
    VideoCapture cap(0);

    Mat src_img;    //原图
    Mat hsv_img;
    Mat gray_img;   //灰度图
    Mat bin_img;    //二值图
    Mat dst_img;    //输出图
    int threshold_value = 80;

    for(;;)
    {
        cap >> src_img;
//        cvtColor(src_img, gray_img, COLOR_BGR2GRAY);
//        threshold(gray_img, bin_img, threshold_value, 255, THRESH_BINARY);
        cvtColor(src_img,hsv_img,COLOR_BGR2HSV);
        inRange(hsv_img,Scalar(0,60,60),Scalar(10,255,255),bin_img);
        //GaussianBlur(bin_img,bin_img,Size(7,7),0,0);
        medianBlur(bin_img,bin_img,9);
        Canny(bin_img,bin_img,120,240);

        vector<vector<Point>>contours;
        vector<Vec4i>hierarchy;
        findContours(bin_img,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE,Point(0,0));
        for(int i = 0;i<contours.size();++i)
        {
            RotatedRect r_rect_i = minAreaRect(contours[i]);
        }
        imshow("th",bin_img);
        imshow("input" ,src_img);
        //imshow("output",dst_img);
        int key = waitKey(1);
        if(char(key) == 27)
        {
            break;
        }
    }
    return 0;
}
