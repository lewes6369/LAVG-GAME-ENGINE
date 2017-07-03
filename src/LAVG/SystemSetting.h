#pragma once
#include <GL/glew.h>
// System Settings
// author by Liu Hao
// 2017/06/13

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480

struct WindowSize
{
	GLdouble width;
	GLdouble height;
};

class SystemSetting
{
	//Functions:
	public :
		static SystemSetting* GetInstance()
		{
			static SystemSetting* instance;
			if (!instance)
				instance = new SystemSetting;
			return instance;
		}

		//may Load From Lua Or Ini
		void InitLoadSettings()
		{
			m_size.width = DEFAULT_WIDTH;
			m_size.height = DEFAULT_HEIGHT;
			m_scale = 1.f;
		}

		void setResolution(float width,float height)
		{
			m_scale = width / DEFAULT_WIDTH;
			m_size.width = width;
			m_size.height = m_scale * DEFAULT_HEIGHT;
		}

		double getWidth()  { return m_size.width; }
		double getHeight() { return m_size.height; }
		float getScale() { return m_scale; }

	private:
		SystemSetting() {
			InitLoadSettings();
		};
		SystemSetting(const SystemSetting&);

//Attributes
	private:
		WindowSize m_size;
		float m_scale;		//scale window size 
};
