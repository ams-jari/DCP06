@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION
REM Generate DCP06.LEN for simulator localization (Release | Win32 post-build).
REM 1. hpp_to_men.py: generate Text\Languages\en\DCP06.men from DCP06_TOK.HPP
REM 2. TextTool: compile .men -> .LEN (REQUIRED - no fallback; DCP05 strings are wrong for DCP06)
REM 3. Copy DCP06.LEN to simulator Plugin\DCP06\en\ (MkEdit first step does not copy languages)

SET REPO=%~dp0..
SET MEN_FILE=%REPO%\Text\Languages\en\DCP06.men
SET LEN_FILE=%REPO%\Text\Languages\en\DCP06.LEN

REM Get simulator path via PowerShell (reliable; cmd FOR/F fails on paths with spaces)
FOR /F "usebackq delims=" %%A IN (`powershell -NoProfile -Command "(Get-ItemProperty -Path 'HKLM:\Software\Wow6432Node\Leica Geosystems\System 1500\TS\Configuration' -Name DeviceRootPathRelease -ErrorAction SilentlyContinue).DeviceRootPathRelease"`) DO SET SIM_BASE=%%A

echo === Generate DCP06.men from DCP06_TOK.HPP =======================
python "%REPO%\scripts\hpp_to_men.py"

REM TextTool requires LangAndTypes.xml in the same folder as .men to produce .LEN (not just .L)
SET "LANG_EN=%REPO%\Text\Languages\en"
SET "LANGTYPES_SDK=%REPO%\..\Captivate_PluginSDK_x86_v10.00_RC\SWXIII_PluginDevKit\Samples\Common\HelloWorldBase\Text\English\LangAndTypes.xml"
IF NOT DEFINED SYSTEM1500_ROOT SET "SYSTEM1500_ROOT=%REPO%\..\Captivate_PluginSDK_x86_v10.00_RC\SWXIII_PluginDevKit"
IF NOT EXIST "%LANG_EN%\LangAndTypes.xml" (
    IF EXIST "%LANGTYPES_SDK%" (
        copy /Y "%LANGTYPES_SDK%" "%LANG_EN%\LangAndTypes.xml" >nul
        echo Copied LangAndTypes.xml from SDK (required for TextTool to produce .LEN)
    ) ELSE IF EXIST "%SYSTEM1500_ROOT%\Samples\Common\HelloWorldBase\Text\English\LangAndTypes.xml" (
        copy /Y "%SYSTEM1500_ROOT%\Samples\Common\HelloWorldBase\Text\English\LangAndTypes.xml" "%LANG_EN%\LangAndTypes.xml" >nul
        echo Copied LangAndTypes.xml from SDK (required for TextTool to produce .LEN)
    ) ELSE (
        echo WARNING: LangAndTypes.xml not found. TextTool may not produce DCP06.LEN.
    )
)
IF ERRORLEVEL 1 (
    echo ERROR: hpp_to_men.py failed. Cannot generate language files.
    EXIT /B 1
)

echo === Compile DCP06.men to DCP06.LEN (TextTool) ===================
REM Use PowerShell to find and run TextTool (avoids 32-bit cmd path issues with 64-bit Program Files)
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0run_texttool.ps1" "%MEN_FILE%"
IF ERRORLEVEL 1 (
    echo ERROR: TextTool failed or not found.
    EXIT /B 1
)

IF NOT EXIST "%LEN_FILE%" (
    echo ERROR: DCP06.LEN was not produced.
    EXIT /B 1
)

echo === Copy DCP06.LEN to simulator Plugin\DCP06\en\ ===================
SET COPY_OK=0

REM 1. Registry path (DeviceRootPathRelease)
IF DEFINED SIM_BASE (
    SET "DCP06_EN=!SIM_BASE!\Leica Geosystems\Leica Captivate\System\Plugin\DCP06\en"
    IF NOT EXIST "!DCP06_EN!" mkdir "!DCP06_EN!"
    copy /Y "%LEN_FILE%" "!DCP06_EN!\DCP06.LEN" >nul
    IF NOT ERRORLEVEL 1 (
        SET COPY_OK=1
        echo Installed to registry path: !DCP06_EN!
    )
)

REM 2. Common Captivate install paths (MkEdit -I uses these) + Captivate 10 simulator
REM 3. SDK Applications path (when using symlink: Applications\Common\DCP06 -> repo)
SET "SDK_DCP06_EN=%SYSTEM1500_ROOT%\Applications\Common\DCP06\Text\Languages\en"
IF DEFINED SYSTEM1500_ROOT IF EXIST "!SDK_DCP06_EN!" (
    copy /Y "%LEN_FILE%" "!SDK_DCP06_EN!\DCP06.LEN" >nul 2>&1
    IF NOT ERRORLEVEL 1 (
        SET COPY_OK=1
        echo Installed to SDK Applications path: !SDK_DCP06_EN!
    )
)
FOR %%P IN (
    "C:\Users\Public\Documents\Leica Captivate\CS_x64\Leica Geosystems\Leica Captivate\System\Plugin\DCP06\en"
    "C:\Users\Public\Documents\Leica Captivate\CS_32\Leica Geosystems\Leica Captivate\System\Plugin\DCP06\en"
    "C:\Users\dell\Desktop\AMS\Captivate 10 simulator\Leica Captivate\TS\Leica Geosystems\Leica Captivate\System\Plugin\DCP06\en"
) DO (
    IF NOT EXIST %%~P mkdir %%~P 2>nul
    copy /Y "%LEN_FILE%" "%%~P\DCP06.LEN" >nul 2>&1
    IF NOT ERRORLEVEL 1 (
        SET COPY_OK=1
        echo Installed to %%~P
    )
)

IF !COPY_OK!==0 (
    echo WARNING: Could not copy DCP06.LEN to any simulator path.
    echo Check registry DeviceRootPathRelease and build_lang.bat paths.
    EXIT /B 1
)
echo DCP06.LEN installed successfully.
EXIT /B 0
