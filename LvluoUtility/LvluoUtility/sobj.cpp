#include "sobj.h"
#include "resourcemanager.h"
#include "primitivemanager.h"
#include "sobjfactory.h"
#include "statecommand.h"
#include "history.h"
#include "tool.h"

// 默认初始化数据
const glm::vec3 DefaultPosition = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 DefaultRotation = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 DefaultSize = glm::vec3(1.0f, 1.0f, 1.0f);

const float MoveZSpeed = 0.1f; // 深度移动速度
const float MoveXYSpeed = 0.2f; // XY方向移动速度

int SObj::sObjID = 0;

SObj::SObj()
{

}

SObj::SObj(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, Dimension dimen) : camera(camera), sceneSize(sceneSize),
	ids(ids), dimen(dimen)
{
	init();
}

SObj::SObj(SObj *other)
{
	init();
	this->camera = other->camera;
	this->sceneSize = other->sceneSize;
	this->ids = other->ids;
	this->dimen = other->dimen;

	setParamWidget();
	inflateParamWidget(other->position, other->rotation, other->size);
	connectParamWidget();
	updateWinObjRatio();
}

SObj::~SObj()
{
	delete positionDoubleSpinBoxVec3;
	delete rotationDoubleSpinBoxVec3;
	delete sizeDoubleSpinBoxVec3;
	delete paramWidget;
}

void SObj::logic(float dt)
{
	if (isUp)
	{
		position.y += MoveXYSpeed * dt;
	}
	if (isDown)
	{
		position.y -= MoveXYSpeed * dt;
	}
	if (isLeft)
	{
		position.x -= MoveXYSpeed * dt;
	}
	if (isRight)
	{
		position.x += MoveXYSpeed * dt;
	}

	rawM = glm::eulerAngleXYZ(glm::radians(rotation.x),
		glm::radians(rotation.y),
		glm::radians(rotation.z)) *
		glm::scale(size);
	rawM = glm::translate(center + position) *
		glm::eulerAngleXYZ(glm::radians(centerRotation.x),
			glm::radians(centerRotation.y),
			glm::radians(centerRotation.z)) *
		glm::scale(centerSize) *
		glm::translate(-center) * rawM;
	m = rawM;
}

void SObj::render(bool blendOn, bool pickOn)
{
	if (selected && showAABB)
	{
		ResourceManager::getInstance()->getBaseShader()->enable();
		ResourceManager::getInstance()->getBaseShader()->setMVP(camera->getViewProjectionMatrix() * rawM * glm::scale(aabb));
		ResourceManager::getInstance()->getBaseShader()->setColor(PrimitiveManager::AABBColor);

		// 画包围框
		PrimitiveManager::getInstance()->renderAABB();

		// 画8个锚点
		ResourceManager::getInstance()->getBaseShader()->setColor(PrimitiveManager::AnchorColor);
		glm::vec3 aabbAnchorPosition[] = {
			glm::vec3(-aabb.x, -aabb.y, -aabb.z), glm::vec3(-aabb.x, -aabb.y, aabb.z),
			glm::vec3(aabb.x, -aabb.y, aabb.z), glm::vec3(aabb.x, -aabb.y, -aabb.z),
			glm::vec3(-aabb.x, aabb.y, -aabb.z), glm::vec3(-aabb.x, aabb.y, aabb.z),
			glm::vec3(aabb.x, aabb.y, aabb.z), glm::vec3(aabb.x, aabb.y, -aabb.z)
		};
		for (int i = 0; i < 8; i++)
		{
			ResourceManager::getInstance()->getBaseShader()->setMVP(camera->getViewProjectionMatrix() *
				rawM * glm::translate(aabbAnchorPosition[i]) * glm::scale(glm::vec3(PrimitiveManager::AnchorSize)));
			PrimitiveManager::getInstance()->renderAnchor();
		}
	}
}

void SObj::save(QJsonObject &jsonObject)
{
	jsonObject.insert("Position", Tool::vec3ToStr(position));
	jsonObject.insert("Rotation", Tool::vec3ToStr(rotation));
	jsonObject.insert("Size", Tool::vec3ToStr(size));
}

