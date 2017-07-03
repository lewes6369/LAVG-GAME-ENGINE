--[[
	class AnonymousEvent
	With AnonymousEvent, event is sended to handlers who are registered with the same argument type as the event
]]

local Lplus = require "Lplus"
local pairs = pairs
local error = error
local type = type
local Object = Lplus.Object
local Lplus_typeof = Lplus.typeof
local Lplus_isTypeTable = Lplus.isTypeTable
local Lplus_is = Lplus.is
local GcCallbacks = require "Utility.GcCallbacks"
local _ENV = nil

local AnonymousEventManager = Lplus.Class()
do
	local def = AnonymousEventManager.define
	
	--
	-- utilities
	--
	local function clearTable (t)
		for k, _ in pairs(t) do
			t[k] = nil
		end
	end

	local function checkObject (obj, who, argIndex, errLevel)
		if not Lplus_is(obj, Object) then
			error(([[bad argument #%d to %s in 'AnonymousEventManager' (Lplus Object expected, got %s)]]):format(argIndex, who, type(obj)), errLevel+1)
		end
	end

	local function checkTypeTable (typeTable, who, argIndex, errLevel)
		if not Lplus_isTypeTable(typeTable) then
			error(([[bad argument #%d to %s in 'AnonymousEventManager' (type table expected, got %s)]]):format(argIndex, who, type(typeTable)), errLevel+1)
		end
	end

	local function checkSimpleType (value, who, argIndex, needType, errLevel)
		if type(value) ~= needType then
			error(([[bad argument #%d to %s in 'AnonymousEventManager' (%s expected, got %s)]]):format(argIndex, who, needType, type(value)), errLevel+1)
		end
	end

	local function findNewIndexerPos (chain)
		local pos = -1
		while chain[pos] ~= nil do
			pos = pos - 2
		end
		return pos
	end
	
	local function findCurrentIndexerPos (chain)
		local newPos = findNewIndexerPos(chain)
		if newPos == -1 then
			return nil	--nobody is traversal the chain
		else
			return newPos + 2
		end
	end
	
	local function addToChain (chain, handler)
		chain[#chain+1] = handler
	end
	
	local function removeFromChain (chain, handler)
		for iRemove = 1, #chain do
			if chain[iRemove] == handler then
				local indexerPos = findCurrentIndexerPos(chain)
				if indexerPos then
					for pos = indexerPos, -1, 2 do
						local nextIndex = chain[pos]
						if iRemove < nextIndex then
							chain[pos] = nextIndex - 1
						end
						local lastIndex = chain[pos-1]
						if iRemove <= lastIndex then
							chain[pos-1] = lastIndex - 1
						end
					end
				end
				table.remove(chain, iRemove)
				break
			end
		end
	end
	
	local function raiseEvent_internal (self, sender, arg, argTypeTable)
		local handlerChain = self.m_handlerChainMap[argTypeTable]
		if handlerChain then
			local indexerPos = findNewIndexerPos(handlerChain)
			local lastIndexerPos = indexerPos - 1
			handlerChain[indexerPos] = 1
			handlerChain[lastIndexerPos] = #handlerChain
			-- invoke all
			while true do
				local iCurrent = handlerChain[indexerPos]
				if iCurrent > handlerChain[lastIndexerPos] then
					break
				end
				handlerChain[indexerPos] = iCurrent + 1
				
				local handler = handlerChain[iCurrent]
				handler(sender, arg)
			end
			handlerChain[indexerPos] = nil
			handlerChain[lastIndexerPos] = nil
		end
	end
	
	local function raiseEventIncludingBase_internal (self, sender, arg, argTypeTable)
		local baseTypeTable = Lplus_typeof(argTypeTable):getBaseTypeTable()
		--recursively invoke base type
		if baseTypeTable ~= Object then
			raiseEventIncludingBase_internal(self, sender, arg, baseTypeTable)
		end
		
		return raiseEvent_internal(self, sender, arg, argTypeTable)
	end
	
	---------------------------------
	--
	-- public methods
	--
	---------------------------------
	
	def.method("=>", "string").getHandlerInfo = function (self)
		local infoList = {}
		for eventTypeClass, handlers in pairs(self.m_handlerChainMap) do
			for i, handler in ipairs(handlers) do
				table.insert(infoList, FormatFunctionInfo(handler))
			end
		end
		table.sort(infoList)
		return table.concat(infoList, "\n")
	end
	
	--[[
		raise an event, all handlers that are registered with the same type of arg will receive the event
	]]
	def.method("dynamic", Object).raiseEvent = function (self, sender, arg)
		checkObject(arg, "raiseEvent", 3, 2)
		
		return raiseEvent_internal(self, sender, arg, arg:getTypeTable())
	end
	
	--[[
		raise an event, all handlers that are registered with any type that inherit from argTypeTable will receive the event
		handlers on base type are invoked first
	]]
	def.method("dynamic", Object).raiseEventIncludingBase = function (self, sender, arg)
		checkObject(arg, "raiseEventIncludingBase", 3, 2)

		return raiseEventIncludingBase_internal(self, sender, arg, arg:getTypeTable())
	end
	
	--[[
		add an event handler
			handler will receive two paramters: the sender and the argument
			if one handler is added for mutiple times, it will be invoking for mutiple times when event raised
		param argTypeTable: type table of event argument
		param handler: the added handler function
	]]
	def.method("table", "function").addHandler = function (self, argTypeTable, handler)
		checkTypeTable(argTypeTable, "addHandler", 2, 2)
		checkSimpleType(handler, "addHandler", 3, "function", 2)
		
		local handlerChain = self:requireHandlerChain(argTypeTable)
		addToChain(handlerChain, handler)
	end
	
	--[[
		add an event handler with cleaner. when the cleaner collected by gc, the added handler will be automaticly removed
			handler will receive two paramters: the sender and the argument
			if one handler is added for mutiple times, it will be invoking for mutiple times when event raised
		param argTypeTable: type table of event argument
		param handler: the added handler function
		param cleaner: the cleaner used to removed handler
	]]
	def.method("table", "function", GcCallbacks).addHandlerWithCleaner = function (self, argTypeTable, handler, cleaner)
		self:addHandler(argTypeTable, handler)
		cleaner:add(function ()
			self:removeHandler(argTypeTable, handler)
		end)
	end
	
	def.method("table", "function").addOneTimeHandler = function (self, argTypeTable, handler)
		checkTypeTable(argTypeTable, "addHandler", 2, 2)
		checkSimpleType(handler, "addHandler", 3, "function", 2)
		
		local function realHandler (sender, event)
			handler(sender, event)
			self:removeHandler(argTypeTable, realHandler)
		end
		
		self:addHandler(argTypeTable, realHandler)
	end
	
	--[[
		remove an event handler
			if one handler is added for mutiple times, it should be removed for the same times
		param argTypeTable: type table of event argument
		param handler: the added handler function
	]]
	def.method("table", "function").removeHandler = function (self, argTypeTable, handler)
		checkTypeTable(argTypeTable, "removeHandler", 2, 2)
		checkSimpleType(handler, "removeHandler", 3, "function", 2)

		local handlerChain = self.m_handlerChainMap[argTypeTable]
		if handlerChain then
			removeFromChain(handlerChain, handler)
		end
	end
	
	--[[
		clear handlers by argument type
	]]
	def.method("table").clear = function (self, argTypeTable)
		self.m_handlerChainMap[argTypeTable] = nil
	end
	
	--[[
		clear all handlers
	]]
	def.method().clearAll = function (self)
		clearTable(self.m_handlerChainMap)
	end
	
	def.method("=>", "number").getHandlerTotalCount = function (self)
		local count = 0
		for argTypeTable, handlers in pairs(self.m_handlerChainMap) do
			count = count + #handlers
		end
		return count
	end
	
	def.method("=>", "number").getIndexerPosTotalCount = function (self)
		local count = 0
		for argTypeTable, handlers in pairs(self.m_handlerChainMap) do
			local i = -1
			while handlers[i] do
				i = i - 2
				count = count + 1
			end
		end
		return count
	end
	
	---------------------------------
	--
	-- END of public
	--
	---------------------------------
	
	def.method("table", "=>" , "table").requireHandlerChain = function (self, argTypeTable)
		local handlerChain = self.m_handlerChainMap[argTypeTable]
		if handlerChain == nil then
			handlerChain = {}
			self.m_handlerChainMap[argTypeTable] = handlerChain
		end
		return handlerChain
	end
	
	--[[
		argType => handlerChain
		handlerChain:
			[n] = handler
			[-2*n+1] = next processing handler index
			[-2*n] = last processing handler index
	]]
	def.field("table").m_handlerChainMap = function () return {} end
end
AnonymousEventManager.Commit()

return
{
	AnonymousEventManager = AnonymousEventManager
}
