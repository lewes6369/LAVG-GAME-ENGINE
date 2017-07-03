#include "LuaFuncRegister.h"

static void SetConsoleColor(WORD wAttribute)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, wAttribute);
}

inline std::ostream&  COLOR_DEF(std::ostream& ostr)
{
	SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	return ostr;
}

inline std::ostream&  COLOR_RED(std::ostream& ostr)
{
	SetConsoleColor(FOREGROUND_RED);//| FOREGROUND_INTENSITY);
	return ostr;
}

inline std::ostream&  COLOR_YELLOW(std::ostream& ostr)
{
	SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_RED);//| FOREGROUND_INTENSITY);
	return ostr;
}

void printStack(int count)
{
	std::list<std::shared_ptr<LuaBaseValue>> resultList;
	LuaLoader::getLoader()->GetLuaReturnFromStack(count, resultList);
	
	for (auto p : resultList)
		std::cout << p->toString() << std::endl;
}

void paramsError(lua_State *L, int idx, int needType)
{
	std::cout << "wrong input in params"<< idx << " :" << lua_typename(L, lua_type(L, idx)) << std::endl;
	std::cout << "needType :"<< lua_typename(L, needType) << std::endl;
}

//[LuaCSFunction  LuaPrint]
static int LuaPrint(lua_State* L) {
	int nargs = lua_gettop(L);
	std::cout << COLOR_DEF << "Lua Print:";
	printStack(nargs);
	std::cout << std::endl;
	return 0;
}

//[LuaCSFunction  LuaAssert]
static int LuaAssert(lua_State* L) {
	std::cout << COLOR_RED << "Lua Assert or Error:" << std::endl;
	luaL_dostring(L, "return debug.traceback()");
	int nargs = lua_gettop(L);
	printStack(nargs);
	std::cout << COLOR_DEF << std::endl;
	return 0;
}

//[LuaCSFunction  LuaAssert]
static int LuaWarn(lua_State* L) {
	std::cout << COLOR_YELLOW<< "Lua Warn:" << std::endl;
	luaL_dostring(L, "return debug.traceback()");
	int nargs = lua_gettop(L);
	printStack(nargs);
	std::cout << COLOR_DEF << std::endl;
	return 0;
}

// Get Texture
//[LuaCSFunction  GetTexIdByNameFromCache]
static int GetTexIdByNameFromCache(lua_State* L)
{
	int count = lua_gettop(L);
	if (count == 1 && lua_isstring(L, -1))
	{
		const char *imgName = lua_tostring(L,-1);
		int TexId = glb_GetImageRender()->GetTexIdByNameFromCache(imgName);
		lua_pop(L, 1);
		lua_pushinteger(L, TexId);
		return 1;
	}
	else
		assert("GetTexIdByNameFromCache wrong params count!");

	return 0;
}

// Add Texture To Cache
//[LuaCSFunction  AddTextureToCache]
static int AddTextureToCache(lua_State* L)
{
	int count = lua_gettop(L);
	//printStackType(L);
	if (count == 2 && lua_isinteger(L, -2) && lua_isstring(L,-1))
	{
		int TexId = lua_tointeger(L, -2);
		const char *imgName = lua_tostring(L, -1);
		glb_GetImageRender()->AddTextureToCache(TexId,imgName);
		lua_pop(L, 2);
		return 0;
	}
	else
		assert("AddTextureToCache wrong params!");

	return 0;
}

//Render Texture in Rect
//[LuaCSFunction  RenderTextureInRect]
static int RenderTextureInRect(lua_State* L)
{
	int count = lua_gettop(L);
	if (count > 2 )
	{
		if (!lua_isnumber(L, 1))
		{
			paramsError(L, 1, LUA_TNUMBER);
			return 0;
		}
		int TexId = lua_tointeger(L, 1);
		if (!lua_isuserdata(L, 2))
		{
			paramsError(L, 2, LUA_TUSERDATA);
			return 0;
		}
		void * data = lua_touserdata(L, 2);
		cv::Vec2i * win_lt = static_cast <cv::Vec2i *>(data);
		if (!lua_isuserdata(L, 3))
		{
			paramsError(L, 3, LUA_TUSERDATA);
			return 0;
		}
		data = lua_touserdata(L, 3);
		cv::Vec2i * win_rb = static_cast <cv::Vec2i *>(data);
		lua_pop(L,3);
		cv::Vec2f * tex_lb = NULL,* tex_rt =NULL;
		if (lua_isuserdata(L, 1))
		{
			data = lua_touserdata(L, 1);
			tex_lb = static_cast <cv::Vec2f *>(data);
		}
		if (lua_isuserdata(L, 2))
		{
			data = lua_touserdata(L, 2);
			tex_rt = static_cast <cv::Vec2f *>(data);
		}
		
		if (tex_lb && tex_rt)
			glb_GetImageRender()->RenderTextureInRect(TexId, *win_lt, *win_rb, *tex_lb, *tex_rt);
		else
			glb_GetImageRender()->RenderTextureInRect(TexId, *win_lt, *win_rb);

		lua_settop(L, 0);
		return 0;
	}
	else
		assert("RenderTextureInRect wrong params count!");

	return 0;
}

