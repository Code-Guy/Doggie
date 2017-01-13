#ifndef MOVER_H
#define MOVER_H

#include "selector.h"

class Mover
{
public:
	Mover(Camera *camera, Selector *selector);
	~Mover();

	void mousePressEvent(QMouseEvent *evt);
	void mouseMoveEvent(QMouseEvent *evt);
	void mouseReleaseEvent(QMouseEvent *evt);
	void keyPressEvent(QKeyEvent *evt);
	void keyReleaseEvent(QKeyEvent *evt);
	void wheelEvent(QWheelEvent *evt);

private:
	bool checkTinyMove();

	Camera *camera; // 摄像机
	Selector *selector; // 选择器

	QPoint lastMousePos; // 鼠标上一次的位置
	QPoint lastMousePressPos; // 鼠标上一次按下的位置

	std::map<ISObj *, ISObj *> lastSnapshots; // 上次快照
	std::map<ISObj *, ISObj *> curSnapshots; // 当前快照
};

#endif // MOVER_H