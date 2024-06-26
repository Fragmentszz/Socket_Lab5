#include "Socket5_client.h"
#include<QButtonGroup>
#include<QOverload>


void ClientThread(LPVOID lpParam)
{
    Params_client* params = (Params_client*)lpParam;
    MySocket* sClient = params->client;
    Socket5_client* widget = params->widget;
    if (sClient->isBinded())
    {
        while (1)
        {
            pair<bool, string> ret = sClient->receive();
            if (ret.first == 0)
            {
                emit widget->receiveMsg(ret.second);
                if (ret.second == "exit")
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }
}

string createDiv_send(string name, string msg, string nowtime)
{
	string tmp = string("<div style=\"text-align:right\">");
	// 创建发送者消息的HTML，并右对齐
	string html = ( tmp + "(" + nowtime + ")<strong>" + name + "</strong>:<br><font size=\"5\">" + msg + "</font></div>");
	return html;
}

string createDiv_receive(string name, string msg, string nowtime)
{
	string tmp = string("<div ><font color=\"red\">");
	string z = (tmp + "(" + nowtime + ")<strong>" + name + "</strong>:<br><font size=\"5\">" + msg + "</font></font></div>");
	return z;
}

Socket5_client::Socket5_client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Socket5_client()),client(nullptr),connectThread(nullptr),workThread(nullptr),inited(0),clearlq(0)
{
    
    ui->setupUi(this);
    
    scene = new QGraphicsScene(this);
    gView = new MyGraphicsView(this,scene,nullptr);
    gView->setGeometry(0,0,1000,600);
    initTimer();
    init();
    connect(ui->reset, &QPushButton::clicked, this, &Socket5_client::reSet);
    
    connect(ui->AxisX, SIGNAL(valueChanged(int)), this, SLOT(on_AxisX_valueChanged()));
    connect(ui->AxisY, SIGNAL(valueChanged(int)), this, SLOT(on_AxisY_valueChanged()));
    connect(ui->ColorR, SIGNAL(valueChanged(int)), this, SLOT(on_R_valueChanged()));
    connect(ui->ColorG, SIGNAL(valueChanged(int)), this, SLOT(on_G_valueChanged()));
    connect(ui->ColorB, SIGNAL(valueChanged(int)), this, SLOT(on_B_valueChanged()));

    connect(ui->width, SIGNAL(valueChanged(int)), this, SLOT(on_width_valueChanged()));
    connect(ui->height, SIGNAL(valueChanged(int)), this, SLOT(on_height_valueChanged()));

    connect(ui->appendShape, &QPushButton::clicked, this, &Socket5_client::appendShape);

    gView->setMouseTracking(true);

    connect(ui->draw_clear, &QPushButton::clicked, this, &Socket5_client::drawing_clear);
    connect(ui->drawing,&QPushButton::clicked, this, &Socket5_client::drawing_clicked);
    connect(ui->drawColor, &QPushButton::clicked, this, &Socket5_client::drawColor_clicked);

    connect(ui->connect, &QPushButton::clicked, this, &Socket5_client::connectServer);
    
    connect(this, &Socket5_client::connectFinish, this, &Socket5_client::conncetFinish_slots);

    connect(ui->closeConnect, &QPushButton::clicked, this, &Socket5_client::closeConnect);

    connect(this, &Socket5_client::receiveMsg, this, &Socket5_client::msgBoxChange);

    // connect(this, &Socket5_client::actionAppend, this, &Socket5_client::actionAppend_slot);

    connect(this, &Socket5_client::receiveCreateCurve, gView, &MyGraphicsView::createCurve);
    connect(this, &Socket5_client::receiveTransPoint, gView, &MyGraphicsView::addPoints);

    connect(ui->sendMsg, &QPushButton::clicked, this, &Socket5_client::sendMsg);

    // 创建发送者消息的HTML，并右对齐
    string html = createDiv_send("me:", "发送姓名", "发送时间");

    string z = createDiv_receive("发送人昵称", "接收信息", "发送时间");
    
    ui->messageBox->setText(QString::fromStdString(html) + QString::fromStdString(z));

    runThread = new MyThread(this,this);
    connect(runThread,&MyThread::runAction_signal, this, &Socket5_client::actionRun_slot);
    runThread->start();


    // 连接文本变化信号
    connect(ui->messageBox, &QTextEdit::textChanged, [&]() {
        ui->messageBox->moveCursor(QTextCursor::End);
    });
}



Socket5_client::~Socket5_client()
{
    for (auto shape : shapes)
    {
        if (shape.shape)
            delete shape.shape;
    }
    if (scene != nullptr)
    {
        delete scene;
    }

    shapes.clear();
    runThread->exit = 1;
    runThread->wait();
    runThread->quit();
    delete runThread;
    if(ui) delete ui;
    if(gView) delete gView;
    if(timer) delete timer;
    if(client) delete client;
    if(workThread) delete workThread;

    if (WSACleanup() == SOCKET_ERROR)
    {
        cout << "WSACleanup failed" << endl;
    }
    
}

void Socket5_client::actionRun_slot(Action* action)
{
    if (action->action == "update")
    {
        UpdateAction* updateAction = (UpdateAction*)action;
        int index = updateAction->index;
        int dx = updateAction->dx;
        int dy = updateAction->dy;
        int h = updateAction->h;
        int w = updateAction->w;
        int r = updateAction->r;
        int g = updateAction->g;
        int b = updateAction->b;
        if (index < shapes.size())
        {
            updateShpe(index, dx, dy, h, w, QColor(r, g, b));
        }
    }
    else if (action->action == "draw")
    {

        DrawAction* drawAction = (DrawAction*)action;
        int shape = drawAction->shape;
        int dx = drawAction->dx;
        int dy = drawAction->dy;
        int h = drawAction->h;
        int w = drawAction->w;
        int r = drawAction->r;
        int g = drawAction->g;
        int b = drawAction->b;

        if (shape == 1)
        {
            MyEllipse* Eli = new MyEllipse(nullptr, w, h, 8, QColor(r, g, b));
            addShape(Eli);
        }
        if (shape == 0)
        {
            MyRect* rect = new MyRect(nullptr, h, w, 8, QColor(r, g, b));
            addShape(rect);
        }
        else if (shape == 2)
        {
            emit receiveCreateCurve(QColor(r, g, b));
        }
    }
    else if (action->action == "clear")
    {
        ClearAction* clearAction = (ClearAction*)action;
        if (clearAction->index == -1)
        {
            init();
        }
        else
        {
            if (clearAction->index < shapes.size())
            {
                scene->removeItem(shapes[clearAction->index].shape);

                shapes[clearAction->index].shape = nullptr;
                if (shapes[clearAction->index].radio)
                {
                    shapes[clearAction->index].radio->deleteLater();
                    shapes[clearAction->index].radio = nullptr;
                }
            }
        }
    }
    else if (action->action == "exit")
    {
        close();
    }
    else if (action->action == "addPoint")
    {
        emit receiveTransPoint(((AddPointAction*)action)->index, ((AddPointAction*)action)->points);
    }
    else if (action->action == "message")
    {
        receivePureMsg((MessageAction*)action);
    }
    else if (action->action == "choose")
    {
        ChooseAction* chooseAction = (ChooseAction*)action;
        int index = chooseAction->index;
        cout << "choose:" << index << endl;
        if (index >= 0 && index < shapes.size())
        {
            shapes[index].radio->setEnabled(false);
        }
        if (index < 0)
        {
            int nowindex = -index - 1;
            cout << nowindex << endl;
            if (nowindex >= 0 && nowindex < shapes.size())
            {
                shapes[nowindex].radio->setEnabled(true);
            }
        }
    }
}



void Socket5_client::on_AxisX_valueChanged()
{
    if (nowShape >= 0 && nowShape < shapes.size())
    {
        shapes[nowShape].dx = ui->AxisX->value();
		shapes[nowShape].shape->setPos(ui->AxisX->value(), ui->AxisY->value());
        scene->update();
        ChangeSet.insert(nowShape);
	}
}


void Socket5_client::init()
{
    timeChange();
    for (auto shape : shapes)
    {
        shape.radio->deleteLater();
    }
    shapes.clear();
    gView->clearDrawed();
    scene->clear();
    
    int width = gView->width(), height = gView->height();
    ui->AxisX->setRange(-width / 2 - 400, width / 2 + 400);
    ui->AxisY->setRange(-height / 2 - 400, height / 2 + 400);
    ui->AxisX->setValue(0);
    ui->AxisY->setValue(0);

    ui->ColorR->setRange(0, 255);
    ui->ColorG->setRange(0, 255);
    ui->ColorB->setRange(0, 255);
    ui->ColorR->setValue(0);
    ui->ColorG->setValue(0);
    ui->ColorB->setValue(0);


    ui->width->setRange(0, 1000);
    ui->height->setRange(0, 1000);
    ui->width->setValue(0);
    ui->height->setValue(0);


    gView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    gView->setSceneRect(-width / 2, -height / 2, width, height);

    gView->setScene(scene);

    nowShape = -1;
    inited = 1;

    actionPointer = 0;
}

void Socket5_client::addShape(MyShape* shape)
{
    int i = shapes.size();
    shapes.push_back(Shape(shape));
	scene->addItem(shape);
    QRadioButton* radio = new QRadioButton(ui->radioGroup);
    shapes[i].radio = radio;
    radio->setGeometry((i / 4) * 70, (i % 4) * 20 + 20, 50, 20);
    radio->setText(QString::number(i));
    connect(radio, &QRadioButton::clicked, this, &Socket5_client::radioClicked);
    radio->setChecked(false);
    radio->show();
}

void Socket5_client::initTimer()
{
    timer = new QTimer();
    timer->setInterval(1000);
    MyInterval interval = MyInterval(8, 0, 0);

    connect(timer,&QTimer::timeout, this, &Socket5_client::timeChange);
    nowtime = TIME();
    nowtime = nowtime + interval;
    timer->start();


    timer2 = new QTimer();
    timer2->setInterval(100);
    connect(timer2, &QTimer::timeout, this, &Socket5_client::clearTimeOut);

    timer_change = new QTimer();
    timer_change->setInterval(50);

    connect(timer_change, &QTimer::timeout, [&]() {
		if (ChangeSet.size() == 0)
		{
			return;
		}
		for (auto index : ChangeSet)
		{
            change(index);
		}
		ChangeSet.clear();
	});
    timer_change->start();
}

void Socket5_client::updateShpe(int index, int dx, int dy, int h, int w, QColor color)
{
    if (index < shapes.size())
    {
        shapes[index].shape->changeColor_fill(color);
        shapes[index].shape->setHeight(h);
        shapes[index].shape->setWidth(w);
        shapes[index].shape->setPos(dx, dy);
        shapes[index].dx = dx;
        shapes[index].dy = dy;
        scene->update();
    }
}

void Socket5_client::change(int now)
{
    if (now >= 0 && now < shapes.size())
    {
        Message msg(shapes[now].shape, "update", actionPointer, now);
        send(msg.getmsg());
    }
}




void Socket5_client::send(string msg)
{
    cout << "send:" << msg << endl;
    if(client && client->isBinded())
	{
        if (clearlq)
        {
            if (msg.substr(0, 5) == "clear")
            {
                return;
            }
        }
        actionPointer_mutex.lock();
        if (client->send(msg))
        {
            actionPointer++;
        }
        else
        {
            QMessageBox::warning(this, "Warning", "send failed!");
        }
        actionPointer_mutex.unlock();
	}
}

void Socket5_client::on_AxisY_valueChanged()
{
    if (nowShape >= 0 && nowShape < shapes.size())
    {
        shapes[nowShape].dy = ui->AxisY->value();
        shapes[nowShape].shape->setPos(ui->AxisX->value(), ui->AxisY->value());
        scene->update();
        ChangeSet.insert(nowShape);
    }
}

void Socket5_client::radioClicked()
{
	QRadioButton* radio = (QRadioButton*)sender();
 
	int nowShape = radio->text().toInt();
    if (nowShape == this->nowShape)
    {
        if (nowShape >= 0 && nowShape < shapes.size())
        {
            shapes[nowShape].radio->setAutoExclusive(false);
            shapes[nowShape].radio->setChecked(0);
            shapes[nowShape].radio->setAutoExclusive(true);
            shapes[nowShape].radio->setEnabled(true);
        }
        send(Message::chooseMsg(actionPointer, -(nowShape + 1)).getmsg());
        this->nowShape = -1;
    }
    else
    {
        
        QColor color = shapes[nowShape].shape->getColor_fill();
        if(this->nowShape >= 0)
            send(Message::chooseMsg(actionPointer, -(this->nowShape+1)).getmsg());
        send(Message::chooseMsg(actionPointer, nowShape).getmsg());

        QSignalBlocker block(ui->ColorR);
        QSignalBlocker block2(ui->ColorG);
        QSignalBlocker block3(ui->ColorB);
        QSignalBlocker block4(ui->width);
        QSignalBlocker block5(ui->height);
        QSignalBlocker block6(ui->AxisX);
        QSignalBlocker block7(ui->AxisY);

        ui->AxisX->setValue(shapes[nowShape].dx);
        ui->AxisY->setValue(shapes[nowShape].dy);

        ui->ColorR->setValue(color.red());
        ui->ColorG->setValue(color.green());
        ui->ColorB->setValue(color.blue());

        ui->width->setValue(shapes[nowShape].shape->getWidth());
        ui->height->setValue(shapes[nowShape].shape->getHeight());
        shapes[nowShape].radio->setChecked(true);
        this->nowShape = nowShape;
    }


}

void Socket5_client::on_R_valueChanged()
{
    if (nowShape >= 0 && nowShape < shapes.size())
    {
		shapes[nowShape].shape->changeColor_fill(QColor(ui->ColorR->value(), ui->ColorG->value(), ui->ColorB->value()));
        ChangeSet.insert(nowShape);
        scene->update();
    }
}

void Socket5_client::on_G_valueChanged()
{
    if (nowShape >= 0 && nowShape < shapes.size())
    {
        shapes[nowShape].shape->changeColor_fill(QColor(ui->ColorR->value(), ui->ColorG->value(), ui->ColorB->value()));
        ChangeSet.insert(nowShape);
        scene->update();
    }
}

void Socket5_client::on_B_valueChanged()
{

    if (nowShape >= 0 && nowShape < shapes.size())
    {
       
        shapes[nowShape].shape->changeColor_fill(QColor(ui->ColorR->value(), ui->ColorG->value(), ui->ColorB->value()));
        ChangeSet.insert(nowShape);
        scene->update();
    }
}

void Socket5_client::on_width_valueChanged()
{
    if (nowShape >= 0 && nowShape < shapes.size())
    {
        int afterwidth = ui->width->value();
        int nowwidth = shapes[nowShape].shape->getWidth();
        if (afterwidth == nowwidth)
        {
            return;
        }
        shapes[nowShape].shape->setWidth(ui->width->value());
        ChangeSet.insert(nowShape);
        scene->update();
	}
}

void Socket5_client::on_height_valueChanged()
{
    if (nowShape >= 0 && nowShape < shapes.size())
    {
        int afterheight = ui->height->value();
        int nowheight = shapes[nowShape].shape->getHeight();
        if (afterheight == nowheight)
        {
			return;
		}
        shapes[nowShape].shape->setHeight(ui->height->value());
        ChangeSet.insert(nowShape);
        scene->update();
    }
}

void Socket5_client::appendShape()
{
    MyShape* shape = nullptr;
    appendDialog* dialog = new appendDialog(this);
    dialog->exec();
    shape = dialog->getShape();
    if (shape == nullptr)
    {
		return;
	}
    addShape(shape);
    string msg = Message(shape, "draw", actionPointer).getmsg();
    send(msg);
}

void Socket5_client::reSet()
{
    send("clear;" + to_string(actionPointer++) + ";***");
    init();
}


void Socket5_client::timeChange()
{
    nowtime = nowtime + MyInterval(0, 0, 1);
    ui->day->setText(num2QString(nowtime.day, 2));
    ui->month->setText(num2QString(nowtime.month, 2));
    ui->year->setText(num2QString(nowtime.year, 4));
    QStringList list = { num2QString(nowtime.hour,2),num2QString(nowtime.minute,2),num2QString(nowtime.second,2) };
    string time = list.join(":").toStdString();
    ui->time->setText(list.join(":"));
}

void Socket5_client::graphicsViewMouseMove(QMouseEvent* mouseEvent)
{
    qDebug() << mouseEvent->pos();
}

void Socket5_client::drawing_clicked()
{
    if (gView->getDrawing())
    {
        gView->setDrawing(false);
        ui->drawing->setText("Enable");
    }
    else {
        gView->setDrawing(true);
        ui->drawing->setText("Disable");
    }
}



void Socket5_client::drawing_clear()
{
    gView->clearDrawed();
    // init();
}

void Socket5_client::drawColor_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Select Color");
	if (color.isValid())
	{
        gView->setBGColor(color);
        QStringList list = { QString::number(color.red()), QString::number(color.green()), QString::number(color.blue()) };
        //qDebug() << " background-color: rgb(" + list.join(",") + "); " ;
        //qDebug() << color;
        ui->drawColor->setStyleSheet(" background-color: rgb(" + list.join(",")+ "); ");
	}
	scene->update();
}


