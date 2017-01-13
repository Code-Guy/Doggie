#include "modelsobj.h"
#include "resourcemanager.h"
#include "primitivemanager.h"
#include "sobjfactory.h"
#include "tool.h"

// 默认初始化数据
const glm::vec3 DefaultDLDirection = glm::vec3(-1.0f, -1.0f, -1.0f);
const glm::vec3 DefaultDLAmbientColor = glm::vec3(0.3f, 0.3f, 0.3f);
const glm::vec3 DefaultDLDiffuseColor = glm::vec3(0.7f, 0.7f, 0.7f);
const glm::vec3 DefaultDLSpecularColor = glm::vec3(0.8f, 0.8f, 0.8f);

ModelSObj::ModelSObj()
{

}

ModelSObj::ModelSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids) : 
	SObj(camera, sceneSize, ids, Dimension::ThreeD)
{
	init();
}

ModelSObj::ModelSObj(ModelSObj *other) : SObj(other)
{
	init();
	setParamWidget();
	inflateParamWidget(other->dl.direction, other->dl.ambient, other->dl.diffuse, other->dl.specular);
	connectParamWidget();
	isRecord = true;
}

ModelSObj::~ModelSObj()
{
	delete dlDirectionDoubleSpinBoxVec3;
	delete dlAmbientColorLabel;
	delete dlDiffuseColorLabel;
	delete dlSpecularColorLabel;
}

void ModelSObj::destroy()
{
	delete this;
}

void ModelSObj::logic(float dt)
{
	SObj::logic(dt);
}

void ModelSObj::render(bool blendOn, bool pickOn)
{
	SObj::render();

	if (isVisible)
	{
		ResourceManager::getInstance()->getModelShader()->enable();
		ResourceManager::getInstance()->getModelShader()->setPickOn(pickOn);
		ResourceManager::getInstance()->getModelShader()->setBlendOn(blendOn);
		ResourceManager::getInstance()->getModelShader()->setColorID(colorID);
		ResourceManager::getInstance()->getModelShader()->setModel(m);
		ResourceManager::getInstance()->getModelShader()->setViewProjection(
			camera->getViewProjectionMatrix());
		ResourceManager::getInstance()->getModelShader()->setCameraPosition(camera->getPosition());
		ResourceManager::getInstance()->getModelShader()->setDirectionalLight(dl);
		ResourceManager::getInstance()->getModelShader()->setPointLights(pls);
		ResourceManager::getInstance()->getModelShader()->setSpotLight(sl);

		model->render(ResourceManager::getInstance()->getModelShader());
	}
}

void ModelSObj::save(QJsonObject &jsonObject)
{
	SObj::save(jsonObject);

	jsonObject.insert("Direction", Tool::vec3ToStr(dl.direction));
	jsonObject.insert("Ambient Color", Tool::vec3ToStr(dl.ambient));
	jsonObject.insert("Diffuse Color", Tool::vec3ToStr(dl.diffuse));
	jsonObject.insert("Specular Color", Tool::vec3ToStr(dl.specular));
}

void ModelSObj::load(QJsonObject jsonObject)
{
	SObj::load(jsonObject);

	setParamWidget();
	if (jsonObject.isEmpty()) // 用默认数据初始化
	{
		inflateParamWidget(DefaultDLDirection, DefaultDLAmbientColor, DefaultDLDiffuseColor, DefaultDLSpecularColor);
	}
	else // 用载入数据初始化
	{
		inflateParamWidget(Tool::vec3FromStr(jsonObject.value("Direction").toString()),
			Tool::vec3FromStr(jsonObject.value("Ambient Color").toString()),
			Tool::vec3FromStr(jsonObject.value("Diffuse Color").toString()),
			Tool::vec3FromStr(jsonObject.value("Specular Color").toString()));
	}
	connectParamWidget();
	isRecord = true;
}

void ModelSObj::getSnapshot(ISObj *snapshot)
{
	SObj::getSnapshot(snapshot);
	dynamic_cast<ModelSObj *>(snapshot)->dl = dl;
}

void ModelSObj::setSnapshot(ISObj *snapshot)
{
	SObj::setSnapshot(snapshot);
	disconnectParamWidget();
	ModelSObj *castedSnapshot = dynamic_cast<ModelSObj *>(snapshot);
	inflateParamWidget(castedSnapshot->dl.direction,
		castedSnapshot->dl.ambient,
		castedSnapshot->dl.diffuse,
		castedSnapshot->dl.specular);
	connectParamWidget();
	isRecord = true;
}

