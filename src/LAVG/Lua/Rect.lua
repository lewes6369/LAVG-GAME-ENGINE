-- Rect
-- author by Liu Hao
-- 2017/06/19

local Vector2 = require "Vector2"
local Lplus = require "Lplus"
local Rect = Lplus.Class("Rect")
local def = Rect.define

def.field("table").VecTopLeft = function () return {} end--Vector2.zero 
def.field("table").VecBottomRight = function () return {} end--Vector2.one 
def.field("number").Scale = 1	--

def.static("table","table", "=>", Rect).new = function (tl,br)
	local obj = Rect()

	--local mt = getmetatable(Vector2.one)
	--setmetatable(obj.VecTopLeft,mt)
	--setmetatable(obj.VecBottomRight,mt)

	obj.VecTopLeft = Vector2.new(tl.x,tl.y)
	obj.VecBottomRight = Vector2.new(br.x,br.y)

	obj.Scale = 1
	return obj
end

def.method(Rect).DepthCopy = function(self,rect)
	self.VecTopLeft = Vector2.new(rect.VecTopLeft.x,rect.VecTopLeft.y)
	self.VecBottomRight = Vector2.new(rect.VecBottomRight.x,rect.VecBottomRight.y)
	self.Scale = rect.Scale
end

def.method("number").ResizeScale = function(self,scale)
	self.Scale = scale
	self.VecTopLeft = self.VecTopLeft * scale
	self.VecBottomRight =  self.VecBottomRight * scale
end

def.method("number","number","=>","boolean").IsPointInRect = function (self,x,y)
	return  self.VecTopLeft.x < x and x < self.VecBottomRight.x and  
		self.VecTopLeft.y < y and y < self.VecBottomRight.y
end


Rect.Commit()
return Rect
