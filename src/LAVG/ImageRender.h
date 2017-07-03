#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cstring>
#include <list>
#include "FontManager.h"
#include "ImageReader.h"
// Image Render
// Save Load Images Caches
// author by Liu Hao
// 2017/06/12

#define TEX_CACHE_MAX_COUNT 15
// LRU 
struct TextureData
{	
	GLuint texId;
	std::string ImageName;
};

class ImageRender
{
public:
	static ImageRender* GetInstance() {
		static ImageRender *Instance;
		if (!Instance)
			Instance = new ImageRender;

		return Instance;
	}

	// Get Texture
	GLuint GetTexIdByNameFromCache(const char * imgName);

	// Add Texture To Cache
	TextureData& AddTextureToCache(GLuint TexId,const char * imgName);

	// Render Texture in Rect
	void RenderTextureInRect(GLuint TextId, cv::Vec2i &win_lt, cv::Vec2i &win_rb, cv::Vec2f &tex_lb = cv::Vec2f(0.0f, 0.0f),
		cv::Vec2f &tex_rt = cv::Vec2f(1.0f, 1.0f));

	// Mix Texture in Rect
	void MixAlphaTextureInRect(GLuint TexId, cv::Vec2i &win_lt, cv::Vec2i &win_rb, cv::Vec2f &tex_lb = cv::Vec2f(0.0f, 0.0f),
		cv::Vec2f &tex_rt = cv::Vec2f(1.0f, 1.0f));

	// Render Text in Rect
	void RenderText(const wchar_t* text, cv::Vec2i &win_lt, cv::Vec2i &win_rb, float *endPosX = NULL, float *endPosY = NULL);
	// Render Text on Pos
		//(float posX, float posY, const wchar_t* text, float *endPosX = NULL, float *endPosY = NULL);

	// Reload Caches Texture (when size changed)
	void ReloadCacheImages();

	// Remove Texture
	void RemoveTextureFromCache(GLuint TexId);

	private:
		ImageRender();
		ImageRender(const ImageRender&);

	private:
		std::list<TextureData> m_TextureCache;
		FontTexture m_FontTexture;
};