//Render Text on Pos
//[LuaCSFunction  RenderText]
static int RenderText(lua_State* L)
{
	int count = lua_gettop(L);
	if (count > 2)
	{
		if (!lua_isstring(L, 1))
		{
			paramsError(L, 1, LUA_TSTRING);
			return 0;
		}
		WCHAR* str = (WCHAR *)lua_tostring(L, 1);
		if (!lua_isuserdata(L, 2))
		{
			paramsError(L, 2, LUA_TUSERDATA);
			return 0;
		}
		void * data = lua_touserdata(L, 2);
		cv::Vec2i * win_lt = static_cast <cv::Vec2i *>(data);
		if (!lua_isuserdata(L, 3))
		{
			paramsError(L, 3, LUA_TUSERDATA);
			return 0;
		}
		data = lua_touserdata(L, 3);
		cv::Vec2i * win_rb = static_cast <cv::Vec2i *>(data);

		float endPosX, endPosY;
		glb_GetImageRender()->RenderText(str,*win_lt, *win_rb, &endPosX, &endPosY);
		lua_pop(L, 3);
		lua_pushnumber(L, endPosX);
		lua_pushnumber(L, endPosY);
		return 2;
	}
	else
		assert("RenderTextureInRect wrong params count!");

	return 0;
}

static int Vector2i(lua_State* L)
{
	int count = lua_gettop(L);
	if (count == 2 && lua_isnumber(L, -2) && lua_isnumber(L, -1))
	{
		cv::Vec2i *vec = new cv::Vec2i(lua_tointeger(L,-2), lua_tointeger(L,-1));
		lua_pop(L, 2);
		lua_pushlightuserdata(L, vec);
		return 1;
	}
	else
	{
		luaL_dostring(L, "return debug.traceback()");
		std::cout << COLOR_RED << "Vector2i wrong params!" << std::endl;
		printStack(count + 1);
		std::cout << COLOR_DEF << std::endl;
	}
	return 0;
}

static int Vector2f(lua_State* L)
{
	int count = lua_gettop(L);
	if (count == 2 && lua_isnumber(L, -2) && lua_isnumber(L, -1))
	{
		cv::Vec2i *vec = new cv::Vec2i(lua_tonumber(L, -2), lua_tonumber(L, -1));
		lua_pop(L, 2);
		lua_pushlightuserdata(L, vec);
		return 1;
	}
	else
	{
		luaL_dostring(L, "return debug.traceback()");
		std::cout << COLOR_RED << "Vector2F wrong params!" << std::endl;
		printStack(count + 1);
		std::cout << COLOR_DEF << std::endl;
	}
	return 0;
}

//SwapBufferForRender RenderWindow
//[LuaCSFunction  RenderWindow]
static int RenderWindow(lua_State* L)
{
	glfwSwapBuffers(glb_GetGLFWWindow());
	return 0;
}

//SwapBufferForRender return texId,bsuc
//[LuaCSFunction RenderWindow]
static int LoadTextureFromImage(lua_State* L)
{
	int count = lua_gettop(L);
	if (count >= 2)
	{
		if (!lua_isstring(L, 1))
		{
			paramsError(L, 1, LUA_TSTRING);
			return 0;
		}
		const char * filePath = lua_tostring(L, 1);
		if (!lua_isinteger(L, 2))
		{
			paramsError(L, 2, LUA_TNUMBER);
			return 0;
		}
		GLuint texId = lua_tointeger(L, 2);
		bool bSave = false;
		if (count == 3 && lua_isboolean(L, 3))
			bSave = lua_toboolean(L, 3);
		
		bool bSuc = glb_GetImageReader()->LoadTextureFromImage(filePath, texId, bSave);
		lua_settop(L, 0);

		lua_pushinteger(L,texId);
		lua_pushboolean(L, bSuc);
		return 2;
	}
	else
		assert("LoadTextureFromImage wrong params count!");

	return 0;
}

