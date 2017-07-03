#pragma once
// Lua Loader
// author by Liu Hao
// 2017/06/12

#include <string>
#include "LuaIntValue.h"
#include "LuaDoubleValue.h"
#include "LuaStringValue.h"
#include "LuaTableValue.h"
#include "LuaBoolValue.h"
#include "LuaNilValue.h"
#include "LuaFuncRegister.h"
#include <iostream>
#include <list>
#include <memory>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
class LuaFuncRegister;
//LuaLoader Singleton
class LuaLoader
{
public:
	static LuaLoader* getLoader(){
		if (!instance)
			instance = new LuaLoader;
		return instance;
	}

	lua_State *GetLuaState() { return m_L;}

	bool lua_doFile(const char * filename, std::list<std::shared_ptr<LuaBaseValue>> *resultlist = NULL);
	bool lua_doFile(std::string filename, std::list<std::shared_ptr<LuaBaseValue>> *resultlist = NULL) { return lua_doFile(filename.c_str(), resultlist); };
	static int  lua_pcallWithDefErrHdl(int nargs, int nresults);

	//Call Lua Functions
	void OnInit();
	void OnWindowSizeChanged(int width, int height, double scale);
	void OnClickScreen(int button,int action, double posX, double poxY);
	void OnMouseMove(double posX, double poxY);
	void OnEnterMenu();

	// Get Lua Return
	void GetLuaReturnFromStack(int count, std::list<std::shared_ptr<LuaBaseValue>> &, int beginIdx = 1);
	
	//Destroys all objects in the given Lua state,Clear Lua State
	void destroyLoader() {
		if (m_L)
			lua_close(m_L);
		m_L = NULL;
	}

private:
	LuaLoader();
	LuaLoader(const LuaLoader&);

	friend class LuaFuncRegister;

	//Lua Pre Load Trunks
	void LuaPreLoad();

	//Lua State
	static lua_State* m_L;
	
	//Single Instance
	static LuaLoader* instance;

	//Register Instance
	LuaFuncRegister* m_Register;
};