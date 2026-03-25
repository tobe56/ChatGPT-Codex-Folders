$ErrorActionPreference = 'Stop'
$gameRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$manifestPath = Join-Path $gameRoot 'FrontierLedger_Remove.json'

if (Test-Path $manifestPath) {
    $manifest = Get-Content $manifestPath -Raw | ConvertFrom-Json
    foreach ($file in $manifest.files) {
        if (Test-Path $file) {
            Remove-Item $file -Force
        }
    }
    foreach ($entry in $manifest.backups) {
        if (Test-Path $entry.Backup) {
            Move-Item $entry.Backup $entry.Target -Force
        }
    }
    Remove-Item $manifestPath -Force -ErrorAction SilentlyContinue
}

Remove-Item (Join-Path $gameRoot 'FrontierLedger_Remove.ps1') -Force -ErrorAction SilentlyContinue
Write-Host 'Frontier Ledger removed.'
