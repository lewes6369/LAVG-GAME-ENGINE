-- main Entry Point
-- author by Liu Hao
-- 2017/06/18

--local Lplus = require "Lplus"
package.path = package.path .. ";Lua/?.lua"

require "Lua/ClientDef"
require "Lua/Config/res_path"


local Lplus = require "Lplus"
local TheGame = require "TheGame"
local SpriteManager = require "SpriteManager"
local WinSetting = require "WinSetting"

local global = require "Utility.global"
global.disableWriteForThread()

function _G.OnEnterMenu()
	--print("OnEnterMenu")

	TheGame.Instance():EnterStartMenu()
end

function _G.OnWindowSizeChanged(width,height,scale)
	print("OnWindowSizeChanged",width,height,scale)

	WinSetting.Instance():onSizeChanged(width,height,scale)
end

--[[
button:
	GLFW_MOUSE_BUTTON_LEFT      0
	GLFW_MOUSE_BUTTON_RIGHT     1
	GLFW_MOUSE_BUTTON_MIDDLE    2

action:
	GLFW_RELEASE 0
	GLFW_PRESS   1
	GLFW_REPEAT  2
]]
function _G.OnClickScreen(button,action,posX,posY)
	print("OnClickScreen, pos: ",button,action,posX, posY)
	if button == 0 and action == 1 then
		SpriteManager.Instance():OnLButtonClick(posX,posY)
	end
end

function _G.OnMouseMove(posX,posY)
	--print("OnMouseMove pos: ", posX, posY)
	SpriteManager.Instance():OnMouseMove(posX,posY)
end
