#include<Format.h>
#include <iomanip>


Message::Message(string _msg)
	:msg(_msg)
{

}
string to_string(int num)
{
	string str;
	std::stringstream ss;
	ss << num;
	ss >> str;
	return str;
}
string to_string(QColor color)
{
	string str;
	str += to_string(color.red());
	str += ";";
	str += to_string(color.green());
	str += ";";
	str += to_string(color.blue());
	str += ";";
	return str;
}
string to_string(MyShape* shape)
{
	string str;
	str += to_string(shape->cato);
	str += ";";
	str += to_string(shape->x());
	str += ";";
	str += to_string(shape->y());
	str += ";";
	str += to_string(shape->getHeight());
	str += ";";
	str += to_string(shape->getWidth());
	str += ";";
	str += to_string(shape->getColor_fill());
	return str;
}


Message::Message(MyShape* shape,string head)
{
	string msg = head;
	msg += ";";
	msg += to_string(shape);
	this->msg = msg + "***";
}

Message::Message(MyShape* shape, string action, int idx,int index)
{
	if (action == "draw")
	{
		string msg = action;
		msg += ";";
		msg += to_string(idx);
		msg += ";";
		msg += to_string(shape);
		this->msg = msg;
	}
	else if (action == "update")
	{
		string msg = action;
		msg += ";";
		msg += to_string(idx);
		msg += ";";
		msg += to_string(index);
		msg += ";";
		msg += to_string(shape);
		this->msg = msg;
	}
	else if (action == "move")
	{
		string msg = action;
		msg += ";";
		msg += to_string(idx);
		msg += ";";
		msg += to_string(index);
		msg += ";";
		msg += to_string(shape);
		this->msg = msg;
	}
	else if (action == "clear")
	{
		string msg = action;
		msg += ";";
		this->msg = msg;
	}
	this->msg += "***";
}

string Message::getmsg() const
{
	return msg;
}

std::vector<Action*> Message::parse()
{
	using namespace std;
	vector<Action*> actions;
	int k = 0;
	while (k < this->msg.length())
	{
		int nowk = k;
		int cnt = 0;
		for(;k<this->msg.length();k++)
		{
			if (this->msg[k] == '*')
			{
				cnt += 1;
			}
			else cnt = 0;
			if(cnt == 3)
			{
				k += 1;
				break;
			}
		}
		string msg = this->msg.substr(nowk, k - 3 - nowk);

		vector<int> pos;
		for (int i = 0; i < msg.length(); i++)
		{
			if (msg[i] == ';')
			{
				pos.push_back(i);
			}
		}
		vector<string> args;
		if (pos.back() != msg.length() - 1)
		{
			pos.push_back(msg.length() - 1);
		}
		for (int i = 0; i < pos.size(); i++)
		{
			if (i == 0)
			{
				args.push_back(msg.substr(0, pos[i]));
			}
			else
			{
				args.push_back(msg.substr(pos[i - 1] + 1, pos[i] - pos[i - 1] - 1));
			}
		}
		if (args[0] == "update")
		{
			UpdateAction* act = new UpdateAction;
			act->action = args[0];
			act->idx = stoi(args[1]);
			act->index = stoi(args[2]);
			act->dx = stoi(args[4]);
			act->dy = stoi(args[5]);
			act->h = stoi(args[6]);
			act->w = stoi(args[7]);
			act->r = stoi(args[8]);
			act->g = stoi(args[9]);
			act->b = stoi(args[10]);
			actions.push_back(act);
		}
		else if (args[0] == "draw")
		{
			DrawAction* act = new DrawAction;
			act->action = args[0];
			act->idx = stoi(args[1]);
			act->shape = stoi(args[2]);
			if (act->shape == 0 || act->shape == 1)
			{
				act->dx = stoi(args[3]);
				act->dy = stoi(args[4]);
				act->h = stoi(args[5]);
				act->w = stoi(args[6]);
				act->r = stoi(args[7]);
				act->g = stoi(args[8]);
				act->b = stoi(args[9]);
				actions.push_back(act);
			}
			else if (act->shape == 2)
			{
				act->r = stoi(args[3]);
				act->g = stoi(args[4]);
				act->b = stoi(args[5]);
				actions.push_back(act);
			}
			
		}
		else if (args[0] == "clear")
		{
			ClearAction* act = new ClearAction;
			act->action = args[0];
			act->idx = stoi(args[1]);
			if (args.size() > 2)
				act->index = stoi(args[2]);
			else act->index = -1;
			actions.push_back(act);
		}
		else if(args[0] == "addPoint")
		{
			AddPointAction* act = new AddPointAction();
			act->action = args[0];
			act->idx = stoi(args[1]);
			act->index = stoi(args[2]);
			for (int i = 3; i < args.size(); i += 2)
			{
				if (i + 1 >= args.size()) break;
				QPoint t = QPoint(stoi(args[i]), stoi(args[i + 1]));
				act->points.push_back(t);
			}
			actions.push_back(act);
		}
		else if (args[0] == "message")
		{
			MessageAction* act = new MessageAction();
			act->action = args[0];
			act->idx = stoi(args[1]);
			act->name = args[2];
			act->msg = args[3];
			act->nowtime = args[4];
			actions.push_back(act);
		}
		else if (args[0] == "choose")
		{
			ChooseAction* act = new ChooseAction();
			act->action = args[0];
			act->idx = stoi(args[1]);
			act->index = stoi(args[2]);
			actions.push_back(act);
		}
	}
	return actions;
}

Message Message::createCurve(int idx,QColor color)
{
	string msg = "draw;" + to_string(idx) + ";2;" + to_string(color) + "***";
	return Message(msg);
}

Message Message::chooseMsg(int idx, int index)
{
	string msg = "choose;" + to_string(idx) + ";" + to_string(index) + ";***";
	return Message(msg);
}

Message Message::transPoint(int idx, int index, std::vector<QPoint> points)
{
	string msg = "addPoint;" + to_string(idx) + ";" + to_string(index) + ";";
	for (int i = 0; i < points.size(); i++)
	{
		msg += to_string(points[i].x());
		msg += ";";
		msg += to_string(points[i].y());
		msg += ";";
	}
	msg += "***";
	return Message(msg);
}

std::string getCurrentTimeString() {
	auto now = std::chrono::system_clock::now();
	auto itt = std::chrono::system_clock::to_time_t(now);
	itt = itt + 8*3600;
	std::ostringstream ss;
	ss << std::put_time(gmtime(&itt), "%Y-%m-%d %H:%M:%S");
	return ss.str();
}

Message Message::pureMsg(int idx, string name, string msg,string nowtime)
{
	string str = "message;" + to_string(idx) + ";" + name + ";" + msg + ";" + nowtime + ";***";
	return Message(str);
}