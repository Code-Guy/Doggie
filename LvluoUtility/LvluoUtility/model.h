#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "modelshader.h"

#include "lvluoapi.h"

class LVLUO_API Model
{
public:
	Model(QString fileName);
	~Model();

	void render(ModelShader *modelShader);
	void render();

	glm::vec3 getAABB();

private:
	std::vector<Mesh *> meshes;
	glm::vec3 aabb;

	QString directory;

	void processNode(aiNode *node, const aiScene *scene);
	Mesh *processMesh(aiMesh *mesh, const aiScene *scene);

	Texture *loadMaterialTexture(aiMaterial *material, aiTextureType textureType);

	void calcAABB();
};

#endif // MODEL_H
