-- TextureManager
-- author by Liu Hao
-- 2017/06/29
local Lplus = require "Lplus"
local TheGame = Lplus.ForwardDeclare("TheGame")
local TextureManager = Lplus.Class("TextureManager")
local def = TextureManager.define

--加载了的纹理图像 map[resId] = texId
def.field("table").m_LoadedTexIDs = function() return {} end

local instance = nil
def.static("=>",TextureManager).Instance = function ()
	if instance == nil then
		instance = TextureManager()
	end

	return instance
end

def.method("number","=>","number").LoadTexByResID = function(self,resId)
	if resId == 0 or not RES_PATH[resId] then
		--warn("not find the resource Id:".. resId)
		return 0
	end

	local TexId = self:CheckTexFromCacheByResID(resId) 
	if TexId ~= 0 then
		return TexId
	end

	local bsuc
	TexId,bsuc = LAVG_LoadTextureFromImage(RES_PATH[resId],0)
	if TexId == 0 or not bsuc then
		error("LoadTextureFromImage error:" .. RES_PATH[resId] .. TexId .. bsuc)
		return 0
	end
	self.m_LoadedTexIDs[resId] = TexId

	return TexId
end

def.method("number","=>","number").CheckTexFromCacheByResID = function(self,resId)
	if self.m_LoadedTexIDs[resId] then
		TexId = self.m_LoadedTexIDs[resId]
		if LAVG_IsTextureExist(TexId) then
			return TexId
		else
			self.m_LoadedTexIDs[resId] = nil
		end
	end

	return 0
end

TextureManager.Commit()
return TextureManager