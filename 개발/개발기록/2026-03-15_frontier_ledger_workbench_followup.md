# 2026-03-15 Frontier Ledger Workbench Follow-Up

## Purpose

This follow-up records the late-night push after the first workbench interaction slice had already landed.
The focus moved from:

- “can the workbench open and change state?”

to:

- “can the player actually read and understand the workbench in Korean during live gameplay?”

## Main Areas Touched

- native workbench interaction flow
- crafted unlock persistence
- Korean workbench rendering
- material encyclopedia content inside the workbench
- live install / backup / restore cycles for verification

## Real Progress Made

### 1. Workbench Is No Longer Just A Read-Only Concept

The workbench is now a real native interaction layer:

- prompt near the station
- `E` interaction
- tab switching
- result logging
- persistent crafted unlock state

### 2. Crafting Has Concrete Content

The workbench no longer stops at the original handful of craft concepts.
Additional representative craft entries were added so the station feels more like a system and less like a stub:

- `Frontier Tonic Pack`
- `Gunsmith Tool Roll`
- `Deadeye Charm`

### 3. Archive Is Starting To Behave Like A Material Encyclopedia

The archive layer now covers named material groups instead of only abstract workbench status:

- salvage
- hide bundles
- tonic stock
- relic fragments

This is still not the full final “every material item” encyclopedia, but it is the first usable native material reference layer.

## Main Remaining Problem

The blocker is now clearly visual, not systemic.

The workbench body can still feel partially missing or partially degraded in Korean because:

- the renderer path has been iterated several times and still contains overlap / legacy behavior
- some newly introduced Korean labels are not yet guaranteed to be covered in the most reliable render path
- body text quality still depends too much on a fragile mixed Hangul strategy

That means tomorrow should not widen systems again.
Tomorrow should finish the renderer cleanup.

## Recommended Next Step

The best next move is:

1. collapse the workbench Korean render path into one clean implementation
2. route every visible workbench label through that single implementation
3. re-generate only the string coverage needed by the final workbench UI
4. verify with one clean live install and one clean live capture

## Current Assessment

By the end of this follow-up:

- workbench interaction is real
- crafting persistence is real
- material reference is partially real
- Korean workbench readability is much closer
- but the final Korean workbench UI still needs one more cleanup pass before it can be called fully settled

## Refactor Risk Notes

The broader audit today confirmed that future breakage is unlikely to come from missing systems first.
It is more likely to come from the current shape of the native workbench/Korean presentation code.

The highest-risk spots are:

1. `frontier_render_workbench_overlay`
   - multiple Korean-oriented render paths have existed here
   - even when one path is effectively dead, it still increases maintenance risk

2. workbench helper duplication
   - `archive`
   - `result`
   - `current detail`
   - `compact`
   - `korean`

   These families overlap enough that one UI fix can easily miss another path.

3. `frontier_confirm_workbench_action`
   - the function still carries signs of layered iteration
   - it should be collapsed to one authoritative path per locale / mode

4. string path fragmentation
   - some strings still come from `frontier_text`
   - some are literal
   - some are compact helper outputs
   - some are older result/body text

This fragmentation explains why a visible UI issue can appear “fixed” in one live capture but still reappear in another path.

Today’s refactor pass did not fully solve every one of these points, but it confirmed where the next cleanup must happen and removed one extra duplicated Korean render branch so the intended path is at least clearer than before.
