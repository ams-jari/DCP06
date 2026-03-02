# Update #include paths for Option B structure
$root = Split-Path -Parent $PSScriptRoot
$files = Get-ChildItem -Path $root -Recurse -Include *.cpp,*.hpp -File | 
    Where-Object { $_.FullName -notmatch '\\.git\\' }

$map = @{
    'DCP_Defs'='dcp06/core'; 'DCP_Types'='dcp06/core'; 'DCP_Common'='dcp06/core'
    'DCP_Model'='dcp06/core'; 'DCP_MsgBox'='dcp06/core'; 'DCP_InputText'='dcp06/core'
    'DCP_InputTextModel'='dcp06/core'; 'DCP_xyz'='dcp06/core'; 'DCP_Aim'='dcp06/core'
    'DCP_ChangeFace'='dcp06/core'; 'DCP_Tool'='dcp06/core'; 'DCP_AddTool'='dcp06/core'
    'DCP_SelectTool'='dcp06/core'; 'DCP_SelectPoint'='dcp06/core'; 'DCP_SelectMultiPoints'='dcp06/core'
    'DCP_SelectOnePoint'='dcp06/core'; 'DCP_PointBuffModel'='dcp06/core'; 'DCP_Alignment321Model'='dcp06/orientation'; 'DCP_DCP06Alignment321'='dcp06/orientation'; 'DCP_DCP06Alignment321UserDef'='dcp06/orientation'; 'DCP_DCP06BestFit'='dcp06/orientation'
    'DCP_SpecialMenu'='dcp06/core'; 'DCP_AutoMatch'='dcp06/core'; 'DCP_ResShaft'='dcp06/core'
    'DemoLicense'='dcp06/core'; 'UserDefStatus'='dcp06/core'; 'CS35'='dcp06/core'
    'DCP_DCP05GUI'='dcp06/core'; 'DCP_DCP05Meas'='dcp06/core'
    'DCP_DCP05Init'='dcp06/init'; 'DCP_DCP05User'='dcp06/init'; 'DCP_DCP05Unit'='dcp06/init'
    'DCP_DCP05Dom'='dcp06/orientation'; 'DCP_DCP05DomUserDef'='dcp06/orientation'
    'DCP_DCP05Pom'='dcp06/orientation'; 'DCP_DCP05Chst'='dcp06/orientation'
    'DCP_DefinePlane'='dcp06/orientation'; 'DCP_DefinePlaneUserDef'='dcp06/orientation'
    'DCP_DefineLine'='dcp06/orientation'; 'DCP_DefineLineUserDef'='dcp06/orientation'
    'DCP_Offsv'='dcp06/orientation'; 'DCP_MeasV'='dcp06/orientation'
    'DCP_RotatePlane'='dcp06/orientation'; 'DCP_RotateLine'='dcp06/orientation'
    'DCP_ResPlane'='dcp06/orientation'; 'DCP_ResLine'='dcp06/orientation'; 'DCP_ResBestFit'='dcp06/orientation'
    'DCP_BestFitSelectPoints'='dcp06/orientation'
    'DCP_MeasXYZ'='dcp06/measurement'; 'DCP_MeasDist'='dcp06/measurement'
    'DCP_3dmeas'='dcp06/measurement'; 'DCP_3dfbs'='dcp06/measurement'; 'DCP_3dFileView'='dcp06/measurement'
    'DCP_HiddenPoint'='dcp06/measurement'; 'DCP_HiddenPointConf'='dcp06/measurement'
    'DCP_Circle'='dcp06/measurement'; 'DCP_SeparateRec'='dcp06/measurement'; 'DCP_HomePoints'='dcp06/measurement'
    'DCP_MidPointController'='dcp06/measurement'; 'DCP_ScanMeasXYZ'='dcp06/measurement'
    'DCP_ScanFileFunc'='dcp06/measurement'; 'DCP_AdvScan'='dcp06/measurement'; 'DCP_PlaneScanning'='dcp06/measurement'
    'DCP_CalcAngle'='dcp06/calculation'; 'DCP_CalcChst'='dcp06/calculation'; 'DCP_CalcCircle'='dcp06/calculation'
    'DCP_CalcCircleController'='dcp06/calculation'; 'DCP_CalcDist'='dcp06/calculation'
    'DCP_CalcDist2Points'='dcp06/calculation'; 'DCP_Calc321'='dcp06/calculation'; 'DCP_CalcLine'='dcp06/calculation'
    'DCP_CalcLineController'='dcp06/calculation'; 'DCP_CalcLineFit'='dcp06/calculation'; 'DCP_CalcMenu'='dcp06/calculation'
    'DCP_CalcPlane'='dcp06/calculation'; 'DCP_CalcPlaneController'='dcp06/calculation'; 'DCP_CalcBestFit'='dcp06/calculation'
    'DCP_CalculationCircle'='dcp06/calculation'; 'DCP_EditCalcDist'='dcp06/calculation'; 'DCP_EditCalcAngle'='dcp06/calculation'
    'DCP_ViewCdf'='dcp06/calculation'; 'DCP_ViewAgf'='dcp06/calculation'; 'DCP_ResCircle'='dcp06/calculation'
    'DCP_AdfFileFunc'='dcp06/file'; 'DCP_File'='dcp06/file'; 'DCP_SelectFile'='dcp06/file'
    'DCP_DistFile'='dcp06/file'; 'DCP_AngleFile'='dcp06/file'; 'DCP_CircleFile'='dcp06/file'
    'DCP_CircleSave'='dcp06/file'; 'DCP_ShaftFile'='dcp06/file'; 'DCP_ShaftSave'='dcp06/file'
    'DCP_Application'='dcp06/application'; 'DCP_ApplicationMenu'='dcp06/application'
    'DCP_LineSetting'='dcp06/application'; 'DCP_LineSettingConf'='dcp06/application'
    'DCP_Shaft'='dcp06/application'; 'DCP_ShaftLine'='dcp06/application'
    'DCP_LineFitting'='dcp06/application'; 'DCP_LineFitMeas'='dcp06/application'
}

$count = 0
foreach ($file in $files) {
    $content = Get-Content $file.FullName -Raw -ErrorAction SilentlyContinue
    if (-not $content) { continue }
    $orig = $content
    foreach ($key in $map.Keys) {
        $path = $map[$key]
        $content = $content -replace "([`"<])($key)(\.hpp)([`">])", "`$1$path/`$2`$3`$4"
    }
    # Case variants (DCP_3DMeas, DCP_3DFbs)
    $content = $content -replace '([`"<])DCP_3DMeas(\.hpp)([`">])', '${1}dcp06/measurement/DCP_3dmeas${2}${3}'
    $content = $content -replace '([`"<])DCP_3DFbs(\.hpp)([`">])', '${1}dcp06/measurement/DCP_3dfbs${2}${3}'
    if ($content -ne $orig) {
        Set-Content $file.FullName -Value $content -NoNewline
        $count++
        Write-Host "Updated: $($file.FullName)"
    }
}
Write-Host "Total files updated: $count"
