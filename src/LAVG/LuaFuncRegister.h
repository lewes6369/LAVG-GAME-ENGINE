#pragma once
//#include "LuaLoader.h"
#include "GlobalUtility.h"
#include "LuaLoader.h"
#include <iostream>
#include <windows.h>

// LuaFuncRegister
// author by Liu Hao
// 2017/06/14

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

typedef lua_CFunction LuaCSFunction;

struct RegisterFunction
{
	const char * name;	//Lua Function Name
	LuaCSFunction fPtr;	//C Function Ptr
};

class LuaFuncRegister
{
	public:	
		static LuaFuncRegister * getInstance()
		{
			static LuaFuncRegister * instance;
			if (!instance)
				instance = new LuaFuncRegister;
			return instance;
		}
		void RegisterFuncs(lua_State * L);

	private:
		LuaFuncRegister() {};
		LuaFuncRegister(const LuaFuncRegister&);
};