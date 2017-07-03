#pragma once
#include "LuaBaseValue.h"
// Lua Base Value
// author by Liu Hao
// 2017/06/14

//double Value Type
class LuaDoubleValue :public LuaBaseValue
{
	public:
		virtual std::string toString() {
			return std::to_string(getDoubleValue());
		};
		double getDoubleValue() { 
			return *static_cast<double*>(m_data); };
};