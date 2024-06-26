#pragma once
#include <QtWidgets/QMainWindow>
#include<qdialog.h>
#include"ui_appendDialog.h"
#include"Shapes.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; };
QT_END_NAMESPACE


class appendDialog 
	: public QDialog
{
	Q_OBJECT
public :
	appendDialog(QWidget* parent = nullptr);
	MyShape* getShape() const ;
	int getKinds() const;
private:
	Ui::Dialog* ui;
	int R = -1, G = -1, B = -1;
	int kinds = -1;
	int width = -1, height = -1;
};