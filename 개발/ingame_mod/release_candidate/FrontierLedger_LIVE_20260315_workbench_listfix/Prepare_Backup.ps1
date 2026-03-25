param(
    [string]$GameRoot = "E:\SteamLibrary\steamapps\common\Red Dead Redemption 2",
    [string]$BackupRoot = ""
)

$ErrorActionPreference = "Stop"

function Copy-ExistingItem {
    param(
        [string]$SourcePath,
        [string]$DestinationPath
    )

    $parent = Split-Path -Parent $DestinationPath
    if ($parent -and -not (Test-Path $parent)) {
        New-Item -ItemType Directory -Path $parent -Force | Out-Null
    }

    $item = Get-Item $SourcePath -Force
    if ($item.PSIsContainer) {
        Copy-Item $SourcePath $DestinationPath -Recurse -Force
    } else {
        Copy-Item $SourcePath $DestinationPath -Force
    }
}

if (-not (Test-Path $GameRoot)) {
    throw "Game root does not exist: $GameRoot"
}

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
if (-not $BackupRoot) {
    $BackupRoot = Join-Path $scriptRoot "backup_sets"
}

if (-not (Test-Path $BackupRoot)) {
    New-Item -ItemType Directory -Path $BackupRoot -Force | Out-Null
}

$stamp = Get-Date -Format "yyyyMMdd_HHmmss"
$backupDir = Join-Path $BackupRoot "FrontierLedger_RC1_$stamp"
$restoreRoot = Join-Path $backupDir "restore"
New-Item -ItemType Directory -Path $restoreRoot -Force | Out-Null

$targets = @(
    @{ RelativePath = "FrontierLedger.asi"; Type = "file" },
    @{ RelativePath = "FrontierLedger.ini"; Type = "file" },
    @{ RelativePath = "lml\downloader\Frontier Ledger Korean Text"; Type = "directory" },
    @{ RelativePath = "lml\mods.xml"; Type = "file" }
)

$entries = @()
foreach ($target in $targets) {
    $sourcePath = Join-Path $GameRoot $target.RelativePath
    $entry = [PSCustomObject]@{
        relative_path = $target.RelativePath
        item_type = $target.Type
        existed = $false
    }

    if (Test-Path $sourcePath) {
        $entry.existed = $true
        $destinationPath = Join-Path $restoreRoot $target.RelativePath
        Copy-ExistingItem -SourcePath $sourcePath -DestinationPath $destinationPath
    }

    $entries += $entry
}

$manifest = [PSCustomObject]@{
    created_at = (Get-Date).ToString("s")
    game_root = $GameRoot
    restore_root = $restoreRoot
    entries = $entries
}

$manifestPath = Join-Path $backupDir "backup_manifest.json"
$manifest | ConvertTo-Json -Depth 4 | Set-Content -Path $manifestPath -Encoding UTF8

Write-Host "Prepared backup set:"
Write-Host " - $backupDir"
Write-Host "Manifest:"
Write-Host " - $manifestPath"
