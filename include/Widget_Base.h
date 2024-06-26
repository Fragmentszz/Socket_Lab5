#pragma once

#include <QtWidgets/QMainWindow>

class Widget_Base : public QMainWindow
{
	Q_OBJECT
public:
	Widget_Base(QWidget* parent = nullptr);
	~Widget_Base();
	void transPoint(int index, std::vector<QPoint> points);
	void createCuve(QColor color);
};