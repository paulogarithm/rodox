local Events = require("events")

local e = Events.new()

e:once(function ()
    print("Hello world")
end)

-- e:fire()