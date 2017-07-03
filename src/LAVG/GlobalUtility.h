#pragma once
// GlobalUtility
// Get Global Singleton
// author by Liu Hao
// 2017/06/12

#include "SystemSetting.h"
#include "ImageReader.h"
#include "ImageRender.h"
#include "GameManager.h"
#include <GLFW/glfw3.h>
extern SystemSetting* _setting;
extern ImageReader * _Imgreader;
extern ImageRender * _Imgrender;
extern GLFWwindow * _window;
extern GameManager * _game;

extern void glb_DataInit(GLFWwindow * window);
extern SystemSetting *glb_GetSystemSetting();
extern ImageReader * glb_GetImageReader();
extern ImageRender * glb_GetImageRender();
extern GLFWwindow * glb_GetGLFWWindow();
extern GameManager * glb_GetGame();