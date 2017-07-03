-- Node Manager
-- author by Liu Hao
-- 2017/06/19
local Lplus = require "Lplus"
local RenderManager = require "RenderManager"
local WinSetting = require "WinSetting"
local GameSprite = require "GameSprite"
local SpriteManager = require "SpriteManager"
local TextureManager = require "TextureManager"
local Rect = require "Rect"

local NodeManager = Lplus.Class("NodeManager")
local def = NodeManager.define

--specified for fixed pos  
local SPECIAL_POS =
{
	SCENE_BG	 = 1,	--场景BG
	TEXT_NAME_BG = 2,	--文本名字显示位置
	TEXT_NAME_CONTENT = 3,	--文本背景显示位置
	TEXT_BG 	 = 4,	--文本框显示位置
	TEXT_CONTENT = 5, 	--文本内容显示位置
	MENU_POS     = 6,	--菜单显示位置
	CHOICE_POS   = 7,	--选项位置
}

--StateInfo can be used to save or render
local StateInfo = Lplus.Class("StateInfo")
do
	local def = StateInfo.define
	def.field("number").NodeID = 0		--节点ID
	def.field("number").ContentIdx = 0	--文本内容索引
	def.field("number").RoleAId = 0		--角色AId
	def.field("number").RoleAState = 0	--角色A状态
	def.field("number").RoleAPos = 	0	--角色A位置
	def.field("number").RoleBId = 0		--角色BId
	def.field("number").RoleBState = 0	--角色B状态
	def.field("number").RoleBPos = 0	--角色B位置
	def.field("number").BgId = 0		--场景ID
	def.field("number").TextBgId = 0	--文本框ID
	def.field("number").TextNameBgId = 0--名字框ID
	def.field("string").Text = ""		--文本内容
	StateInfo.Commit()
end

local RenderSprite = Lplus.Class("RenderSprite")
do
	local def = RenderSprite.define
	def.field("string").Name = ""
	def.field(GameSprite).Sprite = nil	
	def.field("number").PosIdx = 0
	def.field("number").ResId = 0	
	RenderSprite.Commit()
end

--normal render class
local RENDER_TYPE =
{
	SCENE_BG	 = 1,	
	TEXT_NAME_BG = 2, 	
	TEXT_BG 	 = 3,
	ROLE_A 		 = 4,
	ROLE_B 		 = 5,
}

def.const("table").SPECIAL_POS = SPECIAL_POS
def.const("table").RENDER_TYPE = RENDER_TYPE
def.field(StateInfo).m_curState = nil
def.field("table").m_posRectCfg = function () return {} end	--脚本配置点的rect
def.field("table").m_allNodeCfg = function () return {} end --节点配置
def.field("table").m_completeNodes = function () return {} end --完成的节点 MAP[ID] = true
def.field("table").m_roleCfg = function () return {} end --角色配置
def.field("boolean").m_bHideText = false
def.field("number").m_textTimerId = 0
def.field("boolean").m_bMakeChoose = false
def.field("table").m_RenderSpirtes = function () return {} end --渲染图像 [RENDER_TYPE]= RenderSprite

local instance = nil
def.static("=>",NodeManager).Instance = function ()
	if instance == nil then
		instance = NodeManager()
	end

	return instance
end


def.method().Init = function(self)
	local ret, layout = pcall(dofile, "Lua/Config/ingame_layout.lua")
	if ret and layout then
		self.m_posRectCfg = {}
		local setting = WinSetting.Instance()
		for k,v in ipairs(layout) do
			local cfg = v
			local endPosX = (cfg.posX + cfg.width > setting.m_width) and setting.m_width or (cfg.posX + cfg.width)   
			local endPosY = (cfg.posY + cfg.height > setting.m_height) and setting.m_height or (cfg.posY + cfg.height)  
			local rect = Rect.new({x = cfg.posX,y = cfg.posY},{x = endPosX,y = endPosY})
			table.insert(self.m_posRectCfg,rect)
		end
	else
		error("Load ingame_layout failed")
	end

	local ret, cfg = pcall(dofile, "Lua/Config/node_list.lua")
	if ret and cfg then
		self.m_allNodeCfg = cfg
	else
		error("Load nodeList failed")
	end


	local ret, cfg = pcall(dofile, "Lua/Config/role_config.lua")
	if ret and cfg then
		self.m_roleCfg = cfg
	else
		error("Load role_config failed")
	end
end


def.method("number","=>","boolean").CanEnterNode = function(self,NodeId)
	local nodeCfg =  self.m_allNodeCfg[NodeId]
	if nodeCfg.PreNeedNode then
		for _,group in ipairs(nodeCfg.PreNeedNode) do
			local satisfy = true

			for _,ID in ipairs (group) do
				if not self.m_completeNodes[ID] then
					satisfy = false
					break
				end
			end 

			-- if satisfy any group then can enter
			if satisfy then
				return true
			else
				return false
			end
		end
	end

	return true
end

def.method().ContinueNextContent = function(self)
	if not self.m_curState then
		return 
	end

	local nodeCfg =  self.m_allNodeCfg[self.m_curState.NodeID]
	local NextContentIdx = self.m_curState.ContentIdx + 1
	if nodeCfg.Content[NextContentIdx] then
		self.m_curState.ContentIdx = NextContentIdx
		self:UpdateContentInfo(nodeCfg,NextContentIdx)
		self:RenderNode()
	else
		--make Choose 
		self.m_bMakeChoose = true
		
		--Update Choose Title
		self:RenderNode()
	end
