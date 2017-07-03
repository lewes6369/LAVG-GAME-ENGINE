--[[
    开始菜单设置项

	[1] = {
	--normal,hover,click,disable
	name = "",				--物体名称(可选)
	res = {1,2,3}			--图片id数组(正常状态,悬浮,置灰)
	posX = 10,				--位置坐标X
	posY = 10,				--位置坐标Y
	width = 200,			--显示宽度
	height = 300,			--显示高度
	(colider_margin) = 0,		--点击边界(可选 ,没有则不可点击)
	(bSave) = true,			--是否保存在缓存中(可选，默认保存)
	}
]]

local BG = 1			--背景
local START_GAME = 2	--开始游戏
local LOAD_GAME = 3		--读取进度
local EXIT = 4			--退出游戏

local menu_start =
{
	[BG] = {
		res = {1},
		posX = 0,				--位置坐标X
		posY = 0,				--位置坐标Y
		width = 1024,			--显示宽度
		height = 768,			--显示高度
	},
	[START_GAME] = {
		res = {2,3},
		posX = 100,				--位置坐标X
		posY = 100,				--位置坐标Y
		width = 160,			--显示宽度
		height = 40,			--显示高度
		colider_margin = 5,
	},
	[LOAD_GAME] = {
		res = {4,5},
		posX = 100,				--位置坐标X
		posY = 150,				--位置坐标Y
		width = 160,			--显示宽度
		height = 40,			--显示高度
		colider_margin = 5,
	},
	[EXIT] = {
		res = {6,7},
		posX = 100,				--位置坐标X
		posY = 200,				--位置坐标Y
		width = 160,			--显示宽度
		height = 40,			--显示高度
		colider_margin = 5,
	},
}

return menu_start
