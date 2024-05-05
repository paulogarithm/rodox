--|
--| Rodox Color3 - Data Types
--|

local Color3 = {}
local yourColor3 = {}
local mtColor3 = { __index = yourColor3 }

local sub, tonumber, assert, tostring, error, setmetatable, sformat, warn =
    _G.string.sub, _G.tonumber, _G.assert,
    _G.tostring, _G.error, _G.setmetatable,
    _G.string.format, _G.warn

---

function yourColor3:Lerp(other, alpha)
    return Color3.new(
        self.R + (other.R - self.R) * alpha,
        self.G + (other.G - self.G) * alpha,
        self.B + (other.B - self.B) * alpha
    )
end

function yourColor3:ToHex()
    return sformat("%02x%02x%02x", self.R*255, self.G*255, self.B*255)
end

---

function mtColor3:__tostring()
    return tostring(self.R) .. ", "
        .. tostring(self.G) .. ", "
        .. tostring(self.B)
end

function mtColor3:__eq(other)
    return (self.R == other.R) and (self.G == other.G) and (self.B == other.B)
end

function mtColor3:__newindex()
    warn("the color class is locked.")
end

---

function Color3.new(red, green, blue)
    return setmetatable({R = red, G = green, B = blue}, mtColor3)
end

function Color3.fromRGB(red, green, blue)
    return setmetatable({R = red/255, G = green/255, B = blue/255}, mtColor3)
end

function Color3.fromHex(hex)
    local n, r, g, b = 0, 0, 0, 0

    if (sub(hex, 1, 1) == "#")then
        hex = sub(hex, 2)
    end
    n = assert(tonumber(hex, 16), "bad string: " .. hex)
    if (#hex == 6) then
        r, g, b = (n & 0xFF0000) >> 16, (n & 0xFF00) >> 8, n & 0xFF
        return setmetatable({R = r/255, G = g/255, B = b/255}, mtColor3)
    elseif (#hex ~= 3) then
        error("expected 3 or 6 letters, got " .. tostring(#hex))
    end
    r, g, b = (n & 0xF00) >> 8, (n & 0xF0) >> 4, n & 0xF
    return setmetatable({R = r/15, G = g/15, B = b/15}, mtColor3)
end

return Color3