#ifndef DOUBLESPINBOXLIST_H
#define DOUBLESPINBOXLIST_H

#include <QDoubleSpinBox>
#include <QWidget>
#include <QVBoxLayout>
#include <vector>

#include "lvluoapi.h"

class LVLUO_API DoubleSpinBoxList : public QWidget
{
	Q_OBJECT

public:
	DoubleSpinBoxList(float defaultVal, float minVal, float maxVal, float singleStepVal,
		QWidget *parent = Q_NULLPTR);
	~DoubleSpinBoxList();

	void setListVals(std::vector<float> vals);

signals:
	void listValueChanged(std::vector<float> vals);

private:
	QVBoxLayout *vLayout;

	float defaultVal;
	float minVal;
	float maxVal;
	float singleStepVal;

private slots:
	void listValueChanged();
	void addBtnClicked();
	void removeBtnClicked();
};

#endif // DOUBLESPINBOXLIST_H
