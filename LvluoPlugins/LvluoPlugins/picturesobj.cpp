#include "picturesobj.h"
#include "resourcemanager.h"
#include "primitivemanager.h"
#include "sobjfactory.h"
#include "tool.h"

PictureSObj::PictureSObj()
{
	
}

PictureSObj::PictureSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids) :
	SObj(camera, sceneSize, ids, Dimension::TwoD)
{
	init();
}

PictureSObj::PictureSObj(PictureSObj *other) : SObj(other)
{
	init();
	setParamWidget();
	inflateParamWidget(other->fileName);
	connectParamWidget();
	isRecord = true;
}

PictureSObj::~PictureSObj()
{
	delete fileWidget;
}

void PictureSObj::destroy()
{
	delete this;
}

void PictureSObj::logic(float dt)
{
	SObj::logic(dt);
	m *= glm::scale(aabb);
}

void PictureSObj::render(bool blendOn, bool pickOn)
{
	SObj::render();

	if (isVisible)
	{
		ResourceManager::getInstance()->getTextureShader()->enable();
		ResourceManager::getInstance()->getTextureShader()->setPickOn(pickOn);
		ResourceManager::getInstance()->getTextureShader()->setBlendOn(blendOn);
		ResourceManager::getInstance()->getTextureShader()->setColorID(colorID);
		ResourceManager::getInstance()->getTextureShader()->setMVP(
			camera->getViewProjectionMatrix() * m);

		if (texture)
		{
			texture->bind();
		}
		else
		{
			aabb = Tool::calcTexAABB(defaultTex->getWidth(), defaultTex->getHeight());
			defaultTex->bind();
		}

		PrimitiveManager::getInstance()->renderQuad();
	}
}

void PictureSObj::save(QJsonObject &jsonObject)
{
	SObj::save(jsonObject);

	jsonObject.insert("File Name", fileName);
}

void PictureSObj::load(QJsonObject jsonObject)
{
	SObj::load(jsonObject);

	setParamWidget();
	if (!jsonObject.isEmpty()) // 用载入数据初始化
	{
		inflateParamWidget(jsonObject.value("File Name").toString());
	}
	connectParamWidget();
	isRecord = true;
}

void PictureSObj::getSnapshot(ISObj *snapshot)
{
	SObj::getSnapshot(snapshot);
	dynamic_cast<PictureSObj *>(snapshot)->fileName = fileName;
}

void PictureSObj::setSnapshot(ISObj *snapshot)
{
	SObj::setSnapshot(snapshot);
	disconnectParamWidget();
	inflateParamWidget(dynamic_cast<PictureSObj *>(snapshot)->fileName);
	connectParamWidget();
	isRecord = true;
}

void PictureSObj::init()
{
	texture = nullptr;
	defaultTex = ResourceManager::getInstance()->getTexture(
		"Resources/SObjs/" + Tool::getLayerDir(ids) + "Res/default.jpg");
	aabb = Tool::calcTexAABB(defaultTex->getWidth(), defaultTex->getHeight());
}

void PictureSObj::setParamWidget()
{
	paramWidget->addFile(ids.last(),
		"Pictures (*.png *.jpg *.bmp)",
		fileWidget);
}

void PictureSObj::inflateParamWidget(QString _fileName)
{
	fileWidget->setFileName(_fileName);
	fileNameChanged(_fileName);
}

void PictureSObj::connectParamWidget()
{
	connect(fileWidget, SIGNAL(fileNameChanged(QString)), this, SLOT(fileNameChanged(QString)));
}

void PictureSObj::disconnectParamWidget()
{
	disconnect(fileWidget, SIGNAL(fileNameChanged(QString)), this, SLOT(fileNameChanged(QString)));
}

void PictureSObj::setTexture(QString fileName)
{
	texture = nullptr;
	Texture *newTexture = nullptr;
	if (!fileName.isEmpty())
	{
		newTexture = ResourceManager::getInstance()->getTexture(fileName);
		aabb = Tool::calcTexAABB(newTexture->getWidth(), newTexture->getHeight());
	}
	
	texture = newTexture;
}

void PictureSObj::fileNameChanged(QString fileName)
{
	recordLastSnapshot();
	this->fileName = fileName;
	setTexture(fileName);
	recordCurSnapshot();
}

ISObj *construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject)
{
	ISObj *iSObj = new PictureSObj(camera, sceneSize, ids);
	iSObj->load(jsonObject);

	return iSObj;
}

ISObj *copyer(ISObj *other)
{
	return new PictureSObj(dynamic_cast<PictureSObj *>(other));
}

ISObj *nuller()
{
	return new PictureSObj();
}