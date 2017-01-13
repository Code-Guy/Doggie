#include "resourcemanager.h"

ResourceManager *ResourceManager::instance = nullptr;

ResourceManager::ResourceManager()
{
	// initialize shaders
	textureShader = new TextureShader;
	if (!textureShader->init())
	{
		printf("failed to initialize texture shader\n");
	}

	textureMaskShader = new TextureMaskShader;
	if (!textureMaskShader->init())
	{
		printf("failed to initialize texture mask shader\n");
	}

	baseShader = new BaseShader;
	if (!baseShader->init())
	{
		printf("failed to initialize line shader\n");
	}

	modelShader = new ModelShader;
	if (!modelShader->init())
	{
		printf("failed to initialize model shader\n");
	}

	billboardShader = new BillboardShader;
	if (!billboardShader->init())
	{
		printf("failed to initialize billboard shader\n");
	}

	gridShader = new GridShader;
	if (!gridShader->init())
	{
		printf("failed to initialize grid shader\n");
	}

	pointShader = new PointShader;
	if (!pointShader->init())
	{
		printf("failed to initialize point shader\n");
	}

	// initialize freetype
	// 初始化freetype library
	if (FT_Init_FreeType(&ft))
	{
		printf("init freetype failed\n");
	}

	// 加载字体
	faces.resize(2);
	if (FT_New_Face(ft, "Resources/Fonts/simkai.ttf", 0, &faces[0]))
	{
		printf("load simkai font failed\n");
	}
	if (FT_New_Face(ft, "Resources/Fonts/simhei.ttf", 0, &faces[1]))
	{
		printf("load simhei font failed\n");
	}

	libvlc = libvlc_new(0, nullptr);
}

ResourceManager::~ResourceManager()
{
	delete textureShader;
	delete textureMaskShader;
	delete baseShader;
	delete modelShader;
	delete billboardShader;
	delete gridShader;
	delete pointShader;

	for (auto texture : textures)
	{
		delete texture.second;
	}

	for (auto model : models)
	{
		delete model.second;
	}

	for (FT_Face face : faces)
	{
		FT_Done_Face(face);
	}
	FT_Done_FreeType(ft);

	libvlc_release(libvlc);
}

TextureShader *ResourceManager::getTextureShader()
{
	return textureShader;
}

TextureMaskShader *ResourceManager::getTextureMaskShader()
{
	return textureMaskShader;
}

BaseShader *ResourceManager::getBaseShader()
{
	return baseShader;
}

ModelShader *ResourceManager::getModelShader()
{
	return modelShader;
}

BillboardShader *ResourceManager::getBillboardShader()
{
	return billboardShader;
}

GridShader *ResourceManager::getGridShader()
{
	return gridShader;
}

PointShader *ResourceManager::getPointShader()
{
	return pointShader;
}

Texture *ResourceManager::getTexture(QString fileName, GLuint minFilter, GLuint magFilter)
{
	if (textures.find(fileName) == textures.end())
	{
		textures[fileName] = new Texture(fileName, minFilter, magFilter);
	}

	return textures[fileName];
}

Model *ResourceManager::getModel(QString fileName)
{
	if (models.find(fileName) == models.end())
	{
		models[fileName] = new Model(fileName);
	}

	return models[fileName];
}

FT_Face &ResourceManager::getFace(int i)
{
	return faces[i];
}

libvlc_instance_t *ResourceManager::getLibvlcEngine()
{
	return libvlc;
}

ResourceManager *ResourceManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new ResourceManager;
	}

	return instance;
}