void SObj::load(QJsonObject jsonObject)
{
	setParamWidget();
	if (jsonObject.isEmpty()) // 用默认数据初始化
	{
		inflateParamWidget(DefaultPosition, DefaultRotation, DefaultSize);
	}
	else // 用载入数据初始化
	{
		inflateParamWidget(Tool::vec3FromStr(jsonObject.value("Position").toString()),
			Tool::vec3FromStr(jsonObject.value("Rotation").toString()),
			Tool::vec3FromStr(jsonObject.value("Size").toString()));
	}
	connectParamWidget();
	updateWinObjRatio();
}

void SObj::click(QPoint mousePos)
{
	// 基类目前不需要响应鼠标事件
}

void SObj::rightClick(QPoint mousePos)
{
	// 基类目前不需要响应鼠标事件
}

void SObj::getSnapshot(ISObj *snapshot)
{
	SObj *castedSnapshot = dynamic_cast<SObj *>(snapshot);
	castedSnapshot->position = position;
	castedSnapshot->rotation = rotation;
	castedSnapshot->size = size;
	castedSnapshot->centerRotation = centerRotation;
	castedSnapshot->centerSize = centerSize;
	castedSnapshot->baseCenterSize = baseCenterSize;
}

void SObj::setSnapshot(ISObj *snapshot)
{
	isRecord = false;
	SObj *castedSnapshot = dynamic_cast<SObj *>(snapshot);
	centerRotation = castedSnapshot->centerRotation;
	centerSize = castedSnapshot->centerSize;
	baseCenterSize = castedSnapshot->baseCenterSize;
	disconnectParamWidget();
	inflateParamWidget(castedSnapshot->position, castedSnapshot->rotation, castedSnapshot->size);
	connectParamWidget();
}

void SObj::updateWinObjRatio()
{
	winObjRatio = Tool::calcWinObjRatio(position,
		glm::ivec4(0, 0, sceneSize->x, sceneSize->y), 
		camera->getViewMatrix(), camera->getProjectionMatrix());
}

void SObj::select()
{
	selected = true;
	showAABB = true;
}

void SObj::unSelect()
{
	selected = false;
	showAABB = false;
}

bool SObj::isSelected()
{
	return selected;
}

void SObj::setShowAABB(bool flag)
{
	showAABB = flag;
}

void SObj::setVisible(bool flag)
{
	isVisible = flag;
}

glm::vec3 SObj::getPosition() const
{
	return position;
}

glm::vec3 SObj::getRotation() const
{
	return rotation;
}

glm::ivec2 SObj::getXY() const
{
	return Tool::calcWinPos(position,
		glm::ivec4(0, 0, sceneSize->x, sceneSize->y),
		camera->getViewMatrix(), camera->getProjectionMatrix());
}

QStringList SObj::getIDs() const
{
	return ids;
}

QString SObj::getJoinID() const
{
	return ids.join('-') + "-" + QString::number(id);
}

int SObj::getID() const
{
	return id;
}

glm::mat4 SObj::getModelMatrix()
{
	return m;
}

glm::vec3 SObj::getAABB()
{
	return aabb;
}

QRect SObj::getRect()
{
	glm::vec3 aabbVertices[8];
	getAABBVertices(aabbVertices);

	int left = std::numeric_limits<int>::max();
	int right = std::numeric_limits<int>::lowest();
	int top = std::numeric_limits<int>::max();
	int bottom = std::numeric_limits<int>::lowest();

	glm::ivec2 aabbWinPoses[8];
	for (int i = 0; i < 8; i++)
	{
		aabbWinPoses[i] = Tool::calcWinPos(aabbVertices[i],
			glm::ivec4(0, 0, sceneSize->x, sceneSize->y),
			camera->getViewMatrix(), camera->getProjectionMatrix());
		if (aabbWinPoses[i].x < left)
		{
			left = aabbWinPoses[i].x;
		}
		else if (aabbWinPoses[i].x > right)
		{
			right = aabbWinPoses[i].x;
		}
		if (aabbWinPoses[i].y < top)
		{
			top = aabbWinPoses[i].y;
		}
		else if (aabbWinPoses[i].y > bottom)
		{
			bottom = aabbWinPoses[i].y;
		}
	}

	return QRect(QPoint(left, top), QPoint(right, bottom));
}

