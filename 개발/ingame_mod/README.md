# Frontier Ledger In-Game Mod

The current milestone is the first meaningful prototype-to-native migration pass: the ASI now consumes the document-led ledger pages instead of only the original first-pass stats/perks/crafting set.

This folder contains the first real in-game mod foundation for the Frontier Ledger UI.
It stays entirely inside `E:\RDR2 RPG MODS\개발` and does not copy anything into the live game folder.

## What It Builds

- `FrontierLedger.asi`: native ScriptHookRDR2 plugin
- compact field ledger toggle
- full ledger screen for Beecher's Hope interior use
- generated ledger data from the existing Python prototype
- windowed tab rendering for larger ledger datasets
- package-ready output inside `dist\FrontierLedger`

## Controls

- `F10` (`121`): toggle compact field ledger
- `F11` (`122`): open or close full ledger
- `Left` / `Right`: change tab
- `Up` / `Down`: change selected entry
- `Esc`: close full ledger

## Build

From `E:\RDR2 RPG MODS\개발\ingame_mod`:

```powershell
.\build.ps1
```

The build script will:

1. export current prototype data into `generated\frontier_ledger_generated.h`
2. compile the native ASI with Zig
3. place a copy in `dist\FrontierLedger`

The generated header now includes these major native pages:

- `Overview`
- `Active Trails`
- `Frontier Map`
- `Homefront`
- `Stats`
- `Gear`
- `Armory`
- `Skills`
- `Perks`
- `Crafting`
- `Workshop`
- `Ledger of the Dead`
- `Collection`
- `Farming`
- `Network`
- `World`
- `Ending`
- `Encyclopedia`

It also bakes:

- `baseline_en`
- `simulated_en`
- `endgame_en`
- `baseline_ko`
- `simulated_ko`
- `endgame_ko`

The current native page count is `18`, which is why the sliding tab window is now part of the baseline UI behavior.

## Active Locale Baseline

The current active development baseline is English-first.

- `FrontierLedger.ini` now defaults to `DefaultDataset=live_en`
- `FrontierLedger.ini` now also carries `PreferredLocale=en`
- Korean is intentionally kept as a parked option seam instead of the active development baseline

If the Korean pass needs to resume later, start from:

- `locales\ko\FrontierLedger.ko.example.ini`
- `locales\ko\README.md`

The native runtime now also includes:

- a live frontier-region context derived from player coordinates
- route hints embedded into the compact and full ledger
- automatic region-aware page focus for map/farming/network/world/ending views
- session restore data written to `FrontierLedger.session.ini`
- a persistent `FrontierLedger.profile.ini` live run that tracks discovered regions, field travel, Beecher's Hope returns, and current phase
- `live_en` / `live_ko` dataset aliases that automatically swap between baseline, simulated, and endgame exports as the live run advances
- runtime objective and live phase summaries inside both the compact and full ledger views
- runtime detail overrides for the most important ledger pages so the native UI reacts to the live run state instead of only swapping between static baked datasets
- an English-first locale normalization pass so the saved session dataset no longer drifts the active UI back into Korean while the renderer/layout baseline is being stabilized

## Workspace Smoke Test

From `E:\RDR2 RPG MODS\개발\ingame_mod`:

```powershell
python .\scripts\workspace_smoke_test.py
```

This creates temporary runtime folders, loads the built ASI with `LoadLibrary`, confirms attach/timeout/detach logging, and checks that both the session and live profile files are emitted cleanly for a baseline live run and a seeded endgame live run.

## Preflight Check

You can validate the package before copying anything:

```powershell
.\preflight.ps1
.\preflight.ps1 -GameRoot "E:\SteamLibrary\steamapps\common\Red Dead Redemption 2"
```

This checks the packaged files first, and optionally checks whether the target game root already has the basic ScriptHook / ASI loader prerequisites.

## Release Candidate Package

From `E:\RDR2 RPG MODS\媛쒕컻\ingame_mod`:

```powershell
.\package_release_candidate.ps1
.\scripts\test_install_cycle.ps1
```

This creates a clean release-candidate package under `release_candidate\FrontierLedger_RC1` and proves backup, staged install, and rollback on a fake game root before any real deployment.

## Notes

- The Beecher's Hope house gate is configurable in `FrontierLedger.ini`.
- The current in-game UI now mixes baked document pages with a persistent live frontier run, but it still does not read real save data or apply actual combat/stat changes back into vanilla gameplay systems.
- The full ledger now uses a sliding tab window plus `current/total` page indicators because the native page count is now large enough to overflow the first-pass tab layout.
- No automatic installation step is included, because this workspace should stay separate from the actual game folder.
