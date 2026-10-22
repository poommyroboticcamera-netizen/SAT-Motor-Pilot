param([string]$QtRoot = "$PSScriptRoot/tooling/Qt", [int]$Jobs = 6, [switch]$Clean)
$ErrorActionPreference = 'Stop'
$qt = (Resolve-Path "$QtRoot/5.15.2/mingw81_64").Path
$compiler = (Resolve-Path "$QtRoot/Tools/mingw810_64/bin").Path
$env:PATH = "$compiler;$qt/bin;$env:PATH"
$buildDir = Join-Path $PSScriptRoot 'build'
New-Item -ItemType Directory -Force $buildDir, "$PSScriptRoot/evidence", "$PSScriptRoot/dist/MotorLab" | Out-Null
Push-Location $buildDir
try {
    if ($Clean -and (Test-Path Makefile)) {
        & mingw32-make clean *> "$PSScriptRoot/evidence/clean.log"
        if ($LASTEXITCODE) { throw 'Clean failed' }
    }
    & "$qt/bin/qmake.exe" "$PSScriptRoot/source/motorlab.pro" CONFIG+=release CONFIG-=debug_and_release *> "$PSScriptRoot/evidence/qmake.log"
    if ($LASTEXITCODE) { throw 'qmake failed; see evidence/qmake.log' }
    & mingw32-make "-j$Jobs" *> "$PSScriptRoot/evidence/build.log"
    if ($LASTEXITCODE) { throw 'Build failed; see evidence/build.log' }
    Copy-Item MotorLab.exe "$PSScriptRoot/dist/MotorLab/MotorLab.exe" -Force
    # Qt 5.15.2's windeployqt misclassifies these official MinGW plugins as debug.
    # Deploy the complete matching runtime, then verify it with an isolated launch.
    Copy-Item "$qt/bin/*.dll" "$PSScriptRoot/dist/MotorLab" -Force
    Get-ChildItem "$qt/plugins" -Directory | ForEach-Object {
        Copy-Item $_.FullName "$PSScriptRoot/dist/MotorLab" -Recurse -Force
    }
    Copy-Item "$qt/qml" "$PSScriptRoot/dist/MotorLab" -Recurse -Force
    "[Paths]`nPlugins=.`nQml2Imports=qml" | Set-Content "$PSScriptRoot/dist/MotorLab/qt.conf"
    Copy-Item "$compiler/libgcc_s_seh-1.dll", "$compiler/libstdc++-6.dll", "$compiler/libwinpthread-1.dll" "$PSScriptRoot/dist/MotorLab" -Force
    Copy-Item "$PSScriptRoot/LICENSE", "$PSScriptRoot/THIRD_PARTY_NOTICES.md" "$PSScriptRoot/dist/MotorLab" -Force
    Get-FileHash "$PSScriptRoot/dist/MotorLab/MotorLab.exe" | ConvertTo-Json | Set-Content "$PSScriptRoot/evidence/build-executable-sha256.json"
    Write-Host "Built $PSScriptRoot/dist/MotorLab/MotorLab.exe"
} finally { Pop-Location }
