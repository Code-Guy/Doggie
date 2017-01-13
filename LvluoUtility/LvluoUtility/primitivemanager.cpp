#include "primitivemanager.h"
#include <vector>

const glm::vec3 PrimitiveManager::AABBColor = glm::vec3(1.0f, 1.0f, 0.0f); // 虚线框颜色
const glm::vec3 PrimitiveManager::AnchorColor = glm::vec3(0.0f, 0.0f, 1.0f); // 锚点颜色
const glm::vec3 PrimitiveManager::FrameColor = glm::vec3(1.0f, 1.0f, 0.0f); // 选择框颜色
const float PrimitiveManager::AnchorSize = 0.02f; // 锚点大小

PrimitiveManager *PrimitiveManager::instance = nullptr;

PrimitiveManager::PrimitiveManager()
{
	initQuad();
	initQuads();
	initPoint();
	initAABB(aabbVAO, aabbVBO, 10); // 虚线包围框
	initAABB(anchorVAO, anchorVBO, 1); // 锚点
	initFrame(); // 选择框
}

PrimitiveManager::~PrimitiveManager()
{
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteBuffers(1, &quadEBO);

	glDeleteVertexArrays(1, &quadsVAO);
	glDeleteBuffers(1, &quadsPositionVBO);
	glDeleteBuffers(1, &quadsTexCoordVBO);

	glDeleteVertexArrays(1, &pointVAO);
	glDeleteBuffers(1, &pointVBO);

	glDeleteVertexArrays(1, &aabbVAO);
	glDeleteBuffers(1, &aabbVBO);

	glDeleteVertexArrays(1, &frameVAO);
	glDeleteBuffers(1, &frameVBO);
}

void PrimitiveManager::renderQuad()
{
	glBindVertexArray(quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void PrimitiveManager::renderQuads(const std::vector<glm::vec3> &positions, const std::vector<glm::vec2> &texCoords)
{
	glBindVertexArray(quadsVAO);

	glBindBuffer(GL_ARRAY_BUFFER, quadsPositionVBO);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, quadsTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_DYNAMIC_DRAW);

	glDrawArrays(GL_TRIANGLES, 0, positions.size());

	glBindVertexArray(0);
}

void PrimitiveManager::renderPoints(const std::vector<glm::vec3> &positions, GLenum drawMode)
{
	glBindVertexArray(pointVAO);
	glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_DYNAMIC_DRAW);

	glDrawArrays(drawMode, 0, positions.size());
}

void PrimitiveManager::renderAABB()
{
	glBindVertexArray(aabbVAO);
	glDrawArrays(GL_LINES, 0, 240);
	glBindVertexArray(0);
}

void PrimitiveManager::renderAnchor()
{
	glBindVertexArray(anchorVAO);
	glDrawArrays(GL_LINES, 0, 24);
	glBindVertexArray(0);
}

void PrimitiveManager::renderFrame()
{
	glBindVertexArray(frameVAO);
	glDrawArrays(GL_LINES, 0, 8);
	glBindVertexArray(0);
}

PrimitiveManager *PrimitiveManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new PrimitiveManager;
	}
	return instance;
}

void PrimitiveManager::initQuad()
{
	GLfloat quadVertices[] =
	{
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};
	GLuint quadIndices[] =
	{
		0, 3, 1,
		1, 3, 2
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glGenBuffers(1, &quadEBO);

	glBindVertexArray(quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void PrimitiveManager::initQuads()
{
	glGenVertexArrays(1, &quadsVAO);
	glGenBuffers(1, &quadsPositionVBO);
	glGenBuffers(1, &quadsTexCoordVBO);

	glBindVertexArray(quadsVAO);

	glBindBuffer(GL_ARRAY_BUFFER, quadsPositionVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, quadsTexCoordVBO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void PrimitiveManager::initPoint()
{
	glGenVertexArrays(1, &pointVAO);
	glGenBuffers(1, &pointVBO);

	glBindVertexArray(pointVAO);

	glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void PrimitiveManager::initAABB(GLuint &vao, GLuint &vbo, int aabbDivision)
{
	std::vector<glm::vec3> aabbVertices;
	std::vector<glm::vec3> aabbEdgeVertices[12];
	float aabbSpan = 2.0f / (2 * aabbDivision - 1);
	for (int i=0; i<aabbDivision; i++)
	{
		float beg = -1.0f + 2 * i * aabbSpan;
		float end = beg + aabbSpan;

		aabbEdgeVertices[0].push_back(glm::vec3(beg, -1.0f, -1.0f));
		aabbEdgeVertices[0].push_back(glm::vec3(end, -1.0f, -1.0f));

		aabbEdgeVertices[1].push_back(glm::vec3(beg, 1.0f, -1.0f));
		aabbEdgeVertices[1].push_back(glm::vec3(end, 1.0f, -1.0f));

		aabbEdgeVertices[2].push_back(glm::vec3(beg, -1.0f, 1.0f));
		aabbEdgeVertices[2].push_back(glm::vec3(end, -1.0f, 1.0f));

		aabbEdgeVertices[3].push_back(glm::vec3(beg, 1.0f, 1.0f));
		aabbEdgeVertices[3].push_back(glm::vec3(end, 1.0f, 1.0f));

		aabbEdgeVertices[4].push_back(glm::vec3(-1.0f, beg, -1.0f));
		aabbEdgeVertices[4].push_back(glm::vec3(-1.0f, end, -1.0f));

		aabbEdgeVertices[5].push_back(glm::vec3(1.0f, beg, -1.0f));
		aabbEdgeVertices[5].push_back(glm::vec3(1.0f, end, -1.0f));

		aabbEdgeVertices[6].push_back(glm::vec3(-1.0f, beg, 1.0f));
		aabbEdgeVertices[6].push_back(glm::vec3(-1.0f, end, 1.0f));

		aabbEdgeVertices[7].push_back(glm::vec3(1.0f, beg, 1.0f));
		aabbEdgeVertices[7].push_back(glm::vec3(1.0f, end, 1.0f));

		aabbEdgeVertices[8].push_back(glm::vec3(-1.0f, -1.0f, beg));
		aabbEdgeVertices[8].push_back(glm::vec3(-1.0f, -1.0f, end));

		aabbEdgeVertices[9].push_back(glm::vec3(1.0f, -1.0f, beg));
		aabbEdgeVertices[9].push_back(glm::vec3(1.0f, -1.0f, end));

		aabbEdgeVertices[10].push_back(glm::vec3(-1.0f, 1.0f, beg));
		aabbEdgeVertices[10].push_back(glm::vec3(-1.0f, 1.0f, end));

		aabbEdgeVertices[11].push_back(glm::vec3(1.0f, 1.0f, beg));
		aabbEdgeVertices[11].push_back(glm::vec3(1.0f, 1.0f, end));
	}

	for (int i=0; i<12; i++)
	{
		aabbVertices.insert(aabbVertices.end(), aabbEdgeVertices[i].begin(), aabbEdgeVertices[i].end());
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * aabbVertices.size(), aabbVertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void PrimitiveManager::initFrame()
{
	std::vector<glm::vec3> frameVertices;
	frameVertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	frameVertices.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
	frameVertices.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
	frameVertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
	frameVertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
	frameVertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
	frameVertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
	frameVertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));

	glGenVertexArrays(1, &frameVAO);
	glGenBuffers(1, &frameVBO);

	glBindVertexArray(frameVAO);

	glBindBuffer(GL_ARRAY_BUFFER, frameVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * frameVertices.size(), frameVertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}