#include "doublespinboxlist.h"
#include <QHBoxLayout>
#include <QToolButton>

DoubleSpinBoxList::DoubleSpinBoxList(float defaultVal, float minVal, float maxVal, float singleStepVal,
	QWidget *parent) : defaultVal(defaultVal), minVal(minVal), maxVal(maxVal), singleStepVal(singleStepVal),
	QWidget(parent)
{
	QHBoxLayout *hLayout = new QHBoxLayout;
	QToolButton *addBtn = new QToolButton;
	QToolButton *removeBtn = new QToolButton;
	addBtn->setIcon(QIcon("Resources/Icons/add.png"));
	addBtn->setAutoRaise(true);
	removeBtn->setIcon(QIcon("Resources/Icons/remove.png"));
	removeBtn->setAutoRaise(true);

	connect(addBtn, SIGNAL(clicked()), this, SLOT(addBtnClicked()));
	connect(removeBtn, SIGNAL(clicked()), this, SLOT(removeBtnClicked()));

	hLayout->addStretch();
	hLayout->addWidget(addBtn);
	hLayout->addStretch();
	hLayout->addWidget(removeBtn);
	hLayout->addStretch();

	vLayout = new QVBoxLayout;
	vLayout->setSpacing(8);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addLayout(vLayout);
	mainLayout->addStretch();
	mainLayout->addLayout(hLayout);

	setLayout(mainLayout);
}

DoubleSpinBoxList::~DoubleSpinBoxList()
{
	delete vLayout;
}

void DoubleSpinBoxList::setListVals(std::vector<float> vals)
{
	for (float val : vals)
	{
		QDoubleSpinBox *doubleSpinBox = new QDoubleSpinBox;
		doubleSpinBox->setMinimum(minVal);
		doubleSpinBox->setMaximum(maxVal);
		doubleSpinBox->setSingleStep(singleStepVal);
		doubleSpinBox->setValue(val);

		connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(listValueChanged()));

		vLayout->addWidget(doubleSpinBox);
	}

	listValueChanged();
}

void DoubleSpinBoxList::listValueChanged()
{
	std::vector<float> vals;
	for (int i=0; i<vLayout->count(); i++)
	{
		QDoubleSpinBox *doubleSpinBox = qobject_cast<QDoubleSpinBox *>(vLayout->itemAt(i)->widget());
		vals.push_back(doubleSpinBox->value());
	}

	emit listValueChanged(vals);
}

void DoubleSpinBoxList::addBtnClicked()
{
	QDoubleSpinBox *doubleSpinBox = new QDoubleSpinBox;
	doubleSpinBox->setMinimum(minVal);
	doubleSpinBox->setMaximum(maxVal);
	doubleSpinBox->setSingleStep(singleStepVal);
	doubleSpinBox->setValue(defaultVal);

	connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(listValueChanged()));
	vLayout->addWidget(doubleSpinBox);

	listValueChanged();
}

void DoubleSpinBoxList::removeBtnClicked()
{
	if (vLayout->count() != 1)
	{
		QDoubleSpinBox *lastDoubleSpinBox = qobject_cast<QDoubleSpinBox *>(vLayout->itemAt(vLayout->count() - 1)->widget());
		vLayout->removeWidget(lastDoubleSpinBox);
		delete lastDoubleSpinBox;

		listValueChanged();
	}
}