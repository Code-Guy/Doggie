#ifndef TOOL_H
#define TOOL_H

#include <string>
#include <QString>
#include <QStringList>
#include <vector>
#include <limits>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include <GL/glew.h>

#include "lvluoapi.h"

#define FLOAT_EQUAL_EPSILON 0.0001f
#define SAFE_DELETE(p) if (p) { delete p; }
#define TEXTURE_AABB_DEPTH 0.0001f

class LVLUO_API Tool
{
public:
	static bool readFile(const char *fileName, std::string &ret);
	static QString getLayerDir(QStringList ids);
	static std::vector<QString> traverseFile(QString directory);
	static std::vector<QString> traverseDir(QString directory, QStringList filter);
	static bool floatEqual(float lhs, float rhs);
	static bool vec3Equal(glm::vec3 lhs, glm::vec3 rhs);

	// �������������Ӧ�Ĵ�������
	static glm::ivec2 calcWinPos(glm::vec3 objPos, 
		glm::ivec4 viewport, 
		glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	// ���㴰�������Ӧ����������
	static glm::vec3 calcObjPos(glm::vec3 winPos,
		glm::ivec4 viewport,
		glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	// ������Ļ���뵽�������ı���ֵ
	static glm::vec2 calcWinObjRatio(glm::vec3 position,
		glm::ivec4 viewport,
		glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	static glm::vec3 calcTexAABB(int w, int h); // ����2dͼƬ��Ӧ��aabb��С

	static glm::vec3 uniformCubeSample(glm::vec3 cube); // ����������ֲ�
	static glm::vec3 uniformSphereSample(glm::vec3 sphere); // ��������ֲ�
	static int getRandomInt(int lower, int upper); // ����ĳһ����������

	static glm::vec3 reduceRotation(glm::vec3 rotation); // �ѽǶȹ�Լ��0��360֮��

	static QString vec3ToStr(glm::vec3 v);
	static glm::vec3 vec3FromStr(QString str);
	static QString ivec2ToStr(glm::ivec2 v);
	static glm::ivec2 ivec2FromStr(QString str);

	static bool removeFolder(const QString &path);
	static void decompose(const glm::mat4 &m, glm::vec3 &t, glm::vec3 &r, glm::vec3 &s);

private:
	static void getGLViewProjectMatrix(glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
		GLdouble mvMatrix[16], GLdouble projMatrix[16]);
	static float getRandomFloat(float lower, float upper); // ����ĳһ��������������
};

#endif // TOOL_H
