# 2026-03-13 In-Game Mod Live Run Push

## Why This Pass Happened

The 90% milestone left one important gap: the ASI could render a broad document-shaped ledger, but it still behaved mostly like a stateful viewer around baked datasets. The next step was to make the workspace build feel more like a real mod by letting live play-space movement create persistent frontier state.

## What Changed

- Added a second persistence file, `FrontierLedger.profile.ini`, next to `FrontierLedger.session.ini`.
- Introduced a live frontier profile inside `frontier_ledger.c` that now tracks:
  - discovered region anchors
  - cumulative field travel
  - Beecher's Hope return cycles
  - a three-phase live frontier state
- Added `live_en` and `live_ko` dataset aliases. These aliases now resolve to `baseline`, `simulated`, or `endgame` exports based on the live frontier profile.
- Reworked the compact and full ledger summaries so they now show:
  - live phase label
  - progress summary
  - current frontier objective
- Added page-level live overrides for `Overview`, `Trails`, `Frontier Map`, `Homefront`, `Farming`, `Network`, `World`, and `Ending` so the selected entry and summary text can react to the current run state without waiting for a full dataset rebuild.
- Added an isolated workspace smoke harness at `ingame_mod/scripts/workspace_smoke_test.py`.

## Why This Matters

This pass does not yet hook real save data or apply gameplay-stat changes back into the vanilla combat systems, but it does move the native ASI from "static baked ledger viewer" toward "persistent in-game run tracker." That distinction matters for the portfolio because the project now demonstrates:

- prototype-to-native migration
- state persistence beyond plain UI session restore
- runtime context derived from the real game world
- automated smoke validation for a DLL build without requiring a live install

## Verification

- `python -m unittest discover -s tests`
- `.\build.ps1`
- `.\preflight.ps1 -GameRoot "E:\SteamLibrary\steamapps\common\Red Dead Redemption 2"`
- `python .\scripts\workspace_smoke_test.py`
- smoke now validates both a baseline live run and a seeded endgame live run so the `live_en` alias is proven to resolve across multiple exported campaign states from the same DLL build.

## Cold Assessment

This push closes a real gap, but it still does not make the project a full 100% "finished gameplay overhaul." The biggest remaining gap is still direct gameplay effect application:

- real save/native progression reads
- perk and crafting effects applied to live combat systems
- NPC/world trigger chains that mutate the actual sandbox instead of only the ledger state
