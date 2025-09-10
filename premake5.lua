workspace "Fort-Chess"
    architecture "x64"
    startproject "fort-chess"
    location "."
    configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder
IncludeDir = {}
IncludeDir["raylib"] = "../vendors/raylib/include"


LibDir = {}
LibDir["raylib"] = "../vendors/raylib/lib"



group "fort-chess"
    include "src/fort-chess.lua"