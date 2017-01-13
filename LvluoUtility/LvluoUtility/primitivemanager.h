#ifndef PRIMITIVEMANAGER_H
#define PRIMITIVEMANAGER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "lvluoapi.h"

class LVLUO_API PrimitiveManager
{
public:
	~PrimitiveManager();

	void renderQuad();
	void renderQuads(const std::vector<glm::vec3> &positions, const std::vector<glm::vec2> &texCoords);
	void renderPoints(const std::vector<glm::vec3> &positions, GLenum drawMode);
	void renderAABB();
	void renderAnchor();
	void renderFrame();

	static const glm::vec3 AABBColor; // ���߿���ɫ
	static const glm::vec3 AnchorColor; // ê����ɫ
	static const glm::vec3 FrameColor; // ѡ�����ɫ

	static const float AnchorSize; // ê���С
	
	static PrimitiveManager *getInstance();

private:
	PrimitiveManager();

	void initQuad();
	void initQuads();
	void initPoint();
	void initAABB(GLuint &vao, GLuint &vbo, int aabbDivision);
	void initFrame();

	GLuint quadVAO, quadVBO, quadEBO;
	GLuint quadsVAO, quadsPositionVBO, quadsTexCoordVBO;
	GLuint pointVAO, pointVBO;
	GLuint aabbVAO, aabbVBO;
	GLuint anchorVAO, anchorVBO;
	GLuint frameVAO, frameVBO;

	static PrimitiveManager *instance;
};

#endif // PRIMITIVEMANAGER_H
