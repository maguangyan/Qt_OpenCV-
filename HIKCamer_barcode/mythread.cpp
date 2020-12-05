#include "mythread.h"
#include "QDebug"

myThread::myThread(QObject *parent) : QThread(parent)
{

}

myThread::~myThread()
{
    qDebug() << "myThread::~myThread()";
}

void myThread::run()
{

}
