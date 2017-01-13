#include "doublespinboxvec4.h"
#include <QHBoxLayout>

DoubleSpinBoxVec4::DoubleSpinBoxVec4(float minVal, float maxVal, float singleStepVal,
	QWidget *parent) : QWidget(parent)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(8);
	hLayout->setContentsMargins(0, 0, 0, 0);
	hLayout->setAlignment(Qt::AlignLeft);

	for (int i = 0; i < 4; i++)
	{
		doubleSpinBox[i] = new QDoubleSpinBox;
		doubleSpinBox[i]->setMinimum(minVal);
		doubleSpinBox[i]->setMaximum(maxVal);
		doubleSpinBox[i]->setSingleStep(singleStepVal);
		connect(doubleSpinBox[i], SIGNAL(valueChanged(double)), this, SLOT(vec4ValueChanged()));
		hLayout->addWidget(doubleSpinBox[i]);
	}

	setLayout(hLayout);
}

DoubleSpinBoxVec4::~DoubleSpinBoxVec4()
{
	for (int i = 0; i < 4; i++)
	{
		delete doubleSpinBox[i];
	}
}

void DoubleSpinBoxVec4::setVec4(glm::vec4 v)
{
	for (int i = 0; i < 4; i++)
	{
		doubleSpinBox[i]->setValue(v[i]);
	}
}

void DoubleSpinBoxVec4::vec4ValueChanged()
{
	glm::vec4 v;
	for (int i = 0; i < 4; i++)
	{
		v[i] = doubleSpinBox[i]->value();
	}

	emit vec4ValueChanged(v);
}