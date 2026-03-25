param(
    [string]$GameRoot = "E:\SteamLibrary\steamapps\common\Red Dead Redemption 2",
    [Parameter(Mandatory = $true)]
    [string]$BackupDir
)

$ErrorActionPreference = "Stop"

function Restore-ItemFromBackup {
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

$manifestPath = Join-Path $BackupDir "backup_manifest.json"
if (-not (Test-Path $manifestPath)) {
    throw "Missing backup manifest: $manifestPath"
}

$manifest = Get-Content -Path $manifestPath -Raw | ConvertFrom-Json
$restoreRoot = $manifest.restore_root
if (-not $restoreRoot -or -not (Test-Path $restoreRoot)) {
    throw "Backup restore root is missing: $restoreRoot"
}

foreach ($entry in $manifest.entries) {
    $targetPath = Join-Path $GameRoot $entry.relative_path
    if (Test-Path $targetPath) {
        Remove-Item $targetPath -Recurse -Force
    }

    if ($entry.existed) {
        $sourcePath = Join-Path $restoreRoot $entry.relative_path
        if (Test-Path $sourcePath) {
            Restore-ItemFromBackup -SourcePath $sourcePath -DestinationPath $targetPath
        }
    }
}

Write-Host "Rollback complete:"
Write-Host " - $GameRoot"
Write-Host "Restored from:"
Write-Host " - $BackupDir"
