#pragma once
#include "LuaBaseValue.h"
// Lua Base Value
// author by Liu Hao
// 2017/06/14

//bool Value Type
class LuaBoolValue : public LuaBaseValue
{
	public:
		virtual std::string toString() {
			return std::string(getBoolValue()? "true":"false");
		};

		bool getBoolValue() {
			return *static_cast<bool*>(m_data);
		};
};