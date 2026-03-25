# Frontier Ledger Development Journal

## Scope

This journal summarizes the development path for the Frontier Ledger work up to the current release-candidate stage.
All work described here was kept inside `E:\RDR2 RPG MODS\개발` unless a specific live-game install test was explicitly called out at the time.

## 2026-03-14 Late UI Follow-Up

- Replayed the latest live-game capture against the runtime log and confirmed that the Korean `F10` compact HUD was opening correctly but still looked too small and too easy to miss in motion.
- Verified that the lingering Beecher's Hope `F11` prompt flicker was no longer mainly a zone-boundary problem. The real issue was render cadence: once only the idle zone prompt remained, the script loop fell back to the slow `Wait(50)` path.
- Based on that finding, the Korean compact HUD was enlarged into a more visible field dashboard and the idle zone prompt path was promoted to the fast-tick cadence so it behaves like an active HUD element while the player stands inside the allowed zone.
- This follow-up stayed narrowly focused on:
  - Korean compact HUD visibility
  - Beecher's Hope prompt stability
  - live-game readability confidence before the next install pass

## 2026-03-12

- Consolidated the original design notes and moved the planning set under the dedicated planning folder.
- Built the first executable `frontier_rpg` prototype for:
  - progression
  - perks
  - crafting
  - workshop upgrades
  - world tier
  - frontier ledger summaries
- Added CLI and prototype UI validation.
- Started the first native ASI foundation under `ingame_mod`.

## 2026-03-13

- Migrated the early ledger pages into the native ASI pipeline.
- Expanded the native page set to include:
  - Overview
  - Active Trails
  - Frontier Map
  - Homefront
  - Gear
  - Armory
  - Ledger of the Dead
  - Collection
  - Farming
  - Network
  - World
  - Ending
- Added session persistence so the native ledger restores page and selection state.
- Added live frontier-region context from player coordinates.
- Added live profile persistence for:
  - discovered regions
  - field travel
  - hub returns
  - phase progression
- Added first live chain tracking through `TrailStage`.

## 2026-03-14

- Hardened live run tracking for:
  - enemy kills
  - loot count
  - animal kills
  - reward earned
- Added conservative gameplay bonuses tied to live progress:
  - stamina sustain
  - weapon wear reduction
- Added session/profile retry, flush, clamp recovery, and throttled logging.
- Added corrupt-profile smoke coverage so damaged runtime state can recover to a safe baseline.
- Tuned RC defaults to safer test-first values.
- Built the first release-candidate package with:
  - tested runtime files
  - backup script
  - install script
  - rollback script
  - install and rollback guide
- Added a release-candidate install-cycle test on a fake game root so backup, staged install, and rollback restore are now verified before any real game-folder deployment.
- Shifted the native fallback defaults to a Korean live-run test profile so a real deploy can run from the ASI alone at the root while still exposing the intended test flow and full-ledger access without a separate root INI.
- Performed a deeper `ingame_mod` pass without touching the live game folder:
  - wired `Stats`, `Skills`, `Perks`, `Crafting`, `Armory`, `Ledger of the Dead`, `Collection`, and `Gear` to live counters plus derived progression layers
  - promoted the runtime from a mostly page-reactive ledger into a live RPG bridge where kills, hunts, looting, returns, rewards, and region pressure now shape more of the page content
- Added a third conservative gameplay-side effect:
  - Deadeye delay bonus
  - It now sits beside stamina sustain and weapon-wear reduction
  - All three effects are tied to live bonus tier, stats, and perks and are reset on detach
- Hardened Korean rendering further for the native HUD path:
  - widened Hangul bitmap width
  - reduced per-line budget for Korean wrapping
  - increased Korean line height slightly
  - expanded bitmap glyph generation with a dedicated runtime string source so new Korean runtime labels/details are included in the build-time font atlas
- Cleaned up runtime save confidence:
  - session/profile save now touches the target files before write and flush
  - flush fallback no longer reports a false save failure when a non-empty session/profile file was already written successfully
  - workspace smoke logs now end with clean `Session saved` / `Profile saved` lines for `live_ko`
