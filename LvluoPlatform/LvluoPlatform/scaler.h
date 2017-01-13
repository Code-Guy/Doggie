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
	Selector *selector; // 选择器

	glm::ivec2 aabbCenterWinPos; // 包围框窗口中心点
	glm::ivec2 minAnchorWinPos; // 最近的锚点窗口位置

	glm::vec3 ratio; // 缩放比例
	bool constrainScale; // 是否等比例缩放

	std::map<ISObj *, ISObj *> lastSnapshots; // 上次快照
	std::map<ISObj *, ISObj *> curSnapshots; // 当前快照
};

#endif // SCALER_H