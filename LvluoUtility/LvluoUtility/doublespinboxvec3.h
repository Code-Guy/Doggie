#ifndef SPINBOXLIST3_H
#define SPINBOXLIST3_H

#include <QDoubleSpinBox>
#include <glm/glm.hpp>

#include "lvluoapi.h"

class LVLUO_API DoubleSpinBoxVec3 : public QWidget
{
	Q_OBJECT
public:
	DoubleSpinBoxVec3(float minVal, float maxVal, float singleStepVal,
		QWidget *parent = Q_NULLPTR);
	~DoubleSpinBoxVec3();

	void setVec3(glm::vec3 v);

signals:
	void vec3ValueChanged(glm::vec3 val);

private:
	QDoubleSpinBox *doubleSpinBox[3];

private slots:
	void vec3ValueChanged();
};

#endif // SPINBOXLIST3_H
