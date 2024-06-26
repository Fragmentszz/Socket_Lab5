#include <MySocket.h>


MySocket::MySocket(int _version, int _type, int _protocol)
	:version(_version),type(_type),protocol(_protocol)
{
	socket = ::socket(version, type, protocol);
	if (socket == INVALID_SOCKET)
	{
		Log() << "socket failed" << "\n";
	}
	binded = 0;
}

MySocket::MySocket(SOCKET _socket)
	:socket(_socket)
{
	version = AF_INET;
	type = SOCK_STREAM;
	protocol = IPPROTO_TCP;
	binded = 1;
}
MySocket::MySocket(const MySocket& other)
{
	version = AF_INET;
	type = SOCK_STREAM;
	protocol = IPPROTO_TCP;
	socket = other.socket;
	binded = other.binded;
	_addr = other._addr;
	func = other.func;
}
MySocket::MySocket(SOCKET _socket, sockaddr_in addr)
	:socket(_socket),_addr(addr)
{
	version = AF_INET;
	type = SOCK_STREAM;
	protocol = IPPROTO_TCP;
	binded = 1;
}

MySocket::~MySocket()
{
	if(socket != INVALID_SOCKET)
	{
		closesocket(socket);
	}
}
bool MySocket::bind(string ip, u_short port)
{
	sockaddr_in addr;
	addr.sin_family = version;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	if (::bind(socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		Log() << "bind failed" << "\n";
		return false;
	}
	binded = 1;
	return true;
}

bool MySocket::bind(sockaddr_in addr)
{

	if (::bind(socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		Log() << "bind failed" << "\n";
		return false;
	}
	binded = 1;
	_addr = addr;
	return true;
}

MySocket MySocket::listen()
{
	if (!binded)
	{
		Log() << "listen: socket not binded" << "\n";
		return MySocket();
	}
	if(socket == INVALID_SOCKET)
	{
		Log() << "socket invalid" << "\n";
		return MySocket();
	}
	int errCode = 0;
	errCode = ::listen(socket, SOMAXCONN);
	if(errCode == SOCKET_ERROR)
	{
		Log() << "listen failed" << "\n";
		printf("connect :%d\n", WSAGetLastError());// 打印错误编码
		return MySocket();
	}
	// 获取 连接客户端的ip和端口号
	sockaddr_in req_addr;
	int len = sizeof(req_addr);
	SOCKET client = ::accept(socket,(sockaddr*) & req_addr, &len);
	Log() << len << "\n";
	Log() << "client connected" << "\n";
	Log() << inet_ntoa(req_addr.sin_addr) << "\n";
	if(client == INVALID_SOCKET)
	{
		Log() << "accept failed" << "\n";
		return MySocket();
	}
	
	return MySocket(client,req_addr);
}

bool MySocket::connect(string ip, u_short port, int family)
{
	sockaddr_in addr;
	addr.sin_family = family;
	addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(port);
	return connect(&addr);

}


bool MySocket::connect(sockaddr_in* addr)
{
	int res = ::connect(socket, (sockaddr*)addr, sizeof(*addr));
	if (res == SOCKET_ERROR)
	{
		Log() << "connect failed" << "\n";
		printf("connect :%d\n", WSAGetLastError());// 打印错误编码
		binded = 0;
		return false;
	}
	binded = 1;
	return 1;
}

pair<bool,string> MySocket::receive()
{
	if (!binded)
	{
		Log() << "receive: socket not binded" << "\n";
		return { -1,"" };
	}
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	int errCode = recv(socket, buf, sizeof(buf), 0);
	if (errCode == 0)
	{
		Log() << "client disconnected" << "\n";
		return { -2,"client 关闭了连接.." };
	}
	if (errCode == SOCKET_ERROR)
	{
		Log() << "recv failed" << "\n";
		printf("connect :%d\n", WSAGetLastError());// 打印错误编码
		return { -1,"" };
	}
	return { 0,string(buf) };
}

pair<bool, string> MySocket::receive(sockaddr_in serveraddr)
{

	char buf[1024];
	memset(buf, 0, sizeof(buf));
	int lenfrom = sizeof(serveraddr);
	int errCode = recvfrom(socket, buf, sizeof(buf), 0, (sockaddr*)&serveraddr, &lenfrom);
	if (errCode == 0)
	{
		Log() << "client disconnected" << "\n";
		return { -2,"client 关闭了连接.." };
	}
	if (errCode == SOCKET_ERROR)
	{
		Log() << "recv failed" << "\n";
		printf("connect :%d\n", WSAGetLastError());// 打印错误编码
		return { -1,"" };
	}
	return { 0,string(buf) };
}

bool MySocket::send(string msg)
{
	if (!binded)
	{
		Log() << "send: socket not binded" << "\n";
		return false;
	}
	
	char* buf = new char[1024];
	memset(buf, 0, sizeof(buf));
	short len = msg.length();
	if (len == 0)
	{
		return true;
	}
	memcpy(buf, msg.c_str(), msg.length());
	int errCode = ::send(socket, buf, sizeof(char)*len, 0);
	if (errCode == SOCKET_ERROR)
	{
		Log() << "send failed" << "\n";
		printf("connect :%d\n", WSAGetLastError());// 打印错误编码
		return false;
	}
	delete [] buf;
	return true;
}

bool MySocket::isBinded()
{
	return binded;
}

void MySocket::doSomething(sockaddr_in* addr)
{
	if (addr == nullptr)
	{
		addr = &_addr;
	}
	func(socket, addr);
}

SOCKET MySocket::getSocket()
{
	return socket;
}

void MySocket::setFunc(SocketFunc* _func)
{
	func = _func;
}


string naddr2str(const sockaddr* addr)
{
	host_ip ip = host_ip(ntohl(inet_addr(addr->sa_data)));
	return ip.toString();
}
