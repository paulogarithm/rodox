local Events = require("events")

local event = Events.new()
event:once(function()
    print("foo")
end)
print(event)
