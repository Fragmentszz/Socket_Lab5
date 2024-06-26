#pragma once
#include<WinSock2.h>
#include<iostream>
#include"Log.h"
typedef void (SocketFunc)(SOCKET, sockaddr_in*);

struct host_ip
{
	u_long ip;
	host_ip(u_long _ipv4)
	{
		ip = _ipv4;
	}
	string toString()
	{
		string res;
		res += to_string((ip >> 24) % 256) + ".";
		res += to_string((ip >> 16) % 256) + ".";
		res += to_string((ip >> 8) % 256) + ".";
		res += to_string(ip % 256);
		return res;
	}
};


string naddr2str(const sockaddr* addr);

class MySocket
{
public:
	
	/// @brief 
	/// @param version IP版本号
	/// @param type 使用的Socket类型 TCP/UDP/RAW
	/// @param protocol 协议类型 TCP/UDP/ICMP/IGMP
	MySocket(int _version = AF_INET,int _type = SOCK_STREAM,int _protocol = IPPROTO_TCP);
	MySocket(SOCKET _socket);
	MySocket(SOCKET _socket,sockaddr_in addr);
	MySocket(const MySocket& other);
	bool bind(string ip, u_short port);
	bool bind(sockaddr_in addr);
	MySocket listen();
	void doSomething(sockaddr_in* addr = nullptr);
	void setFunc(SocketFunc* _func);
	SOCKET getSocket();
	~MySocket();
	virtual pair<bool,string> receive();
	virtual pair<bool, string> receive(sockaddr_in serveraddr);
	virtual bool send(string msg);
	bool isBinded();
	bool connect(string ip,u_short port,int family = AF_INET);
	bool connect(sockaddr_in* addr);
	
protected:
	SOCKET socket;
	int version, type, protocol;
	bool binded;
	sockaddr_in _addr;
	SocketFunc* func;
};

