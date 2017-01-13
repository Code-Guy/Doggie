#include "opticalflowsobj.h"
#include "resourcemanager.h"
#include "primitivemanager.h"
#include "sobjfactory.h"
#include "tool.h"

#include <algorithm>

// 默认初始化数据
const OpticalFlowSObj::SpaceShape DefaultSpaceShape = OpticalFlowSObj::SpaceShape::Cube;
const glm::vec3 DefaultSpaceSize = glm::vec3(1.0f, 1.0f, 1.0f);
const OpticalFlowSObj::StarShape DefaultStarShape = OpticalFlowSObj::StarShape::Triangle;
const float DefaultStarSize = 0.05f;
const int DefaultStarDensity = 80;
const glm::vec3 DefaultColor = glm::vec3(1.0f, 1.0f, 0.0f);
const bool DefaultLifeTimeOn = true;
const int DefaultLifeTime = 5;
const int DefaultCoherence = 80;
const float DefaultElevation = 0.0f;
const std::vector<float> DefaultAzimuths = std::vector<float>{ 45.0f };
const float DefaultMagnitude = 1.0f;
const float DefaultDuration = 2.0f;
const float DefaultSigma = 1.0f;
const glm::vec3 DefaultOffset = glm::vec3(0.0f, 0.0f, 0.0f);
const int DefaultDivision = 1000;

OpticalFlowSObj::OpticalFlowSObj()
{

}

OpticalFlowSObj::OpticalFlowSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids) : 
	SObj(camera, sceneSize, ids, Dimension::ThreeD)
{
	init();
}

OpticalFlowSObj::OpticalFlowSObj(OpticalFlowSObj *other) : SObj(other)
{
	init();
	setParamWidget();
	inflateParamWidget(other->spaceShape, other->spaceSize,
		other->starShape, other->starSize, other->starDensity, other->starColor,
		other->lifeTimeOn, other->lifeTime,
		other->coherence,
		other->elevation, other->azimuths, other->magnitude,
		other->duration, other->sigma, other->offset, other->division);
	connectParamWidget();

	genStars();
	isRecord = true;
}

OpticalFlowSObj::~OpticalFlowSObj()
{
	delete spaceShapeComboBox;
	delete spaceSizeDoubleSpinBoxVec3;
	delete starShapeComboBox;
	delete starSizeDoubleSpinBox;
	delete starDensitySpinBox;
	delete colorLabel;
	delete lifeTimeOnCheckBox;
	delete lifeTimeSpinBox;
	delete coherenceSpinBox;
	delete elevationDoubleSpinBox;
	delete azimuthDoubleSpinBoxList;
	delete magnitudeDoubleSpinBox;
	delete durationDoubleSpinBox;
	delete sigmaDoubleSpinBox;
	delete offsetDoubleSpinBoxVec3;
	delete divisionSpinBox;
}

void OpticalFlowSObj::destroy()
{
	delete this;
}

void OpticalFlowSObj::logic(float dt)
{
	SObj::logic(dt);

	if (t > duration)
	{
		a = (a + 1) % azimuths.size();
		genStars();
	}
	if (isPlay)
	{
		f++;
		if (lifeTimeOn && f > lifeTime)
		{
			f = 0;
			resampleStars();
		}

		t += dt;
		c += dt * division;
		moveStars();
		oc += dt * division;
	}

	// 按照深度排序
	std::sort(stars.begin(), stars.end());

	positions.clear();
	for (Star &star : stars)
	{
		positions.push_back(star.position);
	}
}

void OpticalFlowSObj::render(bool blendOn, bool pickOn)
{
	SObj::render();

	if (isVisible)
	{
		// 绘制
		ResourceManager::getInstance()->getBillboardShader()->enable();
		ResourceManager::getInstance()->getBillboardShader()->setPickOn(pickOn);
		ResourceManager::getInstance()->getBillboardShader()->setBlendOn(blendOn);
		ResourceManager::getInstance()->getBillboardShader()->setColorID(colorID);
		ResourceManager::getInstance()->getBillboardShader()->setMV(camera->getViewMatrix() * m);
		ResourceManager::getInstance()->getBillboardShader()->setP(camera->getProjectionMatrix());
		ResourceManager::getInstance()->getBillboardShader()->setColor(starColor);
		ResourceManager::getInstance()->getBillboardShader()->setSize(starSize);

		starTex[starShape]->bind();

		PrimitiveManager::getInstance()->renderPoints(positions, GL_POINTS);
	}
}

