project "ProjName"
    location "."
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


    -- Remember to use -- for single line comments and --[[ Comment --]] for multiline comments

    -- Include directories
    includedirs 
    {
        "%{IncludeDir.item}", -- This is the included directory from vendors
        "global",            
        "include",           
        "json",
        "src"
    }

    -- Files
    files 
    {
        "src/**.cpp",           
        "include/**.h",     
        "include/**.inl",     

        "global/**.h",
        "global/**.cpp",

        "json/**.json",

    }

    -- Library directories
    libdirs 
    { 
        "%{LibDir.item}", -- This is the lib directory from vendors
    }

    -- Links
    links 
    { 
        "item", -- The actual name of the library from vendors
    }

    pchheader "headers.h" -- For precompiled headers
    pchsource "global/headers.cpp"
    -- pchsource "headers.cpp" -- For precompiled headers

    defines { "ITEM" } -- Any definintions that are nessesary


    -- Toolset and compiler settings
    filter "toolset:msc"
        buildoptions { "/std:c++23" } 
        
    -- GCC/Clang settings
    filter "toolset:gcc or toolset:clang"
        buildoptions { "-std=c++23" }
    
    -- Configuration settings
    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"
        optimize "Off"
        runtime "Debug"  

    filter "configurations:Release"
        symbols "Off"
        optimize "On"
        defines "NDEBUG"
        runtime "Release"  

    -- Windows system settings
    filter "system:windows"
        systemversion "latest"
        defines "PLATFORM_WINDOWS"
        staticruntime "on"

    -- Linux Settings
    filter "system:linux"
        defines "PLATFORM_LINUX"
        staticruntime "off"
    
    -- Apple Settings
    filter "system:macosx"
        defines "PLATFORM_MACOS"
        buildoptions { "-stdlib=libc++" }

    -- Visual Studio specific settings
    filter "action:vs*"
        defines "_CRT_SECURE_NO_WARNINGS"
    
    filter {}
    