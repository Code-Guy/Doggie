#ifndef SPINBOXLISTIVEC2_H
#define SPINBOXLISTIVEC2_H

#include <QSpinBox>
#include <glm/glm.hpp>

#include "lvluoapi.h"

class LVLUO_API SpinBoxIVec2 : public QWidget
{
	Q_OBJECT
public:
	SpinBoxIVec2(int minVal, int maxVal, int singleStepVal,
		QWidget *parent = Q_NULLPTR);
	~SpinBoxIVec2();

	void setIVec2(glm::ivec2 v);

signals:
	void ivec2ValueChanged(glm::ivec2 val);

private:
	QSpinBox *spinBox[2];

private slots:
	void ivec2ValueChanged();
};

#endif // SPINBOXLISTIVEC2_H
