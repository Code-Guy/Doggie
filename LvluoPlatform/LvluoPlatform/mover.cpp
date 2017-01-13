#include "mover.h"
#include "history.h"
#include "statecommand.h"

const int MaxTinyMoveDis = 8;

Mover::Mover(Camera *camera, Selector *selector) : camera(camera), selector(selector)
{

}

Mover::~Mover()
{

}

void Mover::mousePressEvent(QMouseEvent *evt)
{
	if (evt->button() == Qt::LeftButton)
	{
		lastSnapshots = selector->getSelectedSObjSnapshots();
	}
	lastMousePos = evt->pos();
	lastMousePressPos = evt->pos();
}

void Mover::mouseMoveEvent(QMouseEvent *evt)
{
	QPoint curMousePos = evt->pos();
	QPoint deltaMousePos = curMousePos - lastMousePos;

	if (evt->buttons() & Qt::LeftButton)
	{
		for (ISObj *selectedSObj : selector->getSelectedSObjs())
		{
			selectedSObj->moveDXY(deltaMousePos.x(), deltaMousePos.y());
		}
	}
	else if (evt->buttons() & Qt::MidButton && evt->modifiers() == Qt::ShiftModifier)
	{
		camera->moveDXY(deltaMousePos.x(), deltaMousePos.y());
	}

	lastMousePos = curMousePos;
}

void Mover::mouseReleaseEvent(QMouseEvent *evt)
{
	if (evt->button() == Qt::LeftButton)
	{
		curSnapshots = selector->getSelectedSObjSnapshots();
		if (!checkTinyMove())
		{
			History::getInstance()->execute(new StateCommand(lastSnapshots, curSnapshots));
		}
	}
}

void Mover::keyPressEvent(QKeyEvent *evt)
{
	if (evt->key() == Qt::Key_Up || evt->key() == Qt::Key_Down ||
		evt->key() == Qt::Key_Left || evt->key() == Qt::Key_Right)
	{
		lastSnapshots = selector->getSelectedSObjSnapshots();
	}
	switch (evt->key())
	{
	case Qt::Key_Up:
		for (ISObj *selectedSObj : selector->getSelectedSObjs())
		{
			selectedSObj->setMoveDirection(Up, true);
		}
		break;
	case Qt::Key_Down:
		for (ISObj *selectedSObj : selector->getSelectedSObjs())
		{
			selectedSObj->setMoveDirection(Down, true);
		}
		break;
	case Qt::Key_Left:
		for (ISObj *selectedSObj : selector->getSelectedSObjs())
		{
			selectedSObj->setMoveDirection(Left, true);
		}
		break;
	case Qt::Key_Right:
		for (ISObj *selectedSObj : selector->getSelectedSObjs())
		{
			selectedSObj->setMoveDirection(Right, true);
		}
		break;
	}
}

void Mover::keyReleaseEvent(QKeyEvent *evt)
{
	if (evt->key() == Qt::Key_Up || evt->key() == Qt::Key_Down ||
		evt->key() == Qt::Key_Left || evt->key() == Qt::Key_Right)
	{
		curSnapshots = selector->getSelectedSObjSnapshots();
		History::getInstance()->execute(new StateCommand(lastSnapshots, curSnapshots));
	}
	switch (evt->key())
	{
	case Qt::Key_Up:
		for (ISObj *selectedSObj : selector->getSelectedSObjs())
		{
			selectedSObj->setMoveDirection(Up, false);
		}
		break;
	case Qt::Key_Down:
		for (ISObj *selectedSObj : selector->getSelectedSObjs())
		{
			selectedSObj->setMoveDirection(Down, false);
		}
		break;
	case Qt::Key_Left:
		for (ISObj *selectedSObj : selector->getSelectedSObjs())
		{
			selectedSObj->setMoveDirection(Left, false);
		}
		break;
	case Qt::Key_Right:
		for (ISObj *selectedSObj : selector->getSelectedSObjs())
		{
			selectedSObj->setMoveDirection(Right, false);
		}
		break;
	}
}

void Mover::wheelEvent(QWheelEvent *evt)
{
	int degree = evt->delta() / 8;
	int step = degree / 15;

	if (selector->isNullSelect())
	{
		camera->moveDZ(step);
		// 需要更新所有sobj的屏幕距离到世界距离的比例值
		for (ISObj *sObj : *selector->getSObjs())
		{
			sObj->updateWinObjRatio();
		}
	}
	else
	{
		for (ISObj *selectedSObj : selector->getSelectedSObjs())
		{
			selectedSObj->moveDZ(step);
		}
	}
}

bool Mover::checkTinyMove()
{
	return QPoint(lastMousePos - lastMousePressPos).manhattanLength() < MaxTinyMoveDis;
}