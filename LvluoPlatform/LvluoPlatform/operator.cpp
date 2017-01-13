#include "operator.h"
#include "resourcemanager.h"
#include "primitivemanager.h"
#include "createcommand.h"
#include "removecommand.h"
#include "history.h"
#include "tool.h"

const int RotateMouseAnchorMaxDis = 30; // 要出现缩放鼠标样式鼠标和锚点相隔的最大距离
const int ScaleMouseAnchorMaxDis = 10; // 要出现缩放鼠标样式鼠标和锚点相隔的最大距离

Operator::Operator(Camera *camera, std::vector<ISObj *> *sObjs) : camera(camera)
{
	selector = new Selector(sObjs);
	mover = new Mover(camera, selector);
	scaler = new Scaler(selector);
	rotator = new Rotator(selector);
	paster = new Paster(selector);

	isMousePressed = false;
	isMouseMoved = false;

	cursorShape = NoneShape;
	operationMode = Auto;

	dragSObj = nullptr;
}

Operator::~Operator()
{
	delete selector;
	delete mover;
	delete scaler;
	delete rotator;
	delete paster;
}

void Operator::logic(QPoint mousePos)
{
	selector->logic();
	curMousePos = mousePos;
	if (dragSObj != nullptr)
	{
		dragSObj->moveXY(mousePos.x(), mousePos.y());
	}
	else
	{
		updateSelectElements();
		updateCursorShape(mousePos);
		updateSelectFrameTransformation();
	}
}

void Operator::render()
{
	if (isMousePressed && cursorShape == NoneShape)
	{
		// 画选择框
		ResourceManager::getInstance()->getBaseShader()->enable();
		ResourceManager::getInstance()->getBaseShader()->setMVP(
			camera->getViewProjectionMatrix() *
			glm::translate(frameTranslation) * glm::scale(frameScaling));
		ResourceManager::getInstance()->getBaseShader()->setColor(PrimitiveManager::FrameColor);

		PrimitiveManager::getInstance()->renderFrame();
	}
	if (selector->isMultiSelect())
	{
		glLineWidth(3.0f);
		// 画虚线包围框
		ResourceManager::getInstance()->getBaseShader()->enable();
		ResourceManager::getInstance()->getBaseShader()->setMVP(
			camera->getViewProjectionMatrix() *
			glm::translate(aabbCenter) * glm::scale(aabbSize));
		ResourceManager::getInstance()->getBaseShader()->setColor(PrimitiveManager::AABBColor);

		PrimitiveManager::getInstance()->renderAABB();
		glLineWidth(1.0f);

		// 画8个锚点
		ResourceManager::getInstance()->getBaseShader()->setColor(PrimitiveManager::AnchorColor);
		for (int i = 0; i < 8; i++)
		{
			ResourceManager::getInstance()->getBaseShader()->setMVP(
				camera->getViewProjectionMatrix() *
				glm::translate(anchorPos[i]) * glm::scale(glm::vec3(PrimitiveManager::AnchorSize)));
			PrimitiveManager::getInstance()->renderAnchor();
		}
	}
}

void Operator::resize(int sw, int sh)
{
	sceneSize = glm::ivec2(sw, sh);
	selector->resize(sw, sh);
}

void Operator::mousePressEvent(QMouseEvent *evt)
{
	std::vector<ISObj *> sObjs = selector->getPickedSObjWithPixel(evt->pos());
	// 右键，显示被选中物体的属性菜单
	if (evt->button() == Qt::RightButton)
	{
		if (sObjs.size() == 1)
		{
			sObjs.front()->showParamWidget(evt->globalPos());
		}
	}
	else
	{
		isMousePressed = true;
		lastMousePressPos = evt->pos();
		curMousePressPos = lastMousePressPos;

		if (evt->button() == Qt::MidButton && evt->modifiers() == Qt::ShiftModifier && selector->isNullSelect())
		{
			cursorShape = MoveCameraShape;
		}
		else if (evt->button() == Qt::MidButton)
		{
			cursorShape = RotateFreeShape;
		}
		else if (evt->button() == Qt::LeftButton && evt->modifiers() != Qt::ShiftModifier &&
			!sObjs.empty() && !selector->containSObjs(sObjs))
		{
			selector->clearSelectedSObjs();
			selector->addSelectedSObjs(sObjs);
			cursorShape = DragShape;
		}

		switch (cursorShape)
		{
		case Operator::HoverShape:
		case Operator::DragShape:
		case Operator::MoveCameraShape:
			mover->mousePressEvent(evt);
			break;
		case Operator::RotateTopLeftShape:
		case Operator::RotateTopRightShape:
		case Operator::RotateBottomLeftShape:
		case Operator::RotateBottomRightShape:
		case Operator::RotateFreeShape:
			rotator->mousePressEvent(evt, aabbCenterWinPos);
			break;
		case Operator::ScaleBDiagShape:
		case Operator::ScaleFDiagShape:
			scaler->mousePressEvent(evt, aabbCenterWinPos, minAnchorWinPos);
			break;
		case Operator::NoneShape:
			selector->mousePressEvent(evt);
			break;
		default:
			break;
		}
	}
}

