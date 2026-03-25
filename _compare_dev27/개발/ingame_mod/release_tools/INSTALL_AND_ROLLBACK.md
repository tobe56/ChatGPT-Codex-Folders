# Frontier Ledger RC Install and Rollback

This package is the workspace-validated release candidate build.
It was built and smoke-tested inside `E:\RDR2 RPG MODS\개발\ingame_mod` without copying into the live game folder.

## Package Contents

- `FrontierLedger.asi`
- `FrontierLedger.ini`
- `lml\Frontier Ledger Korean Text\install.xml`
- `lml\Frontier Ledger Korean Text\strings.gxt2`
- `Prepare_Backup.ps1`
- `Install_RC.ps1`
- `Rollback_Backup.ps1`

## 1. Prepare Backup Before Install

From this package folder:

```powershell
.\Prepare_Backup.ps1 -GameRoot "E:\SteamLibrary\steamapps\common\Red Dead Redemption 2"
```

This creates a timestamped backup set under `backup_sets\`.
It captures these targets if they already exist:

- `FrontierLedger.asi`
- `FrontierLedger.ini`
- `lml\downloader\Frontier Ledger Korean Text`
- `lml\mods.xml`

Keep the printed backup directory path. You will need it for rollback.

## 2. Install The RC Package

```powershell
.\Install_RC.ps1 -GameRoot "E:\SteamLibrary\steamapps\common\Red Dead Redemption 2"
```

This copies:

- `FrontierLedger.asi`
- `FrontierLedger.ini`
- `lml\Frontier Ledger Korean Text` into `GameRoot\lml\downloader\Frontier Ledger Korean Text`

If you use LML, enable `Frontier Ledger Korean Text` in LML Mod Manager after copying.

## 3. Roll Back To The Backup Set

```powershell
.\Rollback_Backup.ps1 -GameRoot "E:\SteamLibrary\steamapps\common\Red Dead Redemption 2" -BackupDir "PATH_TO_BACKUP_SET"
```

The rollback script removes the installed RC files and restores anything captured in the backup manifest.

## 4. Recommended First Test

1. Launch the game.
2. Confirm the game boots without an entry crash.
3. Use `F10` for the compact ledger.
4. Use `F11` for the full ledger.
5. Kill enemies, hunt one animal, loot a corpse, and gain some money.
6. Check `Overview`, `Farming`, `Network`, and `World`.
7. Confirm `FrontierLedger.profile.ini` updates with counters and trail stage.

## 5. Log Files To Check

After install, watch these files inside the game root:

- `FrontierLedger.log`
- `FrontierLedger.session.ini`
- `FrontierLedger.profile.ini`
