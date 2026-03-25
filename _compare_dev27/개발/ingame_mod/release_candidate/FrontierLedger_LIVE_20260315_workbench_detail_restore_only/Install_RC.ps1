param(
    [string]$GameRoot = "E:\SteamLibrary\steamapps\common\Red Dead Redemption 2",
    [switch]$SkipLml
)

$ErrorActionPreference = "Stop"

function Copy-PackageItem {
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
        if (Test-Path $DestinationPath) {
            Remove-Item $DestinationPath -Recurse -Force
        }
        Copy-Item $SourcePath $DestinationPath -Recurse -Force
    } else {
        Copy-Item $SourcePath $DestinationPath -Force
    }
}

if (-not (Test-Path $GameRoot)) {
    throw "Game root does not exist: $GameRoot"
}

$packageRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$required = @(
    "FrontierLedger.asi",
    "FrontierLedger.ini"
)

foreach ($relativePath in $required) {
    $packagePath = Join-Path $packageRoot $relativePath
    if (-not (Test-Path $packagePath)) {
        throw "Missing package file: $packagePath"
    }
}

Copy-PackageItem -SourcePath (Join-Path $packageRoot "FrontierLedger.asi") -DestinationPath (Join-Path $GameRoot "FrontierLedger.asi")
Copy-PackageItem -SourcePath (Join-Path $packageRoot "FrontierLedger.ini") -DestinationPath (Join-Path $GameRoot "FrontierLedger.ini")

if (-not $SkipLml) {
    $lmlSource = Join-Path $packageRoot "lml\Frontier Ledger Korean Text"
    if (Test-Path $lmlSource) {
        $lmlTarget = Join-Path $GameRoot "lml\downloader\Frontier Ledger Korean Text"
        Copy-PackageItem -SourcePath $lmlSource -DestinationPath $lmlTarget
    }
}

Write-Host "Installed RC package to:"
Write-Host " - $GameRoot"
Write-Host "Installed runtime files:"
Write-Host " - $(Join-Path $GameRoot 'FrontierLedger.asi')"
Write-Host " - $(Join-Path $GameRoot 'FrontierLedger.ini')"
if (-not $SkipLml) {
    Write-Host " - $(Join-Path $GameRoot 'lml\downloader\Frontier Ledger Korean Text')"
}
Write-Host ""
Write-Host "If you use LML, enable 'Frontier Ledger Korean Text' in LML Mod Manager after copying."