void SObj::getAABBVertices(glm::vec3 aabbVertices[8])
{
	aabbVertices[0] = glm::vec3(rawM * glm::vec4(-aabb.x, -aabb.y, -aabb.z, 1.0f));
	aabbVertices[1] = glm::vec3(rawM * glm::vec4(-aabb.x, -aabb.y, aabb.z, 1.0f));
	aabbVertices[2] = glm::vec3(rawM * glm::vec4(aabb.x, -aabb.y, aabb.z, 1.0f));
	aabbVertices[3] = glm::vec3(rawM * glm::vec4(aabb.x, -aabb.y, -aabb.z, 1.0f));
	aabbVertices[4] = glm::vec3(rawM * glm::vec4(-aabb.x, aabb.y, -aabb.z, 1.0f));
	aabbVertices[5] = glm::vec3(rawM * glm::vec4(-aabb.x, aabb.y, aabb.z, 1.0f));
	aabbVertices[6] = glm::vec3(rawM * glm::vec4(aabb.x, aabb.y, aabb.z, 1.0f));
	aabbVertices[7] = glm::vec3(rawM * glm::vec4(aabb.x, aabb.y, -aabb.z, 1.0f));
}

glm::vec3 SObj::getAABBCenter(glm::vec3 aabbVertices[8])
{
	glm::vec3 aabbCenter = glm::vec3(0.0f);
	for (int i = 0; i < 8; i++)
	{
		aabbCenter += aabbVertices[i];
	}
	aabbCenter /= 8;

	return aabbCenter;
}

Dimension SObj::getDimension()
{
	return dimen;
}

void SObj::moveDZ(int dz)
{
	position.z += dz * MoveZSpeed;
	winObjRatio = Tool::calcWinObjRatio(position,
		glm::ivec4(0, 0, sceneSize->x, sceneSize->y),
		camera->getViewMatrix(), camera->getProjectionMatrix());
}

void SObj::moveDZ(float dz)
{
	position.z += dz;
}

void SObj::moveDXY(int dx, int dy)
{
	position.x += dx * winObjRatio.x;
	position.y -= dy * winObjRatio.y;
}

void SObj::moveXY(int x, int y)
{
	position.x = (x - sceneSize->x / 2) * winObjRatio.x;
	position.y = -(y - sceneSize->y / 2) * winObjRatio.y;
}

void SObj::scale(glm::vec3 ratio)
{
	if (centerTranformOn)
	{
		centerSize = baseCenterSize * ratio;
	}
	else
	{
		size = baseSize * ratio;
	}
}

void SObj::setBaseSize(glm::vec3 ratio)
{
	if (centerTranformOn)
	{
		baseCenterSize *= ratio;
	}
	else
	{
		baseSize *= ratio;
	}
}

void SObj::rotateDz(float angle)
{
	if (centerTranformOn)
	{
		centerRotation.z += angle;
	}
	else
	{
		rotation.z += angle;
	}
}

void SObj::rotateDxy(float angleX, float angleY)
{
	if (centerTranformOn)
	{
		centerRotation.x += angleX;
		centerRotation.y += angleY;
	}
	else
	{
		rotation.x += angleX;
		rotation.y += angleY;
	}
}

void SObj::updateTransform()
{
	if (centerTranformOn)
	{
		Tool::decompose(rawM, position, rotation, size);
		baseSize = size;
		centerRotation = glm::vec3(0.0f);
		centerSize = glm::vec3(1.0f);
		baseCenterSize = glm::vec3(1.0f);
	}
}

void SObj::setCenter(glm::vec3 center)
{
	this->center = center - position;
	centerTranformOn = !Tool::vec3Equal(center, position);
}

void SObj::setMoveDirection(MoveDirection moveDir, bool flag)
{
	switch (moveDir)
	{
	case Up:
		isUp = flag;
		break;
	case Down:
		isDown = flag;
		break;
	case Left:
		isLeft = flag;
		break;
	case Right:
		isRight = flag;
		break;
	}
}

void SObj::showParamWidget(QPoint mousePos)
{
	disconnectParamWidget();
	positionDoubleSpinBoxVec3->setVec3(position);
	rotationDoubleSpinBoxVec3->setVec3(Tool::reduceRotation(rotation));
	sizeDoubleSpinBoxVec3->setVec3(size);
	connectParamWidget();

	paramWidget->move(mousePos - QPoint(20, 10));
	paramWidget->show();
	paramWidget->activateWindow();
}

void SObj::closeParamWidget()
{
	if (!paramWidget->isHidden())
	{
		paramWidget->hide();
	}
}

