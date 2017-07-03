-- Game Sprite
-- author by Liu Hao
-- 2017/06/18


local Lplus = require "Lplus"
local Rect = require "Rect"
local Vector2 = require "Vector2"
local GameSprite = Lplus.Class("GameSprite")
local def = GameSprite.define

def.field("string").name = ""
def.field("number").Id = 0
def.field("number").state = SpriteState.NORMAL		
def.field("boolean").isShow = false
def.field("boolean").isEnabled = false	--EnableColider
def.field(Rect).RenderRect = nil 		--渲染位置大小
def.field(Rect).ColiderRect = nil		--碰撞体大小

def.field("number").NormalTexture = 0
def.field("number").HoverTexture = 0	--onHover
--def.field("number").ClickTexture = 0  --onClick
def.field("number").DisabledTexture = 0 --onDisabled

def.field("function").onHoverFuc = nil
def.field("function").onClickFuc = nil
def.field("function").onDisabledFuc = nil

local unqiueId = 0
def.static("string", "=>", GameSprite).new = function (name)
	unqiueId = unqiueId + 1
	local obj = GameSprite()
	obj.name = name
	obj.Id = unqiueId
	return obj
end


def.method().DestorySprite = function(self)
	self.isShow = false

	if self.NormalTexture ~= 0 then
		LAVG_DeleteTexture(self.NormalTexture)
	end

	if self.HoverTexture ~= 0 then
		LAVG_DeleteTexture(self.HoverTexture)
	end
	
	if self.DisabledTexture ~= 0 then
		LAVG_DeleteTexture(self.DisabledTexture)
	end
end

GameSprite.Commit()
return GameSprite