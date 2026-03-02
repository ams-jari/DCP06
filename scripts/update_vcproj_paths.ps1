# Update DCP06.vcproj paths for Option B structure
$vcproj = Join-Path (Split-Path -Parent $PSScriptRoot) "Project\MSVS\DCP06.vcproj"
$content = Get-Content $vcproj -Raw

# Source file path mapping (Src -> src/domain)
$srcMap = @{
    '\.\.\\\.\.\\Src\\CS35\.cpp'='..\..\src\core\CS35.cpp'
    '\.\.\\\.\.\\Src\\DCP06\.cpp'='..\..\src\application\DCP06.cpp'
    '\.\.\\\.\.\\Src\\stdafx\.cpp'='..\..\src\core\stdafx.cpp'
    '\.\.\\\.\.\\Src\\DemoLicense\.cpp'='..\..\src\core\DemoLicense.cpp'
    '\.\.\\\.\.\\Src\\UserDefStatus\.cpp'='..\..\src\core\UserDefStatus.cpp'
    '\.\.\\\.\.\\Src\\SelectOnePoint\.cpp'='..\..\src\core\SelectOnePoint.cpp'
    '\.\.\\\.\.\\Src\\DCP_Common\.cpp'='..\..\src\core\DCP_Common.cpp'
    '\.\.\\\.\.\\Src\\DCP_Model\.cpp'='..\..\src\core\DCP_Model.cpp'
    '\.\.\\\.\.\\Src\\DCP_MsgBox\.cpp'='..\..\src\core\DCP_MsgBox.cpp'
    '\.\.\\\.\.\\Src\\DCP_InputText\.cpp'='..\..\src\core\DCP_InputText.cpp'
    '\.\.\\\.\.\\Src\\DCP_InputTextModel\.cpp'='..\..\src\core\DCP_InputTextModel.cpp'
    '\.\.\\\.\.\\Src\\DCP_xyz\.cpp'='..\..\src\core\DCP_xyz.cpp'
    '\.\.\\\.\.\\Src\\DCP_Aim\.cpp'='..\..\src\core\DCP_Aim.cpp'
    '\.\.\\\.\.\\Src\\DCP_ChangeFace\.cpp'='..\..\src\core\DCP_ChangeFace.cpp'
    '\.\.\\\.\.\\Src\\DCP_Tool\.cpp'='..\..\src\core\DCP_Tool.cpp'
    '\.\.\\\.\.\\Src\\DCP_AddTool\.cpp'='..\..\src\core\DCP_AddTool.cpp'
    '\.\.\\\.\.\\Src\\DCP_SelectTool\.cpp'='..\..\src\core\DCP_SelectTool.cpp'
    '\.\.\\\.\.\\Src\\DCP_SelectPoint\.cpp'='..\..\src\core\DCP_SelectPoint.cpp'
    '\.\.\\\.\.\\Src\\DCP_SelectMultiPoints\.cpp'='..\..\src\core\DCP_SelectMultiPoints.cpp'
    '\.\.\\\.\.\\Src\\DCP_PointBuffModel\.cpp'='..\..\src\core\DCP_PointBuffModel.cpp'
    '\.\.\\\.\.\\Src\\DCP_SpecialMenu\.cpp'='..\..\src\core\DCP_SpecialMenu.cpp'
    '\.\.\\\.\.\\Src\\DCP_AutoMatch\.cpp'='..\..\src\measurement\DCP_AutoMatch.cpp'
    '\.\.\\\.\.\\Src\\DCP_ResShaft\.cpp'='..\..\src\core\DCP_ResShaft.cpp'
    '\.\.\\\.\.\\Src\\DCP_DCP05GUI\.cpp'='..\..\src\core\DCP_DCP05GUI.cpp'
    '\.\.\\\.\.\\Src\\DCP_DCP05Meas\.cpp'='..\..\src\core\DCP_DCP05Meas.cpp'
    '\.\.\\\.\.\\Src\\DCP_DCP05Init\.cpp'='..\..\src\init\DCP_DCP05Init.cpp'
    '\.\.\\\.\.\\Src\\DCP_DCP05User\.cpp'='..\..\src\init\DCP_DCP05User.cpp'
    '\.\.\\\.\.\\Src\\DCP_DCP05Unit\.cpp'='..\..\src\init\DCP_DCP05Unit.cpp'
    '\.\.\\\.\.\\Src\\DCP_DCP05Dom\.cpp'='..\..\src\orientation\DCP_DCP05Dom.cpp'
    '\.\.\\\.\.\\Src\\DCP_DCP05DomUserDef\.cpp'='..\..\src\orientation\DCP_DCP05DomUserDef.cpp'
    '\.\.\\\.\.\\Src\\DCP_DCP05Pom\.cpp'='..\..\src\orientation\DCP_DCP05Pom.cpp'
    '\.\.\\\.\.\\Src\\DCP_DCP05Chst\.cpp'='..\..\src\orientation\DCP_DCP05Chst.cpp'
    '\.\.\\\.\.\\Src\\DCP_DefinePlane\.cpp'='..\..\src\orientation\DCP_DefinePlane.cpp'
    '\.\.\\\.\.\\Src\\DCP_DefinePlaneUserDef\.cpp'='..\..\src\orientation\DCP_DefinePlaneUserDef.cpp'
    '\.\.\\\.\.\\Src\\DCP_DefineLine\.cpp'='..\..\src\orientation\DCP_DefineLine.cpp'
    '\.\.\\\.\.\\Src\\DCP_DefineLineUserDef\.cpp'='..\..\src\orientation\DCP_DefineLineUserDef.cpp'
    '\.\.\\\.\.\\Src\\DCP_Offsv\.cpp'='..\..\src\orientation\DCP_Offsv.cpp'
    '\.\.\\\.\.\\Src\\DCP_MeasV\.cpp'='..\..\src\orientation\DCP_MeasV.cpp'
    '\.\.\\\.\.\\Src\\DCP_RotatePlane\.cpp'='..\..\src\orientation\DCP_RotatePlane.cpp'
    '\.\.\\\.\.\\Src\\DCP_RotateLine\.cpp'='..\..\src\orientation\DCP_RotateLine.cpp'
    '\.\.\\\.\.\\Src\\DCP_ResPlane\.cpp'='..\..\src\orientation\DCP_ResPlane.cpp'
    '\.\.\\\.\.\\Src\\DCP_ResLine\.cpp'='..\..\src\orientation\DCP_ResLine.cpp'
    '\.\.\\\.\.\\Src\\DCP_ResPom\.cpp'='..\..\src\orientation\DCP_ResPom.cpp'
    '\.\.\\\.\.\\Src\\DCP_PomSelectPoints\.cpp'='..\..\src\orientation\DCP_PomSelectPoints.cpp'
    '\.\.\\\.\.\\Src\\DCP_MeasXYZ\.cpp'='..\..\src\measurement\DCP_MeasXYZ.cpp'
    '\.\.\\\.\.\\Src\\DCP_MeasDist\.cpp'='..\..\src\measurement\DCP_MeasDist.cpp'
    '\.\.\\\.\.\\Src\\DCP_3dmeas\.cpp'='..\..\src\measurement\DCP_3dmeas.cpp'
    '\.\.\\\.\.\\Src\\DCP_3dfbs\.cpp'='..\..\src\measurement\DCP_3dfbs.cpp'
    '\.\.\\\.\.\\Src\\DCP_3dFileView\.cpp'='..\..\src\measurement\DCP_3dFileView.cpp'
    '\.\.\\\.\.\\Src\\DCP_HiddenPoint\.cpp'='..\..\src\measurement\DCP_HiddenPoint.cpp'
    '\.\.\\\.\.\\Src\\DCP_HiddenPointConf\.cpp'='..\..\src\measurement\DCP_HiddenPointConf.cpp'
    '\.\.\\\.\.\\Src\\DCP_Circle\.cpp'='..\..\src\measurement\DCP_Circle.cpp'
    '\.\.\\\.\.\\Src\\DCP_SeparateRec\.cpp'='..\..\src\measurement\DCP_SeparateRec.cpp'
    '\.\.\\\.\.\\Src\\DCP_HomePoints\.cpp'='..\..\src\measurement\DCP_HomePoints.cpp'
    '\.\.\\\.\.\\Src\\DCP_MidPointController\.cpp'='..\..\src\measurement\DCP_MidPointController.cpp'
    '\.\.\\\.\.\\Src\\DCP_ScanMeasXYZ\.cpp'='..\..\src\measurement\DCP_ScanMeasXYZ.cpp'
    '\.\.\\\.\.\\Src\\DCP_ScanFileFunc\.cpp'='..\..\src\measurement\DCP_ScanFileFunc.cpp'
    '\.\.\\\.\.\\Src\\DCP_AdvScan\.cpp'='..\..\src\measurement\DCP_AdvScan.cpp'
    '\.\.\\\.\.\\Src\\DCP_PlaneScanning\.cpp'='..\..\src\measurement\DCP_PlaneScanning.cpp'
    '\.\.\\\.\.\\Src\\DCP_CalcAngle\.cpp'='..\..\src\calculation\DCP_CalcAngle.cpp'
    '\.\.\\\.\.\\Src\\DCP_CalcChst\.cpp'='..\..\src\calculation\DCP_CalcChst.cpp'
    '\.\.\\\.\.\\Src\\DCP_calccircle\.cpp'='..\..\src\calculation\DCP_calccircle.cpp'
    '\.\.\\\.\.\\Src\\DCP_CalcCircleController\.cpp'='..\..\src\calculation\DCP_CalcCircleController.cpp'
    '\.\.\\\.\.\\Src\\DCP_CalcDist\.cpp'='..\..\src\calculation\DCP_CalcDist.cpp'
    '\.\.\\\.\.\\Src\\DCP_CalcDist2Points\.cpp'='..\..\src\calculation\DCP_CalcDist2Points.cpp'
    '\.\.\\\.\.\\Src\\DCP_CalcDom\.cpp'='..\..\src\calculation\DCP_CalcDom.cpp'
    '\.\.\\\.\.\\Src\\DCP_calcline\.cpp'='..\..\src\calculation\DCP_calcline.cpp'
    '\.\.\\\.\.\\Src\\DCP_CalcLineController\.cpp'='..\..\src\calculation\DCP_CalcLineController.cpp'
    '\.\.\\\.\.\\Src\\DCP_CalcLineFit\.cpp'='..\..\src\calculation\DCP_CalcLineFit.cpp'
    '\.\.\\\.\.\\Src\\DCP_CalcMenu\.cpp'='..\..\src\calculation\DCP_CalcMenu.cpp'
    '\.\.\\\.\.\\Src\\DCP_calcplane\.cpp'='..\..\src\calculation\DCP_calcplane.cpp'
    '\.\.\\\.\.\\Src\\DCP_CalcPlaneController\.cpp'='..\..\src\calculation\DCP_CalcPlaneController.cpp'
    '\.\.\\\.\.\\Src\\DCP_CalcPom\.cpp'='..\..\src\calculation\DCP_CalcPom.cpp'
    '\.\.\\\.\.\\Src\\DCP_CalculationCircle\.cpp'='..\..\src\calculation\DCP_CalculationCircle.cpp'
    '\.\.\\\.\.\\Src\\DCP_EditCalcDist\.cpp'='..\..\src\calculation\DCP_EditCalcDist.cpp'
    '\.\.\\\.\.\\Src\\DCP_EditCalcAngle\.cpp'='..\..\src\calculation\DCP_EditCalcAngle.cpp'
    '\.\.\\\.\.\\Src\\DCP_ViewCdf\.cpp'='..\..\src\calculation\DCP_ViewCdf.cpp'
    '\.\.\\\.\.\\Src\\DCP_ViewAgf\.cpp'='..\..\src\calculation\DCP_ViewAgf.cpp'
    '\.\.\\\.\.\\Src\\DCP_ResCircle\.cpp'='..\..\src\calculation\DCP_ResCircle.cpp'
    '\.\.\\\.\.\\Src\\DCP_AdfFileFunc\.cpp'='..\..\src\file\DCP_AdfFileFunc.cpp'
    '\.\.\\\.\.\\Src\\DCP_File\.cpp'='..\..\src\file\DCP_File.cpp'
    '\.\.\\\.\.\\Src\\DCP_SelectFile\.cpp'='..\..\src\file\DCP_SelectFile.cpp'
    '\.\.\\\.\.\\Src\\DCP_DistFile\.cpp'='..\..\src\file\DCP_DistFile.cpp'
    '\.\.\\\.\.\\Src\\DCP_AngleFile\.cpp'='..\..\src\file\DCP_AngleFile.cpp'
    '\.\.\\\.\.\\Src\\DCP_CircleFile\.cpp'='..\..\src\file\DCP_CircleFile.cpp'
    '\.\.\\\.\.\\Src\\DCP_CircleSave\.cpp'='..\..\src\file\DCP_CircleSave.cpp'
    '\.\.\\\.\.\\Src\\DCP_ShaftFile\.cpp'='..\..\src\file\DCP_ShaftFile.cpp'
    '\.\.\\\.\.\\Src\\DCP_ShaftSave\.cpp'='..\..\src\file\DCP_ShaftSave.cpp'
    '\.\.\\\.\.\\Src\\DCP_Application\.cpp'='..\..\src\application\DCP_Application.cpp'
    '\.\.\\\.\.\\Src\\DCP_ApplicationMenu\.cpp'='..\..\src\application\DCP_ApplicationMenu.cpp'
    '\.\.\\\.\.\\Src\\DCP_LineSetting\.cpp'='..\..\src\application\DCP_LineSetting.cpp'
    '\.\.\\\.\.\\Src\\DCP_LineSettingConf\.cpp'='..\..\src\application\DCP_LineSettingConf.cpp'
    '\.\.\\\.\.\\Src\\DCP_Shaft\.cpp'='..\..\src\application\DCP_Shaft.cpp'
    '\.\.\\\.\.\\Src\\DCP_ShaftLine\.cpp'='..\..\src\application\DCP_ShaftLine.cpp'
    '\.\.\\\.\.\\Src\\DCP_LineFitting\.cpp'='..\..\src\application\DCP_LineFitting.cpp'
    '\.\.\\\.\.\\Src\\DCP_LineFitMeas\.cpp'='..\..\src\application\DCP_LineFitMeas.cpp'
}

