#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

#include "texture.h"

#include "lvluoapi.h"

struct LVLUO_API Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct LVLUO_API Material
{
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
	Texture *diffuseTexture;
	Texture *specularTexture;
	Texture *normalTexture;
	bool useDiffuseTexture;
	bool useSpecularTexture;
	bool useNormalTexture;

	float shininess;
};

class LVLUO_API Mesh
{
public:
	Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices, const Material &material);
	~Mesh();

	void render();

	Material &getMaterial();
	glm::vec3 getAABB();

private:
	GLuint VAO, VBO, EBO;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Material material;

	glm::vec3 aabb;

	void init();
	void calcAABB();
};

#endif // MESH_H
