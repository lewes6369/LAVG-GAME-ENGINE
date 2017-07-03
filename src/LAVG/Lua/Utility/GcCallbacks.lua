--[[
	class GcCallbacks
	execute callbacks on gc
]]

local Lplus = require "Lplus"
local newproxy = newproxy
local getmetatable = getmetatable
local setmetatable = setmetatable
local error = error
local pairs = pairs
local tostring = tostring
local type = type
local _G = _G
local _VERSION = _VERSION
local is_5_1 = (_VERSION == "Lua 5.1")
local _ENV = nil


local GcCallbacks = Lplus.Class()
do
	local def = GcCallbacks.define
	
	--
	-- utilities
	--
	local function clearTable (t)
		for k, _ in pairs(t) do
			t[k] = nil
		end
	end
	
	local createGcProxy
	if is_5_1 then
		function createGcProxy (callback)
			local proxy = newproxy(true)
			local meta = getmetatable(proxy)
			meta.__gc = callback
			return proxy
		end
	else
		function createGcProxy (callback)
			local proxy = {}
			local meta = {}
			meta.__gc = callback
			return setmetatable(proxy, meta)
		end
	end
	
	local function checkNonNil (obj, who, argIndex, errLevel)
		if obj == nil then
			error(([[bad argument #%d to %s in 'GcCallbacks' (Non-nil expected, got nil)]]):format(argIndex, who, type(obj)), errLevel+1)
		end
	end

	---------------------------------
	--
	-- public methods
	--
	---------------------------------
	
	--[[
		add callback on gc
		param callback: function to be called on gc
	]]
	def.method("function").add = function (self, callback)
		checkNonNil(callback, "add", 2, 2)

		self:checkGcProxy()
		local callbacks = self.m_callbacks
		callbacks[#callbacks+1] = callback
	end
	
	--[[
		call callbacks manually, and clear callbacks
	]]
	def.method().dispose = function (self)
		self.m_needDisposeErrMsg = nil
		
		local proxyCallback = self.m_proxyCallback   --调用callback函数
		if proxyCallback then
			proxyCallback()
		end
	end
	
	--[[
		set GcCallback to expect dispose
		If dispose is not invoked, GcCallback will report an error on gc
		
		param errMsg: add into error info when report error
	]]
	def.method("string").setNeedDispose = function (self, errMsg)
		self.m_needDisposeErrMsg = errMsg or ""
	end
	
	---------------------------------
	--
	-- END of public
	--
	---------------------------------

	def.field("dynamic").m_gcProxy = nil
	def.field("function").m_proxyCallback = nil
	def.field("table").m_callbacks = nil
	def.field("dynamic").m_needDisposeErrMsg = nil	-- nil or "string"
	
	-- create gc proxy if need
	def.method().checkGcProxy = function (self)
		if self.m_gcProxy == nil then
			self.m_callbacks = {}
			
			local function proxyCallback ()
				if self.m_needDisposeErrMsg then
					warn("dispose need to be invoked: " .. tostring(self.m_needDisposeErrMsg), 2)
				end
				
				local callbacks = self.m_callbacks
				for i = 1, #callbacks do
					local f = callbacks[i]
					if f then	--in case that dispose in callback
						f()
					end
				end
				clearTable(callbacks)
			end
			
			self.m_gcProxy = createGcProxy(proxyCallback) --实际上是空table,只是带有GC的metatable,GC的时候会调用CallBack
			self.m_proxyCallback = proxyCallback          --callback函数
		end
	end
end

return GcCallbacks.Commit()