- Re-ran verification after the polish pass:
  - `ingame_mod\build.ps1`
  - `python -m unittest discover -s tests`
  - `python -m frontier_rpg ui --check`
  - `python .\scripts\workspace_smoke_test.py`
  - `python .\scripts\workspace_smoke_test.py --keep-runtime`
  - `.\preflight.ps1`
  - all passed from `E:\RDR2 RPG MODS\개발`

## Current Status

The project is now at the point where the native build is no longer only a page viewer.
It tracks a live frontier run, persists that run, surfaces the state inside the existing ledger pages, and applies small but real gameplay bonuses back into the runtime.

The remaining gap to a full overhaul is not prototype breadth anymore.
The remaining gap is deeper vanilla-runtime integration such as true event hooks, richer reward classification, and broader gameplay-side effect wiring.

## Korean Readability Follow-Up

After the first live-folder verification, the remaining visible problem was not ledger boot, input, counters, or live bonuses.
The remaining problem was Korean readability in the in-game HUD path.

This follow-up stayed inside `E:\RDR2 RPG MODS\개발` and focused only on the native `ingame_mod` renderer:

- Korean page titles were shortened specifically for the tab strip and detail header.
- Korean live summaries were rewritten to favor dense counter-driven lines instead of long prose.
- Korean entry details were shortened to quick status lines that keep the live counters visible without flooding the panel.
- The compact ledger was widened and switched to a shorter Korean objective string.
- The full ledger got wider spacing and lower visible entry counts so the list no longer tries to render more rows than the panel can safely hold.

Verification after this readability pass:

- `ingame_mod\build.ps1`
- `python -m unittest discover -s tests`
- `python -m frontier_rpg ui --check`
- `python .\scripts\workspace_smoke_test.py`
- `.\preflight.ps1`

All of the above passed again from the development workspace without touching the live game root.

## Renderer-Level Root Cause And Fix

The last visible Korean issue turned out not to be dataset selection, live counters, or LML text loading.
Logs already showed:

- `RequestedDataset=live_ko`
- `Dataset=baseline_ko`
- `Compact ledger opened`
- `Full ledger opened by keypress`

So the real problem was the final Korean renderer path.
The native HUD was still drawing Hangul through a very hard 1-bit bitmap font, which is much less forgiving than Latin text when the HUD scale is small.

This pass changed the renderer itself:

- the bitmap font generator now produces a soft fringe mask plus a solid stroke mask
- the runtime draws the soft mask first, then the solid mask
- build-time font size and thresholds were tuned upward for Hangul
- missing Korean HUD strings were added to the runtime glyph-source list so the generated atlas matches what the native HUD is actually trying to draw

That keeps the existing in-game path intact while addressing the actual visual bottleneck instead of only rearranging the same low-fidelity glyphs.

## Root Fix Follow-Up

One more pass showed the earlier renderer hardening still left two structural problems:

- bitmap rows were still too dense for the smallest Korean HUD text bands
- bitmap run rectangles were being drawn at fractional HUD coordinates, which encouraged blur/smear on thin Hangul strokes

The follow-up changed the renderer assumptions directly:

- the bitmap build now targets a smaller HUD-specific Hangul sample and emits a 12-row bitmap header
- line advance was reduced so the same HUD scale yields larger visible Korean pixels
- small Korean bitmap text now disables the soft fringe entirely and renders only the solid stroke mask
- bitmap rectangles are snapped to the HUD pixel grid before draw calls
- Korean glyph advance was widened slightly to keep neighboring syllable blocks from visually merging
- Korean compact/full ledger layouts were widened, row counts were lowered, and text scales were raised so the new sharper glyphs have room to breathe
- the packaged runtime defaults were corrected back to in-game rendering with `UseDesktopOverlay=0`

This follow-up is the first pass that changes the actual Korean bitmap rasterization strategy and the panel geometry together, rather than only tuning strings around the same renderer behavior.

## Field Prompt Follow-Up

