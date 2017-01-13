#include "simplefacesobj.h"
#include "resourcemanager.h"
#include "primitivemanager.h"
#include "sobjfactory.h"
#include "tool.h"

// 默认属性参数
const float DefaultBgPointLifeTime = 3.0f;
const glm::vec3 DefaultFaceEllipse = glm::vec3(0.8f, 0.4f, 0.4f);
const int DefaultPointSize = 4;
const glm::vec3 DefaultPointColor = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 DefaultBgPointMoveDir = glm::vec3(-1.0f, -1.0f, 0.0f);
const int DefaultBgPointSpawnSpeed = 200;
const float DefaultBgPointMoveSpeed = 0.02f;
const bool DefaultBgPointThroughFace = false;
const int DefaultFacePointSpawnSpeed = 50;
const float DefaultFacePointLifeTime = 1.0f;

SimpleFaceSObj::SimpleFaceSObj()
{

}

SimpleFaceSObj::SimpleFaceSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids) : 
	SObj(camera, sceneSize, ids, Dimension::ThreeD)
{
	init();
}

SimpleFaceSObj::SimpleFaceSObj(SimpleFaceSObj *other) : SObj(other)
{
	init();
	setParamWidget();
	inflateParamWidget(other->pointSize, other->pointColor,
		other->bgPointMoveDir, other->bgPointSpawnSpeed, other->bgPointMoveSpeed, other->bgPointThroughFace,
		other->facePointSpawnSpeed, other->facePointLifeTime);
	connectParamWidget();
	isRecord = true;
}

SimpleFaceSObj::~SimpleFaceSObj()
{
	
}

void SimpleFaceSObj::destroy()
{
	delete this;
}

void SimpleFaceSObj::logic(float dt)
{
	SObj::logic(dt);

	if (isPlay)
	{
		emitBgPoints(dt);
		updateBgPoints(dt);
		emitFacePoints(dt);
		updateFacePoints(dt);
	}
}

void SimpleFaceSObj::render(bool blendOn, bool pickOn)
{
	SObj::render();

	if (isVisible)
	{
		ResourceManager::getInstance()->getPointShader()->enable();
		ResourceManager::getInstance()->getPointShader()->setMVP(camera->getViewProjectionMatrix() * m);
		ResourceManager::getInstance()->getPointShader()->setColor(pointColor);
		ResourceManager::getInstance()->getPointShader()->setColorID(colorID);
		ResourceManager::getInstance()->getPointShader()->setBlendOn(blendOn);
		ResourceManager::getInstance()->getPointShader()->setPickOn(pickOn);

		glPointSize(pointSize);
		PrimitiveManager::getInstance()->renderPoints(bgPointPositions, GL_POINTS);
		PrimitiveManager::getInstance()->renderPoints(facePointPositions, GL_POINTS);
		glPointSize(1.0f);
	}
}

void SimpleFaceSObj::click(QPoint mousePos)
{
	SObj::click(mousePos);

	isPlay = !isPlay;
}

void SimpleFaceSObj::rightClick(QPoint mousePos)
{
	SObj::rightClick(mousePos);

	showParamWidget(mousePos);
}

void SimpleFaceSObj::save(QJsonObject &jsonObject)
{
	SObj::save(jsonObject);
	jsonObject.insert("Point Size", pointSize);
	jsonObject.insert("Point Color", Tool::vec3ToStr(pointColor));
	jsonObject.insert("Background Point Move Direction", Tool::vec3ToStr(bgPointMoveDir));
	jsonObject.insert("Background Point Move Speed", bgPointMoveSpeed);
	jsonObject.insert("Background Point Spawn Speed", bgPointSpawnSpeed);
	jsonObject.insert("Background Point Through Face On", bgPointThroughFace);
	jsonObject.insert("Face Point Spawn Speed", facePointSpawnSpeed);
	jsonObject.insert("Face Point Life Time", facePointLifeTime);
}

void SimpleFaceSObj::load(QJsonObject jsonObject)
{
	SObj::load(jsonObject);

	setParamWidget();
	if (jsonObject.isEmpty()) 
	{
		inflateParamWidget(DefaultPointSize, DefaultPointColor,
			DefaultBgPointMoveDir, DefaultBgPointSpawnSpeed, DefaultBgPointMoveSpeed, DefaultBgPointThroughFace,
			DefaultFacePointSpawnSpeed, DefaultFacePointLifeTime); // 用默认数据初始化
	}
	else
	{
		inflateParamWidget(jsonObject.value("Point Size").toInt(),
			Tool::vec3FromStr(jsonObject.value("Point Color").toString()),
			Tool::vec3FromStr(jsonObject.value("Background Point Move Direction").toString()),
			jsonObject.value("Background Point Spawn Speed").toInt(),
			jsonObject.value("Background Point Move Speed").toDouble(),
			jsonObject.value("Background Point Through Face On").toBool(),
			jsonObject.value("Face Point Spawn Speed").toInt(),
			jsonObject.value("Face Point Life Time").toDouble()); // 用载入数据初始化
	}
	connectParamWidget();
	isRecord = true;
}

