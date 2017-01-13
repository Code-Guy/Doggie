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

	glm::ivec2 aabbCenterWinPos; // 包围框窗口中心点
	QPoint lastMousePos; // 鼠标上一次的位置
	const float MouseRotateSensitivity = 0.3f; // 鼠标旋转物体的灵敏度

	std::map<ISObj *, ISObj *> lastSnapshots; // 上次快照
	std::map<ISObj *, ISObj *> curSnapshots; // 当前快照
};

#endif // ROTATER_H
