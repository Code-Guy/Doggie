#include "tool.h"

#include <fstream>
#include <algorithm>
#include <QDir>
#include <QFileInfoList>

bool Tool::readFile(const char *fileName, std::string &ret)
{
	std::ifstream f(fileName);

	if (f.is_open())
	{
		std::string line;
		while (std::getline(f, line))
		{
			ret.append(line);
			ret.append("\n");
		}

		f.close();

		return true;
	}

	return false;
}

QString Tool::getLayerDir(QStringList ids)
{
	QString layerDir;
	for (QString id : ids)
	{
		layerDir += id + "/";
	}

	return layerDir;
}

std::vector<QString> Tool::traverseFile(QString directory)
{
	std::vector<QString> fileNames;

	QDir dir(directory);
	QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);

	for (QFileInfo file : fileList)
	{
		QString fileName = file.filePath();
		fileNames.push_back(fileName);
	}

	std::sort(fileNames.begin(), fileNames.end());
	return fileNames;
}

std::vector<QString> Tool::traverseDir(QString directory, QStringList filter)
{
	std::vector<QString> dirs;

	QDir dir(directory);
	QFileInfoList fileInfoList = dir.entryInfoList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);

	bool terminal = true;
	for (QFileInfo fileInfo : fileInfoList)
	{
		QString baseName = fileInfo.baseName();
		QString filePath = fileInfo.filePath();
		QString path = fileInfo.path();

		if (!filter.contains(baseName))
		{
			terminal = false;
			std::vector<QString> subDirs = traverseDir(filePath, filter);
			dirs.insert(dirs.end(), subDirs.begin(), subDirs.end());
		}
	}

	if (terminal)
	{
		dirs.push_back(directory);
	}

	return dirs;
}

bool Tool::floatEqual(float lhs, float rhs)
{
	return abs(lhs - rhs) < FLOAT_EQUAL_EPSILON;
}

bool Tool::vec3Equal(glm::vec3 lhs, glm::vec3 rhs)
{
	return floatEqual(lhs.x, rhs.x) && floatEqual(lhs.y, rhs.y) && floatEqual(lhs.z, rhs.z);
}

glm::ivec2 Tool::calcWinPos(glm::vec3 objPos,
	glm::ivec4 viewport, 
	glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	GLdouble mvMatrix[16];
	GLdouble projMatrix[16];
	GLdouble winX, winY, winZ;

	getGLViewProjectMatrix(viewMatrix, projectionMatrix, mvMatrix, projMatrix);
	gluProject(objPos.x, objPos.y, objPos.z, mvMatrix, projMatrix, glm::value_ptr(viewport), &winX, &winY, &winZ);

	return glm::ivec2(winX, viewport[3] - winY - 1);
}

glm::vec3 Tool::calcObjPos(glm::vec3 winPos,
	glm::ivec4 viewport,
	glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	GLdouble mvMatrix[16];
	GLdouble projMatrix[16];
	GLdouble objX, objY, objZ;

	getGLViewProjectMatrix(viewMatrix, projectionMatrix, mvMatrix, projMatrix);
	gluUnProject(winPos.x, viewport[3] - winPos.y - 1, winPos.z, mvMatrix, projMatrix, glm::value_ptr(viewport), &objX, &objY, &objZ);

	return glm::vec3(objX, objY, objZ);
}

glm::vec2 Tool::calcWinObjRatio(glm::vec3 position,
	glm::ivec4 viewport,
	glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	GLdouble mvMatrix[16];
	GLdouble projMatrix[16];
	GLdouble winX, winY, winZ;

	getGLViewProjectMatrix(viewMatrix, projectionMatrix, mvMatrix, projMatrix);
	gluProject(position.x, position.y, position.z, mvMatrix, projMatrix, glm::value_ptr(viewport), &winX, &winY, &winZ);

	GLdouble objPos1[3], objPos2[3];
	gluUnProject(0, 0, winZ, mvMatrix, projMatrix, glm::value_ptr(viewport), &objPos1[0], &objPos1[1], &objPos1[2]);
	gluUnProject(100, 100, winZ, mvMatrix, projMatrix, glm::value_ptr(viewport), &objPos2[0], &objPos2[1], &objPos2[2]);

	return glm::vec2(objPos2[0] - objPos1[0], objPos2[1] - objPos1[1]) / glm::vec2(100, 100);
}

