#ifndef PARAMWIDGET_H
#define PARAMWIDGET_H

#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>

#include <glm/glm.hpp>

#include "colorlabel.h"
#include "filewidget.h"
#include "doublespinboxlist.h"
#include "doublespinboxvec2.h"
#include "doublespinboxvec3.h"
#include "doublespinboxvec4.h"
#include "spinboxivec2.h"

#include "lvluoapi.h"

class LVLUO_API ParamWidget : public QWidget
{
	Q_OBJECT

public:
	ParamWidget(QString id, QWidget *parent = Q_NULLPTR);
	~ParamWidget();

	// 添加各种类型的属性
	void addBool(QString id,
		QCheckBox *&checkBox); // bool

	void addFloat(QString id,
		float minVal, float maxVal, float singleStepVal,
		QDoubleSpinBox *&doubleSpinBox); // float

	void addFloats(QString id,
		float defaultVal,
		float minVal, float maxVal, float singleStepVal,
		DoubleSpinBoxList *&doubleSpinBoxList); // float vector

	void addInt(QString id,
		int minVal, int maxVal, int singleStepVal,
		QSpinBox *&spinBox); // int

	void addVec2(QString id, 
		float minVal, float maxVal, float singleStepVal,
		DoubleSpinBoxVec2 *&doubleSpinBoxVec2); // vec2

	void addIVec2(QString id,
		int minVal, int maxVal, int singleStepVal,
		SpinBoxIVec2 *&spinBoxIVec2); // ivec2

	void addVec3(QString id, 
		float minVal, float maxVal, float singleStepVal,
		DoubleSpinBoxVec3 *&doubleSpinBoxVec3); // vec3

	void addVec4(QString id,
		float minVal, float maxVal, float singleStepVal,
		DoubleSpinBoxVec4 *&doubleSpinBoxVec4); // vec4

	void addString(QString id,
		QLineEdit *&lineEdit); // string

	void addText(QString id,
		QTextEdit *&textEdit); // text

	void addColor(QString id,
		ColorLabel *&colorLabel); // color

	void addEnum(QString id, 
		QStringList enums,
		QComboBox *&comboxBox); // enum

	void addFile(QString id,
		QString filter,
		FileWidget *&fileWidget); // file browser

	void addSeperateLine(); // seperate line

protected:
	virtual void keyPressEvent(QKeyEvent *evt);

private:
	QVBoxLayout *mainLayout;
};

#endif // PARAMWIDGET_H