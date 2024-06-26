#include<Shapes.h>


MyShape::MyShape(QObject* parent, qreal _penwidth, int _width, int _height, QColor _color1, QColor _color2)
{
	width = _width;
	height = _height;
	penWidth = _penwidth;
	color1 = _color1;
	color2 = _color2;
}

void MyShape::changeColor_fill(QColor _color)
{
	color1 = _color;
}
void MyShape::changeColor_contour(QColor _color)
{
	color2 = _color;
}
QRectF MyShape::boundingRect() const
{
	return QRectF(-(width + penWidth) / 2, - (height + penWidth) / 2, width + penWidth, height + penWidth);
}

void MyShape::setPenWidth(qreal _penWidth)
{
	 penWidth = _penWidth; 
}

QColor MyShape::getColor_fill() const
{
	return color1;
}

void MyShape::setWidth(int _width)
{
	width = _width;
}

int MyShape::getWidth() const
{
	return width;
}

void MyShape::setHeight(int _height)
{
	height = _height;
}

int MyShape::getHeight() const
{
	return height;
}


MyRect::MyRect(QObject* parent, int _width, int _height, qreal _penwidth, QColor _color1, QColor _color2)
	:MyShape(parent, _penwidth, _width, _height, _color1, _color2)
{
	cato = 0;
}

void MyRect::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QPen pen(color2);
	pen.setWidth(penWidth);
	painter->setPen(pen);
	QBrush brush(color1);
	painter->setBrush(brush);
	painter->drawRect(-width / 2, -height / 2, width, height);
}

MyEllipse::MyEllipse(QObject* parent, int _width, int _height, qreal _penwidth, QColor _color1, QColor _color2)
	:MyShape(parent, _penwidth, _width, _height, _color1, _color2)
{
	cato = 1;
}

void MyEllipse::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QPen pen(color2);
	pen.setWidth(penWidth);
	painter->setPen(pen);
	painter->setBrush(color1);

	painter->drawEllipse(-width / 2, -height/2, width, height);	
}

MyCurve::MyCurve(QObject* parent, int _width, int _height, qreal _penwidth, QColor _color1, QColor _color2)
	:MyShape(parent, _penwidth, _width, _height, _color1, _color2)
{
	cato = 2;
}

void MyCurve::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QPen pen(color1);
	pen.setWidth(penWidth);
	painter->setPen(pen);
	painter->drawPoints(points.data(), points.size());
}

QPoint MyCurve::addPoint(QPoint point)
{
	points.push_back(QPoint(point.x(),point.y()));
	return QPoint(point.x(), point.y());
}

void MyCurve::clearPoints()
{
	points.clear();
}

MyCurve::~MyCurve()
{
	points.clear();
}
