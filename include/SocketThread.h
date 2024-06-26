#pragma once
#include"MySocket.h"
class Socket5_server;
class Socket5_client;
typedef void (*ThreadProc)(LPVOID lpParam);
struct Params
{
	MySocket* client;
	Socket5_server* widget;
};
struct Params_client
{
	MySocket* client;
	Socket5_client* widget;
	string ip;
	u_short port;
};
class SocketThread
{

public:
	SocketThread(void* _params, ThreadProc _proc);
	virtual void run();
protected:
	void* params;
	HANDLE hThread;
	ThreadProc threadProc;
};