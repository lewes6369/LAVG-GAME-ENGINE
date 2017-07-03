#include "FontManager.h"

FontTexture::FontTexture()
{
	//FontTexture("simkai.ttf", 20);
}

FontTexture::FontTexture(const char* fontFile, int fontSize)
{
	_fontSize = fontSize;
	_xStart = 0;
	_yStart = 0;
	memset(_charsCache, 0, sizeof(_charsCache));
	FT_Init_FreeType(&_library);
	FT_New_Face(_library, fontFile, 0, &_face);
	//FT_Set_Char_Size(_face, fontSize << 6, fontSize << 6, 72, 72);
	FT_Set_Pixel_Sizes(_face, fontSize, fontSize);

	glGenTextures(1, &_fontTexId);
	if (_fontTexId == 0)
	{
		assert("FontTexture Init Wrong");
		return;
	}

	glBindTexture(GL_TEXTURE_2D, _fontTexId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, FONT_TEX_WIDTH, FONT_TEX_HEIGHT, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
}

CharGlyph* FontTexture::getCharacter(wchar_t ch)
{
	if (_charsCache[ch]._vecCoordlt != VEC2_ZERO ||
		_charsCache[ch]._vecCoordrb != VEC2_ZERO)
		return &_charsCache[ch];
	else
	{
		if (_xStart + _fontSize > FONT_TEX_WIDTH) {
			_xStart = 0;
			_yStart += _fontSize;
		}

		FT_Load_Glyph(_face, FT_Get_Char_Index(_face, ch), FT_LOAD_DEFAULT);
		FT_Glyph glyph;
		FT_Get_Glyph(_face->glyph, &glyph);

		FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		FT_Bitmap& bitmap = bitmap_glyph->bitmap;

		if (bitmap.width == 0 || bitmap.rows == 0)
		{
			_xStart += _fontSize / 2;
			_charsCache[ch]._vecCoordlt = cv::Vec2i(_xStart, _yStart);
			_charsCache[ch]._vecCoordrb = cv::Vec2i(_xStart, _yStart);
			_charsCache[ch]._texCoord = VEC2_ZERO;
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, _fontTexId);

			_charsCache[ch]._vecCoordlt = cv::Vec2i(_xStart, _yStart);
			_charsCache[ch]._vecCoordrb = cv::Vec2i(_xStart + bitmap.width, _yStart + bitmap.rows);
			_charsCache[ch]._texCoord = cv::Vec2i(bitmap_glyph->left, bitmap_glyph->top);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexSubImage2D(GL_TEXTURE_2D, 0, _xStart, _yStart, bitmap.width, bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, bitmap.buffer);
			_xStart += bitmap.width + 1;
		}
	}
	return  &_charsCache[ch];
}