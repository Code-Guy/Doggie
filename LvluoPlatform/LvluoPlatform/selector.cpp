#include "selector.h"
#include "sobjfactory.h"
#include "resourcemanager.h"
#include "history.h"
#include "groupcommand.h"
#include "ungroupcommand.h"
#include "tool.h"

#include <QRect>
#include <algorithm>

Selector::Selector(std::vector<ISObj *> *sObjs) : sObjs(sObjs)
{
	pickFBO = new PickFBO;
	pickFBO->bind();
	pickFBO->drawBuffer(GL_COLOR_ATTACHMENT0);
	pickFBO->readBuffer(GL_COLOR_ATTACHMENT0);
	pickFBO->unBind();

	grouper = new Grouper;
}

Selector::~Selector()
{
	delete pickFBO;
	delete grouper;
}

void Selector::logic()
{
	selectedSObjs.clear();
	for (ISObj *sObj : *sObjs)
	{
		if (sObj->isSelected())
		{
			selectedSObjs.push_back(sObj);
		}
		if (!grouper->containSObj(sObj))
		{
			grouper->addSObj(sObj);
		}
	}

	static std::vector<ISObj *> lastSeletedSObjs;
	if (selectedSObjs != lastSeletedSObjs)
	{
		glm::vec3 center = glm::vec3(0, 0, 0);
		for (ISObj *selectedSObj : selectedSObjs)
		{
			selectedSObj->updateTransform();
			center += selectedSObj->getPosition();
		}
		center /= selectedSObjs.size();
		for (ISObj *selectedSObj : selectedSObjs)
		{
			selectedSObj->setCenter(center);
		}
		lastSeletedSObjs = selectedSObjs;
	}
}

void Selector::resize(int sw, int sh)
{
	sceneSize = glm::ivec2(sw, sh);
}

const std::vector<ISObj *> &Selector::getSelectedSObjs()
{
	return selectedSObjs;
}

std::vector<ISObj *> *Selector::getSObjs()
{
	return sObjs;
}

void Selector::addSelectedSObj(ISObj *selectedSObj)
{
	selectedSObj->select();
}

void Selector::addSelectedSObjs(const std::vector<ISObj *> &selectedSObjs)
{
	for (ISObj *selectedSObj : selectedSObjs)
	{
		selectedSObj->select();
	}
}

bool Selector::containSObj(ISObj *sObj)
{
	return sObj->isSelected();
}

bool Selector::containSObjs(const std::vector<ISObj *> &sObjs)
{
	for (ISObj *sObj : sObjs)
	{
		if (!containSObj(sObj))
		{
			return false;
		}
	}
	return true;
}

void Selector::clearSelectedSObjs()
{
	for (ISObj *sObj : *sObjs)
	{
		sObj->unSelect();
	}
}

std::map<ISObj *, ISObj *> Selector::getSelectedSObjSnapshots()
{
	std::map<ISObj *, ISObj *> snapshots;
	for (ISObj * selectedSObj : selectedSObjs)
	{
		ISObj *snapshot = SObjFactory::null(selectedSObj);
		selectedSObj->getSnapshot(snapshot);
		snapshots[selectedSObj] = snapshot;
	}

	return snapshots;
}

