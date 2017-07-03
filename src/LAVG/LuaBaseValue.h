#pragma once
#include <string>
// Lua Base Value
// author by Liu Hao
// 2017/06/14

//abstract base Value Type
//template <typename T>
class LuaBaseValue
{
	public:
		virtual void setValue(void * data,int _size) { 
			if (m_data)
				free(m_data);
			m_data = malloc(_size);
			memcpy(m_data, data,_size); 
		};

		virtual ~LuaBaseValue() 
		{
			if (m_data)
				free(m_data);
			m_data = nullptr;
		};

		//make it be a abstract class
		virtual std::string toString() = 0;
	protected:
		void * m_data;	//data
};