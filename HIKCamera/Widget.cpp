#include "Widget.h"
#include "ui_Widget.h"
#include "ThreadFromQThread.h"
#include "ThreadObject.h"
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
  ,m_objThread(NULL)
  ,m_obj(NULL)
  ,m_currentRunLoaclThread(NULL)
{
    ui->setupUi(this);
    //控件初始化
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
    ui->progressBar_heart->setRange(0,100);
    ui->progressBar_heart->setValue(0);
    //按钮的信号槽关联 按钮创建控制线程（局部线程）

    // 开启子线程 在子线程m_thread的run()里面会循环调用doSomething
    connect(ui->pushButton_qthread1,&QPushButton::clicked
            ,this,&Widget::onButtonQThreadClicked);


    // 在主线程widget里面调用SetSomething
    connect(ui->pushButton_qthread1_setSomething,&QPushButton::clicked
            ,this,&Widget::onButtonQthread1SetSomethingClicked);
    // 在主线程widget里面调用GetSomething
    connect(ui->pushButton_qthread1_getSomething,&QPushButton::clicked
            ,this,&Widget::onButtonQthread1GetSomethingClicked);
    // 在主线程widget里面退出子线程
    connect(ui->pushButton_qthreadQuit,&QPushButton::clicked
            ,this,&Widget::onButtonQthreadQuitClicked);
    // 在主线程widget里面终止子线程
    connect(ui->pushButton_qthreadTerminate,&QPushButton::clicked
            ,this,&Widget::onButtonQthreadTerminateClicked);
    // 在主线程widget里面退出子程序
    connect(ui->pushButton_qthreadExit,&QPushButton::clicked
            ,this,&Widget::onButtonQThreadExitClicked);

    // 在主线程widget里面调用DoSomthing
    connect(ui->pushButton_doSomthing,&QPushButton::clicked
            ,this,&Widget::onButtonQThreadDoSomthingClicked);
    // 在主线程里边创建子线程(局部线程)
    connect(ui->pushButton_qthreadRunLocal,&QPushButton::clicked
            ,this,&Widget::onButtonQThreadRunLoaclClicked);

    // s使用QObject的方式创建线程
    connect(ui->pushButton_qobjectStart,&QPushButton::clicked
            ,this,&Widget::onButtonObjectMove2ThreadClicked);
    connect(ui->pushButton_qobjectStart_2,&QPushButton::clicked
            ,this,&Widget::onButtonObjectMove2Thread2Clicked);
    connect(ui->pushButton_qobjectStop,&QPushButton::clicked
            ,this,&Widget::onButtonObjectThreadStopClicked);

    // 主程序 利用定时器，控制heartBeat变化
    connect(&m_heart,&QTimer::timeout,this,&Widget::heartTimeOut);
    m_heart.setInterval(1000);

    //全局线程的创建 线程始终存在，并监视以下信号 （该线程给Widget发信号）
    //当检测到有人发布message信号的时候，把message的内容显示在text里边
    //当检测到有人发布progress信号的时候，控制第二行的progressBar的值
    //当检测有人发布finished信号的时候，在text框里边显示"ThreadFromQThread finish"
    m_thread = new ThreadFromQThread(this);
    connect(m_thread,&ThreadFromQThread::message
            ,this,&Widget::receiveMessage);
    connect(m_thread,&ThreadFromQThread::progress
            ,this,&Widget::progress);
    connect(m_thread,&QThread::finished
            ,this,&Widget::onQThreadFinished);

    m_heart.start();
}



Widget::~Widget()
{
    qDebug() << "start destroy widget";
    m_thread->stopImmediately();
    //局部线程的终结
    if(m_currentRunLoaclThread)
    {
        m_currentRunLoaclThread->stopImmediately();
    }
    m_thread->wait();
    delete ui;

    if(m_objThread)
    {
        m_objThread->quit();
    }
    m_objThread->wait();
    qDebug() << "end destroy widget";
}

