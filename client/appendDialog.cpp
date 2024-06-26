#include "appendDialog.h"

appendDialog::appendDialog(QWidget* parent)
	:QDialog(parent),
	ui(new Ui::Dialog())
{
	ui->setupUi(this);

	connect(ui->ok, &QPushButton::clicked, this, [this]() {
		kinds = ui->comboBox->currentIndex();
		R = ui->R->value();
		G = ui->G->value();
		B = ui->B->value();
		width = ui->width->value();
		height = ui->height->value();
		this->accept();
	});
}

MyShape* appendDialog::getShape() const
{
	if (kinds == 0)
	{
		return new MyRect(nullptr,width, height,8,QColor(R,G,B));
	}
	else
	{
		return new MyEllipse(nullptr, width, height, 8, QColor(R, G, B));
	}
	return nullptr;
}

int appendDialog::getKinds() const
{
	return kinds;
}