After the Korean ledger itself became more readable, the next live issue surfaced in the field prompt layer:
the Beecher's Hope guidance prompt could appear to tremble near the usable zone.

The likely root cause was not the ledger renderer this time, but the lack of hysteresis in zone membership:

- zone state refreshed every 250 ms
- the prompt depended directly on current `inside_zone`
- tiny coordinate drift at the boundary could make the prompt pop in and out

The follow-up added:

- a short grace window after the player was last confidently inside the zone
- a slightly wider release radius for previously-inside players
- explicit zone transition logging
- a shorter Korean prompt string with a slightly larger prompt box

This pass narrows the remaining visual instability in the field layer without widening the system or adding new pages.

## Full Overlay Tab Visibility Follow-Up

Another live usability issue showed up after the Korean renderer became readable enough to inspect the rest of the overlay:
the full ledger still felt like it only exposed a small slice of the page set at once.

The problem was not missing content.
The native dataset still carried the full page set, but the Korean full overlay was packing those page tabs into a single narrow row.

The fix kept the existing page model intact and changed only the in-game presentation:

- Korean full-overlay tabs now render as a two-row strip
- the tab header background was deepened to make room for the second row
- content panels were pushed down slightly so the list/detail panels stay clean
- the sliding tab window and current-page indicator were preserved

That means the user now sees many more sections at once in the full ledger without widening the system, adding pages, or changing the existing navigation flow.

## Late-Day UI Audit Snapshot

After the latest live-game capture review on `2026-03-14`, the remaining bottleneck was no longer whether the ledger opened.
The ledger now opens, pages switch, and live data flows.
The remaining issue is whether the current Korean UI is readable and self-explanatory enough to expose what is already implemented.

### Recorded Late-Day Time Window

- `17:19 ~ 17:30 KST`
  - clean reinstall preparation
  - live Korean visibility re-check
  - prompt readability review from the latest test capture
- `17:30 ~ 17:34 KST`
  - Beecher's Hope prompt jitter follow-up
  - zone hysteresis and prompt-copy review
- `17:34 ~ 17:35 KST`
  - full-overlay tab-density follow-up
  - implemented-vs-pending UI audit capture

### What Is Already Implemented In The Native UI

- compact ledger open/close
- full ledger open/close
- page navigation
- entry navigation
- live counter-driven summaries
- session/profile restore
- live summaries inside `Overview`, `Farming`, `Network`, and `World`
- native RPG pages for `Stats`, `Skills`, `Perks`, `Crafting`, `Armory`, `Ledger of the Dead`, `Collection`, and `Gear`
- entry-specific detail text for `Stats`, `Skills`, and `Perks`

### What Exists But Is Still Being Hidden By Presentation Quality

- The stats page is not empty. It already has selectable entries and per-entry detail text.
- The skills/perks/crafting family is not missing. Their page bodies and details already exist in the native ledger.
- The full page set is not limited to only a few tabs. Korean tab density is still the reason it can feel visually smaller than it is.

### What Is Still Missing On The Interaction Side

- direct stat allocation
- direct perk confirmation/unlock
- direct crafting execution
- a general accept/confirm interaction layer in the native ledger

### Immediate UI Priority Order

1. stabilize Korean readability
2. stabilize the field prompt visually
3. make list/detail hierarchy obvious in the full ledger
4. only after that, add direct interaction flows

## Readability And Detail-Panel Follow-Up

The next pass stayed focused on presentation instead of interaction.
Before starting, the live Frontier Ledger install was removed from the real game root again so all changes could be developed and verified only from `E:\RDR2 RPG MODS\개발`.

This pass targeted two visible issues at once:

- Korean readability still felt softer than the surrounding game HUD
- the full ledger did not make the existing list/detail structure obvious enough

The changes were intentionally narrow:

