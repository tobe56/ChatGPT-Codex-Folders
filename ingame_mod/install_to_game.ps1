param(
    [string]$GameRoot = "E:\SteamLibrary\steamapps\common\Red Dead Redemption 2"
)

$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$distRoot = Join-Path $projectRoot "dist\FrontierLedger"
$packageItems = @(
    [PSCustomObject]@{
        Source = Join-Path $distRoot "FrontierLedger.asi"
        Target = Join-Path $GameRoot "FrontierLedger.asi"
    },
    [PSCustomObject]@{
        Source = Join-Path $distRoot "FrontierLedger.ini"
        Target = Join-Path $GameRoot "FrontierLedger.ini"
    },
    [PSCustomObject]@{
        Source = Join-Path $distRoot "lml\Frontier Ledger Korean Text\install.xml"
        Target = Join-Path $GameRoot "lml\downloader\Frontier Ledger Korean Text\install.xml"
    },
    [PSCustomObject]@{
        Source = Join-Path $distRoot "lml\Frontier Ledger Korean Text\strings.gxt2"
        Target = Join-Path $GameRoot "lml\downloader\Frontier Ledger Korean Text\strings.gxt2"
    }
)

if (-not (Test-Path $GameRoot)) {
    throw "Game root does not exist: $GameRoot"
}

foreach ($item in $packageItems) {
    if (-not (Test-Path $item.Source)) {
        throw "Missing package file: $($item.Source)"
    }
}

$stamp = Get-Date -Format "yyyyMMdd_HHmmss"
$backupRecords = @()

foreach ($item in $packageItems) {
    if (Test-Path $item.Target) {
        $backupPath = "$($item.Target).bak_codex_$stamp"
        Move-Item $item.Target $backupPath -Force
        $backupRecords += [PSCustomObject]@{
            Target = $item.Target
            Backup = $backupPath
        }
    }
}

foreach ($item in $packageItems) {
    $targetDir = Split-Path -Parent $item.Target
    if (-not (Test-Path $targetDir)) {
        New-Item -ItemType Directory -Path $targetDir -Force | Out-Null
    }
    Copy-Item $item.Source $item.Target -Force
}

$removeScriptPath = Join-Path $GameRoot "FrontierLedger_Remove.ps1"
$manifestPath = Join-Path $GameRoot "FrontierLedger_Remove.json"
$logPath = Join-Path $GameRoot "FrontierLedger.log"

$manifest = [PSCustomObject]@{
    installed_at = (Get-Date).ToString("s")
    game_root = $GameRoot
    files = @($packageItems | ForEach-Object { $_.Target }) + @($logPath)
    backups = $backupRecords
}

$manifest | ConvertTo-Json -Depth 4 | Set-Content -Encoding UTF8 $manifestPath

$removeScript = @"
`$ErrorActionPreference = 'Stop'
`$gameRoot = Split-Path -Parent `$MyInvocation.MyCommand.Path
`$manifestPath = Join-Path `$gameRoot 'FrontierLedger_Remove.json'

if (Test-Path `$manifestPath) {
    `$manifest = Get-Content `$manifestPath -Raw | ConvertFrom-Json
    foreach (`$file in `$manifest.files) {
        if (Test-Path `$file) {
            Remove-Item `$file -Force
        }
    }
    foreach (`$entry in `$manifest.backups) {
        if (Test-Path `$entry.Backup) {
            Move-Item `$entry.Backup `$entry.Target -Force
        }
    }
    Remove-Item `$manifestPath -Force -ErrorAction SilentlyContinue
}

Remove-Item (Join-Path `$gameRoot 'FrontierLedger_Remove.ps1') -Force -ErrorAction SilentlyContinue
Write-Host 'Frontier Ledger removed.'
"@

Set-Content -Path $removeScriptPath -Value $removeScript -Encoding UTF8

Write-Host ""
Write-Host "Installed Frontier Ledger to:"
Write-Host " - $GameRoot"
Write-Host ""
Write-Host "Installed files:"
foreach ($item in $packageItems) {
    Write-Host " - $($item.Target)"
}
Write-Host " - $removeScriptPath"
Write-Host ""
if ($backupRecords.Count -gt 0) {
    Write-Host "Backups:"
    $backupRecords | ForEach-Object { Write-Host " - $($_.Backup)" }
} else {
    Write-Host "Backups: none"
}
