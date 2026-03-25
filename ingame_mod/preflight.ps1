param(
    [string]$GameRoot = ""
)

$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$distRoot = Join-Path $projectRoot "dist\FrontierLedger"
$asiPath = Join-Path $distRoot "FrontierLedger.asi"
$iniPath = Join-Path $distRoot "FrontierLedger.ini"

$issues = @()
$notes = @()

if (-not (Test-Path $asiPath)) {
    $issues += "Missing packaged ASI: $asiPath"
} else {
    $notes += "Packaged ASI found: $asiPath"
}

if (-not (Test-Path $iniPath)) {
    $issues += "Missing packaged INI: $iniPath"
} else {
    $notes += "Packaged INI found: $iniPath"
}

if ($GameRoot) {
    if (-not (Test-Path $GameRoot)) {
        $issues += "Game root does not exist: $GameRoot"
    } else {
        $scriptHookPath = Join-Path $GameRoot "ScriptHookRDR2.dll"
        $loaderHints = @(
            (Join-Path $GameRoot "version.dll"),
            (Join-Path $GameRoot "dinput8.dll"),
            (Join-Path $GameRoot "vfs.asi")
        )

        if (Test-Path $scriptHookPath) {
            $notes += "ScriptHookRDR2.dll found."
        } else {
            $issues += "ScriptHookRDR2.dll is missing from the game root."
        }

        if ($loaderHints | Where-Object { Test-Path $_ }) {
            $notes += "At least one ASI loader hint is present."
        } else {
            $issues += "No common ASI loader file was found (version.dll, dinput8.dll, vfs.asi)."
        }

        if (Test-Path (Join-Path $GameRoot "FrontierLedger.asi")) {
            $notes += "A FrontierLedger.asi already exists in the target game root."
        }
    }
}

Write-Host ""
Write-Host "Frontier Ledger Preflight"
Write-Host "========================="

if ($notes.Count -gt 0) {
    Write-Host ""
    Write-Host "Notes:"
    $notes | ForEach-Object { Write-Host " - $_" }
}

if ($issues.Count -gt 0) {
    Write-Host ""
    Write-Host "Issues:"
    $issues | ForEach-Object { Write-Host " - $_" }
    exit 1
}

Write-Host ""
Write-Host "Preflight passed."