void SimpleFaceSObj::getSnapshot(ISObj *snapshot)
{
	SObj::getSnapshot(snapshot);
	SimpleFaceSObj *castedSnapshot = dynamic_cast<SimpleFaceSObj *>(snapshot);
	castedSnapshot->pointSize = pointSize;
	castedSnapshot->pointColor = pointColor;
	castedSnapshot->bgPointMoveDir = bgPointMoveDir;
	castedSnapshot->bgPointSpawnSpeed = bgPointSpawnSpeed;
	castedSnapshot->bgPointMoveSpeed = bgPointMoveSpeed;
	castedSnapshot->bgPointThroughFace = bgPointThroughFace;
	castedSnapshot->facePointSpawnSpeed = facePointSpawnSpeed;
	castedSnapshot->facePointLifeTime = facePointLifeTime;
}

void SimpleFaceSObj::setSnapshot(ISObj *snapshot)
{
	SObj::setSnapshot(snapshot);
	SimpleFaceSObj *castedSnapshot = dynamic_cast<SimpleFaceSObj *>(snapshot);
	disconnectParamWidget();
	inflateParamWidget(castedSnapshot->pointSize, castedSnapshot->pointColor,
		castedSnapshot->bgPointMoveDir, castedSnapshot->bgPointSpawnSpeed, castedSnapshot->bgPointMoveSpeed, castedSnapshot->bgPointThroughFace,
		castedSnapshot->facePointSpawnSpeed, castedSnapshot->facePointLifeTime);
	connectParamWidget();
	isRecord = true;
}

void SimpleFaceSObj::init()
{
	bgPointLifeTime = DefaultBgPointLifeTime;
	faceEllipse = DefaultFaceEllipse;
	isPlay = true;
}

void SimpleFaceSObj::setParamWidget()
{
	paramWidget->addInt("Point Size", 1, 20, 1, pointSizeSpinBox);
	paramWidget->addColor("Point Color", pointColorLabel);
	paramWidget->addVec3("Background Point Move Direction", -1.0f, 1.0f, 0.1f, bgPointMoveDirDoubleSpinBoxVec3);
	paramWidget->addFloat("Background Point Move Speed", 0.01f, 1.0f, 0.1f, bgPointMoveSpeedDoubleSpinBox);
	paramWidget->addInt("Background Point Spawn Speed", 1, 500, 1, bgPointSpawnSpeedSpinBox);
	paramWidget->addBool("Background Point Through Face On", bgPointThroughFaceCheckBox);
	paramWidget->addInt("Face Point Spawn Speed", 1, 500, 1, facePointSpawnSpeedSpinBox);
	paramWidget->addFloat("Face Point Life Time", 0.1f, 5.0f, 0.1f, facePointLifeTimeDoubleSpinBox);
}

void SimpleFaceSObj::inflateParamWidget(int _pointSize, glm::vec3 _pointColor,
	glm::vec3 _bgPointMoveDir, int _bgPointSpawnSpeed, float _bgPointMoveSpeed, bool _bgPointThroughFace,
	int _facePointSpawnSpeed, float _facePointLifeTime)
{
	pointSizeSpinBox->setValue(_pointSize);
	pointColorLabel->setColor(_pointColor);
	bgPointMoveDirDoubleSpinBoxVec3->setVec3(glm::normalize(_bgPointMoveDir));
	bgPointSpawnSpeedSpinBox->setValue(_bgPointSpawnSpeed);
	bgPointMoveSpeedDoubleSpinBox->setValue(_bgPointMoveSpeed);
	bgPointThroughFaceCheckBox->setChecked(_bgPointThroughFace);
	facePointSpawnSpeedSpinBox->setValue(_facePointSpawnSpeed);
	facePointLifeTimeDoubleSpinBox->setValue(_facePointLifeTime);

	pointSizeSpinBoxValueChanged(_pointSize);
	pointColorLabelColorChanged(_pointColor);
	bgPointMoveDirDoubleSpinBoxVec3ValueChanged(_bgPointMoveDir);
	bgPointSpawnSpeedSpinBoxValueChanged(_bgPointSpawnSpeed);
	bgPointMoveSpeedDoubleSpinBoxValueChanged(_bgPointMoveSpeed);
	bgPointThroughFaceCheckBoxStateChanged(_bgPointThroughFace);
	facePointSpawnSpeedSpinBoxValueChanged(_facePointSpawnSpeed);
	facePointLifeTimeDoubleSpinBoxValueChanged(_facePointLifeTime);
}

