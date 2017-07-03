--[[
	此文件负责读取所有节点
]]
local list = list or {}

local LoadList = {
	"Lua/Config/node1.lua",
	--"Lua/Config/node2.lua",
	--add here
}

local mt_list = setmetatable({}, {__index=list, __newindex=function (t, k, v)
	if list[k] then error("replicated node Id: " .. tostring(k)) end
	list[k] = v
end})

do 
	for k,v in ipairs(LoadList) do
		local ret,temp = pcall(dofile, v)
		for _,node in ipairs(temp) do
			--add Node as Map
			mt_list[node.NodeID] = node
		end
	end
end

return list