- the Korean full overlay was widened
- the left entry list got more room and taller rows
- the right detail panel got more room and stronger structure labels
- Korean tabs were widened by dropping the two-row strip to four tabs per row
- Korean visible entry capacity was reduced so the list does not overfill the enlarged row geometry
- selected rows gained a stronger accent strip
- Korean detail text scale was raised
- Hangul spacing at draw time was widened slightly
- the bitmap font build moved to a larger source size with a lower solid threshold

This pass matters because the stats/skills/perks family was already implemented in the native ledger.
The main remaining problem was that the current presentation made it too easy to mistake “hard to read” for “not implemented”.

Verification after the pass:

- `ingame_mod\build.ps1`
- `python -m unittest discover -s tests`
- `python -m frontier_rpg ui --check`
- `python .\scripts\workspace_smoke_test.py`
- `.\preflight.ps1`

All passed again from the development workspace.

## Korean Renderer Replacement Direction

The next investigation finally clarified why repeated readability passes were not solving the live problem:
the Korean UI was not failing because of missing data or missing page wiring.
It was failing because the Hangul-heavy full ledger was still trying to behave like a page browser on top of a custom bitmap renderer.

What the live captures showed:

- panel rectangles still rendered
- input and page state still worked
- logs still showed stable `F10` / `F11` behavior
- but the Korean text layer could visually collapse under full-overlay density

So this pass changed direction.
Instead of trying to preserve the original Korean page-browser presentation, the native runtime now switches to a simpler Korean dashboard model:

- short Korean card titles
- ASCII-heavy metric/value lines
- far fewer wrapped Hangul paragraphs
- no dependency on the old Korean full-page tab/detail density for basic readability

Two structural changes matter here:

1. Korean runtime text now uses a split render path.
   - short Hangul headings still use the bitmap path
   - ASCII-safe value lines now use the stable native text path even in the Korean dataset

2. Korean `F10` and `F11` no longer try to present the same dense page-browser UI as the English runtime.
   - `F10` now renders a compact Korean dashboard
   - `F11` now renders a larger Korean dashboard built from short cards such as `개요`, `진행`, `전투`, `사냥`, `능력`, `기술`, `제작`, `보너스`, and `현재`

This keeps the actual live systems intact while swapping out the Korean presentation layer for something that has a much better chance of surviving real in-game rendering.

Verification after the renderer-direction swap:

- `ingame_mod\build.ps1`
- `python -m unittest discover -s tests`
- `python -m frontier_rpg ui --check`
- `python .\scripts\workspace_smoke_test.py`
- `.\preflight.ps1`

All passed from the development workspace without touching the live game root.

## Current UI Status Memo (`2026-03-14 late`)

By the end of the late `2026-03-14` pass, the Korean UI problem had changed categories.
It was no longer accurate to describe the state as “the HUD is not built” or “the UI fails to appear.”

The current honest status is:

- `F10` now appears in live footage as a compact Korean field HUD.
- `F11` now appears in live footage as a larger Korean dashboard.
- Korean text visibility has crossed the minimum viability threshold.
- The main remaining issue is polish and readability, not total absence.

What is implemented beneath that visible layer:

- live counters
- trail stage / live phase progression
- saved session/profile state
- bonuses
- stats / skills / perks / crafting / armory / dead / collection runtime wiring

What is still incomplete at the presentation level:

- Korean font and box sizing still need another polish pass
- `F10` is still a reduced compact summary, not the full original field payload
- `F11` in Korean is still a dashboard fallback, not yet the final Korean form of the original tab/list/detail browsing concept
- interaction-heavy actions should remain deferred until the Korean presentation layer is stable enough

This distinction matters for later work.
The project should no longer spend time rediscovering whether the Korean HUD can appear at all.
That question is now answered.
The next UI work should focus on:

1. final readability polish
2. compact `F10` payload improvement
3. only then, interaction work

The late-night mixed-stack crash investigation also clarified a separate rule:
crash isolation and UI polish should be tracked separately.
A temporary `FrontierLedger-only` isolation attempt changed the whole live mod stack and caused a separate load-stage crash, so that experiment was rolled back.
Future crash diagnosis should avoid mixing “UI polish” and “whole-stack surgery” into the same step.
