--[[
    游戏中布局设置 位置索引
]]

local POS_TYPE =
{
	SCENE_BG	 = 1,	--场景BG
	TEXT_NAME_BG = 2,	--文本背景显示位置
	TEXT_NAME_CONTENT = 3,	--文本背景显示位置
	TEXT_BG 	 = 4,	--文本背景显示位置
	TEXT_CONTENT = 5, 	--文本内容显示位置
	MENU_POS     = 6,	--菜单显示位置
	CHOICE_POS   = 7,	--选项位置
	ROLE_LEFT    = 8,	--角色在左侧显示位置
	ROLE_RIGHT   = 9,	--角色在右侧显示位置
	ROLE_MIDDLE  = 10,	--角色在中部显示位置
	ROLE_USE1    = 11,	--角色未使用1
	ROLE_USE2    = 12,	--角色未使用2
	--若要加为角色位置，则在后面补充即可
}

local ingame_layout =
{
	[POS_TYPE.SCENE_BG] = {
		posX = 0,				
		posY = 0,				
		width = 640,			
		height = 480,			
	},
	[POS_TYPE.TEXT_NAME_BG] = {
		posX = 50,				
		posY = 320,				
		width = 100,			
		height = 50,			
	},
	[POS_TYPE.TEXT_NAME_CONTENT] = {
		posX = 60,				
		posY = 340,				
		width = 80,			
		height = 30,				
	},
	[POS_TYPE.TEXT_BG] = {
		posX = 20,				
		posY = 340,				
		width = 600,			
		height = 140,				
	},
	[POS_TYPE.TEXT_CONTENT] = {
		posX = 60,				
		posY = 380,				
		width = 550,			
		height = 90,				
	},
	[POS_TYPE.MENU_POS] = {
		posX = 100,				
		posY = 200,				
		width = 160,			
		height = 40,			
	},
	[POS_TYPE.CHOICE_POS] = {
		posX = 100,				
		posY = 200,				
		width = 160,			
		height = 40,			
	},
	[POS_TYPE.ROLE_LEFT] = {
		posX = 20,				
		posY = 40,				
		width = 170,			
		height = 410,			
	},
	[POS_TYPE.ROLE_RIGHT] = {
		posX = 450,				
		posY = 40,				
		width = 170,			
		height = 410,			
	},
	[POS_TYPE.ROLE_MIDDLE] = {
		posX = 100,				
		posY = 150,				
		width = 160,			
		height = 40,			
	},
}

return ingame_layout