void OpticalFlowSObj::save(QJsonObject &jsonObject)
{
	SObj::save(jsonObject);

	jsonObject.insert("Space Shape", spaceShape);
	jsonObject.insert("Space Size", Tool::vec3ToStr(spaceSize));
	jsonObject.insert("Star Shape", starShape);
	jsonObject.insert("Star Size", starSize);
	jsonObject.insert("Star Density", starDensity);
	jsonObject.insert("Star Color", Tool::vec3ToStr(starColor));
	jsonObject.insert("Life Time On/Off", lifeTimeOn);
	jsonObject.insert("Life Time", lifeTime);
	jsonObject.insert("Coherence", coherence);
	jsonObject.insert("Elevation", elevation);
	jsonObject.insert("Azimuths", getAzimuthsStr());
	jsonObject.insert("Magnitude", magnitude);
	jsonObject.insert("Duration", duration);
	jsonObject.insert("Sigma", sigma);
	jsonObject.insert("Offset", Tool::vec3ToStr(offset));
	jsonObject.insert("Division", division);
}

void OpticalFlowSObj::load(QJsonObject jsonObject)
{
	SObj::load(jsonObject);

	setParamWidget();
	if (jsonObject.isEmpty()) // 用默认数据初始化
	{
		inflateParamWidget(DefaultSpaceShape, DefaultSpaceSize,
			DefaultStarShape, DefaultStarSize, DefaultStarDensity, DefaultColor,
			DefaultLifeTimeOn, DefaultLifeTime,
			DefaultCoherence,
			DefaultElevation, DefaultAzimuths, DefaultMagnitude,
			DefaultDuration, DefaultSigma, DefaultOffset, DefaultDivision);
	}
	else // 用载入数据初始化
	{
		inflateParamWidget(jsonObject.value("Space Shape").toInt(), Tool::vec3FromStr(jsonObject.value("Space Size").toString()),
			jsonObject.value("Star Shape").toInt(), jsonObject.value("Star Size").toDouble(), jsonObject.value("Star Density").toInt(), 
			Tool::vec3FromStr(jsonObject.value("Star Color").toString()),
			jsonObject.value("Life Time On/Off").toBool(), jsonObject.value("Life Time").toInt(),
			jsonObject.value("Coherence").toInt(),
			jsonObject.value("Elevation").toDouble(), fromAzimuthsStr(jsonObject.value("Azimuths").toString()), jsonObject.value("Magnitude").toDouble(),
			jsonObject.value("Duration").toDouble(), jsonObject.value("Sigma").toDouble(), Tool::vec3FromStr(jsonObject.value("Offset").toString()), jsonObject.value("Division").toInt());
	}
	connectParamWidget();

	genStars();
	isRecord = true;
}

void OpticalFlowSObj::getSnapshot(ISObj *snapshot)
{
	SObj::getSnapshot(snapshot);
	OpticalFlowSObj *castedSnapshot = dynamic_cast<OpticalFlowSObj *>(snapshot);
	castedSnapshot->spaceShape = spaceShape;
	castedSnapshot->spaceSize = spaceSize;
	castedSnapshot->starShape = starShape;
	castedSnapshot->starSize = starSize;
	castedSnapshot->starDensity = starDensity;
	castedSnapshot->starColor = starColor;
	castedSnapshot->lifeTimeOn = lifeTimeOn;
	castedSnapshot->lifeTime = lifeTime;
	castedSnapshot->coherence = coherence;
	castedSnapshot->elevation = elevation;
	castedSnapshot->azimuths = azimuths;
	castedSnapshot->magnitude = magnitude;
	castedSnapshot->duration = duration;
	castedSnapshot->sigma = sigma;
	castedSnapshot->offset = offset;
	castedSnapshot->division = division;
}

