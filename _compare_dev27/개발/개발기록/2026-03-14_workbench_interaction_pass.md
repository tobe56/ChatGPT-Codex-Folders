# 2026-03-14 Workbench Interaction Pass

This pass stayed inside `E:\RDR2 RPG MODS\개발` and did not touch the live game root.

## Goal

Turn the previously read-only crafting/status layer inside `ingame_mod` into a real native interaction slice without adding new ledger pages or expanding the Python prototype.

## What Changed

- Added a Beecher's Hope workbench interaction zone using the existing live zone/context runtime.
- Added a confirm key path (`E`) in `ingame_mod` so the workbench can be opened and used in-game.
- Added a dedicated workbench modal with three tabs:
  - `Craft`
  - `Archive`
  - `Result`
- Kept `F10` and `F11` intact; the workbench is a separate modal layer instead of another ledger page.
- Added session persistence for:
  - active workbench tab
  - craft-tab selection
  - archive-tab selection
  - result-tab selection
- Added profile persistence for crafted unlocks through `CraftedMask`.

## Why This Matters

Before this pass, crafting inside the native runtime was only a visible status layer.
The ledger could show crafting-related data, but the player could not actually commit a recipe from the in-game mod.

After this pass:

- ready recipes can be confirmed from the workbench
- confirmed recipes persist across loads
- crafted unlocks immediately flow back into the existing live progression and bonus layer
- the same modal also carries compact encyclopedia-style notes and result reporting

## Interaction Model

- field prompt near the workbench: `Workbench [E] craft | Ledger [F11]`
- `E`: open or confirm inside the workbench
- `Left / Right`: switch tabs
- `Up / Down`: move entries
- `Esc`: close

## Runtime Effect

Crafting now changes real native run state through `CraftedMask`.
That persistent state is re-applied by the live progression refresh so crafted unlocks can affect:

- workshop / armory depth
- stat growth edges
- skill growth edges
- run-bonus summaries
- live gameplay bonuses already attached to the runtime

## Verification

- `ingame_mod\build.ps1`
- `python -m unittest discover -s tests`
- `python -m frontier_rpg ui --check`
- `python .\scripts\workspace_smoke_test.py`
- `.\preflight.ps1`

All passed from the development workspace.

## Workbench Polish Follow-Up

After the first interaction slice was proven to work, the next pass focused on making the workbench readable and useful instead of leaving it as a thin overlay.

This follow-up stayed in `E:\RDR2 RPG MODS\개발` and kept the scope tightly inside `ingame_mod`.

### What Changed In The Polish Pass

- turned the workbench modal into a darker board-style UI with a stronger full-screen scrim
- limited the visible list rows so the craft list can grow without spilling past the panel
- strengthened the separation between:
  - left recipe/archive/result list
  - right detail panel
  - lower run summary strip
- added three more representative craft entries:
  - `Frontier Tonic Pack`
  - `Gunsmith Tool Roll`
  - `Deadeye Charm`
- expanded the archive tab so it now carries material encyclopedia notes for:
  - salvage
  - hide bundles
  - tonic stock
  - relic fragments
- rewrote the current-detail and result-detail text so the modal shows:
  - recipe requirements
  - recipe effect
  - state / gate
  - clearer result copy after confirm or failure

### Why This Pass Matters

The first interaction slice proved that `E`, `CraftedMask`, and the workbench tabs all worked.
But it was still too easy for the modal to feel like a transparent overlay instead of a usable station.

This polish pass pushed it closer to an actual workbench flow:

- more readable panel hierarchy
- more concrete item identity
- stronger material explanation
- better Korean result copy

### Verification After The Polish Pass

- `ingame_mod\build.ps1`
- `python -m unittest discover -s tests`
- `python -m frontier_rpg ui --check`
- `python .\scripts\workspace_smoke_test.py`
- `.\preflight.ps1`

All passed again from the development workspace.

## Korean Workbench Renderer Follow-Up

The next failure case clarified that the workbench header and tabs were visible, but the body still was not readable enough in live Korean footage.
That meant the problem was no longer just panel opacity.
The remaining bottleneck was the body renderer:

- too much long Korean detail text
- too much dependence on wrapped Hangul drawing inside the modal body
- not enough contrast between the board and its inner sections

So the workbench renderer was tightened again with a Korean-only body path:

- short Korean headings
- ASCII material/state/effect lines
- token-line rendering instead of long wrapped paragraphs
- a stronger opaque board with a full-screen scrim
- a bounded visible-entry list instead of trying to show every item at once

This matters because it changes the workbench from:

- “the header/tabs are there, but the body is hard to read”

to:

- “the player can read the selected recipe, its material requirement, its state, and its outcome without relying on dense wrapped Hangul”

### Additional Craft Coverage Added In The Same Pass

Three more representative craft items were wired into the native workbench:

- `Frontier Tonic Pack`
- `Gunsmith Tool Roll`
- `Deadeye Charm`

And the archive tab was expanded to carry specific material encyclopedia notes for:

- salvage
- hide bundles
- tonic stock
- relic fragments

### Verification After The Korean Renderer Follow-Up

- `ingame_mod\build.ps1`
- `python -m unittest discover -s tests`
- `python -m frontier_rpg ui --check`
- `python .\scripts\workspace_smoke_test.py`
- `.\preflight.ps1`

All passed again from the development workspace.

## Live Visibility Follow-Up

The next live capture showed that the workbench still had one major usability problem:
the player could see the header and tab strip, but the body still behaved like only the first item was readable.

That pointed to the remaining bottleneck:

- list rows still depended too much on Hangul-heavy labels
- the footer/help copy still used weak low-contrast text
- the workbench body needed a more explicit “operator panel” presentation

So the Korean workbench pass was tightened again:

- row labels were shortened into ASCII list labels
- body token lines were enlarged
- selected-row/status text contrast was raised
- the modal now presents itself as a simple `LIST / DETAIL / RUN` board

This follow-up is less about adding new systems and more about making the already-built workbench actually usable in live play footage.
