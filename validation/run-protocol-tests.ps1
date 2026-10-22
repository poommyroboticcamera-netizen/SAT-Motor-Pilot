param([string]$QtRoot = (Join-Path $PSScriptRoot '../tooling/Qt'))
$ErrorActionPreference = 'Stop'
$QtRoot = [IO.Path]::GetFullPath($QtRoot)
$qmake = Join-Path $QtRoot '5.15.2/mingw81_64/bin/qmake.exe'
$make = Join-Path $QtRoot 'Tools/mingw810_64/bin/mingw32-make.exe'
if (!(Test-Path -LiteralPath $qmake)) { throw "Qt qmake missing: $qmake" }
if (!(Test-Path -LiteralPath $make)) { throw "MinGW make missing: $make" }
$env:PATH = (Split-Path $make) + ';' + (Split-Path $qmake) + ';' + $env:PATH
$build = Join-Path $PSScriptRoot 'build'
New-Item -ItemType Directory -Force -Path $build | Out-Null
Push-Location -LiteralPath $build
try {
    & $qmake (Join-Path $PSScriptRoot 'protocol-tests.pro')
    if ($LASTEXITCODE -ne 0) { throw 'qmake failed' }
    & $make '-j4'
    if ($LASTEXITCODE -ne 0) { throw 'Compilation failed' }
    & './release/protocol-tests.exe' '-o' 'results.xml,junitxml' '-o' 'results.txt,txt'
    if ($LASTEXITCODE -ne 0) { throw 'Protocol tests failed; inspect results.txt' }
    Get-Content -LiteralPath 'results.txt'
} finally { Pop-Location }
