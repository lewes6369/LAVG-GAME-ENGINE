#pragma once
#include "LuaBaseValue.h"
// Lua Base Value
// author by Liu Hao
// 2017/06/14

//string Value Type
class LuaStringValue :public LuaBaseValue
{
	public:
		virtual std::string toString() {
			return std::string(getCStringValue());
		};
		const char* getCStringValue() { return static_cast<const char*>(m_data); };
};