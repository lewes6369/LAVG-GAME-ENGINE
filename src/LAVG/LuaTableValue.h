#pragma once
#include "LuaBaseValue.h"
#include <map>
#include <memory>
// Lua Table Value
// author by Liu Hao
// 2017/06/14

//table Value Type
class LuaTableValue :public LuaBaseValue
{
	public:
		//clear map value
		LuaTableValue();

		//Insert Value In map
		virtual void insertValue(std::string key, std::shared_ptr<LuaBaseValue> ValueType);

		virtual std::string toString();

		std::shared_ptr<LuaBaseValue> getTableValueByKey(std::string key) {
			return m_map[key] ? m_map[key] : nullptr;
		};

	protected:
		std::map<std::string, std::shared_ptr<LuaBaseValue>> m_map;
	
	private:
		//Table Don't Use the func, Use insertValue Instead
		virtual void setValue(void * data, int _size) { return; };
};