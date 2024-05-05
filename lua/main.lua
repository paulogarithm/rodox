require("rodox-api")

local color = Color3.fromHex("b00b69")
print(color.R * 255, color.G *255, color.B * 255)

local red = Color3.fromHex("FF0000")
local magenta = Color3.fromHex("ec008c")
local black = Color3.fromHex("000")
local white = Color3.fromHex("#FFF")
print(red.R, magenta.G, black.B, white.R)

white = Color3.new(1, 1, 1)
black = Color3.new(0, 0, 0)

local gray10 = white:Lerp(black, 0.1)
print(gray10)  --> 0.9, 0.9, 0.9

local gray50 = white:Lerp(black, 0.5)
print(gray50)  --> 0.5, 0.5, 0.5

red = Color3.fromRGB(255, 0, 0)
magenta = Color3.fromRGB(236, 0, 140)

local redHex = red:ToHex()
print(redHex)  --> ff0000

local magentaHex = magenta:ToHex()
print(magentaHex)  --> ec008c