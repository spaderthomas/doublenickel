IF NOT "%~1"=="" (SHIFT & GOTO %~1)

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
    set "DN_LIB_CIMGUI=cimgui-1.91.9-windows-x64.lib"
    set "DN_LIB_STEAM=steam_api64.lib"
    set "DN_DLL_CIMGUI=%DN_LIB%\cimgui-1.91.9-windows-x64.dll"
    set "DN_DLL_STEAM=%DN_LIB%\steam_api64.dll"
    set "DN_DLLS="%DN_DLL_CIMGUI%" "%DN_DLL_STEAM%""

set "DN_SWITCH_INCLUDE=/I%DN% /I%DN_INCLUDE% /I%DN_IMGUI% /I%DN_FREETYPE%"
set "DN_SWITCH_LIBS="%DN_LIB_FREETYPE%" "%DN_LIB_GLFW%" "%DN_LIB_GLAD%" "%DN_LIB_LUA%" "%DN_LIB_STEAM%" "%DN_LIB_CIMGUI%""

set "DN_FOREGROUND_BLUE=[34m"
set "DN_FOREGROUND_MAGENTA=[35m"
set "DN_FOREGROUND_RESET=[0m"

EXIT /B

:DN_ECHO_COLOR
  @ECHO ON
  @ECHO %~1 %~2: %DN_FOREGROUND_RESET% %~3
  @ECHO OFF
  EXIT /B

:DN_ECHO_INFO
  CALL :DN_ECHO_COLOR %DN_FOREGROUND_BLUE% DN_BUILD_TOOLS "%~1"
  EXIT /B

:DN_ECHO_USER
  CALL :DN_ECHO_COLOR %DN_FOREGROUND_MAGENTA% DN_USER "%~1"
  EXIT /B

:DN_INITIALIZE_VS_PROMPT
  SET DN_VS_YEAR="%~1"
  IF %DN_VS_YEAR%=="" (SET "DN_VS_YEAR=2022")
  SET DN_VS_VERSION="%~2"
  IF %DN_VS_VERSION%=="" (SET "DN_VS_VERSION=Community")
  SET DN_VS_PREFIX="%~3"
  IF %DN_VS_PREFIX%=="" (SET "DN_VS_PREFIX=C:\Program Files")

  SET DN_VSDEVCMD="%DN_VS_PREFIX%\Microsoft Visual Studio\%DN_VS_YEAR%\%DN_VS_VERSION%\Common7\Tools\VsDevCmd.bat"
  SET DN_VSDEVCMD_FLAGS=/arch=amd64 /host_arch=amd64 /no_logo

  CALL %DN_VSDEVCMD% %DN_VSDEVCMD_FLAGS%
  CALL :DN_ECHO_INFO "Initialized Visual Studio command prompt with arguments: %DN_VSDEVCMD_FLAGS%"
  EXIT /B

:DN_COPY_DLLS
  for %%f in (%DN_DLLS%) do (
    COPY "%%f" "%~1"
  )
  EXIT /B

:NORMALIZEPATH
  SET RETVAL=%~f1
  IF NOT "%~2"=="" (
    SET "%~2=%RETVAL%"
  )
  EXIT /B