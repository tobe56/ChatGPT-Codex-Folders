# 2026-03-13 In-Game Modization 50% Milestone

## Summary

This pass moved the first large chunk of the document-led Python prototype into the native `ingame_mod` track without touching the live game folder.

## What moved into the native export

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

The export pipeline also now bakes `endgame_en` and `endgame_ko` so the ASI can be validated against baseline, first-run, and post-ending states.

## Native UI change required by the migration

The original ASI tab row assumed a very small page count. After the prototype pages were migrated, the full ledger needed a real tab window instead of shrinking every tab until the page titles became unreadable.

`frontier_ledger.c` now:

- supports up to 24 pages
- renders only a sliding page window in the tab row
- shows the current page index against total pages
- keeps the existing left/right page navigation model

This is not a cosmetic-only change. It is the first time the in-game foundation can realistically host a document-shaped multi-page RPG ledger.

## Verification

The migration was validated entirely inside the workspace:

- `powershell -File E:\RDR2 RPG MODS\개발\ingame_mod\build.ps1`
- `python -m unittest discover -s tests`
- `python -m frontier_rpg overview`
- `python -m frontier_rpg endgame`

All of these passed, and the regenerated package was written to `E:\RDR2 RPG MODS\개발\ingame_mod\dist\FrontierLedger`.
