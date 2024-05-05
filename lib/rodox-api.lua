--|
--| rodox-api.lua
--|
--| Bonjour à tous.
--| This file contains all the globals variables needed for rodox.  Make sure
--| to build the files, and set the lua path/cpath to this directory before
--| running your project.
--|
--| This software is under the MIT Licence. See the LICENSE file for more
--| details.
--|
--| Made with <3 by paulogarithm
--| 

local COLORSUPPORT = string.find(os.getenv("TERM") or "", "color")

function _G.warn(...)
    local info = debug.getinfo(2, "Sl")
    local s = string.format(
        "%s%s-  Client  -  %s:%d%s",
        COLORSUPPORT and "\x1b[m" or "",
        COLORSUPPORT and "\x1b[90m" or "",
        info.short_src, info.currentline,
        COLORSUPPORT and "\x1b[m" or ""
    )
    if COLORSUPPORT then io.write("\x1b[1;33m") end
    print(..., s)
end

_G.Adder = require("adder")
_G.Events = require("events")
_G.Vector3 = require("__vector3")
_G.Color3 = require("__color3")
