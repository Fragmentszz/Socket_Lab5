#pragma once
#include<string>
#include<vector>
#include<set>
#include<Shapes.h>
#include<vector>
#include<sstream>
using string = std::string;
class Socket5_server;
class Socket5_client;
std::string getCurrentTimeString();
class Action
{
public:
	string action;
	int idx;
	bool operator<(const Action& a) const
	{
		return idx < a.idx;
	}
};

using ActionBoxIter = std::set<const Action*>::iterator;

class ActionCmp
{
public:
	bool operator()(const Action* x, const Action* y)
	{
		return x->idx < y->idx;
	}
};

using ActionBox = std::set<Action*>;

class UpdateAction
	:public Action
{
public:
	int index;
	int dx, dy;
	int h, w;
	int r, g, b;
};

class DrawAction
	:public Action
{
public:
	int shape=-1;
	int dx=0, dy=0;
	int h = 0, w=0;
	int r, g, b;
};

class ClearAction
	:public Action
{
public:
	int index;
};
class AddPointAction
	:public Action
{
public:
	int index;
	std::vector<QPoint> points;
};

class MessageAction
	:public Action
{
public:
	string name;
	string msg;
	string nowtime;
};
class ChooseAction
	:public Action
{
public:
	int index;
};
class Message
{
public:
	Message(string _msg);
	string msg;
	std::vector<Action*> parse();
	Message(MyShape* shape,string head);
	Message(MyShape* shape, string action, int idx, int index = 0);
	string getmsg() const;
	static Message createCurve(int idx,QColor color);
	static Message transPoint(int idx, int index, std::vector<QPoint> points);
	static Message pureMsg(int idx, string name, string msg,string nowtime);
	static Message chooseMsg(int idx, int index);
};
string to_string(MyShape* shape);