void Widget::onButtonQThreadClicked()
{
    ui->progressBar->setValue(0);
    if(m_thread->isRunning())
    {
        return;
    }
    m_thread->start();
}

void Widget::progress(int val)
{
    ui->progressBar->setValue(val);
}

void Widget::receiveMessage(const QString &str)
{
    ui->textBrowser->append(str);
}

void Widget::heartTimeOut()
{
    static int s_heartCount = 0;
    ++s_heartCount;
    if(s_heartCount > 100)
    {
        s_heartCount = 0;
    }
    ui->progressBar_heart->setValue(s_heartCount);
}

void Widget::onButtonQthread1SetSomethingClicked()
{
    m_thread->setSomething();
}

void Widget::onButtonQthread1GetSomethingClicked()
{
    m_thread->getSomething();
}

void Widget::onButtonQthreadQuitClicked()
{
    ui->textBrowser->append("m_thread->quit() but not work");
    m_thread->quit();
}

void Widget::onButtonQthreadTerminateClicked()
{
    m_thread->terminate();
}

void Widget::onButtonQThreadDoSomthingClicked()
{
    m_thread->doSomething();
}

void Widget::onButtonQThreadExitClicked()
{
    m_thread->exit();
}

void Widget::onButtonQThreadRunLoaclClicked()
{
    //局部线程的创建的创建
    if(m_currentRunLoaclThread)
    {
        m_currentRunLoaclThread->stopImmediately();
    }
    ThreadFromQThread* thread = new ThreadFromQThread(NULL);
    connect(thread,&ThreadFromQThread::message
            ,this,&Widget::receiveMessage);
    connect(thread,&ThreadFromQThread::progress
            ,this,&Widget::progress);
    connect(thread,&QThread::finished
            ,this,&Widget::onQThreadFinished);
    connect(thread,&QThread::finished
            ,thread,&QObject::deleteLater);//线程结束后调用deleteLater来销毁分配的内存
    connect(thread,&QObject::destroyed,this,&Widget::onLocalThreadDestroy);
    thread->start();
    m_currentRunLoaclThread = thread;
}

void Widget::onQThreadFinished()
{
    ui->textBrowser->append("ThreadFromQThread finish");
}


void Widget::onLocalThreadDestroy(QObject *obj)
{
    if(qobject_cast<QObject*>(m_currentRunLoaclThread) == obj)
    {
        m_currentRunLoaclThread = NULL;
    }
}

void Widget::startObjThread()
{
    if(m_objThread)
    {
        return;
    }
    m_objThread= new QThread();
    m_obj = new ThreadObject();
    m_obj->moveToThread(m_objThread);
    connect(m_objThread,&QThread::finished,m_objThread,&QObject::deleteLater);
    connect(m_objThread,&QThread::finished,m_obj,&QObject::deleteLater);
    connect(this,&Widget::startObjThreadWork1,m_obj,&ThreadObject::runSomeBigWork1);
    connect(this,&Widget::startObjThreadWork2,m_obj,&ThreadObject::runSomeBigWork2);
    connect(m_obj,&ThreadObject::progress,this,&Widget::progress);
    connect(m_obj,&ThreadObject::message,this,&Widget::receiveMessage);

    m_objThread->start();
}

void Widget::onButtonObjectMove2ThreadClicked()
{
    if(!m_objThread)
    {
        startObjThread();
    }

    emit startObjThreadWork1();//主线程通过信号换起子线程的槽函数

    ui->textBrowser->append("start Obj Thread work 1");
}

void Widget::onButtonObjectMove2Thread2Clicked()
{
    if(!m_objThread)
    {
        startObjThread();
    }
    emit startObjThreadWork2();//主线程通过信号换起子线程的槽函数

    ui->textBrowser->append("start Obj Thread work 2");
}


void Widget::onButtonObjectThreadStopClicked()
{
    if(m_objThread)
    {
        if(m_obj)
        {
            m_obj->stop();
        }
    }
}




