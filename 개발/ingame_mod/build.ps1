$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$generatedDir = Join-Path $projectRoot "generated"
$buildDir = Join-Path $projectRoot "build"
$distDir = Join-Path $projectRoot "dist\\FrontierLedger"
$distLmlDir = Join-Path $distDir "lml\\Frontier Ledger Korean Text"
$headerPath = Join-Path $generatedDir "frontier_ledger_generated.h"
$stringsPath = Join-Path $generatedDir "strings.gxt2"
$bitmapHeaderPath = Join-Path $generatedDir "frontier_bitmap_font_generated.h"
$bitmapRuntimeStringsPath = Join-Path $projectRoot "runtime_bitmap_strings.txt"
$pythonExe = (Get-Command python).Source
$zigExe = Join-Path $projectRoot "..\\tools\\zig\\zig-x86_64-windows-0.14.1\\zig.exe"

if (-not (Test-Path $zigExe)) {
    throw "Zig compiler not found at $zigExe"
}

New-Item -ItemType Directory -Force $generatedDir | Out-Null
New-Item -ItemType Directory -Force $buildDir | Out-Null
New-Item -ItemType Directory -Force $distDir | Out-Null
New-Item -ItemType Directory -Force $distLmlDir | Out-Null

& $pythonExe (Join-Path $projectRoot "scripts\\export_ledger_data.py") --output $headerPath --strings-output $stringsPath
if ($LASTEXITCODE -ne 0) {
    throw "Header generation failed."
}

& $pythonExe (Join-Path $projectRoot "scripts\\generate_bitmap_font.py") `
    --strings-input $stringsPath `
    --extra-strings-input $bitmapRuntimeStringsPath `
    --font-size 14 `
    --threshold 18 `
    --solid-threshold 64 `
    --output $bitmapHeaderPath
if ($LASTEXITCODE -ne 0) {
    throw "Bitmap font generation failed."
}

& $zigExe cc `
    -std=c11 `
    -O2 `
    -DNDEBUG `
    -target x86_64-windows-gnu `
    -shared `
    -static `
    -I (Join-Path $projectRoot "src") `
    -I $generatedDir `
    -o (Join-Path $buildDir "FrontierLedger.asi") `
    (Join-Path $projectRoot "src\\frontier_ledger.c") `
    (Join-Path $projectRoot "src\\ledger_config.c") `
    (Join-Path $projectRoot "src\\script_hook_bindings.c") `
    -lkernel32 `
    -luser32 `
    -lgdi32

if ($LASTEXITCODE -ne 0) {
    throw "Native build failed."
}

Copy-Item (Join-Path $buildDir "FrontierLedger.asi") (Join-Path $distDir "FrontierLedger.asi") -Force
Copy-Item (Join-Path $projectRoot "FrontierLedger.ini") (Join-Path $distDir "FrontierLedger.ini") -Force
Copy-Item $stringsPath (Join-Path $distLmlDir "strings.gxt2") -Force
Copy-Item (Join-Path $projectRoot "lml\\Frontier Ledger Korean Text\\install.xml") (Join-Path $distLmlDir "install.xml") -Force

Write-Host ""
Write-Host "Build complete."
Write-Host "ASI:  $(Join-Path $buildDir 'FrontierLedger.asi')"
Write-Host "Dist: $(Join-Path $distDir 'FrontierLedger.asi')"
