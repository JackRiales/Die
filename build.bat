@echo off

REM Preprocessor defines
SET ProcDef=_DEBUG

REM File targets
SET bindir=bin
SET bin=Die.exe

REM Source names
SET Sources=..\die.c

REM Compiler and linker flags
SET ccflags=/Od /nologo /MTd /fp:fast /Oi /W4 /FC /Zi /Gm -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -wd4996
SET ccflags=-D_DEBUG=1 %ccflags%
SET ldflags=user32.lib

REM Need to call vcvarsall so we have instant access to cl
call "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/vcvarsall.bat"

REM Renew build and bin folders
if not exist %bindir% mkdir %bindir%

cd %bindir%

REM Primary executable
cl %ccflags% %Sources% -Fmdie.map /D%ProcDef% /Fe%bin% %ldflags%

cd ..