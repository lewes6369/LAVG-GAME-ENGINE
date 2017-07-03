#pragma once
#include "ft2build.h"
#include "freetype/ftglyph.h"
#include <GL/glew.h>
#include <opencv2/core.hpp>
#include <assert.h>
#include <gl/GLU.h>
#include <math.h>

#define  VEC2_ZERO (cv::Vec2i(0,0))
#define  FONT_TEX_WIDTH 1024
#define  FONT_TEX_HEIGHT 1024
#define  MAX_CHAR_CACHE_COUNT 65535

struct  CharGlyph
{
	cv::Vec2i _vecCoordlt;	//顶点坐标左上 top left
	cv::Vec2i _vecCoordrb;	//顶点坐标右下 right bottom
	cv::Vec2i _texCoord;	//纹理坐标     texture top left    
};

class  FontTexture
{
public:
	FontTexture();

	//FontTexture Constructer
	FontTexture(const char* fontFile, int fontSize);

	//Get Font Character Glyph
	CharGlyph*  getCharacter(wchar_t ch);

	//Get texture Id
	GLuint      getFontTexId() { 
		return _fontTexId; }

	//Get font Size
	GLint      getFontSize() {
		return _fontSize;
	}
protected:

	CharGlyph   _charsCache[MAX_CHAR_CACHE_COUNT];
	FT_Library  _library;			//FTT字体库
	FT_Face     _face;				//FTT字体
	unsigned    _fontTexId;			//字体显示纹理ID
	int         _yStart;
	int         _xStart;
	int         _fontSize;
};