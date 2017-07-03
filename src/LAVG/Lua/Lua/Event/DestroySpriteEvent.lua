local Lplus = require "Lplus"

local DestroySpriteEvent = Lplus.Class("DestroySpriteEvent")
local def = DestroySpriteEvent.define
def.field("number").Id = 0
def.field("string").name = ""
def.static("number","string","=>",DestroySpriteEvent).new = function(Id,name)
	local obj = DestroySpriteEvent()
	obj.Id = Id
	obj.name = name
	return obj
end
DestroySpriteEvent.Commit()

return DestroySpriteEvent