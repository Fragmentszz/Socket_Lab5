#include "Log.h"

Log::Log(string filename)
{
	fout.open(filename, ios::app);
}

Log::~Log()
{
	fout.close();
}

Log& Log::operator<<(const string& str)
{
	fout << str;
	return *this;
	// TODO: 在此处插入 return 语句
}
Log& Log::operator<<(const char* str)
{
	fout << str;
	return *this;
	// TODO: 在此处插入 return 语句
}

Log& Log::operator<<(char ch)
{
	fout << ch;
	return *this;
}
Log& Log::operator<<(int num)
{
	fout << num;
	return *this;
	// TODO: 在此处插入 return 语句
}