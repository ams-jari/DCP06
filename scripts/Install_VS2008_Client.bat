@echo off
REM VS 2008 Client Setup - Bypass missing WCU (run as Administrator)
REM
REM Use this if the main VS 2008 launcher closes immediately after
REM "Install Visual Studio 2008" - typically because the wcu folder is missing.
REM
REM Before running: Enable .NET Framework 3.5 in Windows Features.
REM Right-click this file -> Run as administrator.

set "VS2008_ROOT=C:\Users\dell\Desktop\AMS\Development\VS2008"
set "CLIENT_SETUP=%VS2008_ROOT%\setup\setup.exe"

if not exist "%CLIENT_SETUP%" (
    echo ERROR: setup\setup.exe not found at %VS2008_ROOT%
    echo Check VS2008_ROOT path in this script.
    pause
    exit /b 1
)

echo Running VS 2008 Client setup (bypassing WCU)...
echo Make sure .NET 3.5 is enabled in Windows Features.
echo.
"%CLIENT_SETUP%"
pause