bool isValidIPv4(const std::string& ip) {
    // 分割字符串
    std::vector<std::string> parts;
    std::stringstream ss(ip);
    std::string segment;
    while (std::getline(ss, segment, '.')) {
        parts.push_back(segment);
    }

    // 必须正好有4个部分
    if (parts.size() != 4) {
        return false;
    }
    for (const auto& part : parts) {
        // 检查每个部分是否为数字
        for (char c : part) {
            if (!isdigit(c)) return false;
        }

        // 转换为整数
        int num = std::stoi(part);

        // 检查每个数字是否在0到255之间
        if (num < 0 || num > 255) {
            return false;
        }
    }
    return true;
}

void workT(LPVOID lpParam)
{
    Params_client* params = (Params_client*)lpParam;        // 拆解参数
    MySocket* client = params->client;
    Socket5_client* widget = params->widget;        
    while (1)           // 不断接受消息
    {
        pair<bool,string> res = client->receive();      // 从该socket接受数据
        if (res.first == 0)
        {
            string msg = res.second;
            emit widget->receiveMsg(msg);			   // 发送信号，加入消息盒子
        }
        else
            break;
    }
    delete params;
}

void connectT(LPVOID lpParam)
{
    Params_client* params = (Params_client*)lpParam;
    MySocket* client = params->client;
    Socket5_client* widget = params->widget;
    
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.S_un.S_addr = inet_addr(params->ip.c_str());
    serveraddr.sin_port = htons(params->port);
    cout << "connecting..." << endl;
    if (client->connect(params->ip,params->port))
    {
        Params_client* params = new Params_client();
        params->client = client;
        params->widget = widget;
        SocketThread* tmp = new SocketThread(params,workT);
        emit widget->connectFinish(tmp);
    }
    else
        emit widget->connectFinish(nullptr);
    delete params;
}

