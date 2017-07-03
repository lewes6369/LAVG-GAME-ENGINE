-- Vector2
-- author by Liu Hao
-- 2017/06/18

local Vector2 = Vector2 and Vector2 or {}

Vector2 = {
	new = function (_x,_y)
		local ret = { x = _x and _x or 0 , y = _y and _y or 0 }
		setmetatable(ret,Vector2)
		return ret
	end,

	__add = function( lhs, rhs)
		local ret = { x = lhs.x + rhs.x, y = lhs.y + rhs.y}
		setmetatable(ret, Vector2)
		return ret
	end,

	__sub = function( lhs,rhs )
		local ret = { x = lhs.x - rhs.x, y = lhs.y - rhs.y}
		setmetatable( ret, Vector2)
		return ret
	end,

	__mul = function( self, factor)
		local ret = { x = self.x * factor, y = self.y * factor}
		setmetatable(ret, Vector2)
		return ret
	end,


	__div = function( self, factor)
		return Vector2.__mul(self,1/factor)
	end,

	__unm = function( self )
		local ret = { x = -self.x, y = -self.y}
		setmetatable(ret,Vector2)
		return ret
	end,

	__eq = function( lhs, rhs)
		if math.abs( lhs.x - rhs.x) > 0.000001 then return false end
		if math.abs( lhs.y - rhs.y) > 0.000001 then return false end
		return true
	end,

	__tostring = function (self)
		local s = string.format("Vector2: %.6f, %.6f",self.x,self.y)
		return s
	end,
		

	get_Length = function( self )
		local len = math.sqrt( self.x*self.x + self.y*self.y)
		return len
	end,

	Normalize = function( self )
		local len = Vector2.get_Length(self)
		if len > 1e-5 then
			self.x = 0; self.y = 0;
		else
			self.x = self.x / len; self.y = self.y / len;
		end
	end,

	Dot = function ( lhs, rhs)
		local ret = lhs.x * rhs.x + lhs.y*rhs.y
		return ret
	end,


}


Vector2.__index = function(t,k)
	local mt = getmetatable(t)
	local v = mt[k]
	if v then return v end
	v = mt["get_" .. k]
	if v then return v(t) end
	return nil
end



Vector2.zero = Vector2.new(0,0)
Vector2.one = Vector2.new(1,1)
Vector2.right = Vector2.new(1,0)
Vector2.up = Vector2.new(0,1)

return Vector2

---------------------
