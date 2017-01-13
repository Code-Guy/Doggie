#include "demosobj.h"
#include "resourcemanager.h"
#include "primitivemanager.h"
#include "sobjfactory.h"
#include "tool.h"

// 默认属性数据
glm::vec3 DefaultColor = glm::vec3(1.0f, 1.0f, 0.0f);

DemoSObj::DemoSObj()
{

}

DemoSObj::DemoSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids) :
	SObj(camera, sceneSize, ids, Dimension::TwoD)
{
	init();
}

DemoSObj::DemoSObj(DemoSObj *other) : SObj(other)
{
	init();
	setParamWidget();
	inflateParamWidget(other->color);
	connectParamWidget();
	isRecord = true;
}

DemoSObj::~DemoSObj()
{
	delete colorLabel;
}

void DemoSObj::destroy()
{
	delete this;
}

void DemoSObj::logic(float dt)
{
	SObj::logic(dt);
}

void DemoSObj::render(bool blendOn, bool pickOn)
{
	SObj::render();

	if (isVisible)
	{
		// 设置shader
		PointShader *pointShader = ResourceManager::getInstance()->getPointShader();
		pointShader->enable();
		pointShader->setMVP(camera->getViewProjectionMatrix() * m);
		pointShader->setColor(color);
		pointShader->setColorID(colorID);
		pointShader->setBlendOn(blendOn);
		pointShader->setPickOn(pickOn);

		// 设置数据
		PrimitiveManager::getInstance()->renderQuad();
	}
}

void DemoSObj::save(QJsonObject &jsonObject)
{
	SObj::save(jsonObject);

	jsonObject.insert("Color", Tool::vec3ToStr(color));
}

void DemoSObj::load(QJsonObject jsonObject)
{
	SObj::load(jsonObject);

	setParamWidget();
	if (jsonObject.isEmpty()) // 用载入数据初始化
	{
		inflateParamWidget(DefaultColor);
	}
	else
	{
		inflateParamWidget(Tool::vec3FromStr(jsonObject.value("Color").toString()));
	}
	connectParamWidget();
	isRecord = true;
}

void DemoSObj::getSnapshot(ISObj *snapshot)
{
	SObj::getSnapshot(snapshot);
	dynamic_cast<DemoSObj *>(snapshot)->color = color;
}

void DemoSObj::setSnapshot(ISObj *snapshot)
{
	SObj::setSnapshot(snapshot);
	disconnectParamWidget();
	inflateParamWidget(dynamic_cast<DemoSObj *>(snapshot)->color);
	connectParamWidget();
	isRecord = true;
}

void DemoSObj::init()
{
	aabb = glm::vec3(1.0f, 1.0f, FLOAT_EQUAL_EPSILON);
}

void DemoSObj::setParamWidget()
{
	paramWidget->addColor("Color", colorLabel);
}

void DemoSObj::inflateParamWidget(glm::vec3 _color)
{
	colorLabel->setColor(_color);
	colorChanged(_color);
}

void DemoSObj::connectParamWidget()
{
	connect(colorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(colorChanged(glm::vec3)));
}

void DemoSObj::disconnectParamWidget()
{
	disconnect(colorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(colorChanged(glm::vec3)));
}

void DemoSObj::colorChanged(glm::vec3 color)
{
	recordLastSnapshot();
	this->color = color;
	recordCurSnapshot();
}

ISObj *construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject)
{
	ISObj *iSObj = new DemoSObj(camera, sceneSize, ids);
	iSObj->load(jsonObject);

	return iSObj;
}

ISObj *copyer(ISObj *other)
{
	return new DemoSObj(dynamic_cast<DemoSObj *>(other));
}

ISObj *nuller()
{
	return new DemoSObj();
}