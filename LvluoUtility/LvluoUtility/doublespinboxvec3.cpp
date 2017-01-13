#include "doublespinboxvec3.h"
#include <QHBoxLayout>

DoubleSpinBoxVec3::DoubleSpinBoxVec3(float minVal, float maxVal, float singleStepVal,
	QWidget *parent) : QWidget(parent)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(8);
	hLayout->setContentsMargins(0, 0, 0, 0);
	hLayout->setAlignment(Qt::AlignLeft);

	for (int i=0; i<3; i++)
	{
		doubleSpinBox[i] = new QDoubleSpinBox;
		doubleSpinBox[i]->setMinimum(minVal);
		doubleSpinBox[i]->setMaximum(maxVal);
		doubleSpinBox[i]->setSingleStep(singleStepVal);
		connect(doubleSpinBox[i], SIGNAL(valueChanged(double)), this, SLOT(vec3ValueChanged()));
		hLayout->addWidget(doubleSpinBox[i]);
	}

	setLayout(hLayout);
}

DoubleSpinBoxVec3::~DoubleSpinBoxVec3()
{
	for (int i = 0; i < 3; i++)
	{
		delete doubleSpinBox[i];
	}
}

void DoubleSpinBoxVec3::setVec3(glm::vec3 v)
{
	for (int i = 0; i < 3; i++)
	{
		doubleSpinBox[i]->setValue(v[i]);
	}
}

void DoubleSpinBoxVec3::vec3ValueChanged()
{
	glm::vec3 v;
	for (int i = 0; i < 3; i++)
	{
		v[i] = doubleSpinBox[i]->value();
	}

	emit vec3ValueChanged(v);
}