foreach ($k in $srcMap.Keys) {
    $content = $content -replace $k, $srcMap[$k]
}
# Fix math paths (Src\math -> src\math)
$content = $content -replace '\.\.\\\.\.\\Src\\math\\', '..\..\src\math\'

# stdafx.h - was in Src, now in src/core
$content = $content -replace 'RelativePath="\.\.\\\.\.\\Src\\stdafx\.h"', 'RelativePath="..\..\src\core\stdafx.h"'
$content = $content -replace 'RelativePath="\.\.\\\.\.\\Src\\math\\stdafx\.h"', 'RelativePath="..\..\src\core\stdafx.h"'

# AdditionalIncludeDirectories: Hdr;Src;Text -> include;include\dcp06\math;src\core;Text
$content = $content -replace '\.\.\\\.\.\\Hdr;\.\.\\\.\.\\Text;\.\.\\\.\.\\Src;', '..\..\include;..\..\include\dcp06\math;..\..\src\core;..\..\Text;'
$content = $content -replace '\.\.\\\.\.\\Hdr;\.\.\\\.\.\\Src;\.\.\\\.\.\\Text;', '..\..\include;..\..\include\dcp06\math;..\..\src\core;..\..\Text;'
# DCP_DCP05Unit file override
$content = $content -replace 'AdditionalIncludeDirectories="\.\.\\\.\.\\Text"', 'AdditionalIncludeDirectories="..\..\include;..\..\include\dcp06\math;..\..\src\core;..\..\Text"'

