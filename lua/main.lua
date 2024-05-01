require("rodox-api")

local myAdder = Adder.new()
myAdder:inc(3)
myAdder:display()

local myEvent = Events.new()

myEvent:once(function()
    print("hello")
end)

myEvent:fire()
