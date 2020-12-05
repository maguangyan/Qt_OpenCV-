#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class myThread : public QThread
{
    Q_OBJECT
public:
    explicit myThread(QObject *parent = nullptr);
    ~myThread();

    void run();
signals:

};

#endif // MYTHREAD_H
