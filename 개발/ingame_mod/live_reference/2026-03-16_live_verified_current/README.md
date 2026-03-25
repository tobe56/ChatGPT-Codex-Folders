# Live Verified Snapshot 2026-03-16

This folder preserves the exact Frontier Ledger package that was installed in the live Red Dead Redemption 2 game root when the user reported that the mod was displaying correctly.

## Capture Reason

Preserve the currently readable live build before further UI or content work changes the baseline.

## Source

- `E:\SteamLibrary\steamapps\common\Red Dead Redemption 2`
- install manifest timestamp: `2026-03-16T21:04:15`

## Snapshot Contents

- `game_root/FrontierLedger.asi`
- `game_root/FrontierLedger.ini`
- `game_root/FrontierLedger.log`
- `game_root/FrontierLedger.profile.ini`
- `game_root/FrontierLedger.session.ini`
- `game_root/FrontierLedger_Remove.json`
- `game_root/FrontierLedger_Remove.ps1`
- `game_root/lml/downloader/Frontier Ledger Korean Text/install.xml`
- `game_root/lml/downloader/Frontier Ledger Korean Text/strings.gxt2`

## Current Runtime State Captured

- dataset: `live_en`
- preferred locale: `en`
- renderer: `UseDesktopOverlay=0`
- live phase: `1`
- trail stage: `1`
- discovered region mask: `9`
- hub returns: `1`
- current page: `17`
- workbench tab: `2`

## Hash Match Against Workspace Outputs

- `FrontierLedger.asi`
  - SHA256: `57388928639968562E0C18B7870E8A2AD56C2B57D3CE1DF66363125781C3254E`
  - matches `ingame_mod/dist/FrontierLedger/FrontierLedger.asi`
- `strings.gxt2`
  - SHA256: `5E6FB65D1B85341AC7A8D4E82500C6B76942B05CBF3E529C327E75573A97D0B7`
  - matches `ingame_mod/dist/FrontierLedger/lml/Frontier Ledger Korean Text/strings.gxt2`

## How To Use This Snapshot

- Treat this folder as the restore/reference baseline for the current readable live build.
- If a future change breaks layout, strings, or live readability, diff the active `dist` outputs against this snapshot first.
- Do not edit files here in place unless intentionally replacing the preserved baseline with a new verified live snapshot.
