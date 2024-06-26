#pragma once
#include"_basic.h"


QString num2QString(int num, int wei, std::string head, std::string end)
{
    std::string res = "";
    std::string tmp = "";
    int x = num;
    for (int i = 0; i < wei || x > 0; i++, x /= 10)
    {
        tmp += x % 10 + '0';
    }
    for (int i = tmp.size() - 1; i >= 0; i--)
    {
        res += tmp[i];
    }
    return QString::fromStdString(head + res + end);
}

TIME::TIME(int _year, int _month, int _day, int _hour, int _minute, int _second)
{
    year = _year;
    month = _month;
    day = _day;
    hour = _hour;
    minute = _minute;
    second = _second;
}

TIME::TIME()
{
    time_t timep;
    struct tm* p;
    time(&timep);
    p = gmtime(&timep);
    year = p->tm_year + 1900;
    month = p->tm_mon + 1;
    day = p->tm_mday;
    hour = p->tm_hour;
    minute = p->tm_min;
    second = p->tm_sec;
}

TIME& TIME::operator+(const MyInterval& b)
{
    second += b.second;
    if (second >= 60)
    {
        second -= 60;
        minute++;
    }
    minute += b.minute;
    if (minute >= 60)
    {
        minute -= 60;
        hour++;
    }
    hour += b.hour;
    if (hour >= 24)
    {
        hour -= 24;
        day++;
    }
    if (day > days[month])
    {
        if (month == 2 && year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
        {
            if (day > 29)
            {
                day -= 29;
                month = 3;
            }
        }
        else {
            day -= days[month];
            month++;
        }
    }
    if (month >= 13)
    {
        year++;
        month = 1;
    }
    return *this;
}

void TIME::print() const
{
    std::cout << year << " " << month << " " << day << " " << hour << " " << minute << " " << second << std::endl;
}

MyInterval::MyInterval(int _hour, int _minute, int _second)
{
    hour = _hour;
    minute = _minute;
    second = _second;
}
