#pragma once
#include<QGraphicsItem>
#include<QPainter>
#include<vector>

class MyShape
    : public QGraphicsItem
{
public:
    MyShape(QObject* parent = nullptr,qreal _penwidth = 8,int _width = 0, int _height = 0,QColor _color1 = QColor(0,0,0),QColor _color2 = QColor(0,0,0));
    virtual void changeColor_fill(QColor color);
    virtual void changeColor_contour(QColor color);
    QRectF boundingRect() const override;
    void setPenWidth(qreal _penWidth);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override = 0;
    QColor getColor_fill()const;
    void setWidth(int _width);
    int getWidth() const;
    void setHeight(int _height);
    int getHeight() const;
    int cato;
protected:
    int width;
    int height;
    qreal penWidth;
    QColor color1,color2;

    

};


class MyRect
    : public MyShape
{
public:
    MyRect(QObject* parent = nullptr, int _width = 0, int _height = 0, qreal _penwidth = 8, QColor _color1 = QColor(0, 0, 0), QColor _color2 = QColor(0, 0, 0));
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};

class MyEllipse
    : public MyShape
{
public:
    MyEllipse(QObject* parent = nullptr, int _width = 0, int _height = 0, qreal _penwidth = 8, QColor _color1 = QColor(0, 0, 0), QColor _color2 = QColor(0, 0, 0));
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};

class MyCurve
    :public MyShape
{
public:
    MyCurve(QObject* parent = nullptr, int _width = 0, int _height = 0, qreal _penwidth = 8, QColor _color1 = QColor(0, 0, 0), QColor _color2 = QColor(0, 0, 0));
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QPoint addPoint(QPoint point);
    void clearPoints();
    ~MyCurve();
private:
    std::vector<QPoint> points;
};