-- The Game
-- author by Liu Hao
-- 2017/06/18

local Lplus = require "Lplus"
local GameSprite = require "GameSprite"
local WinSetting = require "WinSetting"
local SpriteManager = require "SpriteManager"
local RenderManager = require "RenderManager"
local NodeManager = require "NodeManager"
local Vector2 = require "Vector2"
local AnonymousEventManager = require "Utility.AnonymousEvent".AnonymousEventManager
local Rect = require "Rect"
local StartMenu = require "StartMenu"
local TheGame = Lplus.Class("TheGame")
local UserData = require "UserData"
local def = TheGame.define

def.field("number").m_GameState = GameState.INIT
def.const(AnonymousEventManager).EventManager = AnonymousEventManager()

local instance = nil
def.static("=>",TheGame).Instance = function ()
	if instance == nil then
		instance = TheGame()
	end

	return instance
end


def.method().onInit = function(self)
	UserData.Instance():Init()
	WinSetting.Instance():Init()
	RenderManager.Instance():Init()
	NodeManager.Instance():Init()
end

def.method().EnterStartMenu = function(self)
	self:onInit()

	self.m_GameState = GameState.STARTMENU

	StartMenu.Instance():onStartMenu()
end

def.method().StartNewGame = function(self)
	print("StartNewGame")

	if self.m_GameState ~= GameState.STARTMENU then
		warn("Wrong State")
		return
	end

	--clear cache
	SpriteManager.Instance():ClearAllSprites()
	RenderManager.Instance():ClearRenderQueue()


	--enter node
	NodeManager.Instance():EnterNode(1,1)

end

def.method().LoadGame = function(self)
	warn("LoadGame")
end

def.method().ExitGame = function(self)
	warn("ExitGame")
end


TheGame.Commit()
return TheGame