#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "GlobalUtility.h"
#include "SystemSetting.h"
#include "LuaLoader.h"
#include "GameManager.h"

/*************************************************************************
The MIT License(MIT)

Copyright(c) 2017 lewes/Liu Hao lewes6369@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*************************************************************************/


void window_close_callback(GLFWwindow* window)
{
	glfwSetWindowShouldClose(window, GLFW_FALSE);
}

void error_callback(int error, const char* description)
{
	puts(description);
}

void window_refresh_callback(GLFWwindow* window)
{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();

		//glOrtho(0.0, glb_GetSystemSetting()->getWidth(), glb_GetSystemSetting()->getHeight(), 0.0, -1.0, 1.0);

		//glb_GetImageRender()->RenderTextureInRect(TexID, cv::Vec2i(10, 10), cv::Vec2i(WIDTH - 10, HEIGHT - 10));
		
		//std::list<std::shared_ptr<LuaBaseValue>> resultList;
		//LuaLoader *loader = LuaLoader::getLoader();
		//loader->lua_doFile("Lua/start.lua");
		
		//glfwSwapBuffers(window);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double posX, posY;
	glfwGetCursorPos(window, &posX, &posY);
	LuaLoader::getLoader()->OnClickScreen(button,action,posX,posY);
}

void cursor_move_callback(GLFWwindow* window, double x, double y)
{
	LuaLoader::getLoader()->OnMouseMove(x, y);
}

void window_resize_callback(GLFWwindow* window, int width, int height)
{
	//glfwSetWindowMonitor(window, NULL, 20, 20, width, height, GLFW_DONT_CARE);
	glViewport(0, 0, width, height);
	LuaLoader::getLoader()->OnWindowSizeChanged(width, height, glb_GetSystemSetting()->getScale());
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	SystemSetting * inst = SystemSetting::GetInstance();
	int width = inst->getWidth();
	int height = inst->getHeight();
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "LAVG_GAME", NULL, NULL);

	//glfwSetWindowSizeLimits(window, 0, 0, 1, 2048);
	/* Set Hint GLFW_RESIZABLE */
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	
	/* Customer Init*/
	glb_DataInit(window);

	/*Set Cursor*/
	//GLFWimage * GLFWImage = glb_GetImageReader()->readGLFWimage("pic/cursor.png");
	//glfwSetWindowIcon(window, 1, GLFWImage);
	//GLFWcursor* Cursor = glfwCreateCursor(GLFWImage, 0, 0);
	//glfwSetCursor(window, Cursor);

	/* Set the window's close callback function */
	glfwSetWindowCloseCallback(window, window_close_callback);

	/* Set the window's refresh callback function */
	glfwSetWindowRefreshCallback(window, window_refresh_callback);

	/* Set the window's error callback function */
	glfwSetErrorCallback(error_callback);

	/* Set the window's Mouse Button callback function */
	glfwSetMouseButtonCallback(window,mouse_button_callback);

	/* Set the window's Cursor Move callback function */
	glfwSetCursorPosCallback(window,cursor_move_callback);

	/* Set the window's Resize callback function */
	glfwSetWindowSizeCallback(window, window_resize_callback);

	LuaLoader * loader = LuaLoader::getLoader();
	/* On Init Loader*/
	loader->OnInit();

	/* On Enter Menu*/
	loader->OnEnterMenu();

	GameManager * game = GameManager::GetInstance();
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//glClear(GL_COLOR_BUFFER_BIT);

		// - Update
		game->Update();

		/* Poll for and process events */
		glfwPollEvents();

		// - Render Function
		//window_refresh_callback(window);
		
		//std::cout << "nseconds:" << glfwGetTimerValue() << std::endl;
		//std::cout << "Frequency:" << glfwGetTimerFrequency() << std::endl;
	}

	glfwTerminate();
	return 0;
}


