--[[
	utility to disable global variable accessing
	
	usage:
		global.disable ()
]]

--local setfenv = setfenv
local error = error
local setmetatable = setmetatable
local debug = debug
local _G = _G

--Lua 5.2 remove the setfenv func
local setfenv = setfenv or function(f, t)
     f = (type(f) == 'function' and f or debug.getinfo(f + 1, 'f').func)
     local name
     local up = 0
     repeat
         up = up + 1
         name = debug.getupvalue(f, up)
     until name == '_ENV' or name == nil
     if name then
         debug.upvaluejoin(f, up, function() return name end, 1)  -- use unique upvalue
         debug.setupvalue(f, up, t)
     end
end

local global = {}

local function internalDisable (bDisableRead, bDisableWrite, bForThread, fLevel)
	local realFLevel = fLevel+1
	
	if not bDisableRead and not bDisableWrite then	--enable 
		if bForThread then
			setfenv(0, _G) --回到当前环境
		end
		setfenv(realFLevel, _G) 
	else
		local meta = {}
		
		if bDisableRead then
			function meta.__index (_, name)
				Debug.LogWarning(debug.traceback("bad access to '" .. name .. "' (global variable reading disabled)"), 2)
				return _G[name]
			end
		else
			meta.__index = _G
		end
		
		if bDisableWrite then
			function meta.__newindex (_, name, value)
				Debug.LogWarning(debug.traceback("bad access to '" .. name .. "' (global variable writing disabled)"), 2)
				_G[name] = value
			end
		else
			meta.__newindex = _G
		end
		
		local new_G = setmetatable({}, meta)
		if bForThread then
			setfenv(0, new_G)
		end
		setfenv(realFLevel, new_G)
	end
end

function global.disable ()
	internalDisable(true, true, false, 2)
end

function global.disableWrite ()
	internalDisable(false, true, false, 2)
end

function global.enable ()
	internalDisable(false, false, false, 2)
end

function global.disableForThread ()
	internalDisable(true, true, true, 2)
end

function global.disableWriteForThread ()
	internalDisable(false, true, true, 2)
end

function global.enableForThread ()
	internalDisable(false, false, true, 2)
end

global._G = _G

return global
