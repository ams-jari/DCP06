# Option B restructure - Move files to include/dcp06 and src domain folders
$root = Split-Path -Parent $PSScriptRoot  # DCP06 root
$hdr = Join-Path $root "Hdr"
$src = Join-Path $root "Src"
$inc = Join-Path $root "include\dcp06"
$srcOut = Join-Path $root "src"

# Core
$coreHdr = @("DCP_Defs.hpp","DCP_Types.hpp","DCP_Common.hpp","DCP_Model.hpp","DCP_MsgBox.hpp","DCP_InputText.hpp","DCP_InputTextModel.hpp","DCP_xyz.hpp","DCP_Aim.hpp","DCP_ChangeFace.hpp","DCP_Tool.hpp","DCP_AddTool.hpp","DCP_SelectTool.hpp","DCP_SelectPoint.hpp","DCP_SelectMultiPoints.hpp","DCP_SelectOnePoint.hpp","DCP_PointBuffModel.hpp","DCP_DomModel.hpp","DCP_SpecialMenu.hpp","DCP_AutoMatch.hpp","DCP_ResShaft.hpp","DemoLicense.hpp","UserDefStatus.hpp","CS35.hpp","_HEW_HelloWorldGUI.hpp","_HEW_Application.hpp","_HEW_Model.hpp","_HEW_Types.hpp")
$coreSrc = @("DCP_Common.cpp","DCP_Model.cpp","DCP_MsgBox.cpp","DCP_InputText.cpp","DCP_InputTextModel.cpp","DCP_xyz.cpp","DCP_Aim.cpp","DCP_ChangeFace.cpp","DCP_Tool.cpp","DCP_AddTool.cpp","DCP_SelectTool.cpp","DCP_SelectPoint.cpp","DCP_SelectMultiPoints.cpp","SelectOnePoint.cpp","DCP_PointBuffModel.cpp","DCP_DomModel.cpp","DCP_SpecialMenu.cpp","DCP_AutoMatch.cpp","DCP_ResShaft.cpp","DemoLicense.cpp","UserDefStatus.cpp","CS35.cpp","stdafx.cpp","stdafx.h")

# Init
$initHdr = @("DCP_DCP05Init.hpp","DCP_DCP05User.hpp","DCP_DCP05Unit.hpp")
$initSrc = @("DCP_DCP05Init.cpp","DCP_DCP05User.cpp","DCP_DCP05Unit.cpp")

# Orientation
$orientHdr = @("DCP_DCP05Dom.hpp","DCP_DCP05DomUserDef.hpp","DCP_DCP05Pom.hpp","DCP_DCP05Chst.hpp","DCP_DefinePlane.hpp","DCP_DefinePlaneUserDef.hpp","DCP_DefineLine.hpp","DCP_DefineLineUserDef.hpp","DCP_Offsv.hpp","DCP_MeasV.hpp","DCP_RotatePlane.hpp","DCP_RotateLine.hpp","DCP_ResPlane.hpp","DCP_ResLine.hpp","DCP_ResPom.hpp")
$orientSrc = @("DCP_DCP05Dom.cpp","DCP_DCP05DomUserDef.cpp","DCP_DCP05Pom.cpp","DCP_DCP05Chst.cpp","DCP_DefinePlane.cpp","DCP_DefinePlaneUserDef.cpp","DCP_DefineLine.cpp","DCP_DefineLineUserDef.cpp","DCP_Offsv.cpp","DCP_MeasV.cpp","DCP_RotatePlane.cpp","DCP_RotateLine.cpp","DCP_ResPlane.cpp","DCP_ResLine.cpp","DCP_ResPom.cpp","DCP_PomSelectPoints.cpp")
$orientHdr += "DCP_PomSelectPoints.hpp"

# Measurement
$measHdr = @("DCP_MeasXYZ.hpp","DCP_MeasDist.hpp","DCP_3dmeas.hpp","DCP_3dfbs.hpp","DCP_3dFileView.hpp","DCP_HiddenPoint.hpp","DCP_HiddenPointConf.hpp","DCP_Circle.hpp","DCP_SeparateRec.hpp","DCP_HomePoints.hpp","DCP_MidPointController.hpp","DCP_ScanMeasXYZ.hpp","DCP_ScanFileFunc.hpp","DCP_AdvScan.hpp","DCP_PlaneScanning.hpp")
$measSrc = @("DCP_MeasXYZ.cpp","DCP_MeasDist.cpp","DCP_3dmeas.cpp","DCP_3dfbs.cpp","DCP_3dFileView.cpp","DCP_HiddenPoint.cpp","DCP_HiddenPointConf.cpp","DCP_Circle.cpp","DCP_SeparateRec.cpp","DCP_HomePoints.cpp","DCP_MidPointController.cpp","DCP_ScanMeasXYZ.cpp","DCP_ScanFileFunc.cpp","DCP_AdvScan.cpp","DCP_PlaneScanning.cpp")

