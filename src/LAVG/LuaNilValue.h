#pragma once
#include "LuaBaseValue.h"
// Lua Nil Value
// author by Liu Hao
// 2017/06/14

//nil Value Type
class LuaNilValue : public LuaBaseValue
{
public:
	virtual std::string toString() {
		return std::string("is nil");
	};

};