void Socket5_client::connectServer()
{
    if(isconnecting)
	{
		QMessageBox::warning(this, "Warning", "正在连接...");
		return;
	}
    isconnecting = 1;
    QString ip = ui->ip->toPlainText();
    if (!isValidIPv4(ip.toStdString()))
    {
        QMessageBox::warning(this, "Warning", "Invalid IPv4 address!");
		return;
    }
    short port = ui->port->value();
    if (client)
    {
        delete client;
    }
    if(connectThread)
	{
		delete connectThread;
	}

    client = new MySocket();
    Params_client* params = new Params_client();
    params->client = client;
    params->ip = ip.toStdString();
    params->port = port;

    params->widget = this;

    connectThread = new SocketThread(params,connectT);
    connectThread->run();
    
}

void Socket5_client::conncetFinish_slots(SocketThread* thread)
{
    workThread = thread;
    if (thread == nullptr)
    {
        QMessageBox::warning(this, "Warning", "connect failed!");
    }
    else
    {
        QMessageBox::information(this, "Information", "connect successfully!");
        workThread->run();

    }
    isconnecting = 0;
}

void Socket5_client::closeConnect()
{
    if (client)
    {
        delete client;
        client = nullptr;
    }
    if (connectThread)
    {
        delete connectThread;
        connectThread = nullptr;
    }
	if(workThread)
	{
		delete workThread;
        workThread = nullptr;
	}

}

