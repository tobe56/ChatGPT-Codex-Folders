# RDR2 Epilogue RPG Workspace

This folder now contains two connected tracks:

- `frontier_rpg`: the Python prototype used to validate RPG systems and UI data
- `ingame_mod`: the native ScriptHookRDR2 mod foundation that builds `FrontierLedger.asi`

The current development priority is now aligned with the design docs instead of duplicating vanilla HUD data.

The Python side turns the design into executable logic for:

- frontier level and XP
- stat point allocation
- perk unlock rules
- crafting and workbench progression
- weapon condition and familiarity
- world tier promotion
- story gate validation
- active trail routing
- frontier map intel
- Beecher's Hope hub feature progression
- affinity and relationship progression
- Frontier Ledger style summaries

## Run The Prototype

From `E:\RDR2 RPG MODS\개발`:

```bash
python -m frontier_rpg overview
python -m frontier_rpg ledger
python -m frontier_rpg simulate
python -m frontier_rpg trails
python -m frontier_rpg intel
python -m frontier_rpg hub
python -m frontier_rpg gear
python -m frontier_rpg armory
python -m frontier_rpg dead
python -m frontier_rpg collection
python -m frontier_rpg homefront
python -m frontier_rpg farming
python -m frontier_rpg network
python -m frontier_rpg world
python -m frontier_rpg ending
python -m frontier_rpg affinity
python -m frontier_rpg endgame
python -m frontier_rpg ui
python -m frontier_rpg ui --check
python -m unittest discover -s tests
```

## Memory Docs

Reusable memory documents now live under:

- `../Memory/TroubleShootings.md`
- `../Memory/API_Index.md`
- `../Memory/Frontier_Ledger_Domain_Knowledge.md`
- `../Memory/Architecture_Domain_Knowledge.md`

Use these before broad refactors, repeated debugging, or large workspace searches.

## Build The In-Game Mod

From `E:\RDR2 RPG MODS\개발\ingame_mod`:

```powershell
.\build.ps1
```

This generates:

- `ingame_mod\build\FrontierLedger.asi`
- `ingame_mod\dist\FrontierLedger\FrontierLedger.asi`
- `ingame_mod\dist\FrontierLedger\FrontierLedger.ini`

This workspace build step does not install anything into the actual game folder.

The native export now carries the first major document-aligned page migration:

- `Overview`
- `Active Trails`
- `Frontier Map`
- `Homefront`
- `Affinity`
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

The full ledger UI now uses a sliding tab window so these pages remain usable in the ASI build even after the page count moved beyond the original first-pass layout.

The latest native pass also adds:

- live runtime context from player coordinates
- region-aware route hints in the compact and full ledger
- automatic focus for region-sensitive pages
- session persistence through `FrontierLedger.session.ini`
- persistent live frontier progression through `FrontierLedger.profile.ini`
- `live_en` / `live_ko` dataset aliases that auto-shift between baseline, simulated, and endgame page sets based on real field travel, region discovery, and Beecher's Hope returns
- compact and full ledger runtime summaries for live phase, progress, and current frontier objective
- live detail overrides for `Overview`, `Trails`, `Frontier Map`, `Homefront`, `Farming`, `Network`, `World`, and `Ending`, so the ASI pages now react to the persistent run state instead of staying static once loaded

## Prototype Focus Areas

`frontier_rpg` now exposes the doc-led systems that were missing from the first pass:

- `Active Trails`: main story, contracts, boss hunts, house objectives, repeatables
- `Frontier Map`: region danger, node focus, faction pressure, boss clue lanes
- `Beecher's Hope Hub`: ledger table, contract wall, stash, map board, armory, trophy board, support station
- `Homefront`: Abigail / Jack / Uncle support state and unlock roles
- `Farming`: hunter / raider / contract / frontier work / boss lane progression
- `Network`: repurposed subsystem tracks for bounties, jobs, contracts, stagecoaches, duels, companions, properties, banks
- `World State`: tier-by-tier enemy, loot, boss, recipe, story, and route escalation
- `Ending`: final act route plus post-ending sandbox state
- `Gear`: grounded accessory, charm, holster, satchel, coat, saddle slots
- `Armory`: weapon condition, familiarity, archetype path, installed mods
- `Ledger of the Dead`: discovered bosses, hidden silhouettes, territory and rewards
- `Collection`: relic sets, broker marks, house trophies, archive progress
- `Workshop Routing`: recipe readiness plus hub feature unlock state

