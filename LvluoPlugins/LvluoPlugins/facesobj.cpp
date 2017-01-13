#include "facesobj.h"
#include "resourcemanager.h"
#include "primitivemanager.h"
#include "sobjfactory.h"
#include "tool.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// 默认初始化数据
const int DefaultSpreadLayer = 50; // 传播层数
const int DefaultWaveSpawnSpeed = 20; // wave出生的速度
const int DefaultPointSpeed = 80; // 点的速度（每秒传播多少个模型点）
const float DefaultPointSize = 3.0f; // 点的大小
const glm::vec3 DefaultPointColor = glm::vec3(1.0f, 0.0f, 0.0f); // 点的颜色

FaceSObj::FaceSObj()
{

}

FaceSObj::FaceSObj(const Camera *camera, glm::ivec2 *sceneSize, QStringList ids) :
	SObj(camera, sceneSize, ids, Dimension::ThreeD)
{
	init();
}

FaceSObj::FaceSObj(FaceSObj *other) : SObj(other)
{
	init();
	setParamWidget();
	inflateParamWidget(other->waveSpreadLayer, other->waveSpawnSpeed,
		other->pointSpeed, other->pointSize, other->pointColor);
	connectParamWidget();
	isRecord = true;
}

FaceSObj::~FaceSObj()
{
	for (Wave *wave : waves)
	{
		delete wave;
	}

	delete waveSpreadLayerSpinBox;
	delete waveSpawnSpeedSpinBox;
	delete pointSpeedSpinBox;
	delete pointSizeDoubleSpinBox;
	delete pointColorLabel;
}

void FaceSObj::destroy()
{
	delete this;
}

void FaceSObj::logic(float dt)
{
	SObj::logic(dt);

	if (isPlay)
	{
		positions.clear();
		spawnWave(dt);
		updateWave(dt);
	}
}

void FaceSObj::render(bool blendOn, bool pickOn)
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
		PrimitiveManager::getInstance()->renderPoints(positions, GL_POINTS);
		glPointSize(1.0f);
	}
}

void FaceSObj::save(QJsonObject &jsonObject)
{
	SObj::save(jsonObject);

	jsonObject.insert("Spread Layer", waveSpreadLayer);
	jsonObject.insert("Spawn Speed", waveSpawnSpeed);
	jsonObject.insert("Spread Speed", pointSpeed);
	jsonObject.insert("Point Size", pointSize);
	jsonObject.insert("Point Color", Tool::vec3ToStr(pointColor));
}

void FaceSObj::load(QJsonObject jsonObject)
{
	SObj::load(jsonObject);

	setParamWidget();
	if (jsonObject.isEmpty()) // 用载入数据初始化
	{
		inflateParamWidget(DefaultSpreadLayer, DefaultWaveSpawnSpeed,
			DefaultPointSpeed, DefaultPointSize, DefaultPointColor);
	}
	else
	{
		inflateParamWidget(jsonObject.value("Spread Layer").toInt(),
		jsonObject.value("Spawn Speed").toInt(),
		jsonObject.value("Spread Speed").toInt(),
		jsonObject.value("Point Size").toDouble(),
		Tool::vec3FromStr(jsonObject.value("Point Color").toString()));
	}
	connectParamWidget();
	isRecord = true;
}

void FaceSObj::click(QPoint mousePos)
{
	SObj::click(mousePos);

	isPlay = !isPlay;
}

void FaceSObj::rightClick(QPoint mousePos)
{
	SObj::rightClick(mousePos);

	showParamWidget(mousePos);
}

void FaceSObj::getSnapshot(ISObj *snapshot)
{
	SObj::getSnapshot(snapshot);
	FaceSObj *castedSnapshot = dynamic_cast<FaceSObj *>(snapshot);
	castedSnapshot->waveSpreadLayer = waveSpreadLayer;
	castedSnapshot->waveSpawnSpeed = waveSpawnSpeed;
	castedSnapshot->pointSpeed = pointSpeed;
	castedSnapshot->pointSize = pointSize;
	castedSnapshot->pointColor = pointColor;
}