void Operator::mouseMoveEvent(QMouseEvent *evt)
{
	isMouseMoved = true;
	curMousePressPos = evt->pos();

	switch (cursorShape)
	{
	case Operator::HoverShape:
	case Operator::DragShape:
	case Operator::MoveCameraShape:
		mover->mouseMoveEvent(evt);
		break;
	case Operator::RotateTopLeftShape:
	case Operator::RotateTopRightShape:
	case Operator::RotateBottomLeftShape:
	case Operator::RotateBottomRightShape:
	case Operator::RotateFreeShape:
		rotator->mouseMoveEvent(evt);
		break;
	case Operator::ScaleBDiagShape:
	case Operator::ScaleFDiagShape:
		scaler->mouseMoveEvent(evt);
		break;
	case Operator::NoneShape:
		break;
	default:
		break;
	}
}

void Operator::mouseReleaseEvent(QMouseEvent *evt)
{
	if (dragSObj)
	{
		if (evt->pos() != dragSObjPos)
		{
			dragSObj->setVisible(true);
			History::getInstance()->execute(new CreateCommand(selector->getSObjs(), dragSObj));
			dragSObj = nullptr;
			dragSObjPos = QPoint(0, 0);
		}
		else
		{
			dragSObjPos = QPoint(0, 0);
		}
	}
	else
	{
		switch (cursorShape)
		{
		case Operator::HoverShape:
		case Operator::DragShape:
			mover->mouseReleaseEvent(evt);
			break;
		case Operator::RotateTopLeftShape:
		case Operator::RotateTopRightShape:
		case Operator::RotateBottomLeftShape:
		case Operator::RotateBottomRightShape:
		case Operator::RotateFreeShape:
			rotator->mouseReleaseEvent(evt);
			break;
		case Operator::ScaleBDiagShape:
		case Operator::ScaleFDiagShape:
			scaler->mouseReleaseEvent(evt);
			break;
		case Operator::NoneShape:
			selector->mouseReleaseEvent(evt);
			break;
		default:
			break;
		}

		if (!isMouseMoved)
		{
			// 处理sobj的鼠标事件
			ISObj *pickedSObj = selector->getPickedSObjWithAABB(evt->pos());
			if (pickedSObj != nullptr)
			{
				if (evt->button() == Qt::LeftButton)
				{
					pickedSObj->click(evt->globalPos());
				}
				else if (evt->button() == Qt::RightButton)
				{
					pickedSObj->rightClick(evt->globalPos());
				}
			}
		}
	}

	isMousePressed = false;
	isMouseMoved = false;
	cursorShape = NoneShape;
}

void Operator::mouseDoubleClickEvent(QMouseEvent *evt)
{
	if (evt->button() == Qt::LeftButton)
	{
		std::vector<ISObj *> sObjs = selector->getPickedSObjWithPixel(evt->pos());
		if (sObjs.size() == 1)
		{
			sObjs.front()->showParamWidget(evt->globalPos());
		}
	}
}

void Operator::keyPressEvent(QKeyEvent *evt)
{
	switch (evt->key())
	{
	case Qt::Key_Q:
		operationMode = Auto;
		break;
	case Qt::Key_W:
		operationMode = Move;
		break;
	case Qt::Key_E:
		operationMode = Scale;
		break;
	case Qt::Key_R:
		operationMode = Rotate;
		break;
	default:
		break;
	}
	mover->keyPressEvent(evt);
	scaler->keyPressEvent(evt);
}

void Operator::keyReleaseEvent(QKeyEvent *evt)
{
	mover->keyReleaseEvent(evt);
	scaler->keyReleaseEvent(evt);
}

void Operator::wheelEvent(QWheelEvent *evt)
{
	mover->wheelEvent(evt);
}

