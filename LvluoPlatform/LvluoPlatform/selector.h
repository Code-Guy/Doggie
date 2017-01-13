#ifndef SELECTOR_H
#define SELECTOR_H

#include <vector>
#include <map>
#include <QMouseEvent>
#include <QKeyEvent>

#include "grouper.h"
#include "pickfbo.h"

class Selector
{
public:
	Selector(std::vector<ISObj *> *sObjs);
	~Selector();

	void logic();
	void resize(int sw, int sh);

	std::vector<ISObj *> *getSObjs();
	const std::vector<ISObj *> &getSelectedSObjs();

	void addSelectedSObj(ISObj *selectedSObj);
	void addSelectedSObjs(const std::vector<ISObj *> &selectedSObjs);
	bool containSObj(ISObj *sObj);
	bool containSObjs(const std::vector<ISObj *> &sObjs);

	void clearSelectedSObjs();

	std::map<ISObj *, ISObj *> getSelectedSObjSnapshots();

	std::vector<ISObj *> getPickedSObjWithPixel(QPoint mousePos);
	ISObj *getPickedSObjWithAABB(QPoint mousePos);

	bool isHoverSelectedSObj(QPoint mousePos);

	bool isMultiSelect();
	bool isSingleSelect();
	bool isNullSelect();

	void mousePressEvent(QMouseEvent *evt);
	void mouseReleaseEvent(QMouseEvent *evt);

	void group();
	void ungroup();

	Grouper *getGrouper();

private:
	void pick(); // ���ؼ�ʰȡ
	std::vector<ISObj *> getPickedSObjs(QRect rect);

	PickFBO *pickFBO; // ʰȡ֡����
	Grouper *grouper; // �����

	std::vector<ISObj *> *sObjs; // Stimulus Objects
	std::vector<ISObj *> selectedSObjs; // ��ѡ�е����壬֧�ֶ�ѡ
	QPoint lastMousePressPos; // �����һ�ΰ��µ�λ��
	glm::ivec2 sceneSize;
};

struct FarFirstComparer
{
	bool operator() (const ISObj *lhs, const ISObj *rhs)
	{
		return lhs->getPosition().z > rhs->getPosition().z;
	}
};

#endif // SELECTOR_H
