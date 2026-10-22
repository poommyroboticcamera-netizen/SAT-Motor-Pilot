param([string]$Executable = "$PSScriptRoot/../dist/SATMotorPilot/SATMotorPilot.exe", [string]$Output = "$PSScriptRoot/../evidence/ui/motorlab")
$ErrorActionPreference = 'Stop'
$Executable = (Resolve-Path $Executable).Path
$Output = [IO.Path]::GetFullPath($Output)
New-Item -ItemType Directory -Force $Output | Out-Null
$env:MOTORLAB_AUDIT_DIR = $Output
$env:QT_QPA_PLATFORM = 'windows'
$env:QT_OPENGL = 'software'
$env:QT_QUICK_BACKEND = 'software'
Remove-Item Env:QT_PLUGIN_PATH,Env:QML2_IMPORT_PATH -ErrorAction SilentlyContinue
$oldPath = $env:PATH
try {
    $env:PATH = "$env:SystemRoot/System32;$env:SystemRoot"
    $run = Start-Process -FilePath $Executable -WorkingDirectory (Split-Path $Executable) -ArgumentList @('--debugOutFile',"$Output/runtime.log") -PassThru -WindowStyle Hidden
    if (!$run.WaitForExit(30000)) { throw "Audit timeout, PID $($run.Id)" }
    if ($run.ExitCode -ne 0) { throw "Audit exited $($run.ExitCode)" }
    $report = Get-Content "$Output/runtime-audit.json" -Raw | ConvertFrom-Json
    if (!$report.interfaceFound -or $report.pages.Count -lt 1 -or @($report.configurationRoundtrips | Where-Object { !$_.pass }).Count) { throw 'Audit incomplete or local persistence failed' }
    if ($report.applicationName -eq 'SAT Motor Pilot' -and (@($report.workspaceNavigation).Count -ne 4 -or @($report.workspaceNavigation | Where-Object { !$_.pass }).Count -ne 0)) { throw 'SAT workspace navigation failed; inspect runtime-audit.json' }
    @{exitCode=$run.ExitCode;platform=$report.platform;qtVersion=$report.qtVersion;pageCount=$report.pages.Count;pathPolicy='Windows system directories only';transport='audit blocks physical packets';companyTheme='#304B8A'} | ConvertTo-Json | Set-Content "$Output/launch.json"
    Write-Host "Audited $($report.pages.Count) disconnected pages; local XML checks passed."
} finally {
    $env:PATH = $oldPath
    Remove-Item Env:MOTORLAB_AUDIT_DIR -ErrorAction SilentlyContinue
}
