#include "rotator.h"
#include "history.h"
#include "statecommand.h"

#include <algorithm>

Rotator::Rotator(Selector *selector) : selector(selector)
{

}

Rotator::~Rotator()
{

}

void Rotator::mousePressEvent(QMouseEvent *evt, glm::ivec2 aabbCenterWinPos)
{
	lastSnapshots = selector->getSelectedSObjSnapshots();
	this->aabbCenterWinPos = aabbCenterWinPos;
	lastMousePos = evt->pos();
}

void Rotator::mouseMoveEvent(QMouseEvent *evt)
{
	QPoint curMousePos = evt->pos();

	if (evt->buttons() & Qt::LeftButton)
	{
		glm::ivec2 centerToLastMouse = glm::ivec2(lastMousePos.x() - aabbCenterWinPos.x, lastMousePos.y() - aabbCenterWinPos.y);
		glm::ivec2 centerToCurMouse = glm::ivec2(curMousePos.x() - aabbCenterWinPos.x, curMousePos.y() - aabbCenterWinPos.y);

		float angle = std::atan2(centerToLastMouse.y * centerToCurMouse.x - centerToLastMouse.x * centerToCurMouse.y,
			centerToLastMouse.x * centerToCurMouse.x + centerToLastMouse.y * centerToCurMouse.y);
		angle = glm::degrees(angle);

		for (ISObj *selectedSObj : selector->getSelectedSObjs())
		{
			selectedSObj->rotateDz(angle);
		}
	}
	else if (evt->buttons() & Qt::MiddleButton)
	{
		QPoint diffMousePos = curMousePos - lastMousePos;

		bool threeDRotateOn = true;
		for (ISObj *selectedSObj : selector->getSelectedSObjs())
		{
			if (selectedSObj->getDimension() == TwoD)
			{
				threeDRotateOn = false;
				break;
			}
		}

		if (threeDRotateOn)
		{
			for (ISObj *selectedSObj : selector->getSelectedSObjs())
			{
				selectedSObj->rotateDxy(diffMousePos.y() * MouseRotateSensitivity, diffMousePos.x() * MouseRotateSensitivity);
			}
		}
	}

	lastMousePos = curMousePos;
}

void Rotator::mouseReleaseEvent(QMouseEvent *evt)
{
	curSnapshots = selector->getSelectedSObjSnapshots();
	History::getInstance()->execute(new StateCommand(lastSnapshots, curSnapshots));
}