void Socket5_client::msgBoxChange(string msg)
{
    Message message = Message(msg);
    cout << "recev msg:" << msg << endl;
    vector<Action*> actions = message.parse();
    actionPointer_mutex.lock();             // 加锁，访问互斥资源消息盒子
    for (auto action : actions)
    {
        actionbox.insert(action);
    }
    actionPointer_mutex.unlock();   // 释放锁

}



void Socket5_client::clearTimeOut()
{
    timer2->stop();
    clearlq = 0;
}

void Socket5_client::sendMsg()
{
    string msg = ui->input->toPlainText().toStdString();
    string name = ui->name->toPlainText().toStdString();
    string nowtime = getCurrentTimeString();
    Message m = Message::pureMsg(actionPointer, name, msg,nowtime);
    send(m.getmsg());
    ui->input->clear();
    // 创建发送者消息的HTML，并右对齐
    string html = createDiv_send("Me", msg, nowtime);
    nowMsg = nowMsg + html;
    ui->messageBox->setText(QString::fromStdString(nowMsg));
}


// 111.231.71.182
//void Socket5_client::receivePureMsg(MessageAction* action)
//{
//    QString name = QString::fromStdString(action->name);
//    QString msg = QString::fromStdString(action->msg);
//    QString nowtime = QString::fromStdString(action->nowtime);
//
//    // 创建接收者消息的HTML，并左对齐，同时设置颜色为红色
//    QString html = "<div style='text-align: left;'><b>" + name + " (" + nowtime + "):</b> "
//        "<font color='red'>" + msg + "</font></div>";
//    // 将HTML追加到消息框中
//    ui->messageBox->setText(ui->messageBox->toPlainText() + html);
//}

void Socket5_client::receivePureMsg(MessageAction* action)
{
    string html = createDiv_receive(action->name, action->msg, action->nowtime);
    nowMsg = nowMsg + html;
    ui->messageBox->setText(QString::fromStdString(nowMsg));
}

void Socket5_client::createCurve_slot(QColor color)
{
	string msg = Message::createCurve(actionPointer, color).getmsg();
    send(msg);
}

void Socket5_client::transPoint_slot(int index, std::vector<QPoint> points)
{
    string msg = Message::transPoint(actionPointer,index, points).getmsg();
    send(msg);
}


