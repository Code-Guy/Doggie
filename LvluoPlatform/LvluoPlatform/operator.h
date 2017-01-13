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

	void updateCursorShape(QPoint mousePos); // ���������ʽ
	void updateSelectElements(); // ����ѡ��Ԫ��
	void updateMultiSelectElements(); // ���¶�ѡԪ�أ�AABB+Anchor��
	void updateSelectFrameTransformation(); // ����ѡ����ģ�ͱ任

	void findNearMouseAnchorDis(QPoint mousePos);

	void checkMove(QPoint mousePos);
	void checkRotate(QPoint mousePos);
	void checkScale();

	void switchRotate();
	void switchScale();

	CustomCursorShape cursorShape; // �����ʽ
	OperationMode operationMode; // ����ģʽ

	Camera *camera; // �����
	Selector *selector; // ѡ����
	Mover *mover; // ƽ����
	Scaler *scaler; // ������
	Rotator *rotator; // ��ת��
	Paster *paster; // ������

	ISObj *dragSObj; // ���϶���sobj
	QPoint dragSObjPos; // �϶�sobj�����λ��

	QPoint lastMousePressPos; // ��һ����갴�µ�λ��
	QPoint curMousePressPos; // ��ǰ��갴�µ�λ��
	QPoint curMousePos; // ��ǰ���λ��

	bool isMousePressed; // ����Ƿ���
	bool isMouseMoved; // ����Ƿ��ƶ�

	// ����Ԫ���п���Ϊ��ѡ��Ҳ�п����Ƕ�ѡ
	glm::vec3 anchorPos[8]; // ê���λ��
	glm::vec3 middleAnchorPos[12]; // �е�ê���λ��
	glm::vec3 aabbCenter; // ��Χ������ĵ�
	glm::vec3 aabbSize; //��Χ��Ĵ�С

	glm::ivec2 aabbCenterWinPos; // ��Χ�򴰿����ĵ�
	glm::ivec2 minAnchorWinPos; // �����ê�㴰��λ��
	int minMouseAnchorDis; // ����������ê�㴰��λ�õ������پ���
	glm::ivec2 centerToNearestAnchorDir; // ��Χ�򴰿����ĵ㵽�����ê�㴰��λ�õķ�������

	// ѡ����ģ�ͱ任
	glm::vec3 frameTranslation;
	glm::vec3 frameScaling;

	glm::ivec2 sceneSize; // ������С
};

#endif // Operator_H