void OpticalFlowSObj::setSnapshot(ISObj *snapshot)
{
	SObj::setSnapshot(snapshot);
	disconnectParamWidget();
	OpticalFlowSObj *castedSnapshot = dynamic_cast<OpticalFlowSObj *>(snapshot);
	inflateParamWidget(castedSnapshot->spaceShape, castedSnapshot->spaceSize,
	castedSnapshot->starShape, castedSnapshot->starSize, castedSnapshot->starDensity, castedSnapshot->starColor,
	castedSnapshot->lifeTimeOn, castedSnapshot->lifeTime,
	castedSnapshot->coherence,
	castedSnapshot->elevation, castedSnapshot->azimuths, castedSnapshot->magnitude,
	castedSnapshot->duration, castedSnapshot->sigma, castedSnapshot->offset, castedSnapshot->division);
	connectParamWidget();
	isRecord = true;
}

void OpticalFlowSObj::click(QPoint mousePos)
{
	SObj::click(mousePos);

	isPlay = !isPlay;
	if (isPlay && t == 0.0f)
	{
		genStars();
	}
}

void OpticalFlowSObj::rightClick(QPoint mousePos)
{
	SObj::rightClick(mousePos);

	showParamWidget(mousePos);
}

void OpticalFlowSObj::init()
{
	a = 0;
	isPlay = true;
	baseAABB = aabb;

	starTex[0] = ResourceManager::getInstance()->getTexture(
		"Resources/SObjs/" + Tool::getLayerDir(ids) + "Res/triangle.png");
	starTex[1] = ResourceManager::getInstance()->getTexture(
		"Resources/SObjs/" + Tool::getLayerDir(ids) + "Res/dot.png");
}

void OpticalFlowSObj::setParamWidget()
{
	paramWidget->addEnum("Space Shape", QStringList{ "Cube", "Sphere" }, spaceShapeComboBox);
	paramWidget->addVec3("Space Size", 1.0f, 10.0f, 1.0f,
		spaceSizeDoubleSpinBoxVec3);
	paramWidget->addEnum("Star Shape", QStringList{ "Triangle", "Dot" }, starShapeComboBox);
	paramWidget->addFloat("Star Size", 0.01f, 1.0f, 0.01f, starSizeDoubleSpinBox);
	paramWidget->addInt("Star Density", 1, 1000, 1, starDensitySpinBox);
	paramWidget->addColor("Color", colorLabel);
	paramWidget->addBool("Life Time On/Off", lifeTimeOnCheckBox);
	paramWidget->addInt("Life Time", 3, 20, 1, lifeTimeSpinBox);
	paramWidget->addInt("Coherence", 0, 100, 1, coherenceSpinBox);
	paramWidget->addFloat("Elevation", -90.0f, 90.0f, 1.0f, elevationDoubleSpinBox);
	paramWidget->addFloats("Azimuths", 0.0f, 0.0f, 360.0f, 1.0f, azimuthDoubleSpinBoxList);
	paramWidget->addFloat("Magnitude", 0.1f, 100.0f, 0.1f, magnitudeDoubleSpinBox);
	paramWidget->addFloat("Duration", 1.0f, 60.0f, 1.0f, durationDoubleSpinBox);
	paramWidget->addFloat("Sigma", 0.1f, 10.0f, 0.1f, sigmaDoubleSpinBox);
	paramWidget->addVec3("Offset", 0.0f, 10.0f, 0.1f,
		offsetDoubleSpinBoxVec3);
	paramWidget->addInt("Division", 1, 2000, 1, divisionSpinBox);
}

