#include "shapesobj.h"
#include "resourcemanager.h"
#include "primitivemanager.h"
#include "sobjfactory.h"
#include "tool.h"

// 默认初始化数据
const glm::vec3 DefaultColor = glm::vec3(0.0f, 0.0f, 0.0f);

ShapeSObj::ShapeSObj()
{

}

ShapeSObj::ShapeSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids) : 
	SObj(camera, sceneSize, ids, Dimension::TwoD)
{
	init();
}

ShapeSObj::ShapeSObj(ShapeSObj *other) : SObj(other)
{
	init();
	setParamWidget();
	inflateParamWidget(other->color);
	connectParamWidget();
	isRecord = true;
}

ShapeSObj::~ShapeSObj()
{
	delete colorLabel;
}

void ShapeSObj::destroy()
{
	delete this;
}

void ShapeSObj::logic(float dt)
{
	SObj::logic(dt);
	m *= glm::scale(aabb);
}

void ShapeSObj::render(bool blendOn, bool pickOn)
{
	SObj::render();

	if (isVisible)
	{
		ResourceManager::getInstance()->getTextureMaskShader()->enable();
		ResourceManager::getInstance()->getTextureMaskShader()->setPickOn(pickOn);
		ResourceManager::getInstance()->getTextureMaskShader()->setBlendOn(blendOn);
		ResourceManager::getInstance()->getTextureMaskShader()->setColorID(colorID);
		ResourceManager::getInstance()->getTextureMaskShader()->setMVP(
			camera->getViewProjectionMatrix() * m);
		ResourceManager::getInstance()->getTextureMaskShader()->setColor(color);

		texture->bind();

		PrimitiveManager::getInstance()->renderQuad();
	}
}

void ShapeSObj::save(QJsonObject &jsonObject)
{
	SObj::save(jsonObject);

	jsonObject.insert("Color", Tool::vec3ToStr(color));
}

void ShapeSObj::load(QJsonObject jsonObject)
{
	SObj::load(jsonObject);

	setParamWidget();
	if (jsonObject.isEmpty()) // 用默认数据初始化
	{
		inflateParamWidget(DefaultColor);
	}
	else // 用载入数据初始化
	{
		inflateParamWidget(Tool::vec3FromStr(jsonObject.value("Color").toString()));
	}
	connectParamWidget();
	isRecord = true;
}

void ShapeSObj::getSnapshot(ISObj *snapshot)
{
	SObj::getSnapshot(snapshot);
	dynamic_cast<ShapeSObj *>(snapshot)->color = color;
}

void ShapeSObj::setSnapshot(ISObj *snapshot)
{
	SObj::setSnapshot(snapshot);
	disconnectParamWidget();
	inflateParamWidget(dynamic_cast<ShapeSObj *>(snapshot)->color);
	connectParamWidget();
	isRecord = true;
}

void ShapeSObj::init()
{
	texture = ResourceManager::getInstance()->getTexture(
		"Resources/SObjs/" + Tool::getLayerDir(ids) + "Res/" + ids.last().toLower() + ".png");
	aabb = Tool::calcTexAABB(texture->getWidth(), texture->getHeight());
}

void ShapeSObj::setParamWidget()
{
	paramWidget->addColor("Color", colorLabel);
}

void ShapeSObj::inflateParamWidget(glm::vec3 _color)
{
	colorLabel->setColor(_color);
	colorChanged(_color);
}

void ShapeSObj::connectParamWidget()
{
	connect(colorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(colorChanged(glm::vec3)));
}

void ShapeSObj::disconnectParamWidget()
{
	disconnect(colorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(colorChanged(glm::vec3)));
}

void ShapeSObj::colorChanged(glm::vec3 color)
{
	recordLastSnapshot();
	this->color = color;
	recordCurSnapshot();
}

ISObj *construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject)
{
	ISObj *iSObj = new ShapeSObj(camera, sceneSize, ids);
	iSObj->load(jsonObject);

	return iSObj;
}

ISObj *copyer(ISObj *other)
{
	return new ShapeSObj(dynamic_cast<ShapeSObj *>(other));
}

ISObj *nuller()
{
	return new ShapeSObj();
}