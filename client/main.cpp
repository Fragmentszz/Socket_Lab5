#include "Socket5_client.h"
#include <QtWidgets/QApplication>
#include<WinSock2.h>

int main(int argc, char *argv[])
{

    WSAData data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
    {
        cout << "WSAStartup failed" << endl;
        return -1;
    }
    QApplication a(argc, argv);
    Socket5_client w;
    w.show();

    return a.exec();
}
