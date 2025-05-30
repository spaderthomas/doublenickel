IF NOT "%~1"=="" (SHIFT & GOTO %~1)

SET "DN_RELATIVE=%~dp0\.."
SET "DN=%~f0"
for %%I in ("%DN_RELATIVE%") do SET DN=%%~fI

SET "DN_H=%DN%\dn.h"
SET "DN_SOURCE=%DN%\source"
  SET "DN_SCRIPTS=%DN_SOURCE%\scripts"
    SET "DN_DATA=%DN_SCRIPTS%\data"
      SET "DN_FFI_H=%DN_DATA%\dn_ffi.h"
SET "DN_EXTERNAL=%DN%\external"
  SET "DN_INCLUDE=%DN_EXTERNAL%\include"
    SET "DN_INCLUDE_IMGUI=%DN_INCLUDE%\cimgui"
    SET "DN_INCLUDE_FREETYPE=%DN_INCLUDE%\freetype"
    SET "DN_INCLUDE_LUAJIT=%DN_INCLUDE%\lua"
  SET "DN_LIB=%DN_EXTERNAL%\lib\debug"
    SET "DN_LIB_GLFW=%DN_LIB%\glfw3.lib"
    SET "DN_DLL_GLFW=%DN_LIB%\glfw3.dll"
    SET "DN_PDB_GLFW=%DN_LIB%\glfw3.pdb"

    SET "DN_LIB_CIMGUI=%DN_LIB%\cimgui.lib"
    SET "DN_DLL_CIMGUI=%DN_LIB%\cimgui.dll"
    SET "DN_PDB_CIMGUI=%DN_LIB%\cimgui.pdb"

    SET "DN_LIB_LUAJIT=%DN_LIB%\lua51.lib"
    SET "DN_DLL_LUAJIT=%DN_LIB%\lua51.dll"
    SET "DN_PDB_LUAJIT=%DN_LIB%\lua51.pdb"

    SET "DN_LIB_FREETYPE=%DN_LIB%\freetype-2.10.4-windows-x64.lib"

    SET "DN_LIB_GLAD=%DN_LIB%\glad-0.1.36-windows-x64.lib"

    SET "DN_LIB_STEAM=%DN_LIB%\steam_api64.lib"
    SET "DN_DLL_STEAM=%DN_LIB%\steam_api64.dll"

    SET "DN_DLLS="%DN_DLL_CIMGUI%" "%DN_DLL_GLFW%" "%DN_DLL_STEAM%" "%DN_DLL_LUAJIT%""
    SET "DN_PDBS="%DN_PDB_CIMGUI%" "%DN_PDB_GLFW%" "%DN_PDB_LUAJIT%""

SET "DN_SWITCH_INCLUDE=/I%DN% /I%DN_INCLUDE% /I%DN_INCLUDE_IMGUI% /I%DN_INCLUDE_FREETYPE%"
SET "DN_SWITCH_LIBS="%DN_LIB_FREETYPE%" "%DN_LIB_GLFW%" "%DN_LIB_GLAD%" "%DN_LIB_LUAJIT%" "%DN_LIB_STEAM%" "%DN_LIB_CIMGUI%""

SET "DN_FOREGROUND_BLACK=[30m"
SET "DN_FOREGROUND_RED=[31m"
SET "DN_FOREGROUND_GREEN=[32m"
SET "DN_FOREGROUND_YELLOW=[33m"
SET "DN_FOREGROUND_BLUE=[34m"
SET "DN_FOREGROUND_MAGENTA=[35m"
SET "DN_FOREGROUND_CYAN=[36m"
SET "DN_FOREGROUND_WHITE=[37m"
SET "DN_BACKGROUND_BLACK=[40m"
SET "DN_BACKGROUND_RED=[41m"
SET "DN_BACKGROUND_GREEN=[42m"
SET "DN_BACKGROUND_YELLOW=[43m"
SET "DN_BACKGROUND_BLUE=[44m"
SET "DN_BACKGROUND_MAGENTA=[45m"
SET "DN_BACKGROUND_CYAN=[46m"
SET "DN_BACKGROUND_WHITE=[47m"
SET "DN_FOREGROUND_RESET=[0m"

SET "DN_REDIRECT_OUTPUT=>NUL 2>&1"
EXIT /B

