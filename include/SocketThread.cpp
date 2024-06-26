#include<SocketThread.h>

SocketThread::SocketThread(void* _params, ThreadProc _proc)
	:params(_params),threadProc(_proc)
{
	// cout << _params << endl;
	hThread = NULL;

}

void SocketThread::run()
{
	//cout << ((Params_client*)params)->client << endl;
	//cout << ((Params_client*)params)->widget << endl;
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProc, params, 0, NULL);
}