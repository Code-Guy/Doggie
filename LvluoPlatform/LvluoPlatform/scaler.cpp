#include "scaler.h"
#include "tool.h"
#include "history.h"
#include "statecommand.h"

Scaler::Scaler(Selector *selector) : selector(selector)
{
	constrainScale = false;
}

Scaler::~Scaler()
{

}

void Scaler::mousePressEvent(QMouseEvent *evt, glm::ivec2 aabbCenterWinPos, glm::ivec2 minAnchorWinPos)
{
	if (evt->button() == Qt::LeftButton)
	{
		lastSnapshots = selector->getSelectedSObjSnapshots();

		this->aabbCenterWinPos = aabbCenterWinPos;
		this->minAnchorWinPos = minAnchorWinPos;

		ratio = glm::vec3(1.0f);
	}
}

void Scaler::mouseMoveEvent(QMouseEvent *evt)
{
	if (evt->buttons() & Qt::LeftButton)
	{
		QPoint curMousePos = evt->pos();
		glm::ivec2 centerToCurMouse = glm::ivec2(curMousePos.x() - aabbCenterWinPos.x, curMousePos.y() - aabbCenterWinPos.y);
		glm::ivec2 centerToAnchor = minAnchorWinPos - aabbCenterWinPos;

		if (!constrainScale && 
			selector->isSingleSelect() && selector->getSelectedSObjs().front()->getDimension() == TwoD)
		{
			glm::vec3 rotation = selector->getSelectedSObjs().front()->getRotation();
			glm::vec2 alignXAxis = glm::vec2(glm::cos(rotation.z), glm::sin(rotation.z));
			glm::vec2 alignYAxis = glm::vec2(glm::sin(rotation.z), -glm::cos(rotation.z));
			float centerToCurMouseXAlignDis = glm::dot(alignXAxis, glm::vec2(centerToCurMouse));
			float centerToCurMouseYAlignDis = glm::dot(alignYAxis, glm::vec2(centerToCurMouse));
			float centerToAnchorXAlignDis = glm::dot(alignXAxis, glm::vec2(centerToAnchor));
			float centerToAnchorYAlignDis = glm::dot(alignYAxis, glm::vec2(centerToAnchor));
			ratio = glm::vec3(centerToCurMouseXAlignDis / centerToAnchorXAlignDis, centerToCurMouseYAlignDis / centerToAnchorYAlignDis, 1.0f);
		}
		else
		{
			float r = glm::sqrt(glm::dot(glm::vec2(centerToCurMouse), glm::vec2(centerToAnchor))) / glm::length(glm::vec2(centerToAnchor));
			ratio = glm::vec3(r);
		}

		for (ISObj *selectedSObj : selector->getSelectedSObjs())
		{
			selectedSObj->scale(ratio);
		}
	}
}

void Scaler::mouseReleaseEvent(QMouseEvent *evt)
{
	if (evt->button() == Qt::LeftButton)
	{
		for (ISObj *selectedSObj : selector->getSelectedSObjs())
		{
			selectedSObj->setBaseSize(ratio);
		}
		curSnapshots = selector->getSelectedSObjSnapshots();
		History::getInstance()->execute(new StateCommand(lastSnapshots, curSnapshots));
	}
}

void Scaler::keyPressEvent(QKeyEvent *evt)
{
	if (evt->key() == Qt::Key_Shift)
	{
		constrainScale = true;
	}
}

void Scaler::keyReleaseEvent(QKeyEvent *evt)
{
	if (evt->key() == Qt::Key_Shift)
	{
		constrainScale = false;
	}
}