void ModelSObj::init()
{
	model = ResourceManager::getInstance()->getModel("Resources/SObjs/" + Tool::getLayerDir(ids) + "Res/Model/" + ids.last().toLower() + ".obj");
	aabb = model->getAABB();

	for (int i = 0; i < PointLightNum; i++)
	{
		pls[i].ambient = glm::vec3(0.0f);
		pls[i].diffuse = glm::vec3(0.0f);
		pls[i].specular = glm::vec3(0.0f);
		pls[i].constant = 1.0f;
		pls[i].linear = 0.09f;
		pls[i].quadratic = 0.0032f;
	}

	sl.ambient = glm::vec3(0.0f);
	sl.diffuse = glm::vec3(0.0f);
	sl.specular = glm::vec3(0.0f);
	sl.innerCutoff = glm::cos(glm::radians(12.5f));
	sl.outerCutoff = glm::cos(glm::radians(17.5f));
	sl.constant = 1.0f;
	sl.linear = 0.09f;
	sl.quadratic = 0.0032f;
}

void ModelSObj::setParamWidget()
{
	paramWidget->addVec3("Direction",
		-1.0f, 1.0f, 0.01f,
		dlDirectionDoubleSpinBoxVec3);
	paramWidget->addColor("Ambient Color", dlAmbientColorLabel);
	paramWidget->addColor("Diffuse Color", dlDiffuseColorLabel);
	paramWidget->addColor("Specular Color", dlSpecularColorLabel);
}

void ModelSObj::inflateParamWidget(glm::vec3 _dlDirection, glm::vec3 _dlAmbient, glm::vec3 _dlDiffuse, glm::vec3 _dlSpecular)
{
	dlDirectionDoubleSpinBoxVec3->setVec3(_dlDirection);
	dlAmbientColorLabel->setColor(_dlAmbient);
	dlDiffuseColorLabel->setColor(_dlDiffuse);
	dlSpecularColorLabel->setColor(_dlSpecular);

	dlDirectionDoubleSpinBoxVec3Changed(_dlDirection);
	dlAmbientColorChanged(_dlAmbient);
	dlDiffuseColorChanged(_dlDiffuse);
	dlSpecularColorChanged(_dlSpecular);
}

void ModelSObj::connectParamWidget()
{
	connect(dlDirectionDoubleSpinBoxVec3, SIGNAL(vec3ValueChanged(glm::vec3)), this, SLOT(dlDirectionDoubleSpinBoxVec3Changed(glm::vec3)));
	connect(dlAmbientColorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(dlAmbientColorChanged(glm::vec3)));
	connect(dlDiffuseColorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(dlDiffuseColorChanged(glm::vec3)));
	connect(dlSpecularColorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(dlSpecularColorChanged(glm::vec3)));
}

void ModelSObj::disconnectParamWidget()
{
	disconnect(dlDirectionDoubleSpinBoxVec3, SIGNAL(vec3ValueChanged(glm::vec3)), this, SLOT(dlDirectionDoubleSpinBoxVec3Changed(glm::vec3)));
	disconnect(dlAmbientColorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(dlAmbientColorChanged(glm::vec3)));
	disconnect(dlDiffuseColorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(dlDiffuseColorChanged(glm::vec3)));
	disconnect(dlSpecularColorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(dlSpecularColorChanged(glm::vec3)));
}

void ModelSObj::dlDirectionDoubleSpinBoxVec3Changed(glm::vec3 val)
{
	recordLastSnapshot();
	dl.direction = val;
	recordCurSnapshot();
}

void ModelSObj::dlAmbientColorChanged(glm::vec3 color)
{
	recordLastSnapshot();
	dl.ambient = color;
	recordCurSnapshot();
}

void ModelSObj::dlDiffuseColorChanged(glm::vec3 color)
{
	recordLastSnapshot();
	dl.diffuse = color;
	recordCurSnapshot();
}

void ModelSObj::dlSpecularColorChanged(glm::vec3 color)
{
	recordLastSnapshot();
	dl.specular = color;
	recordCurSnapshot();
}

ISObj *construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject)
{
	ISObj *iSObj = new ModelSObj(camera, sceneSize, ids);
	iSObj->load(jsonObject);

	return iSObj;
}

ISObj *copyer(ISObj *other)
{
	return new ModelSObj(dynamic_cast<ModelSObj *>(other));
}

ISObj *nuller()
{
	return new ModelSObj();
}