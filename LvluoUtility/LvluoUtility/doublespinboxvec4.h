#ifndef SPINBOXLIST4_H
#define SPINBOXLIST4_H

#include <QDoubleSpinBox>
#include <glm/glm.hpp>

#include "lvluoapi.h"

class LVLUO_API DoubleSpinBoxVec4 : public QWidget
{
	Q_OBJECT
public:
	DoubleSpinBoxVec4(float minVal, float maxVal, float singleStepVal,
		QWidget *parent = Q_NULLPTR);
	~DoubleSpinBoxVec4();

	void setVec4(glm::vec4 v);

signals:
	void vec4ValueChanged(glm::vec4 val);

private:
	QDoubleSpinBox *doubleSpinBox[4];

private slots:
	void vec4ValueChanged();
};

#endif // SPINBOXLIST2_H
