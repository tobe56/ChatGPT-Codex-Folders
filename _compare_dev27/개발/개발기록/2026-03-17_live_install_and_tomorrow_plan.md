# 2026-03-17 Live Install And Tomorrow Plan

## End-Of-Day State

Today no longer ends at the "prototype-only" or "doc-only" stage.
The workspace now includes:

- affinity stage quests and reward packages
- feature-owned category modules for `Homefront`, `Farming`, `Network`, `World`, and `Ending`
- milestone / package loops in those four broader categories
- a `19-page` native ledger export that includes `Affinity`
- a clean live install in the real RDR2 folder whose ASI / INI / LML hashes match the current workspace `dist` outputs

## Live Install Record

The previous `Frontier Ledger` install was rotated into:

- `E:\SteamLibrary\steamapps\common\Red Dead Redemption 2\codex_backups\FrontierLedger_clean_install_20260317_210545`

The current live files were then refreshed from the workspace `dist` package:

- `FrontierLedger.asi`
- `FrontierLedger.ini`
- `lml/downloader/Frontier Ledger Korean Text/install.xml`
- `lml/downloader/Frontier Ledger Korean Text/strings.gxt2`

These files were checked by SHA256 after the reinstall.

## Live Verification Outcome

The in-game report for the final install was positive:

- the current UI pass displayed correctly
- the inspected `FrontierLedger.log` tail showed no direct `error` / `failed` / `exception` lines
- the live run advanced to `Phase 1 / TrailStage 1`
- one Beecher's Hope hub return was counted
- one `Gunsmithing` skill bonus was persisted in the profile

One caveat remains:

- the live log still prints the stale build label `2026-03-16-live-growth-pass`, so version identity should be checked by file hash before that label

## Tomorrow Focus

Tomorrow should avoid another day of menu/page expansion and instead convert more authored systems into real world behavior.

Priority order:

1. enemy AI and boss kill-resolution hooks
2. world item spawning, material drops, and loot-to-stash routing
3. live perk / stat / skill effects that change gameplay, not only ledger output
4. item economy and balance tuning for drops, recipes, and boss rewards

## Practical Rule For Tomorrow

- keep UI expansion secondary
- prefer world behavior over another summary layer
- treat display-only progress as insufficient unless it changes what happens in the field
