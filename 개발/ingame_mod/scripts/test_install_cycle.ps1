param()

$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)
$packageRoot = Join-Path $projectRoot "release_candidate\FrontierLedger_RC1"
$testRoot = Join-Path $projectRoot "rc_cycle_test_root"
$backupRoot = Join-Path $projectRoot "rc_cycle_test_backups"

if (-not (Test-Path $packageRoot)) {
    throw "Missing release candidate package: $packageRoot"
}

if (Test-Path $testRoot) {
    Remove-Item $testRoot -Recurse -Force
}
if (Test-Path $backupRoot) {
    Remove-Item $backupRoot -Recurse -Force
}

New-Item -ItemType Directory -Path (Join-Path $testRoot "lml\downloader\Frontier Ledger Korean Text") -Force | Out-Null
Set-Content -Path (Join-Path $testRoot "FrontierLedger.asi") -Value "OLD_ASI" -Encoding Ascii
Set-Content -Path (Join-Path $testRoot "FrontierLedger.ini") -Value "OLD_INI" -Encoding Ascii
Set-Content -Path (Join-Path $testRoot "lml\mods.xml") -Value "<mods old=`"1`" />" -Encoding UTF8
Set-Content -Path (Join-Path $testRoot "lml\downloader\Frontier Ledger Korean Text\strings.gxt2") -Value "OLD_GXT" -Encoding Ascii

& (Join-Path $packageRoot "Prepare_Backup.ps1") -GameRoot $testRoot -BackupRoot $backupRoot
$backupDir = Get-ChildItem $backupRoot | Sort-Object LastWriteTime -Descending | Select-Object -First 1 -ExpandProperty FullName
if (-not $backupDir) {
    throw "Backup directory was not created."
}

& (Join-Path $packageRoot "Install_RC.ps1") -GameRoot $testRoot
if (-not (Test-Path (Join-Path $testRoot "FrontierLedger.asi"))) {
    throw "Install did not copy ASI."
}
if (-not (Test-Path (Join-Path $testRoot "FrontierLedger.ini"))) {
    throw "Install did not copy INI."
}
if (-not (Test-Path (Join-Path $testRoot "lml\downloader\Frontier Ledger Korean Text\strings.gxt2"))) {
    throw "Install did not copy LML text pack."
}

$installedSize = (Get-Item (Join-Path $testRoot "FrontierLedger.asi")).Length
if ($installedSize -le 16) {
    throw "Installed ASI still looks like the placeholder."
}

& (Join-Path $packageRoot "Rollback_Backup.ps1") -GameRoot $testRoot -BackupDir $backupDir
$restoredAsi = Get-Content -Path (Join-Path $testRoot "FrontierLedger.asi") -Raw
$restoredIni = Get-Content -Path (Join-Path $testRoot "FrontierLedger.ini") -Raw
$restoredGxt = Get-Content -Path (Join-Path $testRoot "lml\downloader\Frontier Ledger Korean Text\strings.gxt2") -Raw

if ($restoredAsi.Trim() -ne "OLD_ASI") {
    throw "Rollback did not restore original ASI."
}
if ($restoredIni.Trim() -ne "OLD_INI") {
    throw "Rollback did not restore original INI."
}
if ($restoredGxt.Trim() -ne "OLD_GXT") {
    throw "Rollback did not restore original LML text pack."
}
if (-not (Test-Path (Join-Path $testRoot "lml\mods.xml"))) {
    throw "Rollback removed mods.xml unexpectedly."
}

Remove-Item $testRoot -Recurse -Force
Remove-Item $backupRoot -Recurse -Force

Write-Host "RC_INSTALL_CYCLE_OK"
