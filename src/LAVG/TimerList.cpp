#include "TimerList.h"
#include "LuaLoader.h"

int TimerList::_uniqueid = 1;
int TimerList::total_count = 0;

TimerList::TimerList()
{
	m_List = list<Timer>();
	m_TempList = list<Timer>();
	m_TempDelList = list<int>();
	m_bTick = false;
}

int TimerList::AddTimer(float ttl, bool bOnce, int cb, int cbparam)
{
	if (cb < 0)
		return -1;

	Timer tm;
	tm.id = _uniqueid;
	tm.ttl = ttl;
	tm.total_played = 0;
	tm.end_time = glfwGetTime() + ttl;
	tm.callback = cb;
	tm.cbparam = cbparam;
	tm.bOnce = bOnce;

	if (m_bTick)
		m_TempList.push_back(move(tm));
	else
	{
		m_List.push_back(move(tm));
		total_count++;
	}
	return _uniqueid++;
}

int TimerList::DelayTimer(int id, float ttl, float maxDurationToRemove)
{
	if (m_bTick)
		return -1; // Be in Timer self ticking..

	lua_State *L = LuaLoader::getLoader()->GetLuaState();
	for (auto iter = m_List.begin(); iter !=m_List.end(); iter++)
	{
		Timer tm = *iter;
		if (tm.id == id)
		{
			auto curTime = glfwGetTime();

			if (maxDurationToRemove >= 0)
			{
				if (tm.total_played > 0 && tm.total_played >= maxDurationToRemove)
				{
					luaL_unref(L, LUA_REGISTRYINDEX, tm.callback);
					luaL_unref(L, LUA_REGISTRYINDEX, tm.cbparam);
					iter = m_List.erase(iter);
					total_count--;
					return -2; // found but reach maxDuration and Removed
				}

				if (tm.total_played + ttl > maxDurationToRemove)
					ttl = maxDurationToRemove - tm.total_played;
			}

			tm.end_time = curTime + ttl;
			*iter = std::move(tm);
			return id;
		}
	}

	return 0; // not found
}

void TimerList::RemoveTimer(int id)
{
	if (m_bTick)
		m_TempDelList.push_back(id);
	else
	{
		lua_State *L = LuaLoader::getLoader()->GetLuaState();
		for (auto iter = m_List.begin(); iter != m_List.end(); iter++)
		{
			Timer tm = *iter;
			if (tm.id == id)
			{
				luaL_unref(L, LUA_REGISTRYINDEX, tm.callback);
				luaL_unref(L, LUA_REGISTRYINDEX, tm.cbparam);
				iter = m_List.erase(iter);
				total_count--;
				return;
			}
		}
	}
}

void TimerList::SetTimer(int id, float fNextPlayTime)
{
	for (auto iter = m_List.begin(); iter != m_List.end(); iter++)
	{
		if (iter->id == id) {
			iter->end_time = glfwGetTime() + fNextPlayTime;
			return;
		}
	}
}

void TimerList::ResetTimer(int id, float ttl_new/*<0 means not change*/)
{
	for (auto iter = m_List.begin(); iter != m_List.end(); iter++)
	{
		if (iter->id == id) {
			iter->total_played = 0;

			if (ttl_new >= 0)
				iter->ttl = ttl_new;

			iter->end_time = glfwGetTime() + iter->ttl;
			return;
		}
	}
}

void TimerList::Tick(double deltaTime)
{
	if (m_List.size() == 0)
		return;

	float cur = glfwGetTime();;
	float delta = deltaTime;
	m_bTick = true;

	lua_State *L = LuaLoader::getLoader()->GetLuaState();
	for (auto iter = m_List.begin(); iter != m_List.end(); iter++)
	{
		Timer tm = *iter;
		tm.total_played = tm.total_played + delta;

		if (tm.end_time <= cur)
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, tm.callback); // callback
			//lua_pushvalue(L, -1);	//cb, cb

			if (tm.cbparam != LUA_NOREF)
				lua_rawgeti(L, LUA_REGISTRYINDEX, tm.cbparam); // callback, cbparam
			else
				lua_pushnil(L);

			int bRet = LuaLoader::lua_pcallWithDefErrHdl(1, 0); //->cb, [err] 
			if (bRet == 0)
				lua_pop(L, 1); //empty
			else
			{
				if (lua_isstring(L, -1))
					std::cout << lua_tostring(L, -1) << std::endl;
				else
					std::cout << "Timer Func Error: " << bRet << std::endl;
			}

			if (tm.bOnce)
			{

				luaL_unref(L, LUA_REGISTRYINDEX, tm.callback);
				luaL_unref(L, LUA_REGISTRYINDEX, tm.cbparam);
				iter = m_List.erase(iter);
				total_count--;
			}
			else
			{
				tm.end_time = cur + tm.ttl;
				*iter = std::move(tm);
			}
		}
	}

	m_bTick = false;
	if (m_TempList.size() > 0)
	{
		std::move(m_TempList.begin(), m_TempList.end(), m_List.end());
		total_count += m_TempList.size();
		m_TempList.clear();
	}

	if (m_TempDelList.size() > 0)
	{
		for(auto id:m_TempDelList)
			RemoveTimer(id);

		m_TempDelList.clear();
	}
}

void TimerList::Clear()
{
	if (m_List.size() == 0)
		return;

	total_count -= m_List.size();

	lua_State *L = LuaLoader::getLoader()->GetLuaState();
	if (!L)
	{
		m_List.clear();
		return;
	}

	for (auto tm : m_List)
	{
		luaL_unref(L, LUA_REGISTRYINDEX, tm.callback);
		luaL_unref(L, LUA_REGISTRYINDEX, tm.cbparam);
	}

	m_List.clear();
}