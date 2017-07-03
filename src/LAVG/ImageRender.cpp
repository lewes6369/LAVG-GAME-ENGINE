#include "ImageRender.h"

//constructor
ImageRender::ImageRender():m_FontTexture("font.ttf", 20)
{
	m_TextureCache.clear();
}

//LRU algorithm
GLuint ImageRender::GetTexIdByNameFromCache(const char * imgName)
{
	std::string Name(imgName);
	for (auto iter = m_TextureCache.begin(); iter != m_TextureCache.end();iter ++ ) {
		if (iter->ImageName == Name){
			//Move iter to front
			TextureData tex(*iter);
			GLuint id = iter->texId;
			m_TextureCache.erase(iter);
			m_TextureCache.push_front(tex);
			return id;
		}
	}
	return 0;
}

TextureData& ImageRender::AddTextureToCache(GLuint TexId, const char * imgName)
{
	std::string Name(imgName);
	for (auto iter: m_TextureCache) {
		if (iter.ImageName == Name) {
			//Move iter to front
			return iter;
		}
	}
	
	TextureData tex;
	tex.ImageName = imgName;
	tex.texId = TexId;
	if (m_TextureCache.size() == TEX_CACHE_MAX_COUNT)
	{
		GLuint _Id = m_TextureCache.cend()->texId;
		m_TextureCache.pop_back();
		//glInvalidateTexImage(_Id, 0);
		glDeleteTextures(1, &_Id);
	}
	m_TextureCache.push_front(tex);

	return m_TextureCache.front();
}

void ImageRender::RenderTextureInRect(GLuint TextId, cv::Vec2i &win_lt, cv::Vec2i &win_rb, cv::Vec2f &tex_lb,cv::Vec2f &tex_rt)
{
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glEnable(GL_TEXTURE_2D);//图像有效化
		glBindTexture(GL_TEXTURE_2D, TextId);
		glEnable(GL_ALPHA_TEST);//试描画开始
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBegin(GL_QUADS);
			glTexCoord2f(tex_lb[0], tex_rt[1]); glVertex2d(win_lt[0], win_rb[1]);//左下
			glTexCoord2f(tex_lb[0], tex_lb[1]); glVertex2d(win_lt[0], win_lt[1]);//左上
			glTexCoord2f(tex_rt[0], tex_lb[1]); glVertex2d(win_rb[0], win_lt[1]);//右上
			glTexCoord2f(tex_rt[0], tex_rt[1]); glVertex2d(win_rb[0], win_rb[1]);//右下
			glEnd();
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);//试描画结束
	glDisable(GL_TEXTURE_2D);//图像无效
}
void ImageRender::MixAlphaTextureInRect(GLuint TextId, cv::Vec2i &win_lt, cv::Vec2i &win_rb, cv::Vec2f &tex_lb /*= cv::Vec2f(0.0f, 0.0f)*/,
	cv::Vec2f &tex_rt /*= cv::Vec2f(1.0f, 1.0f)*/)

{
	glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, TextId);
		glEnable(GL_ALPHA_TEST);
		glBegin(GL_QUADS);
			glTexCoord2f(tex_lb[0], tex_rt[1]); glVertex2d(win_lt[0], win_rb[1]);//左下
			glTexCoord2f(tex_lb[0], tex_lb[1]); glVertex2d(win_lt[0], win_lt[1]);//左上
			glTexCoord2f(tex_rt[0], tex_lb[1]); glVertex2d(win_rb[0], win_lt[1]);//右上
			glTexCoord2f(tex_rt[0], tex_rt[1]); glVertex2d(win_rb[0], win_rb[1]);//右下
		glEnd();
		glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
}

void ImageRender::RenderText(const wchar_t* text, cv::Vec2i &win_lt, cv::Vec2i &win_rb, float *endPosX /*= NULL*/, float *endPosY /*= NULL*/)
{
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_FontTexture.getFontTexId());
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		float       xStart = win_lt[0], yStart = win_lt[1];
		unsigned    nSize = wcslen(text);
		cv::Vec2i *VecCoordArr = new cv::Vec2i[4 * nSize];
		cv::Vec2f *TexCoordArr = new cv::Vec2f[4 * nSize];
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		GLint fontSize = m_FontTexture.getFontSize();

		int index = 0;
		for (unsigned i = 0; i < nSize; ++i)
		{
			CharGlyph*  ch = m_FontTexture.getCharacter(text[i]);
			int         h = ch->_vecCoordrb[1] - ch->_vecCoordlt[1];
			int         w = ch->_vecCoordrb[0] - ch->_vecCoordlt[0];
			int			ArrStrip = i * 4;

			int	x0 = xStart;
			int y0 = yStart - ch->_texCoord[1];
			if (xStart + w > win_rb[0])	
			{
				//changeLine
				xStart = win_lt[0];
				x0 = xStart;
				yStart += fontSize;
				y0 = yStart - ch->_texCoord[1];
				if (y0 > win_rb[1])	
					//out of rect
					break;
			}

			int x1 = x0 + w;
			int y1 = y0 + h;

			float u0 = float(ch->_vecCoordlt[0]) / FONT_TEX_WIDTH;
			float v0 = float(ch->_vecCoordlt[1]) / FONT_TEX_HEIGHT;
			float u1 = float(ch->_vecCoordrb[0]) / FONT_TEX_WIDTH;
			float v1 = float(ch->_vecCoordrb[1]) / FONT_TEX_HEIGHT;

			VecCoordArr[ArrStrip] = cv::Vec2i(x0, y0);
			TexCoordArr[ArrStrip] = cv::Vec2f(u0, v0);

			VecCoordArr[ArrStrip +1] = cv::Vec2i(x1, y0);
			TexCoordArr[ArrStrip +1] = cv::Vec2f(u1, v0);

			VecCoordArr[ArrStrip +2] = cv::Vec2i(x1, y1);
			TexCoordArr[ArrStrip +2] = cv::Vec2f(u1, v1);

			VecCoordArr[ArrStrip +3] = cv::Vec2i(x0, y1);
			TexCoordArr[ArrStrip +3] = cv::Vec2f(u0, v1);

			xStart += w + ch->_texCoord[0];
		}

		if (endPosX)
			*endPosX = xStart;
		if (endPosY)
			*endPosY = yStart;

		glVertexPointer(2, GL_INT, sizeof(cv::Vec2i), VecCoordArr);
		glTexCoordPointer(2, GL_FLOAT, sizeof(cv::Vec2f), TexCoordArr);
		glDrawArrays(GL_QUADS, 0, 4 * nSize);

		delete[] VecCoordArr;
		delete[] TexCoordArr;

	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	glPopAttrib();
	glPopClientAttrib();
}

void ImageRender::ReloadCacheImages()
{
	GLuint lastTexId = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&lastTexId);
	for (auto iter : m_TextureCache)
	{	
		//glInvalidateTexImage(iter.texId,0);
		glDeleteTextures(1, &iter.texId);
		ImageReader::GetInstance()->LoadTextureFromImage(iter.ImageName.c_str(), iter.texId,true);
	}

	glBindTexture(GL_TEXTURE_2D, lastTexId);
}

void ImageRender::RemoveTextureFromCache(GLuint TexId)
{
	for (auto iter = m_TextureCache.begin(); iter != m_TextureCache.end(); iter++) {
		if (iter->texId == TexId) {
			glDeleteTextures(1, &(iter->texId));
			m_TextureCache.erase(iter);
			return;
		}
	}
}


