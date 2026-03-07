# Find and run TextTool to compile .men -> .LEN
param([string]$MenFile)
$exe = Get-ChildItem 'C:\Program Files (x86)','C:\Program Files' -Recurse -Filter 'TextTool-VBNet.exe' -ErrorAction SilentlyContinue -Depth 6 | Select-Object -First 1
if (-not $exe) {
    Write-Error 'TextTool not found. Install TextToolSetup_v5.1.1.msi from SDK Tools\TextTool\'
    exit 1
}
Write-Host "Using TextTool: $($exe.FullName)"
& $exe.FullName /S $MenFile /G
exit $LASTEXITCODE