:::::::::::::
:: TARGETS ::
:::::::::::::
:DN_BUILD_FFI
  CALL :DN_INITIALIZE_VS_PROMPT

  SET "DN_FFI_H_TMP=%DN_FFI_H%.tmp"
  SET "PREPROCESSOR_DEFINES=/D DN_BUILD_FFI=1"

  @ECHO ON
  cl.exe /EP %PREPROCESSOR_DEFINES% %DN_H% > %DN_FFI_H%
  @ECHO OFF

  FINDSTR /R /V "^$" %DN_FFI_H% > %DN_FFI_H_TMP%
  MOVE /Y %DN_FFI_H_TMP% %DN_FFI_H%
  FINDSTR /R /V /B /C:"@DN_FFI_CANARY" %DN_FFI_H% > %DN_FFI_H_TMP%
  MOVE /Y %DN_FFI_H_TMP% %DN_FFI_H%
  CALL :DN_ECHO_INFO "Successfully rebuilt %DN_FFI_H%"


  EXIT /B

:DN_COPY_DLLS
  CALL :DN_ECHO_INFO "Copying external DLLs to build directory..."

  FOR %%F IN (%DN_DLLS%) DO (
    CALL :DN_ECHO_INFO "Copying %%~F to %~1"
    COPY "%%F" "%~1"
  )

  EXIT /B

:DN_COPY_PDBS
  CALL :DN_ECHO_INFO "Copying external PDBs to build directory..."
  FOR %%F IN (%DN_PDBS%) DO (
    CALL :DN_ECHO_INFO "Copying %%~F to %~1"
    COPY "%%F" "%~1"
  )
  EXIT /B

:::::::::::::::
:: UTILITIES ::
:::::::::::::::
:DN_ECHO_COLOR
  @ECHO ON
  @ECHO %~1%~2:%DN_FOREGROUND_RESET% %~3
  @ECHO OFF
  EXIT /B

:DN_ECHO_COLOR_NO_TAG
  @ECHO ON
  @ECHO %~1%~2%DN_FOREGROUND_RESET%
  @ECHO OFF
  EXIT /B

:DN_ECHO_RESULT
  IF %ERRORLEVEL% EQU 0 (
    CALL :DN_ECHO_INFO "%DN_FOREGROUND_GREEN%OK!%DN_BACKGROUND_RESET%"
  ) ELSE (
    CALL :DN_ECHO_INFO "%DN_FOREGROUND_RED%FAIL!%DN_BACKGROUND_RESET%"
  )
  EXIT /B

:DN_ECHO_ERROR
  CALL :DN_ECHO_COLOR %DN_FOREGROUND_RED% DN_ERROR "%~1"
  EXIT /B

:DN_ECHO_INFO
  CALL :DN_ECHO_COLOR %DN_FOREGROUND_BLUE% DN_BUILD_TOOLS "%~1"
  EXIT /B

:DN_ECHO_USER
  CALL :DN_ECHO_COLOR %DN_FOREGROUND_MAGENTA% DN_MESSAGE "%~1"
  EXIT /B

:DN_ECHO_TARGET
  CALL :DN_ECHO_COLOR %DN_FOREGROUND_YELLOW% DN_TARGET "%~1"
  EXIT /B
  
:DN_UNQUOTE
  SET %~2=%~1:"=%
  EXIT /B

:DN_NORMALIZE_PATH
  SET RETVAL=%~f1
  IF NOT "%~2"=="" (
    SET "%~2=%RETVAL%"
  )
  EXIT /B

:DN_REPLACE_FILE
  IF NOT EXIST %2 (
    CALL :DN_ECHO_ERROR "DN_REPLACE_FILE: Source file %2 does not exist"
    EXIT /B
  )

  IF EXIST %1 (
    DEL %1
  )

  CALL :DN_ECHO_INFO "Replacing %1 with %2"
  COPY %2 %1
  EXIT /B

:DN_INITIALIZE_VS_PROMPT
  IF DEFINED VisualStudioVersion ( EXIT /B )

  SET "DN_VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
  SET "DN_VSWHERE_FLAGS=-latest -products * -requires Microsoft.Component.MSBuild -property installationPath"

  FOR /f "usebackq tokens=*" %%P IN (`"%DN_VSWHERE%" %DN_VSWHERE_FLAGS%`) do (
    ECHO %%P
    set "DN_VS_PREFIX=%%P"
  )

  SET "DN_VSDEVCMD=%DN_VS_PREFIX%\Common7\Tools\VsDevCmd.bat"
  SET "DN_VSDEVCMD_FLAGS=/arch=amd64 /host_arch=amd64 /no_logo"

  CALL :DN_ECHO_INFO "Initializing Visual Studio command prompt"
  CALL :DN_ECHO_INFO "%DN_VSDEVCMD% %DN_VSDEVCMD_FLAGS%"

  CALL "%DN_VSDEVCMD%" %DN_VSDEVCMD_FLAGS%
  
  EXIT /B
