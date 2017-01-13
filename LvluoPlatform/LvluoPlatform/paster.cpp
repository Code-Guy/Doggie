#include "paster.h"
#include "createcommand.h"
#include "removecommand.h"
#include "history.h"
#include "sobjfactory.h"

#include <algorithm>

const float PasteOffsetZ = 0.001f;

Paster::Paster(Selector *selector) : selector(selector)
{
	
}

Paster::~Paster()
{
	
}

void Paster::copy()
{
	if (!selector->isNullSelect())
	{
		copyClipboard.clear();
		cutClipboard.clear();
		for (ISObj *selectedSObj : selector->getSelectedSObjs())
		{
			selectedSObj->updateTransform();
			copyClipboard.push_back(selectedSObj);
		}
		selector->clearSelectedSObjs();
	}
}

void Paster::cut()
{
	if (!selector->isNullSelect())
	{
		copyClipboard.clear();
		cutClipboard.clear();
		for (ISObj *selectedSObj : selector->getSelectedSObjs())
		{
			selectedSObj->updateTransform();
			cutClipboard.push_back(selectedSObj);
		}
		selector->clearSelectedSObjs();
		History::getInstance()->execute(new RemoveCommand(selector->getSObjs(), selector->getSelectedSObjs()), true);
	}
}

void Paster::paste(QPoint p)
{
	selector->clearSelectedSObjs();
	// ºÏ²¢¼ôÌù°å
	std::vector<ISObj *> combinedClipboard;
	combinedClipboard.insert(combinedClipboard.end(), copyClipboard.begin(), copyClipboard.end());
	combinedClipboard.insert(combinedClipboard.end(), cutClipboard.begin(), cutClipboard.end());
	if (!combinedClipboard.empty())
	{
		std::vector<ISObj *> cloneSObjs;
		glm::ivec2 pinnedSObjXY = combinedClipboard.front()->getXY();
		for (ISObj *clipboardSObj : combinedClipboard)
		{
			ISObj *cloneSObj = SObjFactory::copy(clipboardSObj);
			glm::ivec2 cloneSObjXY = cloneSObj->getXY();
			cloneSObj->moveXY(p.x() + cloneSObjXY.x - pinnedSObjXY.x, p.y() + cloneSObjXY.y - pinnedSObjXY.y);
			cloneSObj->moveDZ(PasteOffsetZ);
			cloneSObjs.push_back(cloneSObj);
		}
		selector->addSelectedSObjs(cloneSObjs);
		History::getInstance()->execute(new CreateCommand(selector->getSObjs(), cloneSObjs), true);
	}
}