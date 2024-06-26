#pragma once
#include<QgraphicsView>
#include<QMouseEvent>
#include<Shapes.h>
#include<vector>
#include"Socket5_client.h"


class Socket5_client;


class MyGraphicsView
	:public QGraphicsView
{
public:
	MyGraphicsView(QWidget* parent = nullptr, QGraphicsScene* scene = nullptr, MyCurve* _curve = nullptr);
	void setDrawing(bool flag);
	void setDrawed(MyCurve* _curve);
	bool getDrawing()const;
	void clearDrawed();
	void setBGColor(QColor _color);
	void setNowIndex(int index);
	int createCurve(QColor color);
	void addPoints(int index,std::vector<QPoint> _points);
protected:
	MyCurve* drawed;
	std::vector<MyCurve*> curves;
	bool drawing;
	int nowindex = -1;
	std::vector<QPoint> points;
	bool enableDrawing;
	QGraphicsScene* scene;
	QColor BGcolor;
private:
	std::vector<MyCurve*> drawedShapes;
	void transPoint(int index, std::vector<QPoint> points);
	void createCuve(QColor color);
private slots:
	void mouseMoveEvent(QMouseEvent* ev) override;

	void mousePressEvent(QMouseEvent* ev) override;

	void mouseReleaseEvent(QMouseEvent* ev) override;



	
};
