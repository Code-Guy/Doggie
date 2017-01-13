#ifndef SPINBOXLIST2_H
#define SPINBOXLIST2_H

#include <QDoubleSpinBox>
#include <glm/glm.hpp>

#include "lvluoapi.h"

class LVLUO_API DoubleSpinBoxVec2 : public QWidget
{
	Q_OBJECT
public:
	DoubleSpinBoxVec2(float minVal, float maxVal, float singleStepVal,
		QWidget *parent = Q_NULLPTR);
	~DoubleSpinBoxVec2();

	void setVec2(glm::vec2 v);

signals:
	void vec2ValueChanged(glm::vec2 val);

private:
	QDoubleSpinBox *doubleSpinBox[2];

	private slots:
	void vec2ValueChanged();
};

#endif // SPINBOXLIST2_H