//ReloadTextureImage return TexId,suc
//[LuaCSFunction ReloadTextureImage]
static int ReloadTextureImage(lua_State* L)
{
	int count = lua_gettop(L);
	if (count == 2)
	{
		if (!lua_isstring(L, 1))
		{
			paramsError(L, 1, LUA_TSTRING);
			return 0;
		}
		const char * filePath = lua_tostring(L, 1);
		if (!lua_isinteger(L, 2))
		{
			paramsError(L, 2, LUA_TNUMBER);
			return 0;
		}
		GLuint texId = lua_tointeger(L, 2);
	
		bool bSuc = glb_GetImageReader()->ReloadTextureImage(filePath, texId);
		lua_settop(L, 0);

		lua_pushinteger(L, texId);
		lua_pushboolean(L, bSuc);
		return 2;
	}
	else
		assert("ReloadTextureImage wrong params count!");

	return 0;
}

//DeleteTexture
//[LuaCSFunction DeleteTexture]
static int DeleteTexture(lua_State* L)
{
	int count = lua_gettop(L);
	if (count == 1)
	{
		if (!lua_isinteger(L, 1))
		{
			paramsError(L, 1, LUA_TNUMBER);
			return 0;
		}
		GLuint texId = lua_tointeger(L, 1);
		glDeleteTextures(1, &texId);
		return 0;
	}
	else
		assert("DeleteTexture wrong params count!");

	return 0;
}

//IsTextureExist
//[LuaCSFunction IsTextureExist]
static int IsTextureExist(lua_State* L)
{
	int count = lua_gettop(L);
	if (count == 1)
	{
		if (!lua_isinteger(L, 1))
		{
			paramsError(L, 1, LUA_TNUMBER);
			return 0;
		}
		GLuint texId = lua_tointeger(L, 1);
		bool isExist =  glIsTexture(texId);
		lua_pushboolean(L, isExist);
		return 0;
	}
	else
		assert("IsTextureExist wrong params count!");

	return 0;
}


//BeginRender
//[LuaCSFunction BeginRender]
static int BeginRender(lua_State* L)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, glb_GetSystemSetting()->getWidth(), glb_GetSystemSetting()->getHeight(), 0.0, -1.0, 1.0);

	return 0;
}

//AddTimer
//[LuaCSFunction AddTimer]
static int AddTimer(lua_State* L)
{
	int count = lua_gettop(L);
	if (count >= 3)
	{
		if (!lua_isnumber(L, 1))
		{
			paramsError(L, 1, LUA_TNUMBER);
			return 0;
		}
		float ttl = lua_tonumber(L, 1);
		if (!lua_isboolean(L, 2))
		{
			paramsError(L, 2, LUA_TBOOLEAN);
			return 0;
		}
		bool bOnce = lua_tointeger(L, 2);
		
		lua_pushvalue(L, 3);
		int callbackRef = luaL_ref(L, LUA_REGISTRYINDEX);

		int cbparam;
		if (count == 4 && !lua_isnil(L, 4))
		{
			lua_pushvalue(L, 4);
			cbparam = luaL_ref(L, LUA_REGISTRYINDEX);
		}
		else
			cbparam = LUA_NOREF;

		int id = GameManager::GetInstance()->AddTimer(ttl, bOnce, callbackRef, cbparam);

		lua_settop(L, 0);
		lua_pushinteger(L, id);

		return 1;
	}
	else
		assert("AddTimer wrong params count!");

	return 0;
}

//RemoveTimer
//[LuaCSFunction RemoveTimer]
static int RemoveTimer(lua_State* L)
{
	int count = lua_gettop(L);
	if (count == 1)
	{
		if (!lua_isinteger(L, 1))
		{
			paramsError(L, 1, LUA_TNUMBER);
			return 0;
		}
		int id = lua_tointeger(L, 1);
		GameManager::GetInstance()->RemoveTimer(id);
		return 0;
	}
	else
		assert("DeleteTexture wrong params count!");

	return 0;
}

