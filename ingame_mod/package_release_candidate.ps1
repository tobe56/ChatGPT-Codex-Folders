param(
    [string]$Version = "RC1"
)

$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$distRoot = Join-Path $projectRoot "dist\FrontierLedger"
$releaseRoot = Join-Path $projectRoot "release_candidate"
$packageName = "FrontierLedger_$Version"
$packageRoot = Join-Path $releaseRoot $packageName
$zipPath = Join-Path $releaseRoot "$packageName.zip"

$requiredPaths = @(
    (Join-Path $projectRoot "build\FrontierLedger.asi"),
    (Join-Path $projectRoot "FrontierLedger.ini"),
    (Join-Path $distRoot "lml\Frontier Ledger Korean Text\strings.gxt2"),
    (Join-Path $projectRoot "release_tools\Prepare_Backup.ps1"),
    (Join-Path $projectRoot "release_tools\Install_RC.ps1"),
    (Join-Path $projectRoot "release_tools\Rollback_Backup.ps1"),
    (Join-Path $projectRoot "release_tools\INSTALL_AND_ROLLBACK.md")
)

foreach ($path in $requiredPaths) {
    if (-not (Test-Path $path)) {
        throw "Missing release input: $path"
    }
}

if (Test-Path $packageRoot) {
    Remove-Item $packageRoot -Recurse -Force
}
if (-not (Test-Path $releaseRoot)) {
    New-Item -ItemType Directory -Path $releaseRoot -Force | Out-Null
}

New-Item -ItemType Directory -Path $packageRoot -Force | Out-Null
New-Item -ItemType Directory -Path (Join-Path $packageRoot "lml") -Force | Out-Null

Copy-Item (Join-Path $projectRoot "build\FrontierLedger.asi") (Join-Path $packageRoot "FrontierLedger.asi") -Force
Copy-Item (Join-Path $projectRoot "FrontierLedger.ini") (Join-Path $packageRoot "FrontierLedger.ini") -Force
Copy-Item (Join-Path $distRoot "lml\Frontier Ledger Korean Text") (Join-Path $packageRoot "lml\Frontier Ledger Korean Text") -Recurse -Force
Copy-Item (Join-Path $projectRoot "release_tools\Prepare_Backup.ps1") (Join-Path $packageRoot "Prepare_Backup.ps1") -Force
Copy-Item (Join-Path $projectRoot "release_tools\Install_RC.ps1") (Join-Path $packageRoot "Install_RC.ps1") -Force
Copy-Item (Join-Path $projectRoot "release_tools\Rollback_Backup.ps1") (Join-Path $packageRoot "Rollback_Backup.ps1") -Force
Copy-Item (Join-Path $projectRoot "release_tools\INSTALL_AND_ROLLBACK.md") (Join-Path $packageRoot "INSTALL_AND_ROLLBACK.md") -Force

$checksums = @()
foreach ($relativePath in @(
    "FrontierLedger.asi",
    "FrontierLedger.ini",
    "lml\Frontier Ledger Korean Text\strings.gxt2"
)) {
    $fullPath = Join-Path $packageRoot $relativePath
    $hash = Get-FileHash -Path $fullPath -Algorithm SHA256
    $checksums += "{0}  {1}" -f $hash.Hash.ToLowerInvariant(), $relativePath
}
Set-Content -Path (Join-Path $packageRoot "SHA256SUMS.txt") -Value $checksums -Encoding UTF8

if (Test-Path $zipPath) {
    Remove-Item $zipPath -Force
}
Compress-Archive -Path (Join-Path $packageRoot "*") -DestinationPath $zipPath -CompressionLevel Optimal

Write-Host "Release candidate package created:"
Write-Host " - $packageRoot"
Write-Host " - $zipPath"
