@ECHO OFF
SETLOCAL
REM Run post-build steps only, without recompiling or linking.
REM Use this when you only changed resources (SVG, tokens, DCP06_TOK.HPP) or want to
REM reinstall/copy without a full rebuild. The DLL must already exist from a prior build.
REM
REM Requires: SYSTEM1500_ROOT env var (or set it to Captivate_PluginSDK path).

pushd "%~dp0"
cd ..
set "REPO=%CD%"
popd

set "PROJ_DIR=%REPO%\Project\MSVS"
set "XML_FILE=%PROJ_DIR%\DCP06_Release_Win32.xml"

REM Get SYSTEM1500_ROOT (SDK path)
if not defined SYSTEM1500_ROOT (
    set "SYSTEM1500_ROOT=%REPO%\..\Captivate_PluginSDK_x86_v10.00_RC\SWXIII_PluginDevKit"
)
if not exist "%SYSTEM1500_ROOT%\Tools" (
    echo ERROR: SYSTEM1500_ROOT not set or invalid. Point it to the SDK.
    exit /b 1
)

echo === Convert the svg files from SWXRes_Source folders to pngs =======================
"%SYSTEM1500_ROOT%\Tools\SWXResBuilder\SWXResBuilder.exe" "%REPO%" "%INCSCAPE%"

echo === Generate language files (DCP06.men to DCP06.LEN) =======================
call "%REPO%\scripts\build_lang.bat"
if errorlevel 1 exit /b 1

echo === Install application on local machine ======================================
REM Use simulator path (avoids %% expansion issues in batch). Change if your path differs.
set "INSTALL_ROOT=C:\Users\dell\Desktop\AMS\Captivate 10 simulator\Leica Captivate\TS\Leica Geosystems\Leica Captivate\System"
"%SYSTEM1500_ROOT%\Tools\MKTools\MkEdit.exe" "-M:mk" "-F:%XML_FILE%" "-C" "-I:%INSTALL_ROOT%"
if errorlevel 1 (
    echo NOTE: MkEdit install may fail without valid dongle - licensing check.
    echo       Earlier steps completed. You can still copy DCP06.LEN manually.
)

echo === Generate loadable application file (*.dxx)  ==================================
"%SYSTEM1500_ROOT%\Tools\MKTools\MkEdit.exe" "-M:mk" "-F:%XML_FILE%" "-C"
if errorlevel 1 (
    echo NOTE: MkEdit .dxx step may fail without valid dongle.
)

echo.
echo Post-build steps finished. MkEdit may have failed if no dongle - that is expected.
exit /b 0
