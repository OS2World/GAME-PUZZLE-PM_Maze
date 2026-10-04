@echo off
rem =========================================================================
rem Compile script for Maze (Open Watcom on OS/2 / ArcaOS)
rem =========================================================================

rem Auto-detect Watcom installation (OS/2 typical paths)
if "%WATCOM%"=="" (
    if exist C:\WATCOM\binp\wcc386.exe (
        set WATCOM=C:\WATCOM
    ) else if exist C:\WATCOM\bin\wcc386.exe (
        set WATCOM=C:\WATCOM
    ) else if exist C:\WATCOM2\binp\wcc386.exe (
        set WATCOM=C:\WATCOM2
    ) else if exist D:\WATCOM\binp\wcc386.exe (
        set WATCOM=D:\WATCOM
    ) else (
        echo ERROR: Watcom not found. Set WATCOM environment variable.
        exit 1
    )
)

rem Watcom OS/2 tools are installed under binp (Open Watcom on OS/2)
if exist %WATCOM%\binp\wcc386.exe (
    set PATH=%WATCOM%\binp;%PATH%
) else (
    set PATH=%WATCOM%\bin;%PATH%
)

rem Auto-detect OS/2 Toolkit
if "%OS2TK%"=="" (
    if exist C:\OS2TK45\h\os2.h (
        set OS2TK=C:\OS2TK45
    ) else if exist C:\OS2TK\h\os2.h (
        set OS2TK=C:\OS2TK
    ) else (
        echo WARNING: OS2TK not set, defaulting to C:\OS2TK45
        set OS2TK=C:\OS2TK45
    )
)

rem Set up environment for Open Watcom on OS/2
set PATH=%WATCOM%\bin;%PATH%
set INCLUDE=%WATCOM%\h;%OS2TK%\h;src;%INCLUDE%
set LIB=%WATCOM%\lib386;%OS2TK%\lib;%LIB%

rem Log file
set LOGFILE=compile-wat.log

rem Start logging - write header
echo ========================================== > %LOGFILE%
echo Maze Build Log >> %LOGFILE%
echo Date: %DATE% Time: %TIME% >> %LOGFILE%
echo WATCOM=%WATCOM% >> %LOGFILE%
echo OS2TK=%OS2TK% >> %LOGFILE%
echo ========================================== >> %LOGFILE%
echo. >> %LOGFILE%

rem Also show on screen
echo Building Maze with Open Watcom...
echo WATCOM=%WATCOM%
echo OS2TK=%OS2TK%
echo Log: %LOGFILE%
echo.

rem Clean first
echo [CLEAN] Running wmake clean...
echo [CLEAN] Running wmake clean... >> %LOGFILE%
wmake -f makefile.wat clean >> %LOGFILE% 2>&1
type %LOGFILE%

rem Build - redirect stdout to log, show on screen via TYPE after
echo [BUILD] Running wmake all...
echo [BUILD] Running wmake all... >> %LOGFILE%
wmake -f makefile.wat all >> %LOGFILE% 2>&1

rem Show build output on screen
type %LOGFILE%

rem Check result
if exist bin\Maze.exe (
    echo.
    echo BUILD OK
    echo BUILD OK >> %LOGFILE%
    exit 0
) else (
    echo.
    echo BUILD FAILED
    echo BUILD FAILED >> %LOGFILE%
    exit 1
)