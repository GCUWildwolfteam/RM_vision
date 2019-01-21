#ifndef RMVIDEOCAPTURE_H
#define RMVIDEOCAPTURE_H

#include "configure.h"
#include <opencv2/highgui.hpp>

class RMVideocapture
{
public:

    unsigned char *g_pRgbBuffer; //处理后数据缓存区

    int                     iCameraCounts = 1;
    int                     iStatus=-1;
    tSdkCameraDevInfo       tCameraEnumList;
    int                     hCamera;
    tSdkCameraCapbility     tCapability;      //设备描述信息
    tSdkFrameHead           sFrameInfo;
    BYTE*			        pbyBuffer;
    IplImage                *iplImage = NULL;
    int                     channel=3;
    BOOL                    AEstate=FALSE;

    bool                    iscamera0open = 0;

public:
    RMVideocapture(int cameramode);

    bool isindustryimgInput();
    void cameraReleasebuff();
    int cameraSet();

public:
    ~RMVideocapture(void);

private:

};

#endif // RMVIDEOCAPTURE_H
