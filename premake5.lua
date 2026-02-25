workspace "Solution / Workspace Name"
    architecture "x64"
    startproject "ProjName"

    configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder
IncludeDir = {}
IncludeDir["item"] = "vendors/item/include"


LibDir = {}
LibDir["item"] = "vendors/item/lib"



group "ProjName"
    include "ProjName/ProjName.lua"