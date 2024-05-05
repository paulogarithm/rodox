--|
--| Rodox Vector3 - Data Types
--|

local Vector3 = {}

local vectorMt = {}

local abs, floor, ceil = _G.math.abs, _G.math.floor, _G.math.ceil
local min, max, sqrt = _G.math.min, _G.math.max, _G.math.sqrt
local acos = _G.math.acos
local assert, warn = _G.assert, _G.warn

local function errorCase(x, y, z)
    local t = { x = type(x), y = type(y), z = type(z) }
    for k, v in next, t do
        assert(v == "number", k .. ": number expected, got " .. v)
    end
end

local function vector3CtorProxy(x, y, z, isUnit)
    errorCase(x, y, z)
    local v = {
        X = x,
        Y = y,
        Z = z,
        Magnitude = sqrt(x * x + y * y + z * z),
    }
    if (not isUnit) then
        v.Unit = vector3CtorProxy(
            x / v.Magnitude,
            y / v.Magnitude,
            z / v.Magnitude,
            true
        )
    end
    return setmetatable(v, vectorMt)
end

function Vector3.new(x, y, z)
    return vector3CtorProxy(x, y, z)
end

vectorMt.__index = {}

local function getSign(n)
    return ((n == 0) and 0) or ((n < 0) and -1) or 1
end

local function vectorMap(v, f, arg)
    if (not arg) then
        return Vector3.new(f(v.X), f(v.Y), f(v.Z))
    end
    return Vector3.new(f(v.X, arg.X), f(v.Y, arg.Y), f(v.Z, arg.Z))
end

function vectorMt.__index:Abs()
    return vectorMap(self, abs)
end

function vectorMt.__index:Floor()
    return vectorMap(self, floor)
end

function vectorMt.__index:Ceil()
    return vectorMap(self, ceil)
end

function vectorMt.__index:Min(foreign)
    return vectorMap(self, min, foreign)
end

function vectorMt.__index:Max(foreign)
    return vectorMap(self, max, foreign)
end

function vectorMt.__index:Sign()
    return vectorMap(self, getSign)
end

function vectorMt.__index:Angle(other)
    return acos(self:Dot(other) / (self.Magnitude * other.Magnitude))
end

function vectorMt.__index:Dot(other)
    return (self.X * other.X + self.Y * other.Y + self.Z * other.Z )
end

function vectorMt.__index:Cross(other)
    return self.Magnitude * other.Magnitude * self:Angle(other)
end

function vectorMt.__index:Lerp(goal, alpha)
    return self + (goal - self) * alpha
end


function vectorMt:__newindex()
    warn("the vector class is locked.")
end

function vectorMt:__add(foreign)
    return Vector3.new(
        self.X + foreign.X,
        self.Y + foreign.Y,
        self.Z + foreign.Z
    )
end

function vectorMt:__eq(foreign)
    return self.X == foreign.X and self.Y == foreign.Y and self.Z == foreign.Z
end

function vectorMt:__sub(foreign)
    return Vector3.new(
        self.X - foreign.X,
        self.Y - foreign.Y,
        self.Z - foreign.Z
    )
end

function vectorMt:__mul(foreign)
    if (type(foreign) == "number") then
        return Vector3.new(
            self.X * foreign, self.Y * foreign, self.Z * foreign
        )
    end
    return Vector3.new(
        self.X * foreign.X, self.Y * foreign.Y, self.Z * foreign.Z
    )
end

function vectorMt:__div(foreign)
    if (type(foreign) == "number") then
        return Vector3.new(
            self.X / foreign, self.Y / foreign, self.Z / foreign
        )
    end
    return Vector3.new(
        self.X / foreign.X, self.Y / foreign.Y, self.Z / foreign.Z
    )
end

function vectorMt:__tostring()
    local buf = "{ "
        .. "x = " .. tostring(self.X) .. ", "
        .. "y = " .. tostring(self.Y) .. ", "
        .. "z = " .. tostring(self.Z) .. " }"
    return buf
end

Vector3.one = Vector3.new(1, 1, 1)
Vector3.xAxis = Vector3.new(1, 0, 0)
Vector3.yAxis = Vector3.new(0, 1, 0)
Vector3.zAxis = Vector3.new(0, 0, 1)
Vector3.zero = Vector3.new(0, 0, 0)

return Vector3
