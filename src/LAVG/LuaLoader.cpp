#include "LuaLoader.h"

lua_State *LuaLoader::m_L = NULL;
LuaLoader *LuaLoader::instance = NULL;

LuaLoader::LuaLoader() 
{
	m_L = luaL_newstate();
	m_Register = LuaFuncRegister::getInstance();
	LuaPreLoad();
};

int pcall_callback_err_fun(lua_State * L)
{
	std::cout << "pcall_callback_err_fun:" << std::endl;
	lua_Debug debug = {};
	int ret = lua_getstack(L, 2, &debug);
	lua_getinfo(L, "Sln", &debug);

	std::string err = lua_tostring(L, -1);
	lua_pop(L, 1);
	std::stringstream msg;
	msg << debug.short_src << ":line " << debug.currentline;
	if (debug.name != 0) {
		msg << "(" << debug.namewhat << " " << debug.name << ")";
	}

	msg << " [" << err << "]";
	std::cout << msg.str() << std::endl;
	lua_pushstring(L, msg.str().c_str());
	return 1;
}

bool LuaLoader::lua_doFile(const char * filename,std::list<std::shared_ptr<LuaBaseValue>> *resultlist /*= NULL*/)
{
	int beginTop = lua_gettop(m_L);
	int bRet = luaL_loadfile(m_L, filename);
	if (bRet != 0 )
	{
		std::cout << "Load File Error: " << bRet << std::endl;
		return false;
	}

	int oldTop = lua_gettop(m_L);
	bRet = lua_pcallWithDefErrHdl(0, LUA_MULTRET);
	if (bRet != 0)
	{
		if (lua_isstring(m_L, -1))
			std::cout << lua_tostring(m_L, -1) << std::endl;
		else
			std::cout << "Load File Error: " << bRet << std::endl;

		return false;
	}

	int newTop = lua_gettop(m_L);
	if (newTop >= oldTop)
	{
		if (resultlist)
		{
			resultlist->clear();
			// get All Return in resultlist
			GetLuaReturnFromStack(newTop - oldTop + 1, *resultlist);

			std::cout << "Lua Do File result:" << std::endl;
			for (auto p : *resultlist)
			{
				std::cout << p->toString() << std::endl;
			}
		}

		//revert the top no matter either get Result or not 
		lua_settop(m_L, beginTop);
		return true;
	}
	else
		return true;
}

std::shared_ptr<LuaBaseValue> getLuaSimpleValue(lua_State* L, int index)
{
	if (lua_isboolean(L, index))
	{
		std::shared_ptr<LuaBoolValue> ptr(new LuaBoolValue);
		bool b = lua_toboolean(L, index);
		ptr->setValue(&b, sizeof(bool));
		return ptr;
	}
	else if (lua_isinteger(L, index))
	{
		std::shared_ptr<LuaIntValue> ptr(new LuaIntValue);
		int b = lua_tointeger(L, index);
		ptr->setValue(&b, sizeof(int));
		return ptr;
	}
	else if (lua_isnumber(L, index))
	{
		std::shared_ptr<LuaDoubleValue> ptr(new LuaDoubleValue);
		double b = lua_tonumber(L, index);
		ptr->setValue(&b, sizeof(double));
		return ptr;
	}
	else if (lua_isstring(L, index))
	{
		std::shared_ptr<LuaStringValue> ptr(new LuaStringValue);
		const char * b = lua_tostring(L, index);
		std::string c(b);
		c.append(1,'\0');
		ptr->setValue((void *)c.c_str(), c.length());
		return ptr;
	}
	else if (lua_isnil(L, index))
		return std::shared_ptr<LuaNilValue>(new LuaNilValue);

	return nullptr;
};

std::shared_ptr<LuaTableValue> getLuaTableValue(lua_State* L, int index)
{
	std::shared_ptr<LuaTableValue> ptr(new LuaTableValue);
	lua_pushnil(L);  /* first key */
					   /*table next func*/
	while (lua_next(L, index) != 0) {
		// make a copy for the key on the top
		// otherwise may change the value at the given index
		// this confuses the next call to lua_next.
		int count = lua_gettop(L);
		lua_pushvalue(L, -2); //table key value key
		std::string key = lua_tostring(L, -1);//table key value key

		if (lua_istable(L, -2))//table key tableIn key
		{
			//if Top is table need recursion
			lua_pop(L, 1); //table key tableIn
			//top will push a nil,so index is -2
			std::shared_ptr<LuaBaseValue> valuePtr = getLuaTableValue(L, -2);
			lua_pop(L, 1);// table key
			ptr->insertValue(key, valuePtr);
		}
		else
		{
			std::shared_ptr<LuaBaseValue> valuePtr = getLuaSimpleValue(L, -2);
			lua_pop(L, 2); //table key 
			ptr->insertValue(key, valuePtr);
		}
	}

	return ptr;
}

void LuaLoader::GetLuaReturnFromStack(int count, std::list<std::shared_ptr<LuaBaseValue>>& resultList, int beginIdx /* = 1*/)
{
	for (int i = beginIdx; i <= beginIdx + count - 1; i++) {
		if (lua_istable(m_L, i))
		{
			std::shared_ptr<LuaTableValue> ptr = getLuaTableValue(m_L, i);
			resultList.push_back(ptr);
		}
		else
		{
			std::shared_ptr<LuaBaseValue> ptr = getLuaSimpleValue(m_L, i);
			resultList.push_back(ptr);
		}
	}
}

void LuaLoader::LuaPreLoad()
{
	luaL_openlibs(m_L);
	m_Register->RegisterFuncs(m_L);
}

//Call Lua Functions
void LuaLoader::OnWindowSizeChanged(int width, int height, double scale)
{
	int oldTop = lua_gettop(m_L);
	lua_getglobal(m_L, "OnWindowSizeChanged");
	lua_pushinteger(m_L, width);
	lua_pushinteger(m_L, height);
	lua_pushnumber(m_L, scale);
	lua_pcallWithDefErrHdl(3, 0);
	lua_settop(m_L,oldTop);
}


void LuaLoader::OnClickScreen(int button, int action, double posX, double poxY)
{
	int oldTop = lua_gettop(m_L);
	lua_getglobal(m_L, "OnClickScreen");
	lua_pushinteger(m_L, button);
	lua_pushinteger(m_L, action);
	lua_pushnumber(m_L, posX);
	lua_pushnumber(m_L, poxY);
	lua_pcallWithDefErrHdl(4, 0);
	lua_settop(m_L, oldTop);
}

void LuaLoader::OnMouseMove(double posX, double poxY)
{
	int oldTop = lua_gettop(m_L);
	lua_getglobal(m_L, "OnMouseMove");
	lua_pushnumber(m_L, posX);
	lua_pushnumber(m_L, poxY);
	lua_pcallWithDefErrHdl(2, 0);
	lua_settop(m_L, oldTop);
}

void LuaLoader::OnInit()
{
	lua_doFile("Lua/main.lua");		//Load Entry File 
}

void LuaLoader::OnEnterMenu()
{
	int oldTop = lua_gettop(m_L);
	lua_getglobal(m_L, "OnEnterMenu");
	lua_pcallWithDefErrHdl(0, 0);
	lua_settop(m_L, oldTop);
}

int LuaLoader::lua_pcallWithDefErrHdl(int nargs, int nresults)
{
	int oldTop = lua_gettop(m_L);
	lua_pushcfunction(m_L, pcall_callback_err_fun);
	int pos_err = oldTop - nargs;
	lua_insert(m_L, pos_err);
	return lua_pcall(m_L, nargs, nresults, pos_err);
}