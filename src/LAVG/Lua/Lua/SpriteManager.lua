-- SpriteManager
-- author by Liu Hao
-- 2017/06/18

local Lplus = require "Lplus"
local Rect = require "Rect"
local Vector2 = require "Vector2"
local RenderManager = require "RenderManager"
local TextureManager = require "TextureManager"
local GameSprite = require "GameSprite"
local TheGame = Lplus.ForwardDeclare("TheGame")
local SpriteManager = Lplus.Class("SpriteManager")
local def = SpriteManager.define

def.field("table").m_AllSprites = function() return {} end
def.field("table").m_ShowSprites = function() return {} end	--array id
def.field("table").m_ColiderSprites = function() return {} end	--array id

local instance = nil
def.static("=>",SpriteManager).Instance = function ()
	if instance == nil then
		instance = SpriteManager()
	end

	return instance
end

def.method("number").ClearSprite = function(self,spriteId)
	--print("---ClearSprite ------",spriteId)

	if self.m_AllSprites[spriteId] then
		local name = self.m_AllSprites[spriteId].name
		self.m_AllSprites[spriteId]:DestorySprite()
		self.m_AllSprites[spriteId] = nil

		for k ,v in pairs(self.m_ShowSprites) do
			if v == spriteId then
				table.remove(self.m_ShowSprites,k)
				break
			end
		end

		for k ,v in pairs(self.m_ColiderSprites) do
			if v == spriteId then
				table.remove(self.m_ColiderSprites,k)
				break
			end
		end

		local DestroySpriteEvent = require "Event.DestroySpriteEvent"
		TheGame.EventManager:raiseEvent(nil, DestroySpriteEvent.new(spriteId,name))
	end
end

def.method().ClearAllSprites = function(self)
	--print("~~~~~~ClearAllSprites")
	for k,v in pairs(self.m_AllSprites) do
		local id = v.Id
		local name = v.name
		v:DestorySprite()

		local DestroySpriteEvent = require "Event.DestroySpriteEvent"
		TheGame.EventManager:raiseEvent(nil, DestroySpriteEvent.new(id,name))
	end

	self.m_AllSprites = {}
	self.m_ShowSprites = {}
	self.m_ColiderSprites = {}
end

def.method("number","number").OnLButtonClick = function(self,posX,posY)
	for k,v in pairs(self.m_ColiderSprites) do
		local sprite = self.m_AllSprites[v]
		print("bIn Colider Rect,state",bInRect,v,sprite)--,sprite.state,SpriteState.NORMAL)
		if sprite and sprite.state == SpriteState.HOVER and sprite.ColiderRect:IsPointInRect(posX,posY) then
			sprite.state = SpriteState.CLICK

			--if sprite.ClickTexture ~= 0 then
			--	RenderManager.Instance():RenderSprite(sprite.Id,sprite.ClickTexture,sprite.RenderRect,true)
			--end

			if sprite.onClickFuc then
				sprite.onClickFuc()
			end

			sprite.state = SpriteState.HOVER
			break
		end
	end
end 

def.method("number","number").OnMouseMove = function(self,posX,posY)
	for k,v in pairs(self.m_ShowSprites) do
		--print("--- self.m_ShowSprites -- ",k,v)
		local sprite = self.m_AllSprites[v]
		if sprite and sprite.ColiderRect then
			local bInRect = sprite.ColiderRect:IsPointInRect(posX,posY)

			if sprite.state == SpriteState.NORMAL and bInRect then
				sprite.state = SpriteState.HOVER

				if sprite.HoverTexture ~= 0 then
					RenderManager.Instance():RenderSprite(sprite.Id,sprite.HoverTexture,sprite.RenderRect,true)
				end

				if sprite.onHoverFuc then
					sprite.onHoverFuc()
				end
			elseif sprite.state == SpriteState.HOVER and not bInRect then
				sprite.state = SpriteState.NORMAL
				RenderManager.Instance():RenderSprite(sprite.Id,sprite.NormalTexture,sprite.RenderRect,true)
			end
		end
	end
