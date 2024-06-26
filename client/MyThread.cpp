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
    mutex& actionPointer_mutex = widget->actionPointer_mutex;       // ȡ����
    while (1)
    {
        if (exit == 1)          // �˳�
        {
            break;
        }
        if (actionbox.empty())      // û����Ϣ
        {
            continue;
        }

        actionPointer_mutex.lock();     // ���������ʻ�����Դ��Ϣ����
        if (updatenow)          // ǿ��ˢ�£�(�����µĿͻ����������Ժ�ǿ��ˢ�µ�ǰ��ϢID��
        {
            updatenow = 0;
            actionPointer = (*actionbox.begin())->idx;
        }
        if ((*(actionbox.begin()))->idx == actionPointer)       // ����Ϣ��������ϢID��������Ҫ�ģ�ִ��
        {
            Action* tmp = *(actionbox.begin());
            actionbox.erase(tmp);
            actionPointer = tmp->idx + 1;
            emit runAction_signal(tmp);

        }
        actionPointer_mutex.unlock();   //�ͷ���
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
