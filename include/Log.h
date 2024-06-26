#pragma once
#include<fstream>
#include<string>
using namespace std;
class Log
{
public:
	Log(string filename="CON");
	~Log();
	Log& operator<<(const string& str);
	Log& operator<<(const char* str);
	Log& operator<<(int num);
	Log& operator<<(char ch);

protected:
	ofstream fout;
};