# Calculation
$calcHdr = @("DCP_CalcAngle.hpp","DCP_CalcChst.hpp","DCP_CalcCircle.hpp","DCP_CalcCircleController.hpp","DCP_CalcDist.hpp","DCP_CalcDist2Points.hpp","DCP_CalcDom.hpp","DCP_CalcLine.hpp","DCP_CalcLineController.hpp","DCP_CalcLineFit.hpp","DCP_CalcMenu.hpp","DCP_CalcPlane.hpp","DCP_CalcPlaneController.hpp","DCP_CalcPom.hpp","DCP_CalculationCircle.hpp","DCP_EditCalcDist.hpp","DCP_EditCalcAngle.hpp","DCP_ViewCdf.hpp","DCP_ViewAgf.hpp","DCP_ResCircle.hpp")
$calcSrc = @("DCP_CalcAngle.cpp","DCP_CalcChst.cpp","DCP_calccircle.cpp","DCP_CalcCircleController.cpp","DCP_CalcDist.cpp","DCP_CalcDist2Points.cpp","DCP_CalcDom.cpp","DCP_calcline.cpp","DCP_CalcLineController.cpp","DCP_CalcLineFit.cpp","DCP_CalcMenu.cpp","DCP_calcplane.cpp","DCP_CalcPlaneController.cpp","DCP_CalcPom.cpp","DCP_CalculationCircle.cpp","DCP_EditCalcDist.cpp","DCP_EditCalcAngle.cpp","DCP_ViewCdf.cpp","DCP_ViewAgf.cpp","DCP_ResCircle.cpp")

# File
$fileHdr = @("DCP_AdfFileFunc.hpp","DCP_File.hpp","DCP_SelectFile.hpp","DCP_DistFile.hpp","DCP_AngleFile.hpp","DCP_CircleFile.hpp","DCP_CircleSave.hpp","DCP_ShaftFile.hpp","DCP_ShaftSave.hpp")
$fileSrc = @("DCP_AdfFileFunc.cpp","DCP_File.cpp","DCP_SelectFile.cpp","DCP_DistFile.cpp","DCP_AngleFile.cpp","DCP_CircleFile.cpp","DCP_CircleSave.cpp","DCP_ShaftFile.cpp","DCP_ShaftSave.cpp")

# Application
$appHdr = @("DCP_Application.hpp","DCP_ApplicationMenu.hpp","DCP_LineSetting.hpp","DCP_LineSettingConf.hpp","DCP_Shaft.hpp","DCP_ShaftLine.hpp","DCP_LineFitting.hpp","DCP_LineFitMeas.hpp")
$appSrc = @("DCP_Application.cpp","DCP_ApplicationMenu.cpp","DCP_LineSetting.cpp","DCP_LineSettingConf.cpp","DCP_Shaft.cpp","DCP_ShaftLine.cpp","DCP_LineFitting.cpp","DCP_LineFitMeas.cpp","DCP06.cpp")

function Move-Files($files, $srcDir, $destDir) {
    foreach ($f in $files) {
        $srcPath = Join-Path $srcDir $f
        if (Test-Path $srcPath) {
            Move-Item $srcPath (Join-Path $destDir $f) -Force
            Write-Host "Moved $f"
        }
    }
}

Move-Files $coreHdr $hdr (Join-Path $inc "core")
Move-Files $coreSrc $src (Join-Path $srcOut "core")
Move-Files $initHdr $hdr (Join-Path $inc "init")
Move-Files $initSrc $src (Join-Path $srcOut "init")
Move-Files $orientHdr $hdr (Join-Path $inc "orientation")
Move-Files $orientSrc $src (Join-Path $srcOut "orientation")
Move-Files $measHdr $hdr (Join-Path $inc "measurement")
Move-Files $measSrc $src (Join-Path $srcOut "measurement")
Move-Files $calcHdr $hdr (Join-Path $inc "calculation")
Move-Files $calcSrc $src (Join-Path $srcOut "calculation")
Move-Files $fileHdr $hdr (Join-Path $inc "file")
Move-Files $fileSrc $src (Join-Path $srcOut "file")
Move-Files $appHdr $hdr (Join-Path $inc "application")
Move-Files $appSrc $src (Join-Path $srcOut "application")

# Move DCP_DCP05GUI and DCP_DCP05Meas to core (main controllers)
if (Test-Path (Join-Path $hdr "DCP_DCP05GUI.hpp")) { Move-Item (Join-Path $hdr "DCP_DCP05GUI.hpp") (Join-Path $inc "core") -Force }
if (Test-Path (Join-Path $hdr "DCP_DCP05Meas.hpp")) { Move-Item (Join-Path $hdr "DCP_DCP05Meas.hpp") (Join-Path $inc "core") -Force }
if (Test-Path (Join-Path $src "DCP_DCP05GUI.cpp")) { Move-Item (Join-Path $src "DCP_DCP05GUI.cpp") (Join-Path $srcOut "core") -Force }
if (Test-Path (Join-Path $src "DCP_DCP05Meas.cpp")) { Move-Item (Join-Path $src "DCP_DCP05Meas.cpp") (Join-Path $srcOut "core") -Force }

# Check for DCP_SelectCdfFile
if (Test-Path (Join-Path $hdr "DCP_SelectCdfFile.hpp")) { Move-Item (Join-Path $hdr "DCP_SelectCdfFile.hpp") (Join-Path $inc "file") -Force }

Write-Host "Done."