void SimpleFaceSObj::connectParamWidget()
{
	connect(pointSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(pointSizeSpinBoxValueChanged(int)));
	connect(pointColorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(pointColorLabelColorChanged(glm::vec3)));
	connect(bgPointMoveDirDoubleSpinBoxVec3, SIGNAL(vec3ValueChanged(glm::vec3)), this, SLOT(bgPointMoveDirDoubleSpinBoxVec3ValueChanged(glm::vec3)));
	connect(bgPointSpawnSpeedSpinBox, SIGNAL(valueChanged(int)), this, SLOT(bgPointSpawnSpeedSpinBoxValueChanged(int)));
	connect(bgPointMoveSpeedDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(bgPointMoveSpeedDoubleSpinBoxValueChanged(double)));
	connect(bgPointThroughFaceCheckBox, SIGNAL(stateChanged(int)), this, SLOT(bgPointThroughFaceCheckBoxStateChanged(int)));
	connect(facePointSpawnSpeedSpinBox, SIGNAL(valueChanged(int)), this, SLOT(facePointSpawnSpeedSpinBoxValueChanged(int)));
	connect(facePointLifeTimeDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(facePointLifeTimeDoubleSpinBoxValueChanged(double)));
}

void SimpleFaceSObj::disconnectParamWidget()
{
	disconnect(pointSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(pointSizeSpinBoxValueChanged(int)));
	disconnect(pointColorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(pointColorLabelColorChanged(glm::vec3)));
	disconnect(bgPointMoveDirDoubleSpinBoxVec3, SIGNAL(vec3ValueChanged(glm::vec3)), this, SLOT(bgPointMoveDirDoubleSpinBoxVec3ValueChanged(glm::vec3)));
	disconnect(bgPointSpawnSpeedSpinBox, SIGNAL(valueChanged(int)), this, SLOT(bgPointSpawnSpeedSpinBoxValueChanged(int)));
	disconnect(bgPointMoveSpeedDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(bgPointMoveSpeedDoubleSpinBoxValueChanged(double)));
	disconnect(bgPointThroughFaceCheckBox, SIGNAL(stateChanged(int)), this, SLOT(bgPointThroughFaceCheckBoxStateChanged(int)));
	disconnect(facePointSpawnSpeedSpinBox, SIGNAL(valueChanged(int)), this, SLOT(facePointSpawnSpeedSpinBoxValueChanged(int)));
	disconnect(facePointLifeTimeDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(facePointLifeTimeDoubleSpinBoxValueChanged(double)));
}

void SimpleFaceSObj::emitBgPoints(float dt)
{
	static float bgPointEmitNum = 0;
	bgPointEmitNum += bgPointSpawnSpeed * dt;
	if (bgPointEmitNum > 1)
	{
		int curBgPointEmitNum = (int)bgPointEmitNum;
		bgPointEmitNum -= curBgPointEmitNum;
		for (int i=0; i<curBgPointEmitNum; i++)
		{
			glm::vec3 randomPlanePos = glm::vec3(randomRange(-1.5f, 1.5f), 0.0f, randomRange(-1.5f, 1.5f));
			randomPlanePos = glm::vec3(glm::translate(-bgPointMoveDir * 1.5f) * glm::eulerAngleXYZ(0.0f,
				std::atan2(bgPointMoveDir.z, std::sqrt(bgPointMoveDir.x * bgPointMoveDir.x
					+ bgPointMoveDir.y * bgPointMoveDir.y)),
				-std::atan2(bgPointMoveDir.x, bgPointMoveDir.y)) * 
				glm::vec4(randomPlanePos, 1.0f));
			bgPoints.push_back(Point(randomPlanePos));
		}
	}
}

void SimpleFaceSObj::updateBgPoints(float dt)
{
	bgPointPositions.clear();
	for (auto bgPointIter = bgPoints.begin(); bgPointIter != bgPoints.end(); )
	{
		bgPointIter->t += dt;
		bgPointIter->position += bgPointMoveDir * bgPointMoveSpeed;
		bgPointIter->visible = checkBgPointVisibility(*bgPointIter);
		if (bgPointIter->t > bgPointLifeTime)
		{
			bgPointIter = bgPoints.erase(bgPointIter);
		}
		else
		{
			if (bgPointIter->visible)
			{
				bgPointPositions.push_back(bgPointIter->position);
			}
			bgPointIter++;
		}
	}
}

