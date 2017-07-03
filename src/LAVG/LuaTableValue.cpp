#include "LuaTableValue.h"

LuaTableValue::LuaTableValue()
{
	if (m_data)
		free(m_data);
	m_data = nullptr;

	m_map.clear();
};

std::string LuaTableValue::toString() {
	std::string str("table:\n");
	for (auto iter : m_map)
		str += iter.first + " : " + iter.second->toString() + "\n";

	return str;
};

void LuaTableValue::insertValue(std::string key, std::shared_ptr<LuaBaseValue> ValueType)
{
	m_map.insert(std::make_pair(key, ValueType));
}