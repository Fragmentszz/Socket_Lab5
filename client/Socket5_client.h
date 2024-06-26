#pragma once

#include <QtWidgets/QMainWindow>

#include "ui_Socket5_client.h"
#include<Shapes.h>
#include<qradiobutton.h>

#include<vector>
#include"appendDialog.h"
#include<QTimer>
#include"_basic.h"

#include"MyGraphicsView.h"
#include<QColorDialog>
#include<MySocket.h>
#include<SocketThread.h>
#include<qmessagebox.h>
#include<sstream>
#include<Format.h>
#include<set>
#include"MyThread.h"
#include<mutex>
using namespace std;




QT_BEGIN_NAMESPACE
namespace Ui { class Ui_Socket_sy3Class; };
QT_END_NAMESPACE


struct Shape
{
    MyShape* shape;
    int dx, dy;
    QRadioButton* radio;
    Shape(MyShape* _shape = nullptr, int _dx = 0, int _dy = 0)
    {
        shape = _shape;
        dx = _dx;
        dy = _dy;
        radio = nullptr;
    }
};



class MyThread;
// void runAction(LPVOID param);
class MyGraphicsView;
class Socket5_client : public QMainWindow
{
    Q_OBJECT
public:
    Socket5_client(QWidget *parent = nullptr);
    ~Socket5_client();
    ActionBox actionbox;
    int actionPointer = 0;
    void actionRun_slot(Action* action);
    void createCurve_slot(QColor color);
    void transPoint_slot(int index, std::vector<QPoint> points);
    MyThread* runThread;
    mutex actionPointer_mutex;
private:
    bool isconnecting = 0;
    Ui::Socket5_client *ui;
    vector<Shape> shapes;
    QGraphicsScene* scene;
    QTimer* timer,*timer2,*timer_change;
    TIME nowtime;
    int nowShape = -1;
    void init();
    bool inited = 0;
    void addShape(MyShape* shape);
    void initTimer();
    
    MyCurve* drawed;
    void updateShpe(int index, int dx, int dy, int h, int w, QColor color);
    MyGraphicsView* gView;

    MySocket* client;
    SocketThread* workThread;
    SocketThread* connectThread,*listenThread;

    std::set<MySocket*> clients;
    std::set<SocketThread*> threads;
    std::vector<Params*> params_vec;

    void change(int nowShpee);
    void send(string msg);
    string nowMsg;
    bool clearlq = 1;
    bool timeToChange = false;
    set<int> ChangeSet;
private slots:
    void on_AxisX_valueChanged();
    void on_AxisY_valueChanged();
    void radioClicked();

    void on_R_valueChanged();
    void on_G_valueChanged();
    void on_B_valueChanged();
    
    void on_width_valueChanged();
    void on_height_valueChanged();

    void appendShape();
    void reSet();
    void timeChange();
    void graphicsViewMouseMove(QMouseEvent* event);

    void drawing_clicked();
    void drawing_clear();

    void drawColor_clicked();

    void connectServer();
    void conncetFinish_slots(SocketThread* thread);

    void closeConnect();
    void msgBoxChange(string msg);
    void clearTimeOut();
    void sendMsg();
    void receivePureMsg(MessageAction* action);
    
signals:
    void connectFinish(SocketThread*);
    void receiveMsg(string msg);
    void actionAppend(Action* action);

    void receiveCreateCurve(QColor color);
    void receiveTransPoint(int index, std::vector<QPoint> points);


};