void Operator::addDragSObj(ISObj *dragSObj, QPoint mousePos)
{
	this->dragSObj = dragSObj;
	dragSObjPos = mousePos;

	dragSObj->moveXY(mousePos.x(), mousePos.y());
	dragSObj->setVisible(false);
	selector->clearSelectedSObjs();
	selector->addSelectedSObj(dragSObj);
	selector->getSObjs()->push_back(dragSObj);
}

void Operator::copy()
{
	paster->copy();
}

void Operator::cut()
{
	paster->cut();
}

void Operator::paste()
{
	paster->paste(curMousePos);
}

void Operator::remove()
{
	History::getInstance()->execute(new RemoveCommand(selector->getSObjs(), selector->getSelectedSObjs()), true);
}

void Operator::selectAll()
{
	selector->addSelectedSObjs(*selector->getSObjs());
}

void Operator::group()
{
	selector->group();
}

void Operator::ungroup()
{
	selector->ungroup();
}

Grouper *Operator::getGrouper()
{
	return selector->getGrouper();
}

void Operator::updateCursorShape(QPoint mousePos)
{
	if (selector->isNullSelect())
	{
		if (cursorShape != MoveCameraShape)
		{
			cursorShape = NoneShape;
		}
	}
	else if (!isMousePressed) // 只在鼠标没有按下+选择物体的时候更新鼠标状态
	{
		findNearMouseAnchorDis(mousePos);
		if (operationMode == Auto) // 如果处于自动切换模式
		{
			cursorShape = NoneShape;
			checkMove(mousePos);
			checkRotate(mousePos);
			checkScale();
		}
		else if (operationMode == Move)
		{
			cursorShape = HoverShape;
		}
		else if (operationMode == Rotate)
		{
			switchRotate();
		}
		else if (operationMode == Scale)
		{
			switchScale();
		}
	}
	else if (cursorShape == HoverShape)
	{
		cursorShape = DragShape;
	}

	emit updateCursorShape(cursorShape);
}

void Operator::updateSelectElements()
{
	if (selector->isSingleSelect()) // 单选
	{
		selector->getSelectedSObjs().front()->getAABBVertices(anchorPos);
		aabbCenter = selector->getSelectedSObjs().front()->getAABBCenter(anchorPos);
	}
	else if (selector->isMultiSelect()) // 多选
	{
		updateMultiSelectElements();
	}
}

void Operator::updateMultiSelectElements()
{
	glm::vec3 minCorner = glm::vec3(std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max());
	glm::vec3 maxCorner = glm::vec3(std::numeric_limits<float>::lowest(),
		std::numeric_limits<float>::lowest(),
		std::numeric_limits<float>::lowest());

	for (ISObj *selectedSObj : selector->getSelectedSObjs())
	{
		selectedSObj->setShowAABB(false);

		glm::vec3 aabbVertices[8];
		selectedSObj->getAABBVertices(aabbVertices);
		for (int i = 0; i < 8; i++)
		{
			if (aabbVertices[i].x < minCorner.x)
			{
				minCorner.x = aabbVertices[i].x;
			}
			if (aabbVertices[i].y < minCorner.y)
			{
				minCorner.y = aabbVertices[i].y;
			}
			if (aabbVertices[i].z < minCorner.z)
			{
				minCorner.z = aabbVertices[i].z;
			}

			if (aabbVertices[i].x > maxCorner.x)
			{
				maxCorner.x = aabbVertices[i].x;
			}
			if (aabbVertices[i].y > maxCorner.y)
			{
				maxCorner.y = aabbVertices[i].y;
			}
			if (aabbVertices[i].z > maxCorner.z)
			{
				maxCorner.z = aabbVertices[i].z;
			}
		}
	}

	aabbSize = (maxCorner - minCorner) / 2;
	aabbCenter = (maxCorner + minCorner) / 2;

	anchorPos[0] = aabbCenter + glm::vec3(-aabbSize.x, -aabbSize.y, -aabbSize.z);
	anchorPos[1] = aabbCenter + glm::vec3(-aabbSize.x, -aabbSize.y, aabbSize.z);
	anchorPos[2] = aabbCenter + glm::vec3(aabbSize.x, -aabbSize.y, aabbSize.z);
	anchorPos[3] = aabbCenter + glm::vec3(aabbSize.x, -aabbSize.y, -aabbSize.z);
	anchorPos[4] = aabbCenter + glm::vec3(-aabbSize.x, aabbSize.y, -aabbSize.z);
	anchorPos[5] = aabbCenter + glm::vec3(-aabbSize.x, aabbSize.y, aabbSize.z);
	anchorPos[6] = aabbCenter + glm::vec3(aabbSize.x, aabbSize.y, aabbSize.z);
	anchorPos[7] = aabbCenter + glm::vec3(aabbSize.x, aabbSize.y, -aabbSize.z);
}

