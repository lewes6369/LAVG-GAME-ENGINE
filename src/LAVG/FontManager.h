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
	cv::Vec2i _vecCoordlt;	//������������ top left
	cv::Vec2i _vecCoordrb;	//������������ right bottom
	cv::Vec2i _texCoord;	//��������     texture top left    
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
	FT_Library  _library;			//FTT�����
	FT_Face     _face;				//FTT����
	unsigned    _fontTexId;			//������ʾ����ID
	int         _yStart;
	int         _xStart;
	int         _fontSize;
};