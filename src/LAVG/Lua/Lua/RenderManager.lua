-- RenderManager
-- author by Liu Hao
-- 2017/06/19

local Lplus = require "Lplus"
local Vector2 = require "Vector2"
local Rect = require "Rect"

local DestroySpriteEvent = require "Event.DestroySpriteEvent"
local GameSprite = require "GameSprite"
local TheGame = Lplus.ForwardDeclare("TheGame")
local RenderManager = Lplus.Class("RenderManager")
local def = RenderManager.define

--当前需要渲染的队列
def.field("table").m_CurRenderSpritesQueue = function() return {} end
def.field("table").m_CurRenderTextQueue = function() return {} end

local instance = nil
def.static("=>",RenderManager).Instance = function ()
	if instance == nil then
		instance = RenderManager()
	end

	return instance
end

local RenderParams = Lplus.Class("RenderParams")
do
	local def = RenderParams.define
	def.field("number").SpriteId = 0
	def.field("number").TextureId = 0
	def.field(Rect).RenderRect = nil
	def.field("string").Text = ""
	RenderParams.Commit()
end


def.method().Init = function(self)

	TheGame.EventManager:addHandler(DestroySpriteEvent, function (sender,event)
		self:RemoveSpriteInRenderList(event.Id)
	end)
end

def.method().ClearRenderQueue = function(self)
	self.m_CurRenderSpritesQueue = {}
	self.m_CurRenderTextQueue = {}
end

def.method("number","number",Rect,"boolean").RenderSprite = function(self,SpriteId,TexId,RenderRect,RenderImmediately)
	--print("----RenderSprite :",debug.traceback(),SpriteId,TexId,RenderRect,RenderImmediately)

	local bFind = false
	local params = nil
	for k,v  in ipairs(self.m_CurRenderSpritesQueue) do
		--if already in Queue
		if v.SpriteId == SpriteId then
			bFind = true
			params = v
			break
		end
	end

	params = params or RenderParams()
	params.SpriteId = SpriteId
	params.TextureId = TexId
	params.RenderRect = Rect.new({x = RenderRect.VecTopLeft.x, y=RenderRect.VecTopLeft.y} ,
					{x = RenderRect.VecBottomRight.x , y = RenderRect.VecBottomRight.y})

	if not bFind then
		table.insert(self.m_CurRenderSpritesQueue,params)
	end

	if RenderImmediately then
		self:RenderImmediately()
	end
end


def.method("string",Rect,"boolean").RenderText = function(self,Text,RenderRect,RenderImmediately)
	local params = RenderParams()
	params.Text = Text
	params.RenderRect = Rect.new({x = RenderRect.VecTopLeft.x, y=RenderRect.VecTopLeft.y} ,
					{x = RenderRect.VecBottomRight.x , y = RenderRect.VecBottomRight.y})
	table.insert(self.m_CurRenderTextQueue,params)

	if RenderImmediately then
		self:RenderImmediately()
	end
end

def.method().RenderImmediately = function(self)
	--print("RenderImmediately")
	LAVG_BeginRender()

	for k,v in ipairs(self.m_CurRenderSpritesQueue) do
		local RenderRect = v.RenderRect
		local tf = LAVG_Vector2i(RenderRect.VecTopLeft.x,RenderRect.VecTopLeft.y)
		local br = LAVG_Vector2i(RenderRect.VecBottomRight.x,RenderRect.VecBottomRight.y)
		LAVG_RenderTextureInRect(v.TextureId,tf,br)
	end

	for k,v in ipairs(self.m_CurRenderTextQueue) do
		local RenderRect = v.RenderRect
		local tf = LAVG_Vector2i(RenderRect.VecTopLeft.x,RenderRect.VecTopLeft.y)
		local br = LAVG_Vector2i(RenderRect.VecBottomRight.x,RenderRect.VecBottomRight.y)
		LAVG_RenderText(v.Text,tf,br)
	end

	LAVG_RenderWindow()
end

def.method("number").RemoveSpriteInRenderList = function (self,spriteId)
	local t = {}
	for k,v in ipairs(self.m_CurRenderSpritesQueue) do
		if v.SpriteId ~= spriteId then
			table.insert(t,v)
		end
	end

	self.m_CurRenderSpritesQueue = {}
	for k,v in ipairs(t) do
		table.insert(self.m_CurRenderSpritesQueue,v)
	end
end 

def.method("number").OnWindowResize = function(self,scale)
	print("OnWindowResize")
	for k,v in ipairs(self.m_CurRenderSpritesQueue) do
		v.RenderRect:ResizeScale(scale)
	end

	self:RenderImmediately()
end

RenderManager.Commit()
return RenderManager