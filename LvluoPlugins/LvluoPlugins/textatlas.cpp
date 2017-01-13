#include "textatlas.h"

const int MaxAtlasWidth = 4096;
const int GlyphSpacing = 2;

TextAtlas::TextAtlas()
{
	texture = 0;
}

TextAtlas::~TextAtlas()
{
	if (texture != 0)
	{
		glDeleteTextures(1, &texture);
	}
}

void TextAtlas::bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void TextAtlas::update(std::vector<FT_ULong> uls, FT_Face &face, int fontSize)
{
	w = 0;
	h = 0;
	unsigned int roww = 0;
	unsigned int rowh = 0;

	characters.clear();
	for (FT_ULong ul : uls)
	{
		characters[ul] = GlyphLayout();
	}
	
	FT_Set_Pixel_Sizes(face, 0, fontSize);
	FT_GlyphSlot g = face->glyph; // 获取glyph槽

	// 计算能装载所有字符glyph的最小贴图的大小
	for (FT_ULong ul : uls)
	{
		if (FT_Load_Char(face, ul, FT_LOAD_RENDER))
		{
			printf("load character %c failed\n", ul);
			continue;
		}

		if (roww + g->bitmap.width + GlyphSpacing >= MaxAtlasWidth)
		{
			w = std::max(w, roww);
			h += rowh;
			roww = 0;
			rowh = 0;
		}
		roww += g->bitmap.width + GlyphSpacing;
		rowh = std::max(rowh, g->bitmap.rows + GlyphSpacing);
	}
	w = std::max(w, roww);
	h += rowh;
	
	// 更新text atlas
	if (texture != 0) // 如果原先的贴图已经存在，就删除掉
	{
		glDeleteTextures(1, &texture);
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 更新characters
	int ox = 0;
	int oy = 0;

	rowh = 0;

	for (auto &character : characters)
	{
		if (FT_Load_Char(face, character.first, FT_LOAD_RENDER))
		{
			printf("load character %c failed\n", character.first);
			continue;
		}

		if (ox + g->bitmap.width + GlyphSpacing >= MaxAtlasWidth)
		{
			oy += rowh;
			rowh = 0;
			ox = 0;
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, 
			ox, oy, g->bitmap.width, g->bitmap.rows, 
			GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		character.second.ax = g->advance.x >> 6;
		character.second.ay = g->advance.y >> 6;

		character.second.bw = g->bitmap.width;
		character.second.bh = g->bitmap.rows;

		character.second.bl = g->bitmap_left;
		character.second.bt = g->bitmap_top;

		character.second.tx = ox / (float)w;
		character.second.ty = oy / (float)h;

		rowh = std::max(rowh, g->bitmap.rows + GlyphSpacing);
		ox += g->bitmap.width + GlyphSpacing;
	}
}

TextAtlas::GlyphLayout TextAtlas::getGlyphLayout(FT_ULong ul)
{
	return characters[ul];
}

int TextAtlas::getWidth()
{
	return w;
}

int TextAtlas::getHeight()
{
	return h;
}