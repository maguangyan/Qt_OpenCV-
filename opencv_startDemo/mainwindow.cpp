#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"
#include "QFileDialog"
#include "QtDebug"
#include <QTextCodec>

cv::Mat _src;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 选取文件
    connect(ui->btn_openFile,&QPushButton::clicked,[=](){

        //点击选取文件按钮，弹出文件对话框
        QString path = QFileDialog::getOpenFileName(this,"打开文件",".", tr("Image Files(*.png *.jpg *jpeg *.bmp *.avi *.mp4)"));
//        QTextCodec *code = QTextCodec::codecForName("utf-8");//解决中文路径问题
//        QString pathname = code->fromUnicode(path).data();

        //std::cout << std::string((const char *)path.toLocal8Bit()) <<std::endl;;

        //将路径放入到lineEdit中
        ui->show_filePath->setText(path);
        _src = cv::imread(std::string((const char *)path.toLocal8Bit()));

        if(_src.empty())
        {
            qDebug() << "could not load the image...";
            return;
        }

        cv::Mat src = _src.clone();
        cv::cvtColor(src, src, cv::COLOR_BGR2RGB);
        QImage img = QImage(src.data, src.cols, src.rows, src.step, QImage::Format_RGB888);

        int lb_w = ui->label_src->size().width();
        int lb_h = ui->label_src->size().height();

        QImage result = img.scaled(lb_w, lb_h, Qt::KeepAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示

        QPixmap mp;
        mp = mp.fromImage(result);
        ui->label_src->setPixmap(mp);
        ui->label_src->setAlignment(Qt::AlignCenter);
        src.release();
    });

    // canny提取边缘
    connect(ui->btn_canny,&QPushButton::clicked,[=](){


        cv::Mat src = _src.clone();
        if(src.empty())
        {
            qDebug() << "could not load the image...";
            return;;
        }
        cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
        cv::Mat edge;
        ui->text_cannyLowValue->setText(QString::number(ui->slider_cannyLowValue->value()));
        ui->text_cannyHighValue->setText(QString::number(ui->slider_cannyHighValue->value()));
        cv::Canny(src, edge, ui->slider_cannyLowValue->value(), ui->slider_cannyHighValue->value());
        QImage img = QImage(edge.data, edge.cols, edge.rows, edge.step, QImage::Format_Grayscale8);

        int lb_w = ui->label_canny->size().width();
        int lb_h = ui->label_canny->size().height();
        QImage result = img.scaled(lb_w, lb_h, Qt::KeepAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示

        QPixmap mp;
        mp = mp.fromImage(result);
        ui->label_canny->setPixmap(mp);
        ui->label_canny->setAlignment(Qt::AlignCenter);
        src.release();
    });

    // 二值化
    connect(ui->btn_binary,&QPushButton::clicked,[=](){


        cv::Mat src = _src.clone();
        if(src.empty())
        {
            qDebug() << "could not load the image...";
            return;
        }
        cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
        cv::Mat bin;
        ui->text_binaryValue->setText(QString::number(ui->slider_binaryValue->value()));
        cv::threshold(src, bin, ui->slider_binaryValue->value(), 255, cv::THRESH_BINARY);
        QImage img = QImage(bin.data, bin.cols, bin.rows, bin.step, QImage::Format_Grayscale8);

        int lb_w = ui->label_binary->size().width();
        int lb_h = ui->label_binary->size().height();
        QImage result = img.scaled(lb_w, lb_h, Qt::KeepAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示

        QPixmap mp;
        mp = mp.fromImage(result);
        ui->label_binary->setPixmap(mp);
        ui->label_binary->setAlignment(Qt::AlignCenter);
        src.release();
    });

    //高斯模糊
    connect(ui->btn_gauss, &QPushButton::clicked,[=](){
        cv::Mat pic = _src.clone();
        if(pic.empty())
        {
            qDebug()<<"could not load image ...";
            return;
        }

        cv::Mat gauss_img;
        int Ksize =2 * ui->slider_gaussKernalSize->value() + 1;
        ui->text_gaussKernalSize->setText(QString::number(Ksize));
        cv::GaussianBlur(pic, gauss_img, cv::Size(Ksize, Ksize), 1, 1);
        QImage img = QImage(gauss_img.data, gauss_img.cols, gauss_img.rows, gauss_img.step, QImage::Format_BGR888);

        int lb_w = ui->label_draw->size().width();
        int lb_h = ui->label_draw->size().height();
        QImage result = img.scaled(lb_w, lb_h, Qt::KeepAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示

        QPixmap mp;
        mp = mp.fromImage(result);
        ui->label_draw->setPixmap(mp);
        ui->label_draw->setAlignment(Qt::AlignCenter);
        pic.release();
    });

}






MainWindow::~MainWindow()
{
    delete ui;
    _src.release();
}

