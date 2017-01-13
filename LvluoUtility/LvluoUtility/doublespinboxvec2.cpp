#include "doublespinboxvec2.h"
#include <QHBoxLayout>

DoubleSpinBoxVec2::DoubleSpinBoxVec2(float minVal, float maxVal, float singleStepVal,
	QWidget *parent) : QWidget(parent)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(8);
	hLayout->setContentsMargins(0, 0, 0, 0);
	hLayout->setAlignment(Qt::AlignLeft);

	for (int i = 0; i < 2; i++)
	{
		doubleSpinBox[i] = new QDoubleSpinBox;
		doubleSpinBox[i]->setMinimum(minVal);
		doubleSpinBox[i]->setMaximum(maxVal);
		doubleSpinBox[i]->setSingleStep(singleStepVal);
		connect(doubleSpinBox[i], SIGNAL(valueChanged(double)), this, SLOT(vec2ValueChanged()));
		hLayout->addWidget(doubleSpinBox[i]);
	}

	setLayout(hLayout);
}

DoubleSpinBoxVec2::~DoubleSpinBoxVec2()
{
	for (int i = 0; i < 2; i++)
	{
		delete doubleSpinBox[i];
	}
}

void DoubleSpinBoxVec2::setVec2(glm::vec2 v)
{
	for (int i = 0; i < 2; i++)
	{
		doubleSpinBox[i]->setValue(v[i]);
	}
}

void DoubleSpinBoxVec2::vec2ValueChanged()
{
	glm::vec2 v;
	for (int i = 0; i < 2; i++)
	{
		v[i] = doubleSpinBox[i]->value();
	}

	emit vec2ValueChanged(v);
}