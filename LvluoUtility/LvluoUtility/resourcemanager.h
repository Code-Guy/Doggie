#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "textureshader.h"
#include "texturemaskshader.h"
#include "baseshader.h"
#include "modelshader.h"
#include "billboardshader.h"
#include "gridshader.h"
#include "pointshader.h"

#include "texture.h"
#include "model.h"

#include "lvluoapi.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vlc/vlc.h>

#include <map>

class LVLUO_API ResourceManager
{
public:
	~ResourceManager();

	TextureShader *getTextureShader();
	TextureMaskShader *getTextureMaskShader();
	BaseShader *getBaseShader();
	ModelShader *getModelShader();
	BillboardShader *getBillboardShader();
	GridShader *getGridShader();
	PointShader *getPointShader();

	Texture *getTexture(QString fileName, GLuint minFilter = GL_LINEAR, GLuint magFilter = GL_LINEAR);
	Model *getModel(QString fileName);

	FT_Face &getFace(int i);

	libvlc_instance_t *getLibvlcEngine();

	static ResourceManager *getInstance();

private:
	ResourceManager();

	TextureShader *textureShader;
	TextureMaskShader *textureMaskShader;
	BaseShader *baseShader;
	ModelShader *modelShader;
	BillboardShader *billboardShader;
	GridShader *gridShader;
	PointShader *pointShader;

	std::map<QString, Texture *> textures;
	std::map<QString, Model *> models;

	FT_Library ft; // freetype¿â
	std::vector<FT_Face> faces; // ×ÖÌå

	libvlc_instance_t *libvlc; // libvlcÒýÇæ

	static ResourceManager *instance;
};

#endif // RESOURCEMANAGER_H