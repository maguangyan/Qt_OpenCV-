#ifndef CAMERA_H
#define CAMERA_H

#include "QMainWindow"
#include <QObject>
#include <stdio.h>
#include <process.h>
#include <conio.h>
#include "windows.h"
#include "MvCameraControl.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "string.h"
#include "time.h"
#include <zbar.h>
#include <iostream>

#include "QLabel"
#include "QListWidget"

using namespace zbar;
using namespace std;
using namespace cv;

//namespace Ui { class Camera; }
class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = nullptr);
    void findCamera();
    void connetCamera();
    void startGrapImage();
    void endGrapImage();
    void closeDevice();
    void showImage();

    // 以时间作为图片名 精确到毫秒
    static void GetTimeAsFileName();
    // 识别二维码 手动ROI模式
    void detect_bar(cv::Mat inputImg);
    // 识别二维码 自动识别模式
    static void detect_bar_auto(cv::Mat input);
    // ch:等待按键输入 | en:Wait for key press
    void WaitForKeyPress(void);
    // 输出设备信息
    bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo);
    // convert RGB to BGR
    static int RGB2BGR(unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight);
    // convert data stream in Mat format
    static bool Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char* pData, cv::Mat& mat_img);
    //工作线程
    static  unsigned int __stdcall WorkThread(void* pUser);

    QLabel* label_realtimeView;
    QListWidget* list_cameraInfo;

signals:

private:


};

#endif // CAMERA_H
