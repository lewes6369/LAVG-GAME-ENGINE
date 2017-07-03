#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cstring>
#include "ImageRender.h"
#include "SystemSetting.h"
// Load Image
// author by Liu Hao
// 2017/06/12

class ImageReader
{
	public: 
		static ImageReader* GetInstance() {
			static ImageReader *Instance;
			if (!Instance)
				Instance = new ImageReader();

			return Instance;
		}

		GLFWimage * readGLFWimage(const char* filePath);
		GLFWimage * readGLFWimage(std::string filePath) { readGLFWimage(filePath.c_str()); };

		bool LoadTextureFromImage(const char* filePath, GLuint& texId,bool SaveToCache = false);
		bool ReloadTextureImage(const char* filePath, GLuint& texId);
	private:
		ImageReader() {};
		ImageReader(const ImageReader&);
};