void FaceSObj::setSnapshot(ISObj *snapshot)
{
	SObj::setSnapshot(snapshot);
	disconnectParamWidget();
	FaceSObj *castedSnapshot = dynamic_cast<FaceSObj *>(snapshot);
	inflateParamWidget(castedSnapshot->waveSpreadLayer, castedSnapshot->waveSpawnSpeed,
		castedSnapshot->pointSpeed, castedSnapshot->pointSize, castedSnapshot->pointColor);
	connectParamWidget();
	isRecord = true;
}

void FaceSObj::init()
{
	isPlay = true;
	loadFaceModel("Resources/SObjs/" + Tool::getLayerDir(ids) + "Res/face.ply");
}

void FaceSObj::setParamWidget()
{
	paramWidget->addInt("Spread Layer", 1, 100, 1, waveSpreadLayerSpinBox);
	paramWidget->addInt("Spawn Speed", 1, 100, 1, waveSpawnSpeedSpinBox);
	paramWidget->addInt("Spread Speed", 1, 100, 1, pointSpeedSpinBox);
	paramWidget->addFloat("Point Size", 1.0f, 10.0f, 0.1f, pointSizeDoubleSpinBox);
	paramWidget->addColor("Point Color", pointColorLabel);
}

void FaceSObj::inflateParamWidget(int _waveSpreadLayer, int _waveSpawnSpeed,
	int _pointSpeed, float _pointSize, glm::vec3 _pointColor)
{
	waveSpreadLayerSpinBox->setValue(_waveSpreadLayer);
	waveSpawnSpeedSpinBox->setValue(_waveSpawnSpeed);
	pointSpeedSpinBox->setValue(_pointSpeed);
	pointSizeDoubleSpinBox->setValue(_pointSize);
	pointColorLabel->setColor(_pointColor);

	waveSpreadLayerSpinBoxChanged(_waveSpreadLayer);
	waveSpawnSpeedSpinBoxChanged(_waveSpawnSpeed);
	pointSpeedSpinBoxChanged(_pointSpeed);
	pointSizeDoubleSpinBoxChanged(_pointSize);
	pointColorChanged(_pointColor);
}

void FaceSObj::connectParamWidget()
{
	connect(waveSpreadLayerSpinBox, SIGNAL(valueChanged(int)), this, SLOT(waveSpreadLayerSpinBoxChanged(int)));
	connect(waveSpawnSpeedSpinBox, SIGNAL(valueChanged(int)), this, SLOT(waveSpawnSpeedSpinBoxChanged(int)));
	connect(pointSpeedSpinBox, SIGNAL(valueChanged(int)), this, SLOT(pointSpeedSpinBoxChanged(int)));
	connect(pointSizeDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(pointSizeDoubleSpinBoxChanged(double)));
	connect(pointColorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(pointColorChanged(glm::vec3)));
}

void FaceSObj::disconnectParamWidget()
{
	disconnect(waveSpreadLayerSpinBox, SIGNAL(valueChanged(int)), this, SLOT(waveSpreadLayerSpinBoxChanged(int)));
	disconnect(waveSpawnSpeedSpinBox, SIGNAL(valueChanged(int)), this, SLOT(waveSpawnSpeedSpinBoxChanged(int)));
	disconnect(pointSpeedSpinBox, SIGNAL(valueChanged(int)), this, SLOT(pointSpeedSpinBoxChanged(int)));
	disconnect(pointSizeDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(pointSizeDoubleSpinBoxChanged(double)));
	disconnect(pointColorLabel, SIGNAL(colorChanged(glm::vec3)), this, SLOT(pointColorChanged(glm::vec3)));
}