std::vector<ISObj *> Selector::getPickedSObjWithPixel(QPoint mousePos)
{
	pick();

	ISObj *pickedSObj = nullptr;

	unsigned char pixel[4];
	float depth;
	pickFBO->bind();
	glReadPixels(mousePos.x(), sceneSize.y - mousePos.y() - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	glReadPixels(mousePos.x(), sceneSize.y - mousePos.y() - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	pickFBO->unBind();

	if (!Tool::floatEqual(depth, 1.0f))
	{
		int id = pixel[0] + pixel[1] * 256 + pixel[2] * 256 * 256;

		for (ISObj *sObj : *sObjs)
		{
			if (sObj->getID() == id)
			{
				pickedSObj = sObj;
				break;
			}
		}
	}

	return grouper->getGroup(pickedSObj);
}

ISObj * Selector::getPickedSObjWithAABB(QPoint mousePos)
{
	QRect selectRect(mousePos, mousePos + QPoint(1, 1)); // 用一个1X1的窗口做测试，模拟射线检测
	std::sort(sObjs->begin(), sObjs->end(), FarFirstComparer());
	for (ISObj *sObj : *sObjs)
	{
		if (selectRect.intersects(sObj->getRect()))
		{
			return sObj;
		}
	}

	return nullptr;
}

bool Selector::isHoverSelectedSObj(QPoint mousePos)
{
	QRect selectRect(mousePos, mousePos + QPoint(1, 1)); // 用一个1X1的窗口做测试，模拟射线检测
	for (ISObj *selectedSObj : selectedSObjs)
	{
		if (selectRect.intersects(selectedSObj->getRect()))
		{
			return true;
		}
	}
	return false;
}

bool Selector::isMultiSelect()
{
	return selectedSObjs.size() > 1;
}

bool Selector::isSingleSelect()
{
	return selectedSObjs.size() == 1;
}

bool Selector::isNullSelect()
{
	return selectedSObjs.empty();
}

void Selector::mousePressEvent(QMouseEvent *evt)
{
	if (evt->button() == Qt::LeftButton)
	{
		lastMousePressPos = evt->pos();
	}
}

void Selector::mouseReleaseEvent(QMouseEvent *evt)
{
	if (evt->button() == Qt::LeftButton)
	{
		QPoint curMousePressPos = evt->pos();
		QRect selectRect(std::min(lastMousePressPos.x(), curMousePressPos.x()), std::min(lastMousePressPos.y(), curMousePressPos.y()),
			std::abs(curMousePressPos.x() - lastMousePressPos.x()) + 1, std::abs(curMousePressPos.y() - lastMousePressPos.y()) + 1);

		std::vector<ISObj *> tmpSelectedSObjs = getPickedSObjs(selectRect);

		if (evt->modifiers() != Qt::ShiftModifier) // 没有按住shift
		{
			if (!selectedSObjs.empty())
			{
				clearSelectedSObjs();
			}
			addSelectedSObjs(tmpSelectedSObjs);
		}
		else // 按住shift，可以增选
		{
			for (ISObj *tmpSelectedSobj : tmpSelectedSObjs)
			{
				// 如果之前没有被选择，将其加入选择列表
				if (std::find(selectedSObjs.begin(), selectedSObjs.end(), tmpSelectedSobj) == selectedSObjs.end())
				{
					addSelectedSObj(tmpSelectedSobj);
				}
			}
		}
	}
}

void Selector::group()
{
	if (isMultiSelect())
	{
		History::getInstance()->execute(new GroupCommand(grouper, selectedSObjs), true);
	}
}

void Selector::ungroup()
{
	if (isMultiSelect())
	{
		History::getInstance()->execute(new UngroupCommand(grouper, selectedSObjs), true);
	}
}

Grouper *Selector::getGrouper()
{
	return grouper;
}

void Selector::pick()
{
	pickFBO->bind();
	pickFBO->resize(sceneSize.x, sceneSize.y);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 用背景色/背景深度清除缓冲
	glDisable(GL_BLEND);
	for (ISObj *sObj : *sObjs)
	{
		sObj->render(false, true);
	}
	glEnable(GL_BLEND);

	pickFBO->unBind();

	glFlush();
	glFinish();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

std::vector<ISObj *> Selector::getPickedSObjs(QRect rect)
{
	pick();

	std::vector<ISObj *> pickedSObjs;
	int pixelNum = rect.width() * rect.height() * 4;
	unsigned char *pixel = new unsigned char[pixelNum];
	float *depth = new float[pixelNum];

	pickFBO->bind();
	glReadPixels(rect.bottomLeft().x(), sceneSize.y - rect.bottomLeft().y() - 1,
		rect.width(), rect.height(), GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	glReadPixels(rect.bottomLeft().x(), sceneSize.y - rect.bottomLeft().y() - 1,
		rect.width(), rect.height(), GL_DEPTH_COMPONENT, GL_FLOAT, depth);
	pickFBO->unBind();

	std::vector<int> ids;
	for (int i = 0; i < pixelNum; i += 4)
	{
		if (!Tool::floatEqual(depth[i], 1.0f))
		{
			int id = pixel[i] + pixel[i + 1] * 256 + pixel[i + 2] * 256 * 256;
			ids.push_back(id);
		}
	}

	delete[] pixel;
	delete[] depth;

	for (ISObj *sObj : *sObjs)
	{
		if (std::find(ids.begin(), ids.end(), sObj->getID()) != ids.end())
		{
			std::vector<ISObj *> group = grouper->getGroup(sObj);
			for (ISObj *groupSObj : group)
			{
				if (std::find(pickedSObjs.begin(), pickedSObjs.end(), groupSObj) == pickedSObjs.end())
				{
					pickedSObjs.push_back(groupSObj);
				}
			}
		}
	}

	return pickedSObjs;
}