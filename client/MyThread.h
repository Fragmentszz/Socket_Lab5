#pragma once
#include<QThread>
#include<Format.h>
#include"Socket5_client.h"

class MyThread
    :public QThread
{
    Q_OBJECT
public:
    void* param;
    MyThread(QObject* parent = nullptr, void* _param = nullptr);
    bool exit = 0;
    QTimer* timer;
    bool updatenow = false;
protected:
    void run();
signals:
    void runAction_signal(Action* action);
};