bool FaceSObj::loadFaceModel(QString fileName)
{
	Assimp::Importer importer;
	importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, true);
	const aiScene *scene = importer.ReadFile(qPrintable(fileName),
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals
		| aiProcess_CalcTangentSpace | aiProcess_PreTransformVertices);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		printf("ERROR::ASSIMP::%s\n", importer.GetErrorString());
		return false;
	}

	const aiMesh *mesh = scene->mMeshes[0];
	glm::vec3 minCorner(std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max());
	glm::vec3 maxCorner(std::numeric_limits<float>::lowest(),
		std::numeric_limits<float>::lowest(),
		std::numeric_limits<float>::lowest());
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		const aiVector3D* pos = &(mesh->mVertices[i]);
		glm::vec3 tmpPoint = glm::vec3(pos->x, pos->y, pos->z);
		tmpPoint = glm::vec3(glm::eulerAngleXYZ(0.0f, glm::radians(160.0f), glm::radians(270.0f)) * glm::vec4(tmpPoint, 1.0f));
		verts.push_back(tmpPoint);

		if (tmpPoint.x < minCorner.x)
		{
			minCorner.x = tmpPoint.x;
		}
		if (tmpPoint.y < minCorner.y)
		{
			minCorner.y = tmpPoint.y;
		}
		if (tmpPoint.z < minCorner.z)
		{
			minCorner.z = tmpPoint.z;
		}

		if (tmpPoint.x > maxCorner.x)
		{
			maxCorner.x = tmpPoint.x;
		}
		if (tmpPoint.y > maxCorner.y)
		{
			maxCorner.y = tmpPoint.y;
		}
		if (tmpPoint.z > maxCorner.z)
		{
			maxCorner.z = tmpPoint.z;
		}
	}
	aabb = maxCorner;

	net.resize(mesh->mNumVertices);
	for (int i=0; i<mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		int v0 = face.mIndices[0];
		int v1 = face.mIndices[1];
		int v2 = face.mIndices[2];

		net[v0].push_back(v1);
		net[v0].push_back(v2);

		net[v1].push_back(v0);
		net[v1].push_back(v2);

		net[v2].push_back(v0);
		net[v2].push_back(v1);
	}

	// 删除重复元素
	for (auto &netIter : net)
	{
		std::sort(netIter.begin(), netIter.end());
		netIter.erase(std::unique(netIter.begin(), netIter.end()), netIter.end());
	}

	return true;
}

void FaceSObj::spawnWave(float dt)
{
	static float waveSpawnNum = 0.0f;
	waveSpawnNum += waveSpawnSpeed * dt;
	if (waveSpawnNum > 1)
	{
		int curWaveSpawnNum = (int)waveSpawnNum;
		waveSpawnNum -= curWaveSpawnNum;

		for (int i=0; i<curWaveSpawnNum; i++)
		{
			waves.push_back(new Wave(&verts, &net, &pointSpeed, Tool::getRandomInt(0, verts.size() - 1), waveSpreadLayer));
		}
	}
}

void FaceSObj::updateWave(float dt)
{
	for (auto waveIter = waves.begin(); waveIter != waves.end(); )
	{
		(*waveIter)->logic(dt, positions);
		if ((*waveIter)->isFinished())
		{
			delete *waveIter;
			waveIter = waves.erase(waveIter);
		}
		else
		{
			waveIter++;
		}
	}
}

void FaceSObj::waveSpreadLayerSpinBoxChanged(int val)
{
	recordLastSnapshot();
	waveSpreadLayer = val;
	recordCurSnapshot();
}

void FaceSObj::waveSpawnSpeedSpinBoxChanged(int val)
{
	recordLastSnapshot();
	waveSpawnSpeed = val;
	recordCurSnapshot();
}

void FaceSObj::pointSpeedSpinBoxChanged(int val)
{
	recordLastSnapshot();
	pointSpeed = val;
	recordCurSnapshot();
}

void FaceSObj::pointSizeDoubleSpinBoxChanged(double val)
{
	recordLastSnapshot();
	pointSize = val;
	recordCurSnapshot();
}

void FaceSObj::pointColorChanged(glm::vec3 color)
{
	recordLastSnapshot();
	pointColor = color;
	recordCurSnapshot();
}

ISObj *construtor(const Camera *camera, glm::ivec2 *sceneSize,
	QStringList ids, QJsonObject jsonObject)
{
	ISObj *iSObj = new FaceSObj(camera, sceneSize, ids);
	iSObj->load(jsonObject);

	return iSObj;
}

ISObj *copyer(ISObj *other)
{
	return new FaceSObj(dynamic_cast<FaceSObj *>(other));
}

ISObj *nuller()
{
	return new FaceSObj();
}