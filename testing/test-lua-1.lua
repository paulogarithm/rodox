local Foo = require("foo")

local a = Foo.new()
a:display()

local b = Foo.new()
b:display()
b = nil
print(1)
collectgarbage("collect")
print(2)
