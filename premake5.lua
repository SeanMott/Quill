workspace "Quill"
    --architecture "x86_64"
    architecture "x86"
    startproject "Quill"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Quill"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

    files 
    {
        "src/**.h",
        "src/**.cpp"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
    
        defines
        {
            "Window_Build"
        }
    
    filter "configurations:Debug"
        defines "Q_DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "Q_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "Q_DIST"
        optimize "On"