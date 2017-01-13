#include "sceneproperty.h"

SceneProperty *SceneProperty::instance = nullptr;

SceneProperty::SceneProperty(QObject *parent)
{
	blendOn = false;
	stereoOn = false;
	fd = 15.0f;
	sd = 0.01f;

	paramWidget = new ParamWidget("Properties");
	paramWidget->addBool("Blend On", blendOnCheckBox);
	paramWidget->addBool("Stereo On", stereoOnCheckBox);
	paramWidget->addFloat("Fusion Distance", 0, 50, 0.1, fdDoubleSpinBox);
	paramWidget->addFloat("Eye Distance", 0, 0.1, 0.001, sdDoubleSpinBox);

	blendOnCheckBox->setChecked(blendOn);
	stereoOnCheckBox->setChecked(stereoOn);
	fdDoubleSpinBox->setValue(fd);
	sdDoubleSpinBox->setValue(sd);

	connect(blendOnCheckBox, SIGNAL(stateChanged(int)), this, SLOT(blendOnCheckBoxChanged(int)));
	connect(stereoOnCheckBox, SIGNAL(stateChanged(int)), this, SLOT(stereoOnCheckBoxChanged(int)));
	connect(fdDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(fdDoubleSpinBoxValueChanged(double)));
	connect(sdDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(sdDoubleSpinBoxValueChanged(double)));
}

SceneProperty::~SceneProperty()
{
	delete blendOnCheckBox;
	delete stereoOnCheckBox;
	delete fdDoubleSpinBox;
	delete sdDoubleSpinBox;
	delete paramWidget;
}

void SceneProperty::show()
{
	paramWidget->show();
	paramWidget->activateWindow();
}

bool SceneProperty::getBlendOn()
{
	return blendOn;
}

bool SceneProperty::getStereoOn()
{
	return stereoOn;
}

float SceneProperty::getFd()
{
	return fd;
}

float SceneProperty::getSd()
{
	return sd;
}

SceneProperty *SceneProperty::getInstance()
{
	if (instance == nullptr)
	{
		instance = new SceneProperty;
	}
	return instance;
}

void SceneProperty::blendOnCheckBoxChanged(int val)
{
	blendOn = val;
}

void SceneProperty::stereoOnCheckBoxChanged(int val)
{
	stereoOn = val;
}

void SceneProperty::fdDoubleSpinBoxValueChanged(double val)
{
	fd = val;
}

void SceneProperty::sdDoubleSpinBoxValueChanged(double val)
{
	sd = val;
}