# 2026-03-13 In-Game Modization 70% Push

## Summary

This pass extended the native ASI export beyond the first 50% migration milestone. The goal was to move the remaining high-value document systems that were still trapped in the Python prototype into the workspace-native `ingame_mod` path.

## Newly migrated native pages

- `Farming`
- `Network`
- `World`
- `Ending`

These now sit alongside the already migrated pages:

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
- `Encyclopedia`

That brings the full native ledger export to `18` pages.

## Why this matters

Before this pass, the ASI could show combat growth, hub state, boss archive, and collection layers, but the broader campaign loop still depended on Python-only validation for:

- repeatable farming routes
- subsystem integration progress
- world-state escalation review
- ending and post-ending route state

Now those systems are also present in the native export, which means the in-game mod foundation can represent nearly the full document-shaped campaign structure even before live save hooks are added.

## Additional hardening

- Added `tests/test_ingame_export.py`
- Locked the native page order so export regressions are caught automatically
- Verified that baseline, simulated, and endgame datasets all populate the new pages

## Verification

- `python -m unittest discover -s tests`
- `powershell -File E:\RDR2 RPG MODS\개발\ingame_mod\build.ps1`
- `python -m frontier_rpg endgame`
- generated header inspection for `farming`, `network`, `world`, `ending`
- `powershell -File E:\RDR2 RPG MODS\개발\ingame_mod\preflight.ps1 -GameRoot "E:\SteamLibrary\steamapps\common\Red Dead Redemption 2"`

All passed without copying any new files into the live game folder.
