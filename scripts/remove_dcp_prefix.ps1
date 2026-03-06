# Remove DCP_ prefix from all DCP_* files and update includes
$root = Split-Path -Parent $PSScriptRoot
$srcRoot = Join-Path $root "src"
$incRoot = Join-Path $root "include" "dcp06"

# Build list of DCP_ files: (oldName, newName) - newName = strip DCP_
$renames = @()
Get-ChildItem -Path $root -Recurse -Include "DCP_*.cpp","DCP_*.hpp" -File | 
    Where-Object { $_.FullName -notmatch '\\.git\\' } |
    ForEach-Object {
        $newName = $_.Name -replace '^DCP_', ''
        $renames += [PSCustomObject]@{ Old = $_.FullName; New = Join-Path $_.DirectoryName $newName; BaseName = $_.BaseName; NewBaseName = $newName -replace '\.(cpp|hpp)$','' }
    }

# Create include mapping: DCP_X -> X for path updates
$includeMap = @{}
foreach ($r in $renames) {
    $includeMap[$r.BaseName] = $r.NewBaseName
}
# Case variants in includes
$includeMap['DCP_3DFileView'] = '3dFileView'
$includeMap['DCP_3DFbs'] = '3dfbs'
$includeMap['DCP_3DMeas'] = '3dmeas'

Write-Host "Renaming files..."
foreach ($r in $renames) {
    if ($r.Old -ne $r.New -and (Test-Path $r.Old)) {
        if (Test-Path $r.New) { Remove-Item $r.New -Force }
        Rename-Item $r.Old $r.New -Force
        Write-Host "  $($r.BaseName) -> $($r.NewBaseName)"
    }
}

Write-Host "`nUpdating includes in source files..."
$files = Get-ChildItem -Path $root -Recurse -Include *.cpp,*.hpp,*.vcxproj,*.vcproj -File |
    Where-Object { $_.FullName -notmatch '\\.git\\' -and $_.FullName -notmatch '\\scripts\\' }

$count = 0
foreach ($file in $files) {
    $content = Get-Content $file.FullName -Raw -ErrorAction SilentlyContinue
    if (-not $content) { continue }
    $orig = $content
    
    foreach ($key in $includeMap.Keys) {
        $val = $includeMap[$key]
        # Match: dcp06/xxx/DCP_Name or dcp06/xxx/DCP_Name.hpp
        $content = $content -replace "dcp06/([^/]+/)$([regex]::Escape($key))(\.hpp)?", "dcp06/`$1$val`$2"
        # Match in project files: path\DCP_Name.cpp
        $content = $content -replace "([\\/])$([regex]::Escape($key))(\.cpp)", "`$1$val`$2"
        $content = $content -replace "([\\/])$([regex]::Escape($key))(\.hpp)", "`$1$val`$2"
    }
    
    if ($content -ne $orig) {
        Set-Content $file.FullName -Value $content -NoNewline
        $count++
        Write-Host "  Updated: $($file.Name)"
    }
}
Write-Host "`nTotal files updated: $count"
