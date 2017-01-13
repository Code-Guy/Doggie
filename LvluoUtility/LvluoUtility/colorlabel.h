#ifndef COLORLABEL_H
#define COLORLABEL_H

#include <QLabel>
#include <glm/glm.hpp>

#include "lvluoapi.h"

class LVLUO_API ColorLabel : public QLabel
{
	Q_OBJECT

public:
	ColorLabel(QWidget *parent = Q_NULLPTR);
	~ColorLabel();

	void setColor(glm::vec3 color);

signals:
	void colorChanged(glm::vec3 color);

protected:
	virtual void mousePressEvent(QMouseEvent *evt);
};

#endif // COLORLABEL_H
