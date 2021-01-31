@echo off
echo This script must be run as an administrator

@echo off

:: BatchGotAdmin
:-------------------------------------
REM  --> Check for permissions
    IF "%PROCESSOR_ARCHITECTURE%" EQU "amd64" (
>nul 2>&1 "%SYSTEMROOT%\SysWOW64\cacls.exe" "%SYSTEMROOT%\SysWOW64\config\system"
) ELSE (
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"
)

REM --> If error flag set, we do not have admin.
if '%errorlevel%' NEQ '0' (
    echo Requesting administrative privileges...
    goto UACPrompt
) else ( goto gotAdmin )

:UACPrompt
    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
    set params= %*
    echo UAC.ShellExecute "cmd.exe", "/c ""%~s0"" %params:"=""%", "", "runas", 1 >> "%temp%\getadmin.vbs"

    "%temp%\getadmin.vbs"
    del "%temp%\getadmin.vbs"
    exit /B

:gotAdmin
    pushd "%CD%"
    CD /D "%~dp0"
:--------------------------------------    

echo.
echo ---------------------------
echo Removing Existing Links
echo ---------------------------
@echo on
rmdir IncPSP
rmdir SrcPSP
rmdir IncInterface
rmdir SrcInterface
@echo off
echo.
echo.
echo ---------------------------
echo Adding Personalized Links
echo ---------------------------
@echo on
mklink /D IncPSP ..\..\..\MITCH_2021\IncPSP
mklink /D SrcPSP ..\..\..\MITCH_2021\SrcPSP
mklink /D IncInterface ..\..\..\MITCH_2021\IncInterface
mklink /D SrcInterface ..\..\..\MITCH_2021\SrcInterface
@echo off
echo.
echo ---------------------------
echo Script Complete
echo ---------------------------
pause