void OpticalFlowSObj::inflateParamWidget(int _spaceShape, glm::vec3 _spaceSize, 
	int _starShape, float _starSize, float _starDensity, glm::vec3 _starColor, 
	bool _lifeTimeOn, int _lifeTime, 
	int _coherence, 
	float _elevation, std::vector<float> _azimuths, float _magnitude, 
	float _duration, float _sigma, glm::vec3 _offset, int _division)
{
	spaceShapeComboBox->setCurrentIndex(_spaceShape);
	spaceSizeDoubleSpinBoxVec3->setVec3(_spaceSize);
	starShapeComboBox->setCurrentIndex(_starShape);
	starSizeDoubleSpinBox->setValue(_starSize);
	starDensitySpinBox->setValue(_starDensity);
	colorLabel->setColor(_starColor);
	lifeTimeOnCheckBox->setChecked(_lifeTimeOn);
	lifeTimeSpinBox->setValue(_lifeTime);
	coherenceSpinBox->setValue(_coherence);
	elevationDoubleSpinBox->setValue(_elevation);
	azimuthDoubleSpinBoxList->setListVals(_azimuths);
	magnitudeDoubleSpinBox->setValue(_magnitude);
	durationDoubleSpinBox->setValue(_duration);
	sigmaDoubleSpinBox->setValue(_sigma);
	offsetDoubleSpinBoxVec3->setVec3(_offset);
	divisionSpinBox->setValue(_division);

	spaceShapeComboBoxComboBoxCurrentIndexChanged(_spaceShape);
	spaceSizeDoubleSpinBoxVec3Changed(_spaceSize);
	starShapeComboBoxComboBoxCurrentIndexChanged(_starShape);
	starSizeDoubleSpinBoxChanged(_starSize);
	starDensitySpinBoxChanged(_starDensity);
	colorChanged(_starColor);
	lifeTimeOnCheckBoxStateChanged(_lifeTimeOn);
	lifeTimeSpinBoxChanged(_lifeTime);
	coherenceSpinBoxChanged(_coherence);
	elevationDoubleSpinBoxChanged(_elevation);
	azimuthDoubleSpinBoxListValueChanged(_azimuths);
	magnitudeDoubleSpinBoxChanged(_magnitude);
	durationDoubleSpinBoxChanged(_duration);
	sigmaDoubleSpinBoxChanged(_sigma);
	offsetDoubleSpinBoxVec3Changed(_offset);
	divisionSpinBoxChanged(_division);
}

void OpticalFlowSObj::connectParamWidget()
{
	connect(spaceShapeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(spaceShapeComboBoxComboBoxCurrentIndexChanged(int)));
	connect(spaceSizeDoubleSpinBoxVec3, SIGNAL(vec3ValueChanged(glm::vec3)), this, SLOT(spaceSizeDoubleSpinBoxVec3Changed(glm::vec3)));
	connect(starShapeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(starShapeComboBoxComboBoxCurrentIndexChanged(int)));
	connect(starSizeDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(starSizeDoubleSpinBoxChanged(double)));
	connect(starDensitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(starDensitySpinBoxChanged(int)));
	connect(colorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(colorChanged(glm::vec3)));
	connect(lifeTimeOnCheckBox, SIGNAL(stateChanged(int)), this, SLOT(lifeTimeOnCheckBoxStateChanged(int)));
	connect(lifeTimeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(lifeTimeSpinBoxChanged(int)));
	connect(coherenceSpinBox, SIGNAL(valueChanged(int)), this, SLOT(coherenceSpinBoxChanged(int)));
	connect(elevationDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(elevationDoubleSpinBoxChanged(double)));
	connect(azimuthDoubleSpinBoxList, SIGNAL(listValueChanged(std::vector<float>)), this, SLOT(azimuthDoubleSpinBoxListValueChanged(std::vector<float>)));
	connect(magnitudeDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(magnitudeDoubleSpinBoxChanged(double)));
	connect(durationDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(durationDoubleSpinBoxChanged(double)));
	connect(sigmaDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(sigmaDoubleSpinBoxChanged(double)));
	connect(offsetDoubleSpinBoxVec3, SIGNAL(vec3ValueChanged(glm::vec3)), this, SLOT(offsetDoubleSpinBoxVec3Changed(glm::vec3)));
	connect(divisionSpinBox, SIGNAL(valueChanged(int)), this, SLOT(divisionSpinBoxChanged(int)));
}

