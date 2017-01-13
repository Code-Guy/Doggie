#ifndef SCALER_H
#define SCALER_H

#include "selector.h"

class Scaler
{
public:
	Scaler(Selector *selector);
	~Scaler();

	void mousePressEvent(QMouseEvent *evt, glm::ivec2 aabbCenterWinPos, glm::ivec2 minAnchorWinPos);
	void mouseMoveEvent(QMouseEvent *evt);
	void mouseReleaseEvent(QMouseEvent *evt);
	void keyPressEvent(QKeyEvent *evt);
	void keyReleaseEvent(QKeyEvent *evt);

private:
	Selector *selector; // ѡ����

	glm::ivec2 aabbCenterWinPos; // ��Χ�򴰿����ĵ�
	glm::ivec2 minAnchorWinPos; // �����ê�㴰��λ��

	glm::vec3 ratio; // ���ű���
	bool constrainScale; // �Ƿ�ȱ�������

	std::map<ISObj *, ISObj *> lastSnapshots; // �ϴο���
	std::map<ISObj *, ISObj *> curSnapshots; // ��ǰ����
};

#endif // SCALER_H