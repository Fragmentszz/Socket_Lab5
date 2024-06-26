#pragma once
#include<ctime>
#include<iostream>
#include<string>
#include<QString>
const int days[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

struct MyInterval
{
    int hour, minute, second;
    MyInterval(int _hour = 0, int _minute = 0, int _second = 0);
};
struct TIME
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    TIME(int _year, int _month = 0, int _day = 0, int _hour = 0, int _minute = 0, int _second = 0);

    TIME();
    TIME& operator+(const MyInterval& b);
    void print() const;
};

QString num2QString(int num, int wei, std::string head = "<font color=\"red\">", std::string end = "</font>");