void OpticalFlowSObj::disconnectParamWidget()
{
	disconnect(spaceShapeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(spaceShapeComboBoxComboBoxCurrentIndexChanged(int)));
	disconnect(spaceSizeDoubleSpinBoxVec3, SIGNAL(vec3ValueChanged(glm::vec3)), this, SLOT(spaceSizeDoubleSpinBoxVec3Changed(glm::vec3)));
	disconnect(starShapeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(starShapeComboBoxComboBoxCurrentIndexChanged(int)));
	disconnect(starSizeDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(starSizeDoubleSpinBoxChanged(double)));
	disconnect(starDensitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(starDensitySpinBoxChanged(int)));
	disconnect(colorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(colorChanged(glm::vec3)));
	disconnect(lifeTimeOnCheckBox, SIGNAL(stateChanged(int)), this, SLOT(lifeTimeOnCheckBoxStateChanged(int)));
	disconnect(lifeTimeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(lifeTimeSpinBoxChanged(int)));
	disconnect(coherenceSpinBox, SIGNAL(valueChanged(int)), this, SLOT(coherenceSpinBoxChanged(int)));
	disconnect(elevationDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(elevationDoubleSpinBoxChanged(double)));
	disconnect(azimuthDoubleSpinBoxList, SIGNAL(listValueChanged(std::vector<float>)), this, SLOT(azimuthDoubleSpinBoxListValueChanged(std::vector<float>)));
	disconnect(magnitudeDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(magnitudeDoubleSpinBoxChanged(double)));
	disconnect(durationDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(durationDoubleSpinBoxChanged(double)));
	disconnect(sigmaDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(sigmaDoubleSpinBoxChanged(double)));
	disconnect(offsetDoubleSpinBoxVec3, SIGNAL(vec3ValueChanged(glm::vec3)), this, SLOT(offsetDoubleSpinBoxVec3Changed(glm::vec3)));
	disconnect(divisionSpinBox, SIGNAL(valueChanged(int)), this, SLOT(divisionSpinBoxChanged(int)));
}

void OpticalFlowSObj::genStars()
{
	t = 0.0f;
	c = 0.0f;
	oc = 0.0f;
	f = 0;

	aabb = baseAABB * spaceSize;
	dir.x = -glm::cos(glm::radians(elevation)) * glm::cos(glm::radians(azimuths[a]));
	dir.y = glm::sin(glm::radians(elevation));
	dir.z = glm::cos(glm::radians(elevation)) * glm::sin(glm::radians(azimuths[a]));

	stars.clear();
	int starNum = spaceSize.x * spaceSize.y * spaceSize.z * starDensity;

	for (int i = 0; i < starNum; i++)
	{
		if (spaceShape == Cube)
		{
			stars.push_back(Star(Tool::uniformCubeSample(spaceSize)));
		}
		else if (spaceShape == Sphere)
		{
			stars.push_back(Star(Tool::uniformSphereSample(spaceSize)));
		}
	}
}

void OpticalFlowSObj::resampleStars()
{
	for (Star &star : stars)
	{
		int r = Tool::getRandomInt(0, 99);
		if (r < coherence)
		{
			star.isMove = true;
		}
		else
		{
			if (spaceShape == Cube)
			{
				star.position = Tool::uniformCubeSample(spaceSize);
			}
			else if (spaceShape == Sphere)
			{
				star.position = Tool::uniformSphereSample(spaceSize);
			}
			star.isMove = false;
		}
	}
}

void OpticalFlowSObj::moveStars()
{
	// 进行高斯运动插值
	int beg = (int)oc;
	float span = sigma * 6 / (duration * division);
	int interpNum = (int)c; // 需要插值计算的次数
	c -= interpNum;

	float g = 0.0f; // 高斯运动值
	for (int i = 0; i < interpNum; i++)
	{
		float begVal = beg * span - sigma * 3;
		float endVal = ++beg * span - sigma * 3;

		float begInterpVal = gaussian(begVal);
		float endInterpVal = gaussian(endVal);

		float area = (begInterpVal + endInterpVal) * span / 2.0f;
		g += area;
	}

	// 更新星星的速度和位置
	for (Star &star : stars)
	{
		if (star.isMove)
		{
			star.position += dir * g * magnitude;
		}
	}
}

float OpticalFlowSObj::gaussian(float t)
{
	float mousePos = 1.0f / (std::sqrt(glm::pi<float>() * 2.0f) * sigma);
	float e = std::exp(-t * t / (sigma * sigma * 2.0f));

	return mousePos * e;
}

