@ECHO off
SetLocal EnableDelayedExpansion
IF NOT EXIST bin mkdir bin
IF NOT EXIST bin\int mkdir bin\int

call vcvarsall.bat x64
SET cc=cl.exe

REM ------------------
REM    Main Project
REM ------------------

REM ==============
REM Gets list of all C files
SET c_filenames= 
FOR %%f in (source\*.c) do SET c_filenames=!c_filenames! %%f
REM ==============

REM ==============
SET compiler_flags=/MD /Zc:preprocessor /Zi /wd4090 /wd5105 /FC
SET include_flags=/I.\source\ /I.\third_party\include\ /I.\third_party\source\
SET linker_flags=/link /DEBUG shell32.lib user32.lib winmm.lib userenv.lib gdi32.lib
SET output=/Fe.\bin\testbed /Fo.\bin\int\
SET defines=/D_DEBUG /D_CRT_SECURE_NO_WARNINGS
REM ==============

ECHO Building testbed.exe...
%cc% %compiler_flags% %c_filenames% %defines% %include_flags% %output% %linker_flags%