void Operator::updateSelectFrameTransformation()
{
	if (isMousePressed && cursorShape == NoneShape)
	{
		glm::vec3 lastMousePressObjPos = Tool::calcObjPos(glm::vec3(lastMousePressPos.x(), lastMousePressPos.y(), 0.1f),
			glm::ivec4(0, 0, sceneSize.x, sceneSize.y), camera->getViewMatrix(), camera->getProjectionMatrix());
		glm::vec3 curMousePressObjPos = Tool::calcObjPos(glm::vec3(curMousePressPos.x(), curMousePressPos.y(), 0.1f),
			glm::ivec4(0, 0, sceneSize.x, sceneSize.y), camera->getViewMatrix(), camera->getProjectionMatrix());

		glm::vec3 centerMousePressObjPos = (lastMousePressObjPos + curMousePressObjPos) / 2;

		frameTranslation = centerMousePressObjPos;
		frameScaling = glm::vec3(std::abs(curMousePressObjPos.x - centerMousePressObjPos.x),
			std::abs(curMousePressObjPos.y - centerMousePressObjPos.y),
			1.0f);
	}
}

void Operator::findNearMouseAnchorDis(QPoint mousePos)
{
	aabbCenterWinPos = Tool::calcWinPos(aabbCenter,
		glm::ivec4(0, 0, sceneSize.x, sceneSize.y), camera->getViewMatrix(), camera->getProjectionMatrix());
	minAnchorWinPos = Tool::calcWinPos(anchorPos[0],
		glm::ivec4(0, 0, sceneSize.x, sceneSize.y), camera->getViewMatrix(), camera->getProjectionMatrix());
	minMouseAnchorDis = QPoint(mousePos.x() - minAnchorWinPos.x, mousePos.y() - minAnchorWinPos.y).manhattanLength();

	for (int i = 1; i < 8; i++)
	{
		glm::ivec2 anchorWinPos = Tool::calcWinPos(anchorPos[i],
			glm::ivec4(0, 0, sceneSize.x, sceneSize.y), camera->getViewMatrix(), camera->getProjectionMatrix());
		int mouseAnchorDis = QPoint(mousePos.x() - anchorWinPos.x, mousePos.y() - anchorWinPos.y).manhattanLength();
		if (mouseAnchorDis < minMouseAnchorDis)
		{
			minAnchorWinPos = anchorWinPos;
			minMouseAnchorDis = mouseAnchorDis;
		}
	}
	centerToNearestAnchorDir = minAnchorWinPos - aabbCenterWinPos;
}

void Operator::checkMove(QPoint mousePos)
{
	// 如果鼠标在某个被选择的物体上，表明可以移动
	if (selector->isHoverSelectedSObj(mousePos))
	{
		cursorShape = HoverShape;
	}
}

void Operator::checkRotate(QPoint mousePos)
{
	if (!selector->isHoverSelectedSObj(mousePos) && minMouseAnchorDis < RotateMouseAnchorMaxDis)
	{
		switchRotate();
	}
}

void Operator::checkScale()
{
	if (minMouseAnchorDis < ScaleMouseAnchorMaxDis)
	{
		switchScale();
	}
}

void Operator::switchRotate()
{
	if (centerToNearestAnchorDir.x > 0 && centerToNearestAnchorDir.y > 0)
	{
		cursorShape = RotateBottomRightShape;
	}
	else if (centerToNearestAnchorDir.x < 0 && centerToNearestAnchorDir.y > 0)
	{
		cursorShape = RotateBottomLeftShape;
	}
	else if (centerToNearestAnchorDir.x < 0 && centerToNearestAnchorDir.y < 0)
	{
		cursorShape = RotateTopLeftShape;
	}
	else if (centerToNearestAnchorDir.x > 0 && centerToNearestAnchorDir.y < 0)
	{
		cursorShape = RotateTopRightShape;
	}
}

void Operator::switchScale()
{
	if (centerToNearestAnchorDir.x * centerToNearestAnchorDir.y < 0)
	{
		cursorShape = ScaleBDiagShape;
	}
	else
	{
		cursorShape = ScaleFDiagShape;
	}
}