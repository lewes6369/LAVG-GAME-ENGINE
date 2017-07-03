#pragma once
#include "LuaBaseValue.h"
// Lua Base Value
// author by Liu Hao
// 2017/06/14

//int Value Type
class LuaIntValue :public LuaBaseValue
{
	public:
		virtual std::string toString() {
			return std::to_string(getIntValue());
		};
		int getIntValue() { return *static_cast<int*>(m_data); };
};