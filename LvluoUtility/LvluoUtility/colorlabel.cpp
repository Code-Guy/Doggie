#include "colorlabel.h"
#include <QColorDialog>
#include <QVariant>

ColorLabel::ColorLabel(QWidget *parent) : QLabel(parent)
{
	setFixedWidth(80);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

ColorLabel::~ColorLabel()
{

}

void ColorLabel::setColor(glm::vec3 color)
{
	QColor qColor;
	qColor.setRedF(color.x);
	qColor.setGreenF(color.y);
	qColor.setBlueF(color.z);

	QVariant variant = qColor;
	QString colStr = variant.toString();
	setStyleSheet("QLabel { background-color :" + colStr + "; }");

	emit colorChanged(color);
}

void ColorLabel::mousePressEvent(QMouseEvent *evt)
{
	QColorDialog *dialog = new QColorDialog;
	QColor qColor = dialog->getColor();
	if (qColor.isValid())
	{
		glm::vec3 color(qColor.redF(), qColor.greenF(), qColor.blueF());

		setColor(color);
	}
}