#ifndef ROTATER_H
#define ROTATER_H

#include "selector.h"

class Rotator
{
public:
	Rotator(Selector *selector);
	~Rotator();

	void mousePressEvent(QMouseEvent *evt, glm::ivec2 aabbCenterWinPos);
	void mouseMoveEvent(QMouseEvent *evt);
	void mouseReleaseEvent(QMouseEvent *evt);

private:
	Selector *selector;

	glm::ivec2 aabbCenterWinPos; // ��Χ�򴰿����ĵ�
	QPoint lastMousePos; // �����һ�ε�λ��
	const float MouseRotateSensitivity = 0.3f; // �����ת�����������

	std::map<ISObj *, ISObj *> lastSnapshots; // �ϴο���
	std::map<ISObj *, ISObj *> curSnapshots; // ��ǰ����
};

#endif // ROTATER_H
