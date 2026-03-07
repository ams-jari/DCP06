@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION
REM Generate DCP06.LEN for simulator localization (Release | Win32 post-build).
REM 1. hpp_to_men.py: generate Text\Languages\en\DCP06.men from DCP06_TOK.HPP
REM 2. TextTool: compile .men -> .LEN (REQUIRED - no fallback; DCP05 strings are wrong for DCP06)
REM 3. Copy DCP06.LEN to simulator Plugin\DCP06\en\ (MkEdit first step does not copy languages)

SET REPO=%~dp0..
SET TEXTTOOL="%ProgramFiles(x86)%\Leica Geosystems\Leica Geosystems TextTool\TextTool-VBNet.exe"
SET MEN_FILE=%REPO%\Text\Languages\en\DCP06.men
SET LEN_FILE=%REPO%\Text\Languages\en\DCP06.LEN

REM Get simulator path via PowerShell (reliable; cmd FOR/F fails on paths with spaces)
FOR /F "usebackq delims=" %%A IN (`powershell -NoProfile -Command "(Get-ItemProperty -Path 'HKLM:\Software\Wow6432Node\Leica Geosystems\System 1500\TS\Configuration' -Name DeviceRootPathRelease -ErrorAction SilentlyContinue).DeviceRootPathRelease"`) DO SET SIM_BASE=%%A

echo === Generate DCP06.men from DCP06_TOK.HPP =======================
python "%REPO%\scripts\hpp_to_men.py"
IF ERRORLEVEL 1 (
    echo ERROR: hpp_to_men.py failed. Cannot generate language files.
    EXIT /B 1
)

echo === Compile DCP06.men to DCP06.LEN (TextTool) ===================
IF NOT EXIST %TEXTTOOL% (
    echo ERROR: TextTool not found.
    echo Install TextToolSetup_v5.1.1.msi from SDK Tools\TextTool\
    echo Path: %SYSTEM1500_ROOT%\Tools\TextTool\TextToolSetup_v5.1.1.msi
    echo DCP06 requires its own strings - DCP05 fallback is not acceptable.
    EXIT /B 1
)
%TEXTTOOL% /S "%MEN_FILE%" /G
IF ERRORLEVEL 1 (
    echo ERROR: TextTool failed to compile DCP06.men.
    EXIT /B 1
)

IF NOT EXIST "%LEN_FILE%" (
    echo ERROR: DCP06.LEN was not produced.
    EXIT /B 1
)

echo === Copy DCP06.LEN to simulator Plugin\DCP06\en\ ===================
IF NOT DEFINED SIM_BASE (
    echo ERROR: Could not read simulator path from registry.
    EXIT /B 1
)
SET "DCP06_EN=!SIM_BASE!\Leica Geosystems\Leica Captivate\System\Plugin\DCP06\en"
IF NOT EXIST "!DCP06_EN!" mkdir "!DCP06_EN!"
copy /Y "%LEN_FILE%" "!DCP06_EN!\DCP06.LEN" >nul
IF ERRORLEVEL 1 (
    echo ERROR: Failed to copy DCP06.LEN to simulator.
    EXIT /B 1
)
echo Installed DCP06.LEN to simulator en folder.
EXIT /B 0
