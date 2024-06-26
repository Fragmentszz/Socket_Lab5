#include "MyGraphicsView.h"

MyGraphicsView::MyGraphicsView(QWidget* parent, QGraphicsScene* _scene, MyCurve* _curve)
	:QGraphicsView(parent)
{
	drawing = false;
	enableDrawing = false;
	if(_scene)
		scene = _scene;
	else
	{
		scene = new QGraphicsScene();
		setScene(scene);
	}
}

void MyGraphicsView::setDrawing(bool flag)
{
	enableDrawing = flag;
}

void MyGraphicsView::setDrawed(MyCurve* _curve)
{
	drawed = _curve;
}

bool MyGraphicsView::getDrawing() const
{
	return enableDrawing;
}

void MyGraphicsView::clearDrawed()
{
	for(auto shape : curves)
	{
		if (shape)
		{
			scene->removeItem(shape);
			delete shape;
		}
	}
	curves.clear();
	nowindex = -1;
	scene->update();
}

void MyGraphicsView::setBGColor(QColor _color)
{
	BGcolor = _color;
}

void MyGraphicsView::setNowIndex(int index)
{
	nowindex = index;
}

int MyGraphicsView::createCurve(QColor color)
{
	MyCurve* drawed = new MyCurve(nullptr, 100, 100, 4.0, color, color);
	scene->addItem(drawed);
	drawed->setPos(-this->width() / 2, -this->height() / 2);
	curves.push_back(drawed);
	return nowindex;
}
void MyGraphicsView::addPoints(int index, std::vector<QPoint> _points)
{
	if (index >= 0 && index < curves.size())
	{
		for(auto point : _points)
		{
			curves[index]->addPoint(point);
		}
		scene->update();
	}

}


void MyGraphicsView::mousePressEvent(QMouseEvent* ev)
{
	if (ev->button() == Qt::LeftButton && enableDrawing)
	{
		drawed = new MyCurve(nullptr, 100, 100, 4.0, BGcolor,BGcolor);
		nowindex = curves.size();
		curves.push_back(drawed);
		scene->addItem(drawed);
		drawed->setPos(-this->width() / 2, -this->height() / 2);
		drawing = 1;
		createCuve(BGcolor);
	}
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent* ev)
{
	if (ev->button() == Qt::LeftButton)
	{
		drawing = 0;
		drawed = nullptr;
		if (points.size() > 0)
		{
			transPoint(nowindex, points);
			points.clear();
		}
		nowindex = -1;
	}
}



void MyGraphicsView::transPoint(int index, std::vector<QPoint> points)
{
	((Socket5_client*)this->parent())->transPoint_slot(index, points);
}

void MyGraphicsView::createCuve(QColor color)
{
	((Socket5_client*)this->parent())->createCurve_slot(color);
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent* ev)
{
	if (drawing && enableDrawing && drawed)
	{
		QPoint d = drawed->addPoint(ev->pos());
		points.push_back(ev->pos());
		if (points.size() >= 10)
		{
			cout << "nowindex:" << nowindex << endl;
			transPoint(nowindex, points);
			points.clear();
		}
		scene->update();
	}
}
