-- Start Menu
-- author by Liu Hao
-- 2017/06/19

local Lplus = require "Lplus"
local GameSprite = require "GameSprite"
local Rect = require "Rect"
local SpriteManager = require "SpriteManager"
local TheGame = Lplus.ForwardDeclare("TheGame")

local StartMenu = Lplus.Class("StartMenu")
local def = StartMenu.define

local instance = nil
def.static("=>",StartMenu).Instance = function ()
	if instance == nil then
		instance = StartMenu()
	end

	return instance
end

def.method().onStartMenu = function(self)
	local StartGameFuc = function() TheGame.Instance():StartNewGame() end
	local LoadGameFuc = function() TheGame.Instance():LoadGame() end
	local ExitFunc = function() TheGame.Instance():ExitGame() end 

	local MenuClickFunctions =
	{
		nil,
		StartGameFuc,
		LoadGameFuc,
		ExitFunc,
	}

	local Sprite = nil
	local ret, startConfig = pcall(dofile, "Lua/Config/start_menu.lua")
	if ret and startConfig then
		for i = 1,#startConfig do
			print("start LoadSingleSpriteConfig",i)
			Sprite = StartMenu.LoadSingleSpriteConfig(startConfig[i])
			if Sprite then
				--Add Click Function
				Sprite.onClickFuc = MenuClickFunctions[i]

				--Add Sprite
				SpriteManager.Instance():AddGameSprite(Sprite)

				--ShowSprite
				SpriteManager.Instance():ShowSprite(Sprite.Id,true,i == #startConfig)
			end
		end
	end
end


def.static("table","=>",GameSprite).LoadSingleSpriteConfig = function(cfg) 
	if not cfg then 
		warn("there is no cfg",debug.traceback())
		return
	end

	local RenderRect = Rect.new({x = cfg.posX,y = cfg.posY},{x = cfg.posX + cfg.width,y = cfg.posY + cfg.height})
	local Sprite = SpriteManager.CreateOneSimpleSprite(cfg.name or "",RenderRect,cfg.res[1])

	if cfg.colider_margin and tonumber(cfg.colider_margin) then
		Sprite = SpriteManager.AddSpriteClickClider(Sprite,cfg.res[2] or 0,cfg.res[3] or 0,cfg.colider_margin)
	end

	return Sprite
end


StartMenu.Commit()
return StartMenu