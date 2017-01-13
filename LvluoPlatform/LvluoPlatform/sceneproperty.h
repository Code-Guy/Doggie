#ifndef FILEPROPERTY_H
#define FILEPROPERTY_H

#include "paramwidget.h"

class SceneProperty : public QObject
{
	Q_OBJECT

public:
	SceneProperty(QObject *parent = Q_NULLPTR);
	~SceneProperty();

	void show();

	bool getBlendOn();
	bool getStereoOn();
	float getFd();
	float getSd();

	static SceneProperty *getInstance();

private:
	bool blendOn;
	bool stereoOn;
	float fd;
	float sd;

	ParamWidget *paramWidget;

	QCheckBox *blendOnCheckBox;
	QCheckBox *stereoOnCheckBox;
	QDoubleSpinBox *fdDoubleSpinBox;
	QDoubleSpinBox *sdDoubleSpinBox;

	static SceneProperty *instance;

private slots:
	void blendOnCheckBoxChanged(int val);
	void stereoOnCheckBoxChanged(int val);
	void fdDoubleSpinBoxValueChanged(double val);
	void sdDoubleSpinBoxValueChanged(double val);
};

#endif // FILEPROPERTY_H