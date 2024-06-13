@echo off
SetLocal EnableDelayedExpansion

set cxxs=
for /R %%f in (*.cpp) do (
    set cxxs=!cxxs! %%f
)

set includep=-Iinclude -I%VULKAN_SDK%/Include
set libp=-L%VULKAN_SDK%/Lib -Llib
set libflags=-lvulkan-1 -lassimp.dll -lglfw3dll
set defines=-D VGE_DEBUG

g++ -g %defines% %includep% %libp% %cxxs% %libflags% -o engine.exe
