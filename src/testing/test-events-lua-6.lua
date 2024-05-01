local Events = require("events")

local e = Events.new()

e:once(function ()
    print("Hello world")
end)
e:once(function ()
    print("Foo")
end)

e:fire()