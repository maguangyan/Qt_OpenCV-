#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //创建一个Camera对象
    this->cam = new Camera(this);
    cam->label_realtimeView = ui->lb_realtimeView;
    cam->list_cameraInfo = ui->list_cameraInfo;

    //创建一个algorithm对象
    //this->alg = new myAlgorithmLibrary(this);

    // 查找相机
    connect(ui->btn_findCamera, &QPushButton::clicked, [=](){
        cam->findCamera();
    });

    // 连接相机
    connect(ui->btn_connectCamera, &QPushButton::clicked, [=](){
        cam->connetCamera();
    });

    // 开始取流并显示实时画面
    connect(ui->btn_grap, &QPushButton::clicked, [=](){
        cam->startGrapImage();
    });

    // 停止取流
    connect(ui->btn_stopGrapImage, &QPushButton::clicked, [=](){
        cam->endGrapImage();
    });

    // 关闭设备

    connect(ui->btn_closeDevice, &QPushButton::clicked, [=](){
        cam->closeDevice();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