The native `ingame_mod` build now mirrors this broader page set too, with a 19-page full ledger export instead of the earlier narrow first-pass page list.

## Dataset Options

`FrontierLedger.ini` supports multiple baked datasets plus a locale preference seam:

- `DefaultDataset=simulated_en`: English simulated run
- `DefaultDataset=baseline_en`: English baseline state
- `DefaultDataset=endgame_en`: English post-ending validation state
- `DefaultDataset=live_en`: English live run that advances across baseline, simulated, and endgame states
- `DefaultDataset=simulated_ko`: Korean simulated run
- `DefaultDataset=baseline_ko`: Korean baseline state
- `DefaultDataset=endgame_ko`: Korean post-ending validation state
- `DefaultDataset=live_ko`: Korean live run alias

Current active development baseline:

- `DefaultDataset=live_en`
- `PreferredLocale=en`

This keeps the active workspace in an English-first mode so renderer, wrap, clipping, and workbench behavior can stabilize on short ASCII-safe strings first.

Later Korean re-entry seam:

- change `PreferredLocale=ko`
- change `DefaultDataset=live_ko`
- or start from `ingame_mod\locales\ko\FrontierLedger.ko.example.ini`

These dataset options live in the development workspace and can be baked into later in-game builds without forcing the current baseline back into Korean-first tuning.

## Workspace Smoke Test

From [ingame_mod](E:/RDR2%20RPG%20MODS/개발/ingame_mod):

```powershell
python .\scripts\workspace_smoke_test.py
```

This isolated check loads the built ASI from a temporary runtime folder, verifies attach/timeout/detach logging, and confirms both `FrontierLedger.session.ini` and `FrontierLedger.profile.ini` are produced without touching the live game folder.

## Live Installed Reference

The current known-good live install has been pulled back into the workspace as a reference snapshot:

- `ingame_mod/live_reference/2026-03-16_live_verified_current`

This snapshot includes:

- the exact installed `FrontierLedger.asi`
- the exact installed `FrontierLedger.ini`
- the installed LML text pack files
- the current live `FrontierLedger.log`
- the current live `FrontierLedger.session.ini`
- the current live `FrontierLedger.profile.ini`
- the live removal manifest and removal script

Verification note:

- `game_root/FrontierLedger.asi` in the snapshot matches `ingame_mod/dist/FrontierLedger/FrontierLedger.asi` by SHA256
- `game_root/lml/downloader/Frontier Ledger Korean Text/strings.gxt2` in the snapshot matches the workspace `dist` copy by SHA256

Use this snapshot as the readable live baseline before future UI or content changes.

Latest live install note:

- the real game-root install was refreshed again on `2026-03-17`
- the previous `Frontier Ledger` files were moved into `E:\SteamLibrary\steamapps\common\Red Dead Redemption 2\codex_backups\FrontierLedger_clean_install_20260317_210545`
- the live `FrontierLedger.asi`, `FrontierLedger.ini`, `install.xml`, and `strings.gxt2` now hash-match the current workspace `dist` files
- the live log still prints the stale build-tag string `2026-03-16-live-growth-pass`, so version identity should be checked by hash first

## Files

- `frontier_rpg/models.py`: data models
- `frontier_rpg/content.py`: content definitions seeded from the design docs
- `frontier_rpg/systems.py`: progression, crafting, tier, and story logic
- `frontier_rpg/ui_state.py`: doc-led ledger page state for trails, map intel, homefront, farming, network, world state, ending, gear, armory, boss archive, collection, and hub progression
- `frontier_rpg/cli.py`: command-line entry points
- `tests/test_systems.py`: regression tests for the prototype rules
- `ingame_mod/`: native ScriptHookRDR2 in-game mod foundation