bool SObj::isShowParamWidget()
{
	return !paramWidget->isHidden();
}

void SObj::init()
{
	aabb = glm::vec3(1.0f);
	centerRotation = glm::vec3(0.0f);
	centerSize = glm::vec3(1.0f);
	baseCenterSize = glm::vec3(1.0f);
	centerTranformOn = false;
	selected = false;
	showAABB = false;
	isUp = isDown = isLeft = isRight = false;
	isVisible = true;
	isRecord = false;

	id = sObjID++;
	colorID = getColorID();
}

glm::vec3 SObj::getColorID()
{
	int r = (id & 0x000000FF) >> 0;
	int g = (id & 0x0000FF00) >> 8;
	int b = (id & 0x00FF0000) >> 16;

	return glm::vec3(r / 255.0, g / 255.0, b / 255.0);
}

void SObj::setParamWidget()
{
	paramWidget = new ParamWidget(ids.last());

	paramWidget->addVec3("Position",
		-100.0f, 100.0f, 0.1f,
		positionDoubleSpinBoxVec3); // position
	paramWidget->addVec3("Rotation",
		0.0f, 360.0f, 1.0f,
		rotationDoubleSpinBoxVec3); // rotation
	paramWidget->addVec3("Size",
		0.1f, 100.0f, 0.1f,
		sizeDoubleSpinBoxVec3); // size

	// seperate line
	paramWidget->addSeperateLine();
}

void SObj::inflateParamWidget(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _size)
{
	positionDoubleSpinBoxVec3->setVec3(_position);
	rotationDoubleSpinBoxVec3->setVec3(_rotation);
	sizeDoubleSpinBoxVec3->setVec3(_size);

	positionDoubleSpinBoxVec3Changed(_position);
	rotationDoubleSpinBoxVec3Changed(_rotation);
	sizeDoubleSpinBoxVec3Changed(_size);
}

void SObj::connectParamWidget()
{
	connect(positionDoubleSpinBoxVec3, SIGNAL(vec3ValueChanged(glm::vec3)), this, SLOT(positionDoubleSpinBoxVec3Changed(glm::vec3)));
	connect(rotationDoubleSpinBoxVec3, SIGNAL(vec3ValueChanged(glm::vec3)), this, SLOT(rotationDoubleSpinBoxVec3Changed(glm::vec3)));
	connect(sizeDoubleSpinBoxVec3, SIGNAL(vec3ValueChanged(glm::vec3)), this, SLOT(sizeDoubleSpinBoxVec3Changed(glm::vec3)));
}

void SObj::disconnectParamWidget()
{
	disconnect(positionDoubleSpinBoxVec3, SIGNAL(vec3ValueChanged(glm::vec3)), this, SLOT(positionDoubleSpinBoxVec3Changed(glm::vec3)));
	disconnect(rotationDoubleSpinBoxVec3, SIGNAL(vec3ValueChanged(glm::vec3)), this, SLOT(rotationDoubleSpinBoxVec3Changed(glm::vec3)));
	disconnect(sizeDoubleSpinBoxVec3, SIGNAL(vec3ValueChanged(glm::vec3)), this, SLOT(sizeDoubleSpinBoxVec3Changed(glm::vec3)));
}

void SObj::recordLastSnapshot()
{
	if (isRecord)
	{
		lastSnapshot = dynamic_cast<SObj *>(SObjFactory::null(this));
		getSnapshot(lastSnapshot);
	}
}

void SObj::recordCurSnapshot()
{
	if (isRecord)
	{
		curSnapshot = dynamic_cast<SObj *>(SObjFactory::null(this));
		getSnapshot(curSnapshot);
		History::getInstance()->execute(new StateCommand(
			std::make_pair(this, lastSnapshot), std::make_pair(this, curSnapshot)));
	}
}

void SObj::positionDoubleSpinBoxVec3Changed(glm::vec3 val)
{
	recordLastSnapshot();
	position = val;
	recordCurSnapshot();
}

void SObj::rotationDoubleSpinBoxVec3Changed(glm::vec3 val)
{
	recordLastSnapshot();
	rotation = val;
	recordCurSnapshot();
}

void SObj::sizeDoubleSpinBoxVec3Changed(glm::vec3 val)
{
	recordLastSnapshot();
	size = val;
	baseSize = val;
	recordCurSnapshot();
}