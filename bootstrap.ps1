param([string]$Python = 'python')
$ErrorActionPreference = 'Stop'
Set-Location $PSScriptRoot
& $Python -m pip install --target tooling/python aqtinstall==3.3.0
if ($LASTEXITCODE) { throw 'Python dependency installation failed' }
$env:PYTHONPATH = Join-Path $PSScriptRoot 'tooling/python'
& $Python -m aqt install-qt windows desktop 5.15.2 win64_mingw81 -O tooling/Qt --archives qtbase qtdeclarative qtquickcontrols qtquickcontrols2 qtgraphicaleffects qtserialport qtconnectivity qt3d qtsvg qtgamepad qtlocation qttools qtwinextras
if ($LASTEXITCODE) { throw 'Qt installation failed' }
& $Python -m aqt install-tool windows desktop tools_mingw81 qt.tools.win64_mingw810 -O tooling/Qt
if ($LASTEXITCODE) { throw 'Compiler installation failed' }
