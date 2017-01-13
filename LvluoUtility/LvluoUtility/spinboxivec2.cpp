#include "spinboxivec2.h"
#include <QHBoxLayout>

SpinBoxIVec2::SpinBoxIVec2(int minVal, int maxVal, int singleStepVal,
	QWidget *parent) : QWidget(parent)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->setSpacing(8);
	hLayout->setContentsMargins(0, 0, 0, 0);
	hLayout->setAlignment(Qt::AlignLeft);

	for (int i = 0; i < 2; i++)
	{
		spinBox[i] = new QSpinBox;
		spinBox[i]->setMinimum(minVal);
		spinBox[i]->setMaximum(maxVal);
		spinBox[i]->setSingleStep(singleStepVal);
		connect(spinBox[i], SIGNAL(valueChanged(int)), this, SLOT(ivec2ValueChanged()));
		hLayout->addWidget(spinBox[i]);
	}

	setLayout(hLayout);
}

SpinBoxIVec2::~SpinBoxIVec2()
{
	for (int i = 0; i < 2; i++)
	{
		delete spinBox[i];
	}
}

void SpinBoxIVec2::setIVec2(glm::ivec2 v)
{
	for (int i = 0; i < 2; i++)
	{
		spinBox[i]->setValue(v[i]);
	}
}

void SpinBoxIVec2::ivec2ValueChanged()
{
	glm::vec2 v;
	for (int i = 0; i < 2; i++)
	{
		v[i] = spinBox[i]->value();
	}

	emit ivec2ValueChanged(v);
}