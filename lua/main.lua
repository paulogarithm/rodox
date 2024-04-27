require("rodox-api")

local myAdder = Adder.new()
myAdder:inc(3)
myAdder:display()

local a = Events.once(function()
    print("hello")
end)
print("a pid: " .. tostring(a))

local b = Events.once(function()
    print("world")
end)
print("b pid: " .. tostring(b))