glm::vec3 Tool::calcTexAABB(int w, int h)
{
	if (w > h)
	{
		return glm::vec3(1.0f, (float)h / w, TEXTURE_AABB_DEPTH);
	}
	else
	{
		return glm::vec3((float)w / h, 1.0f, TEXTURE_AABB_DEPTH);
	}
}

glm::vec3 Tool::uniformCubeSample(glm::vec3 cube)
{
	glm::vec3 position;
	position.x = Tool::getRandomFloat(-cube.x, cube.x);
	position.y = Tool::getRandomFloat(-cube.y, cube.y);
	position.z = Tool::getRandomFloat(-cube.z, cube.z);

	return position;
}

glm::vec3 Tool::uniformSphereSample(glm::vec3 sphere)
{
	float a = glm::radians(getRandomFloat(-180.0f, 180.0f));
	float b = acos(2 * getRandomFloat(0.0f, 1.0f) - 1);
	float r = pow(getRandomFloat(0.0f, 1.0f), 1.0f / 3.0f);

	glm::vec3 position;
	position.x = r * sin(b) * cos(a);
	position.y = r * sin(b) * sin(a);
	position.z = r * cos(b);

	return position * sphere;
}

int Tool::getRandomInt(int lower, int upper)
{
	return rand() % (upper - lower + 1) + lower;
}

glm::vec3 Tool::reduceRotation(glm::vec3 rotation)
{
	for (int i=0; i<3; i++)
	{
		int n = (int)rotation[i] / 360;
		rotation[i] = rotation[i] - (rotation[i] < 0 ? n - 1 : n) * 360;
	}
	return rotation;
}

QString Tool::vec3ToStr(glm::vec3 v)
{
	return QString("(%1, %2, %3)").arg(v.x).arg(v.y).arg(v.z);
}

glm::vec3 Tool::vec3FromStr(QString str)
{
	str = str.mid(1, str.length() - 2);
	QStringList strList = str.split(", ");

	return glm::vec3(strList.at(0).trimmed().toFloat(),
		strList.at(1).trimmed().toFloat(),
		strList.at(2).trimmed().toFloat());
}

QString Tool::ivec2ToStr(glm::ivec2 v)
{
	return QString("(%1, %2)").arg(v.x).arg(v.y);
}

glm::ivec2 Tool::ivec2FromStr(QString str)
{
	str = str.mid(1, str.length() - 2);
	QStringList strList = str.split(", ");

	return glm::ivec2(strList.at(0).trimmed().toInt(),
		strList.at(1).trimmed().toInt());
}

bool Tool::removeFolder(const QString &path)
{
	if (path.isEmpty()) 
	{
		return false;
	}
	QDir dir(path);
	if (!dir.exists()) 
	{
		return true;
	}
	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤  
	QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息  
	foreach(QFileInfo file, fileList) 
	{ //遍历文件信息  
		if (file.isFile()) 
		{ // 是文件，删除  
			file.dir().remove(file.fileName());
		}
		else 
		{ // 递归删除  
			removeFolder(file.absoluteFilePath());
		}
	}
	return dir.rmpath(dir.absolutePath()); // 删除文件夹  
}

void Tool::decompose(const glm::mat4 &m, glm::vec3 &t, glm::vec3 &r, glm::vec3 &s)
{
	// 平移
	t = glm::vec3(m[3][0], m[3][1], m[3][2]);

	// 缩放
	s.x = glm::length(glm::vec3(m[0][0], m[0][1], m[0][2]));
	s.y = glm::length(glm::vec3(m[1][0], m[1][1], m[1][2]));
	s.z = glm::length(glm::vec3(m[2][0], m[2][1], m[2][2]));

	// 旋转
	glm::mat4 R = glm::mat4(m[0][0] / s.x, m[0][1] / s.x, m[0][2] / s.x, 0,
		m[1][0] / s.y, m[1][1] / s.y, m[1][2] / s.y, 0,
		m[2][0] / s.z, m[2][1] / s.z, m[2][2] / s.z, 0,
		0, 0, 0, 1);
	glm::extractEulerAngleXYZ(R, r.x, r.y, r.z);
	r = glm::degrees(r);
}

void Tool::getGLViewProjectMatrix(glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
	GLdouble mvMatrix[16], GLdouble projMatrix[16])
{
	//模型视图矩阵 投影矩阵
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mvMatrix[4 * i + j] = viewMatrix[i][j];
			projMatrix[4 * i + j] = projectionMatrix[i][j];
		}
	}
}

float Tool::getRandomFloat(float lower, float upper)
{
	float r = (float)rand() / RAND_MAX;
	return r * (upper - lower) + lower;
}