#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"
#include "QHBoxLayout"
#include "QLabel"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QHBoxLayout *layout = new QHBoxLayout(ui->centralwidget);
    QLabel *label = new QLabel();
    layout->addWidget(label);
    cv::Mat src = cv::imread("E:/Pictures/four_1.png");
    cv::cvtColor(src, src, cv::COLOR_BGR2RGB);
    QImage img = QImage(src.data, src.cols, src.rows, src.step, QImage::Format_RGB888);
    int w = img.width();
    int h = img.height();
    if(w > 800 || h > 800)
    {
        double rate = 888.0 / std::max(w, h);
        int nw = static_cast<int>(rate * w);
        int nh = static_cast<int>(rate * h);
        img = img.scaled(QSize(nw, nh), Qt::KeepAspectRatio);

    }
    QPixmap mp;
    mp = mp.fromImage(img);
    label->setPixmap(mp);
    label->setAlignment(Qt::AlignCenter);
    src.release();
}

MainWindow::~MainWindow()
{
    delete ui;
}

