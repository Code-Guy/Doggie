#ifndef TEXTATLAS_H
#define TEXTATLAS_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>
#include <map>
#include <QString>
#include <gl/glew.h>

class TextAtlas
{
public:
	struct GlyphLayout // 字符布局
	{
		float ax;	// advance.x
		float ay;	// advance.y

		float bw;	// bitmap.width;
		float bh;	// bitmap.height;

		float bl;	// bitmap_left;
		float bt;	// bitmap_top;

		float tx;	// x offset of glyph in texture coordinates
		float ty;	// y offset of glyph in texture coordinates
	};

	TextAtlas();
	~TextAtlas();

	void bind();
	void update(std::vector<FT_ULong> uls, FT_Face &face, int fontSize); // uls: unicode字符集 face: 当前字体
	
	GlyphLayout getGlyphLayout(FT_ULong ul);
	int getWidth();
	int getHeight();

private:
	std::map<FT_ULong, GlyphLayout> characters;

	// 所有glyph拼成的大贴图，可以动态更改
	GLuint texture;
	unsigned int w;
	unsigned int h;
};

#endif // TEXTATLAS_H