end 

def.method("number","boolean").EnableSprite = function(self,spriteId,bEnabled)
	if self.m_AllSprites[spriteId] and self.m_AllSprites[spriteId].isShow then
		if bEnabled then
			for k,v in pairs(self.m_ColiderSprites) do
				if v == spriteId then
					return
				end
			end

			table.insert(self.m_ColiderSprites,spriteId)
		elseif not bEnabled then
			for k,v in pairs(self.m_ColiderSprites) do
				if v == spriteId then
					sprite.state = SpriteState.DISABLE

					if sprite.DisabledTexture ~= 0 then
						RenderManager.Instance():RenderSprite(sprite.Id,sprite.DisabledTexture,sprite.RenderRect,true)
					end

					if sprite.onDisabledFuc then
						sprite.onDisabledFuc()
					end					
				end
			end
		end
	end
end 


def.method("number","boolean","boolean").ShowSprite = function(self,spriteId,bShow,bImmediately)
	if self.m_AllSprites[spriteId] then
		if bShow then
			local bFind = false
			for k,v in pairs(self.m_ShowSprites) do
				if v == spriteId then
					bFind = true
					break
				end
			end
			if not bFind then
				table.insert(self.m_ShowSprites,spriteId) 
			end

			if self.m_AllSprites[spriteId].isEnabled then
				bFind = false
				for k,v in pairs(self.m_ColiderSprites) do
					if v == spriteId then
						bFind = true
					end
				end

				if not bFind then
					table.insert(self.m_ColiderSprites,spriteId) 
				end
			end

			local sprite = self.m_AllSprites[spriteId]
			RenderManager.Instance():RenderSprite(sprite.Id,sprite.NormalTexture,sprite.RenderRect,bImmediately)
		else
			for k,v in pairs(self.m_ShowSprites) do
				if v == spriteId then
					table.remove(self.m_ShowSprites,k)
					RenderManager.Instance():RemoveSpriteInRenderList(spriteId)
					if bImmediately then
						RenderManager.Instance():RenderImmediately()
					end
					return
				end
			end
		end
	end
end 

def.method(GameSprite).AddGameSprite =function(self,sprite)
	if sprite and not self.m_AllSprites[sprite.Id] then
		--print("~~~~~~~AddGameSprite,sprite.Id",sprite.Id)
		self.m_AllSprites[sprite.Id] = sprite
		if sprite.isShow then
			table.insert(self.m_ShowSprites,sprite.Id)
		end

		if sprite.isEnabled then
			table.insert(self.m_ColiderSprites,sprite.Id)
		end
	end
end 

def.static("string",Rect,"number","=>",GameSprite).CreateOneSimpleSprite = function(name,RenderRect,NormalResID)
	local Sprite = GameSprite.new(name or "")
	Sprite.state = SpriteState.NORMAL
	Sprite.isShow = true
	Sprite.isEnabled = false
	Sprite.RenderRect = Rect()
	Sprite.RenderRect:DepthCopy(RenderRect)
	Sprite.NormalTexture = TextureManager.Instance():LoadTexByResID(NormalResID)

	return Sprite
end

def.static(GameSprite,"number","number","number","=>",GameSprite).AddSpriteClickClider = function(Sprite,HoverResID,DisabledResID,colider_margin)
	Sprite.isEnabled = true
	Sprite.HoverTexture = TextureManager.Instance():LoadTexByResID(HoverResID)
	Sprite.DisabledTexture = TextureManager.Instance():LoadTexByResID(DisabledResID)

	local RenderRect = Sprite.RenderRect
	local VectorMargin = Vector2.new(colider_margin,colider_margin)
	local TL = Sprite.RenderRect.VecTopLeft - VectorMargin
	local BR = Sprite.RenderRect.VecBottomRight + VectorMargin
	Sprite.ColiderRect = Rect.new({x = TL.x,y = TL.y},{x = BR.x ,y = BR.y})
	return Sprite
end

SpriteManager.Commit()
return SpriteManager