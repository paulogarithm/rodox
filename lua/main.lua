require("rodox-api")

local a = Vector3.zero
local b = Vector3.new(30, 0, 0)
local c = Vector3.new(1, -2, 3)
print((b - a).Magnitude)
print(b.Unit)
print(b:Max(c))
print(c:Abs())
print(c:Sign())
print(Vector3.new(1, 3, -5):Dot(Vector3.new(4, -2, -1)) == 3)
print(Vector3.zero:Lerp(Vector3.one, .5))