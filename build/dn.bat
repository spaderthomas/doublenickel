set "DN_RELATIVE=%~dp0\.."
set "DN=%~f0"
for %%I in ("%DN_RELATIVE%") do set DN=%%~fI

set "DN_EXTERNAL=%DN%\external"
  set "DN_INCLUDE=%DN_EXTERNAL%\include"
    set "DN_IMGUI=%DN_INCLUDE%\imgui"
    set "DN_FREETYPE=%DN_INCLUDE%\freetype"
  set "DN_LIB=%DN_EXTERNAL%\lib\debug"
    set "DN_LIB_LUA=luajit-2.1.0.3-windows-x64.lib"
    set "DN_LIB_FREETYPE=freetype-2.10.4-windows-x64.lib"
    set "DN_LIB_GLFW=glfw-3.3.8-windows-x64.lib"
    set "DN_LIB_GLAD=glad-0.1.36-windows-x64.lib"
    set "DN_LIB_STEAM=steam_api64.lib"

set "DN_SWITCH_INCLUDE=/I%DN% /I%DN_INCLUDE% /I%DN_IMGUI% /I%DN_FREETYPE%"
set "DN_SWITCH_LIBS="%DN_LIB_FREETYPE%" "%DN_LIB_GLFW%" "%DN_LIB_GLAD%" "%DN_LIB_LUA%" "%DN_LIB_STEAM%""
