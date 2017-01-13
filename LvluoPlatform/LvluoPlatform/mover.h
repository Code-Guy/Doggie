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

	Camera *camera; // �����
	Selector *selector; // ѡ����

	QPoint lastMousePos; // �����һ�ε�λ��
	QPoint lastMousePressPos; // �����һ�ΰ��µ�λ��

	std::map<ISObj *, ISObj *> lastSnapshots; // �ϴο���
	std::map<ISObj *, ISObj *> curSnapshots; // ��ǰ����
};

#endif // MOVER_H