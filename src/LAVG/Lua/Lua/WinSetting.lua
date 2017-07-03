-- WinSetting
-- author by Liu Hao
-- 2017/06/18

local Lplus = require "Lplus"
local UserData = require "UserData"
local RenderManager = require "RenderManager"

local WinSetting = Lplus.Class("WinSetting")
local def = WinSetting.define

def.field("number").m_width = 0
def.field("number").m_height = 0
def.field("boolean").m_bFullScreen = false
def.field("number").m_scale = 0

local instance = nil

def.static("=>",WinSetting).Instance = function ()
	if instance == nil then
		instance = WinSetting()
	end

	return instance
end

def.method().Init = function(self)
	self.m_width = UserData.Instance():GetSystemCfg("Width")
	self.m_height = UserData.Instance():GetSystemCfg("Height")
	self.m_scale = UserData.Instance():GetSystemCfg("Scale")
end

def.method("number","number","number").onSizeChanged = function(self,width,height,scale)
	self.m_width = width
	self.m_height = height
	self.m_scale = scale

	UserData.Instance():SetSystemCfg("Width",self.m_width)
	UserData.Instance():SetSystemCfg("Height",self.m_height)
	UserData.Instance():SetSystemCfg("Scale",self.m_scale)

	RenderManager.Instance():OnWindowResize(scale)
end


WinSetting.Commit()
return WinSetting