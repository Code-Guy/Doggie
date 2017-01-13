#ifndef OPERATOR_H
#define OPERATOR_H

#include <vector>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>

#include "sobj.h"
#include "camera.h"
#include "selector.h"
#include "mover.h"
#include "scaler.h"
#include "rotator.h"
#include "paster.h"

class Operator : public QObject
{
	Q_OBJECT

public:
	enum CustomCursorShape
	{
		HoverShape, DragShape,
		RotateTopLeftShape, RotateTopRightShape, RotateBottomLeftShape, RotateBottomRightShape, RotateFreeShape,
		ScaleBDiagShape, ScaleFDiagShape,
		MoveCameraShape,
		NoneShape
	};

	Operator(Camera *camera, std::vector<ISObj *> *sObjs);
	~Operator();

	void logic(QPoint mousePos);
	void render();

	void resize(int sw, int sh);

	void mousePressEvent(QMouseEvent *evt);
	void mouseMoveEvent(QMouseEvent *evt);
	void mouseReleaseEvent(QMouseEvent *evt);
	void mouseDoubleClickEvent(QMouseEvent *evt);
	void keyPressEvent(QKeyEvent *evt);
	void keyReleaseEvent(QKeyEvent *evt);
	void wheelEvent(QWheelEvent *evt);

	void addDragSObj(ISObj *dragSObj, QPoint mousePos);

	void copy();
	void cut();
	void paste();
	void remove();
	void selectAll();
	void group();
	void ungroup();

	Grouper *getGrouper();

signals:
	void updateCursorShape(Operator::CustomCursorShape cursorShape);

private:
	enum OperationMode
	{
		Auto, Move, Rotate, Scale
	};

	void updateCursorShape(QPoint mousePos); // 更新鼠标样式
	void updateSelectElements(); // 更新选择元素
	void updateMultiSelectElements(); // 更新多选元素（AABB+Anchor）
	void updateSelectFrameTransformation(); // 更新选择框的模型变换

	void findNearMouseAnchorDis(QPoint mousePos);

	void checkMove(QPoint mousePos);
	void checkRotate(QPoint mousePos);
	void checkScale();

	void switchRotate();
	void switchScale();

	CustomCursorShape cursorShape; // 鼠标样式
	OperationMode operationMode; // 操作模式

	Camera *camera; // 摄像机
	Selector *selector; // 选择器
	Mover *mover; // 平移器
	Scaler *scaler; // 缩放器
	Rotator *rotator; // 旋转器
	Paster *paster; // 剪贴板

	ISObj *dragSObj; // 被拖动的sobj
	QPoint dragSObjPos; // 拖动sobj的鼠标位置

	QPoint lastMousePressPos; // 上一次鼠标按下的位置
	QPoint curMousePressPos; // 当前鼠标按下的位置
	QPoint curMousePos; // 当前鼠标位置

	bool isMousePressed; // 鼠标是否按下
	bool isMouseMoved; // 鼠标是否移动

	// 以下元素有可能为单选，也有可能是多选
	glm::vec3 anchorPos[8]; // 锚点的位置
	glm::vec3 middleAnchorPos[12]; // 中点锚点的位置
	glm::vec3 aabbCenter; // 包围框的中心点
	glm::vec3 aabbSize; //包围框的大小

	glm::ivec2 aabbCenterWinPos; // 包围框窗口中心点
	glm::ivec2 minAnchorWinPos; // 最近的锚点窗口位置
	int minMouseAnchorDis; // 鼠标离最近的锚点窗口位置的曼哈顿距离
	glm::ivec2 centerToNearestAnchorDir; // 包围框窗口中心点到最近的锚点窗口位置的方向向量

	// 选择框的模型变换
	glm::vec3 frameTranslation;
	glm::vec3 frameScaling;

	glm::ivec2 sceneSize; // 场景大小
};

#endif // Operator_H