QString OpticalFlowSObj::getAzimuthsStr()
{
	QString azimuthsStr;
	azimuthsStr += "(";
	for (int i=0; i<azimuths.size() - 1; i++)
	{
		azimuthsStr += QString::number(azimuths[i]) + ", ";
	}
	azimuthsStr += QString::number(azimuths.back()) + ")";

	return azimuthsStr;
}

std::vector<float> OpticalFlowSObj::fromAzimuthsStr(QString azimuthsStr)
{
	std::vector<float> loadedAzimuths;
	azimuthsStr = azimuthsStr.mid(1, azimuthsStr.length() - 2);
	QStringList azimuthsStrList = azimuthsStr.split(", ");

	for (QString str : azimuthsStrList)
	{
		loadedAzimuths.push_back(str.trimmed().toFloat());
	}

	return loadedAzimuths;
}

void OpticalFlowSObj::spaceShapeComboBoxComboBoxCurrentIndexChanged(int index)
{
	recordLastSnapshot();
	spaceShape = (SpaceShape)index;
	recordCurSnapshot();
}

void OpticalFlowSObj::spaceSizeDoubleSpinBoxVec3Changed(glm::vec3 val)
{
	recordLastSnapshot();
	spaceSize = val;
	recordCurSnapshot();
}

void OpticalFlowSObj::starShapeComboBoxComboBoxCurrentIndexChanged(int index)
{
	recordLastSnapshot();
	starShape = (StarShape)index;
	recordCurSnapshot();
}

void OpticalFlowSObj::starSizeDoubleSpinBoxChanged(double val)
{
	recordLastSnapshot();
	starSize = val;
	recordCurSnapshot();
}

void OpticalFlowSObj::starDensitySpinBoxChanged(int val)
{
	recordLastSnapshot();
	starDensity = val;
	recordCurSnapshot();
}

void OpticalFlowSObj::colorChanged(glm::vec3 color)
{
	recordLastSnapshot();
	starColor = color;
	recordCurSnapshot();
}

void OpticalFlowSObj::lifeTimeOnCheckBoxStateChanged(int state)
{
	recordLastSnapshot();
	lifeTimeOn = state;
	recordCurSnapshot();
}

void OpticalFlowSObj::lifeTimeSpinBoxChanged(int val)
{
	recordLastSnapshot();
	lifeTime = val;
	recordCurSnapshot();
}

void OpticalFlowSObj::coherenceSpinBoxChanged(int val)
{
	recordLastSnapshot();
	coherence = val;
	recordCurSnapshot();
}

void OpticalFlowSObj::elevationDoubleSpinBoxChanged(double val)
{
	recordLastSnapshot();
	elevation = val;
	recordCurSnapshot();
}

void OpticalFlowSObj::azimuthDoubleSpinBoxListValueChanged(std::vector<float> vals)
{
	azimuths = vals;
	a = 0;
	genStars();
}

void OpticalFlowSObj::magnitudeDoubleSpinBoxChanged(double val)
{
	recordLastSnapshot();
	magnitude = val;
	recordCurSnapshot();
}

void OpticalFlowSObj::durationDoubleSpinBoxChanged(double val)
{
	recordLastSnapshot();
	duration = val;
	recordCurSnapshot();
}

void OpticalFlowSObj::sigmaDoubleSpinBoxChanged(double val)
{
	recordLastSnapshot();
	sigma = val;
	recordCurSnapshot();
}

void OpticalFlowSObj::offsetDoubleSpinBoxVec3Changed(glm::vec3 val)
{
	recordLastSnapshot();
	offset = val;
	recordCurSnapshot();
}

void OpticalFlowSObj::divisionSpinBoxChanged(int val)
{
	recordLastSnapshot();
	division = val;
	recordCurSnapshot();
}

ISObj *construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject)
{
	ISObj *iSObj = new OpticalFlowSObj(camera, sceneSize, ids);
	iSObj->load(jsonObject);

	return iSObj;
}

ISObj *copyer(ISObj *other)
{
	return new OpticalFlowSObj(dynamic_cast<OpticalFlowSObj *>(other));
}

ISObj *nuller()
{
	return new OpticalFlowSObj();
}