#include"MyThread.h"
MyThread::MyThread(QObject* parent, void* _param)
    :QThread(parent),param(_param)
{
    timer = new QTimer(this);
    timer->setInterval(50);

    connect(timer, &QTimer::timeout, this, [=]()
		{
            updatenow = 1;
		});
    timer->start();
}

void MyThread::run()
{
    Socket5_client* widget = (Socket5_client*)param;
    ActionBox& actionbox = widget->actionbox;
    int& actionPointer = widget->actionPointer;
    mutex& actionPointer_mutex = widget->actionPointer_mutex;       // 取参数
    while (1)
    {
        if (exit == 1)          // 退出
        {
            break;
        }
        if (actionbox.empty())      // 没有消息
        {
            continue;
        }

        actionPointer_mutex.lock();     // 加锁，访问互斥资源消息盒子
        if (updatenow)          // 强制刷新，(当有新的客户机连接上以后，强制刷新当前消息ID）
        {
            updatenow = 0;
            actionPointer = (*actionbox.begin())->idx;
        }
        if ((*(actionbox.begin()))->idx == actionPointer)       // 有消息，并且消息ID正是我想要的，执行
        {
            Action* tmp = *(actionbox.begin());
            actionbox.erase(tmp);
            actionPointer = tmp->idx + 1;
            emit runAction_signal(tmp);

        }
        actionPointer_mutex.unlock();   //释放锁
    }
    //if (actionPointer < (*actionbox.begin())->idx)  
    //{
    //    actionPointer = (*actionbox.begin())->idx;
    //}
    //else if (abs((*actionbox.begin())->idx - actionPointer) >= 5)
    //{
    //    actionPointer = (*actionbox.begin())->idx;
    //}
    //else 
}
