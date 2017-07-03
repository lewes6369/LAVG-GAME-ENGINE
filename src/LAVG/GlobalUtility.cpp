#include "GlobalUtility.h"

SystemSetting* _setting = NULL;
ImageReader * _Imgreader = NULL;
ImageRender * _Imgrender = NULL;
GLFWwindow * _window = NULL;
GameManager * _game = NULL;

void glb_DataInit(GLFWwindow * window)
{
	_setting = SystemSetting::GetInstance();
	_Imgreader = ImageReader::GetInstance();
	_Imgrender = ImageRender::GetInstance();
	_window = window;
}

SystemSetting *glb_GetSystemSetting()
{
	return _setting;
}

ImageReader * glb_GetImageReader()
{
	return _Imgreader;
}

ImageRender * glb_GetImageRender()
{
	return _Imgrender;
}

GLFWwindow * glb_GetGLFWWindow()
{
	return _window;
}

GameManager * glb_GetGame()
{
	return _game;
}