end


def.method("string","number","number","number").UpdateSimpleSprite = function(self,Name,Type,PosIdx,ResId)
	if not self.m_RenderSpirtes[Type] then
		local RenderSp= RenderSprite()
		RenderSp.Name = Name
 		RenderSp.PosIdx = PosIdx
 		RenderSp.ResId = ResId
 		RenderSp.Sprite = SpriteManager.CreateOneSimpleSprite(Name,self.m_posRectCfg[PosIdx],ResId)
 		SpriteManager.Instance():AddGameSprite(RenderSp.Sprite)
 		self.m_RenderSpirtes[Type] = RenderSp
	else
		local RenderSp = self.m_RenderSpirtes[Type]
		local TexId = TextureManager.Instance():CheckTexFromCacheByResID(ResId)
		if TexId ~= 0 and RenderSp.Sprite.NormalTexture ~=  TexId then
			LAVG_DeleteTexture(RenderSp.Sprite.NormalTexture)
			RenderSp.Sprite.NormalTexture = TexId
		end
		RenderSp.Sprite.RenderRect = self.m_posRectCfg[PosIdx]
	end

	SpriteManager.Instance():ShowSprite(self.m_RenderSpirtes[Type].Sprite.Id,ResId ~= 0,false)
end


def.method().UpdateBgSprite = function(self)
	self:UpdateSimpleSprite("SCENE_BG",RENDER_TYPE.SCENE_BG,
							SPECIAL_POS.SCENE_BG,self.m_curState.BgId)

	local selfSprite = self.m_RenderSpirtes[RENDER_TYPE.SCENE_BG].Sprite
	local Sprite = SpriteManager.Instance().m_AllSprites[selfSprite.Id]
	SpriteManager.AddSpriteClickClider(Sprite,0,0,0)
	SpriteManager.Instance():EnableSprite(selfSprite.Id,true)
	Sprite.onClickFuc = function () 
		self:ContinueNextContent() 
	end
	selfSprite.onClickFuc = Sprite.onClickFuc
end


def.method().UpdateRole = function(self)
	if self.m_curState.RoleAId ~= 0 then
		local RoleACfg = self.m_roleCfg[self.m_curState.RoleAId] 
		local ResAId = RoleACfg.state[self.m_curState.RoleAState]

		self:UpdateSimpleSprite("ROLE_A",RENDER_TYPE.ROLE_A,
								self.m_curState.RoleAPos,ResAId)
	end

	if self.m_curState.RoleBId ~= 0 then
		local RoleBCfg = self.m_roleCfg[self.m_curState.RoleBId] 
		local ResBId = RoleBCfg.state[self.m_curState.RoleBState]

		self:UpdateSimpleSprite("ROLE_B",RENDER_TYPE.ROLE_B,
								self.m_curState.RoleBPos,ResBId)
	end
end

def.method().UpdateText = function(self)
	self:UpdateSimpleSprite("TEXT_BG",RENDER_TYPE.TEXT_BG,
							SPECIAL_POS.TEXT_BG,self.m_curState.TextBgId)
	--self.m_textTimerId = UpdateText
	--AddTimer(ttl, bOnce, callbackRef, cbparam)
	--RemoveTimer(self.m_textTimerId)

	local rect = self.m_posRectCfg[SPECIAL_POS.TEXT_CONTENT]
	RenderManager.Instance():RenderText(LAVG_Utf8ToUnicode(self.m_curState.Text),rect,false)
	--print("UpdateText endPosx,endPosy",endPosx,endPosy)
end

def.method().UpdateTextName = function(self)
	self:UpdateSimpleSprite("TEXT_NAME_BG",RENDER_TYPE.TEXT_NAME_BG,
						SPECIAL_POS.TEXT_NAME_BG,self.m_curState.TextNameBgId)


	local rect = self.m_posRectCfg[SPECIAL_POS.TEXT_NAME_CONTENT]
	if self.m_curState.RoleAId ~= 0 then
		local RoleACfg = self.m_roleCfg[self.m_curState.RoleAId]
		RenderManager.Instance():RenderText(LAVG_Utf8ToUnicode(RoleACfg.name),rect,false)
	end
end

def.method().RenderNode = function(self)
	RenderManager.Instance():ClearRenderQueue()
	self:UpdateBgSprite()
	self:UpdateRole()
	self:UpdateText()
	self:UpdateTextName()
	--UpdateText()
	RenderManager.Instance():RenderImmediately()
end


def.method("number","number").EnterNode = function(self,nodeID,conIdx)
	if self:CanEnterNode(nodeID) then 
		if not self.m_curState then
			self.m_curState = StateInfo()
		end

		local nodeCfg = self.m_allNodeCfg[nodeID]
		self.m_curState.NodeID = nodeCfg.NodeID
		self.m_curState.ContentIdx = conIdx
		self:UpdateContentInfo(nodeCfg,conIdx)

		self:RenderNode()
	else
		warn("The Node Can not Be Enter In some reason")
	end
end

def.method("table","number").UpdateContentInfo = function(self,nodeCfg,conIdx)
	if not self.m_curState then
		return
	end

	--Lplus.tryget
	local Info = nodeCfg.Content[conIdx]
	for k,v in pairs(Info) do
		self.m_curState[k] = v
	end
end

NodeManager.Commit()
return NodeManager