//ResetTimer
//[LuaCSFunction ResetTimer]
static int ResetTimer(lua_State* L)
{
	int count = lua_gettop(L);
	if (count == 1)
	{
		if (!lua_isinteger(L, 1))
		{
			paramsError(L, 1, LUA_TNUMBER);
			return 0;
		}
		int id = lua_tointeger(L, 1);
		GameManager::GetInstance()->ResetTimer(id);
		return 0;
	}
	else
		assert("DeleteTexture wrong params count!");

	return 0;
}

//SetTimer
//[LuaCSFunction SetTimer]
static int SetTimer(lua_State* L)
{
	int count = lua_gettop(L);
	if (count == 2)
	{
		if (!lua_isinteger(L, 1))
		{
			paramsError(L, 1, LUA_TNUMBER);
			return 0;
		}
		int id = lua_tointeger(L, 1);
		if (!lua_isnumber(L, 2))
		{
			paramsError(L, 2, LUA_TNUMBER);
			return 0;
		}
		float fNextPlayTime = lua_tonumber(L, 2);
		GameManager::GetInstance()->SetTimer(id,fNextPlayTime);
		return 0;
	}
	else
		assert("DeleteTexture wrong params count!");

	return 0;
}

int Utf8ToUnicode(const char * utf8, WCHAR * unicode, int length)
{
	char c;
	int i = 0;
	--length;
	while (c = *utf8) 
	{
		if (c & 0x80) 
		{
			if (c & 0x20) 
			{
				if (i < length)
					unicode[i] = ((utf8[0] & 0xf) << 12) | ((utf8[1] & 0x3f) << 6) | ((utf8[2] & 0x3f));
			
				++i;
				utf8 += 3;
			}
			else
			{
				if (i < length)
					unicode[i] = ((utf8[0] & 0x1f) << 6) | ((utf8[1] & 0x3f));
				++i;
				utf8 += 2;
			}
		}else
		{
			if (i < length)
				unicode[i] = c;
			++i;
			++utf8;
		}
	}

	if (i <= length) {
		unicode[i] = 0;
	}

	return i + 1;
}

//Utf8ToUnicode
//[LuaCSFunction Utf8ToUnicode]
static int Utf8ToUnicode(lua_State* L)
{
	WCHAR a[256];
	WCHAR* unicode = a;
	int length = 0;
	const char * utf8;

	utf8 = luaL_checkstring(L, 1);
	length = Utf8ToUnicode(utf8, unicode, 256);
	if (length <= 256) {
		lua_pushlstring(L, (const char *)unicode, length * 2);
	}
	else
	{
		unicode = (WCHAR*)malloc(length);
		Utf8ToUnicode(utf8, unicode, length);
		lua_pushlstring(L, (const char *)unicode, length * 2);
		free(unicode);
	}

	return 1;
}

//Global Functions
static const struct RegisterFunction Registers[] = {
	{ "print", LuaPrint },
	{ "warn", LuaWarn},
	{ "assert", LuaAssert},
	{ "error", LuaAssert},
	{ "LAVG_GetTexIdByNameFromCache",GetTexIdByNameFromCache },
	{ "LAVG_AddTextureToCache",AddTextureToCache },
	{ "LAVG_RenderTextureInRect",RenderTextureInRect },
	{ "LAVG_RenderText",RenderText },
	{ "LAVG_Vector2i",Vector2i },
	{ "LAVG_Vector2f",Vector2f },
	{ "LAVG_RenderWindow",RenderWindow},
	{ "LAVG_LoadTextureFromImage", LoadTextureFromImage },
	{ "LAVG_ReloadTextureImage",ReloadTextureImage },
	{ "LAVG_DeleteTexture",DeleteTexture},
	{ "LAVG_BeginRender",BeginRender },
	{ "LAVG_AddTimer",AddTimer},
	{ "LAVG_RemoveTimer",RemoveTimer },
	{ "LAVG_ResetTimer",ResetTimer },
	{ "LAVG_SetTimer",SetTimer },
	{ "LAVG_IsTextureExist",IsTextureExist },
	{ "LAVG_Utf8ToUnicode", Utf8ToUnicode },
};

void LuaFuncRegister::RegisterFuncs(lua_State * L)
{
	for (int i = 0; i < sizeof(Registers) / sizeof(RegisterFunction); i++)
	{
		RegisterFunction rf = Registers[i];
		lua_register(L, rf.name, rf.fPtr);
	}
}