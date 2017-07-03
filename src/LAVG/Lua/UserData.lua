-- UserData
-- author by Liu Hao
-- 2017/06/18

local Lplus = require "Lplus"
local malut = require "Utility.malut"

local UserData = Lplus.Class("UserData")
local def = UserData.define

def.field("table").DataTable = function() return {} end
def.field("boolean").HasChangeCfg = false
def.field("boolean").beInitialized = false

local theData = nil

def.static("=>",UserData).Instance = function ()
	if theData == nil then
		theData = UserData()
	end

	return theData
end

local USER_DATA_VERSION = 1

def.method().Init = function(self)
	if self.beInitialized then
		return false
	end
	self:SetSystemCfg("FullScreen",false)
	self:SetSystemCfg("Width", 1024)
	self:SetSystemCfg("Height", 768)
	self:SetSystemCfg("Scale",1.0)

	local data
	ret, data = pcall(dofile, "Lua/UserData/usercfg.lua")
	if ret then
		if type(data) == "table" then
			--self.DataTable.SystemCfg = data.SystemCfg
			--self.DataTable.Version = data.Version
		end
	else
		warn("Fail to load userdataCfg:",data)
	end

	self.DataTable.Version = USER_DATA_VERSION
	self.beInitialized = true
end

def.method("string", "dynamic").SetSystemCfg = function(self, key, value)
	self:SetCfg("SystemCfg", key, value)
end

def.method("string", "=>", "dynamic").GetSystemCfg = function(self, key)
	return self:GetCfg("SystemCfg", key)
end

def.method().SaveDataToFile = function(self)
	if not self.HasChangeCfg then return end

	local path = "Lua/UserData/usercfg.lua"--GameUtil.GetAssetsPath() .. "/UserData/usercfg.lua"
	--GameUtil.CreateDirectoryForFile(path)
	local bSucc, err = malut.toCodeToFile(self.DataTable, path)
	if not bSucc then
		warn(err)
	end
	
	self.HasChangeCfg = false
end

def.method("string", "string", "dynamic").SetCfg = function(self, catalog, key, value)
	local catalogData = self.DataTable[catalog]

	if not catalogData then
		catalogData = {}
		self.DataTable[catalog] = catalogData
	end

	catalogData[key] = value

	self.HasChangeCfg = true
end

def.method("string", "string", "=>", "dynamic").GetCfg = function(self, catalog, key)
	local catalogData = self.DataTable[catalog]
	if not catalogData then
		return nil
	end

	return catalogData[key]
end

UserData.Commit()
return UserData