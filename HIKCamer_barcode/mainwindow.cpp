#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cv::Mat src = cv::imread("E:/workSpace/qt_workspace/HIKCamer_barcode/pictures/test1.png");
    cv::imshow("test", src);
}

MainWindow::~MainWindow()
{
    delete ui;
}