void SimpleFaceSObj::emitFacePoints(float dt)
{
	static float facePointEmitNum = 0;
	facePointEmitNum += facePointSpawnSpeed * dt;
	if (facePointEmitNum > 1)
	{
		int curFacePointEmitNum = (int)facePointEmitNum;
		facePointEmitNum -= curFacePointEmitNum;
		for (int i = 0; i < curFacePointEmitNum; i++)
		{
			glm::vec3 randomEllipsePos = uniformEllipseSurfaceSample(faceEllipse);
			facePoints.push_back(Point(randomEllipsePos));
		}
	}
}

void SimpleFaceSObj::updateFacePoints(float dt)
{
	facePointPositions.clear();
	for (auto facePointIter = facePoints.begin(); facePointIter != facePoints.end(); )
	{
		facePointIter->t += dt;
		if (facePointIter->t > facePointLifeTime)
		{
			facePointIter = facePoints.erase(facePointIter);
		}
		else
		{
			facePointPositions.push_back(facePointIter->position);
			facePointIter++;
		}
	}
}

glm::vec3 SimpleFaceSObj::uniformEllipseSurfaceSample(glm::vec3 ellipse)
{
	float a = glm::radians(randomRange(-180.0f, 180.0f));
	float b = acos(2 * randomRange(0.0f, 1.0f) - 1);

	glm::vec3 position;
	position.x = sin(b) * cos(a);
	position.y = sin(b) * sin(a);
	position.z = cos(b);

	return position * ellipse;
}

float SimpleFaceSObj::randomRange(float lower, float upper)
{
	float r = (float)rand() / RAND_MAX;
	return r * (upper - lower) + lower;
}

bool SimpleFaceSObj::checkBgPointVisibility(Point &point)
{
	if (point.position.x <= -aabb.x || point.position.x >= aabb.x ||
		point.position.y <= -aabb.y || point.position.y >= aabb.y || 
		point.position.z <= -aabb.z || point.position.z >= aabb.z ||
		(!bgPointThroughFace && (point.position.x * point.position.x / (faceEllipse.x * faceEllipse.x) +
		 point.position.y * point.position.y / (faceEllipse.y * faceEllipse.y) +
		 point.position.z * point.position.z / (faceEllipse.z * faceEllipse.z)) < 1.0f))
	{
		return false;
	}
	return true;
}

void SimpleFaceSObj::pointSizeSpinBoxValueChanged(int val)
{
	recordLastSnapshot();
	pointSize = val;
	recordCurSnapshot();
}

void SimpleFaceSObj::pointColorLabelColorChanged(glm::vec3 color)
{
	recordLastSnapshot();
	pointColor = color;
	recordCurSnapshot();
}

void SimpleFaceSObj::bgPointMoveDirDoubleSpinBoxVec3ValueChanged(glm::vec3 val)
{
	recordLastSnapshot();
	bgPointMoveDir = glm::normalize(val);
	recordCurSnapshot();
}

void SimpleFaceSObj::bgPointSpawnSpeedSpinBoxValueChanged(int val)
{
	recordLastSnapshot();
	bgPointSpawnSpeed = val;
	recordCurSnapshot();
}

void SimpleFaceSObj::bgPointMoveSpeedDoubleSpinBoxValueChanged(double val)
{
	recordLastSnapshot();
	bgPointMoveSpeed = val;
	recordCurSnapshot();
}

void SimpleFaceSObj::bgPointThroughFaceCheckBoxStateChanged(int val)
{
	recordLastSnapshot();
	bgPointThroughFace = val;
	recordCurSnapshot();
}

void SimpleFaceSObj::throughFaceCheckBoxChanged(int val)
{
	recordLastSnapshot();
	bgPointThroughFace = val;
	recordCurSnapshot();
}

void SimpleFaceSObj::facePointSpawnSpeedSpinBoxValueChanged(int val)
{
	recordLastSnapshot();
	facePointSpawnSpeed = val;
	recordCurSnapshot();
}

void SimpleFaceSObj::facePointLifeTimeDoubleSpinBoxValueChanged(double val)
{
	recordLastSnapshot();
	facePointLifeTime = val;
	recordCurSnapshot();
}

ISObj *construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject)
{
	ISObj *iSObj = new SimpleFaceSObj(camera, sceneSize, ids);
	iSObj->load(jsonObject);

	return iSObj;
}

ISObj *copyer(ISObj *other)
{
	return new SimpleFaceSObj(dynamic_cast<SimpleFaceSObj *>(other));
}

ISObj *nuller()
{
	return new SimpleFaceSObj();
}