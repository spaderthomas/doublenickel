@echo off
if not defined DevEnvDir (
  call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
)

set "project=SdfClock"
set "DN_EXAMPLES_DIR=..\.."
set "PROJECT_DIR=%DN_EXAMPLES_DIR%\sdf_clock"
set "SOURCE_DIR=%PROJECT_DIR%\source"
set "THIRD_PARTY=%DN_EXAMPLES_DIR%\thirdparty"

call "%THIRD_PARTY%\doublenickel\build\dn.bat"

@echo on
echo %DN_SWITCH_INCLUDE%
mkdir intermediate
copy "%DN_STEAM%" .

@echo on
cl.exe ^
/Zi /Od /JMC ^
/Fe%project%.exe /Fo.\intermediate\ /Fd.\intermediate\ ^
%SOURCE_DIR%\main.c ^
%DN_SWITCH_INCLUDE% ^
/MDd ^
/TC ^
/std:c11 ^
/Zc:wchar_t /Zc:forScope /Zc:inline ^
/experimental:c11atomics ^
/EHa ^
/W3 /wd"4530" /wd"4201" /wd"4577" /wd"4310" /wd"4624" /wd"4099" /wd"4068" /wd"4267" /wd"4244" /wd"4018" ^
/D "DN_EDITOR" /D "_CRT_SECURE_NO_WARNINGS" /D "_SILENCE_CXX17_ALL_DEPRECATION_WARNINGS" ^
/link /LIBPATH:%DN_LIB% /DEBUG:FULL /MACHINE:X64 /NOLOGO /SUBSYSTEM:CONSOLE /INCREMENTAL:NO /NOIMPLIB /NOEXP /PDB:.\intermediate\ /ignore:4099 /ignore:4204 ^
%DN_SWITCH_LIBS% "user32.lib" "opengl32.lib" "gdi32.lib" "Shell32.lib" "Kernel32.lib" "Advapi32.lib" "Ole32.lib" "OleAut32.lib"
pause
