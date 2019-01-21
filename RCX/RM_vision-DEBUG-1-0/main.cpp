#include "cameraconfigure.h"
#include "configure.h"
#include "rmvideocapture.h"
#include "rm_armor_fitted.h"
#include "rm_big_chrysanthemum.h"

int main()
{
    /*----------调用相机----------*/
    VideoCapture capture(capture_defult);
    RMVideocapture cap(0);
    /*----------调用相机----------*/

    /*----------串口部分----------*/
    SerialPort port;
    if(serialisopen == 1)
    {
        port.serialSet(1);
        //initPort();//串口初始化函数
    }
    /*----------串口部分----------*/

    /*----------参数初始化----------*/
    Mat src_img;
    int img_arr_count = 0;
    vector<Mat > img_arr(3);
    /*----------参数初始化----------*/

    /*----------函数初始化----------*/
    RMArmorFitted armor;
    RMBigChrysanthemum agency;
    /*----------函数初始化----------*/
    for(;;)
    {
        if(cap.isindustryimgInput())
        {
            src_img = cvarrToMat(cap.iplImage,true);//这里只是进行指针转换，将IplImage转换成Mat类型
        }
        else
        {
            capture >> src_img;
        }
        int mode = 2;
        /*----------串口解码部分----------*/
        if(mode == 1/*串口读入的标志位*/)
        {
            //装甲识别部分
            armor.imageProcessing(src_img);
            armor.lightFitted();
        }
        else
        {
            agency.imageProcessing(src_img);
            agency.rectFilter();
        }
        //imshow("1",src_img);
        int key = waitKey(1);
        if(char(key) == 27)
        {
            cap.cameraReleasebuff();
            break;
        }
        cap.cameraReleasebuff();
    }
    return 0;
}
