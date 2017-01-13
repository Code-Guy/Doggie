#include "model.h"
#include "resourcemanager.h"

Model::Model(QString fileName)
{
	Assimp::Importer importer;
	importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, true);
	const aiScene *scene = importer.ReadFile(qPrintable(fileName),
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals 
		| aiProcess_CalcTangentSpace | aiProcess_PreTransformVertices);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		printf("ERROR::ASSIMP::%s\n", importer.GetErrorString());
	}

	directory = fileName;
	for (int i=0; i<2; i++)
	{
		directory = directory.left(directory.lastIndexOf("/"));
	}
	
	processNode(scene->mRootNode, scene);

	calcAABB();
}

Model::~Model()
{
	for (Mesh *mesh : meshes)
	{
		delete mesh;
	}
}

void Model::render(ModelShader *modelShader)
{
	for (Mesh *mesh : meshes)
	{
		modelShader->setMaterial(mesh->getMaterial());
		mesh->render();
	}
}

void Model::render()
{
	for (Mesh *mesh : meshes)
	{
		mesh->render();
	}
}

glm::vec3 Model::getAABB()
{
	return aabb;
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh *Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Material material;
	
	// vertex
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
		vertex.bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mTangents[i].z);

		if (mesh->mTextureCoords[0])
		{
			vertex.texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else
		{
			vertex.texCoord = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// index
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
		material.diffuseTexture = loadMaterialTexture(mat, aiTextureType_DIFFUSE);
		material.specularTexture = loadMaterialTexture(mat, aiTextureType_SPECULAR);
		material.normalTexture = loadMaterialTexture(mat, aiTextureType_HEIGHT);

		if (material.diffuseTexture != nullptr)
		{
			material.useDiffuseTexture = true;
		}
		else
		{
			aiColor4D diffuseColor;
			aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor);
			material.useDiffuseTexture = false;
			material.diffuseColor = glm::vec4(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a);
		}

		if (material.specularTexture != nullptr)
		{
			material.useSpecularTexture = true;
		}
		else
		{
			aiColor4D specularColor;
			aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specularColor);
			material.useSpecularTexture = false;
			material.specularColor = glm::vec4(specularColor.r, specularColor.g, specularColor.b, specularColor.a);
		}

		if (material.normalTexture != nullptr)
		{
			material.useNormalTexture = false;
		}
		else
		{
			material.useNormalTexture = false;
		}
	}

	material.shininess = 16.0f;

	return new Mesh(vertices, indices, material);
}

Texture *Model::loadMaterialTexture(aiMaterial *material, aiTextureType textureType)
{
	if (material->GetTextureCount(textureType) <= 0)
	{
		return nullptr;
	}

	// only extract first texture
	aiString name;
	material->GetTexture(textureType, 0, &name);

	return ResourceManager::getInstance()->getTexture(QString("%1/Textures/%2").arg(directory).arg(name.C_Str()));
}

void Model::calcAABB()
{
	aabb = glm::vec3(std::numeric_limits<float>::lowest(),
		std::numeric_limits<float>::lowest(),
		std::numeric_limits<float>::lowest());

	for (Mesh *mesh : meshes)
	{
		glm::vec3 meshAABB = mesh->getAABB();
		if (meshAABB.x > aabb.x)
		{
			aabb.x = meshAABB.x;
		}
		if (meshAABB.y > aabb.y)
		{
			aabb.y = meshAABB.y;
		}
		if (meshAABB.z > aabb.z)
		{
			aabb.z = meshAABB.z;
		}
	}
}