# Remove DCP_SelectCdfFile.hpp (doesn't exist)
$content = $content -replace '(?s)\s*<File\s+RelativePath="\.\.\\\.\.\\Hdr\\DCP_SelectCdfFile\.hpp"\s*/>', ''

# Header file paths: Hdr -> include/dcp06/<domain>
$hdrToDomain = @{
    'CS35.hpp'='core'; 'DCP_Defs.hpp'='core'; 'DCP_Types.hpp'='core'; 'DCP_Common.hpp'='core'
    'DCP_Model.hpp'='core'; 'DCP_MsgBox.hpp'='core'; 'DCP_InputText.hpp'='core'; 'DCP_InputTextModel.hpp'='core'
    'DCP_xyz.hpp'='core'; 'DCP_Aim.hpp'='core'; 'DCP_ChangeFace.hpp'='core'; 'DCP_Tool.hpp'='core'
    'DCP_AddTool.hpp'='core'; 'DCP_SelectTool.hpp'='core'; 'DCP_SelectPoint.hpp'='core'
    'DCP_SelectMultiPoints.hpp'='core'; 'DCP_SelectOnePoint.hpp'='core'; 'DCP_PointBuffModel.hpp'='core'
    'DCP_DomModel.hpp'='core'; 'DCP_SpecialMenu.hpp'='core'; 'DCP_AutoMatch.hpp'='core'
    'DCP_ResShaft.hpp'='core'; 'DemoLicense.hpp'='core'; 'UserDefStatus.hpp'='core'; 'DCP_DCP05GUI.hpp'='core'; 'DCP_DCP05Meas.hpp'='core'
    'DCP_DCP05Init.hpp'='init'; 'DCP_DCP05User.hpp'='init'; 'DCP_DCP05Unit.hpp'='init'
    'DCP_DCP05Dom.hpp'='orientation'; 'DCP_DCP05DomUserDef.hpp'='orientation'; 'DCP_DCP05Pom.hpp'='orientation'; 'DCP_DCP05Chst.hpp'='orientation'
    'DCP_DefinePlane.hpp'='orientation'; 'DCP_DefinePlaneUserDef.hpp'='orientation'; 'DCP_DefineLine.hpp'='orientation'; 'DCP_DefineLineUserDef.hpp'='orientation'
    'DCP_Offsv.hpp'='orientation'; 'DCP_MeasV.hpp'='orientation'; 'DCP_RotatePlane.hpp'='orientation'; 'DCP_RotateLine.hpp'='orientation'
    'DCP_ResPlane.hpp'='orientation'; 'DCP_ResLine.hpp'='orientation'; 'DCP_ResPom.hpp'='orientation'; 'DCP_PomSelectPoints.hpp'='orientation'
    'DCP_MeasXYZ.hpp'='measurement'; 'DCP_MeasDist.hpp'='measurement'; 'DCP_3dmeas.hpp'='measurement'; 'DCP_3dfbs.hpp'='measurement'
    'DCP_3dFileView.hpp'='measurement'; 'DCP_HiddenPoint.hpp'='measurement'; 'DCP_HiddenPointConf.hpp'='measurement'
    'DCP_Circle.hpp'='measurement'; 'DCP_SeparateRec.hpp'='measurement'; 'DCP_HomePoints.hpp'='measurement'
    'DCP_MidPointController.hpp'='measurement'; 'DCP_ScanMeasXYZ.hpp'='measurement'; 'DCP_ScanFileFunc.hpp'='measurement'
    'DCP_AdvScan.hpp'='measurement'; 'DCP_PlaneScanning.hpp'='measurement'
    'DCP_CalcAngle.hpp'='calculation'; 'DCP_CalcChst.hpp'='calculation'; 'DCP_CalcCircle.hpp'='calculation'; 'DCP_CalcCircleController.hpp'='calculation'
    'DCP_CalcDist.hpp'='calculation'; 'DCP_CalcDist2Points.hpp'='calculation'; 'DCP_CalcDom.hpp'='calculation'; 'DCP_CalcLine.hpp'='calculation'
    'DCP_CalcLineController.hpp'='calculation'; 'DCP_CalcLineFit.hpp'='calculation'; 'DCP_CalcMenu.hpp'='calculation'
    'DCP_CalcPlane.hpp'='calculation'; 'DCP_CalcPlaneController.hpp'='calculation'; 'DCP_CalcPom.hpp'='calculation'
    'DCP_CalculationCircle.hpp'='calculation'; 'DCP_EditCalcDist.hpp'='calculation'; 'DCP_EditCalcAngle.hpp'='calculation'
    'DCP_ViewCdf.hpp'='calculation'; 'DCP_ViewAgf.hpp'='calculation'; 'DCP_ResCircle.hpp'='calculation'
    'DCP_AdfFileFunc.hpp'='file'; 'DCP_File.hpp'='file'; 'DCP_SelectFile.hpp'='file'; 'DCP_DistFile.hpp'='file'
    'DCP_AngleFile.hpp'='file'; 'DCP_CircleFile.hpp'='file'; 'DCP_CircleSave.hpp'='file'; 'DCP_ShaftFile.hpp'='file'; 'DCP_ShaftSave.hpp'='file'
    'DCP_Application.hpp'='application'; 'DCP_ApplicationMenu.hpp'='application'; 'DCP_LineSetting.hpp'='application'; 'DCP_LineSettingConf.hpp'='application'
    'DCP_Shaft.hpp'='application'; 'DCP_ShaftLine.hpp'='application'; 'DCP_LineFitting.hpp'='application'; 'DCP_LineFitMeas.hpp'='application'
}
foreach ($hdr in $hdrToDomain.Keys) {
    $domain = $hdrToDomain[$hdr]
    $content = $content -replace [regex]::Escape("RelativePath=`"..\..\Hdr\$hdr`""), "RelativePath=`"..\..\include\dcp06\$domain\$hdr`""
}

# stdafx.h - was in Src, now in src/core
$content = $content -replace 'RelativePath="\.\.\\\.\.\\Src\\stdafx\.h"', 'RelativePath="..\..\src\core\stdafx.h"'
$content = $content -replace 'RelativePath="\.\.\\\.\.\\Src\\math\\stdafx\.h"', 'RelativePath="..\..\src\core\stdafx.h"'

Set-Content $vcproj -Value $content -NoNewline
Write-Host "Updated DCP06.vcproj"
