# 2026-03-15 Workbench Checklist

This checklist tracks the three in-game workbench categories only:

- Craft
- Archive
- Result

The completion rule for this checklist is practical, not theoretical:
an item counts only when it is implemented in `ingame_mod`, can be verified in the native build, and survives the normal workspace verification pass.

## Craft

- [x] Beecher's Hope workbench prompt is rendered near the station
- [x] `E` opens the workbench
- [x] `Craft` tab is selectable
- [x] craft entry list is rendered in-game
- [x] selected craft entry updates the detail panel
- [x] ready / locked / crafted state is shown
- [x] material requirement line is shown
- [x] recipe effect line is shown
- [x] blocked recipe confirm produces result feedback
- [x] ready recipe confirm writes through `CraftedMask`
- [x] crafted unlocks feed the live progression / bonus layer
- [x] additional representative craft items exist beyond the original first pass

Craft completion: `12 / 12`
Craft status: `100%`

## Archive

- [x] `Archive` tab is selectable
- [x] archive entry list is rendered in-game
- [x] material-oriented archive entries exist
- [x] salvage archive detail exists
- [x] hide archive detail exists
- [x] tonic archive detail exists
- [x] relic archive detail exists
- [x] route / threat / homestead archive notes exist
- [x] archive detail can be pinned into the result panel
- [x] archive detail has a Korean-specific render path

Archive completion: `10 / 10`
Archive status: `100%`

## Result

- [x] `Result` tab is selectable
- [x] latest result entry exists
- [x] active bonus entry exists
- [x] workbench state entry exists
- [x] next recommendation entry exists
- [x] result content is updated by craft confirm
- [x] result content is updated by archive pin
- [x] result tab respects read-only behavior
- [x] Korean-specific result detail helper exists

Result completion: `9 / 9`
Result status: `100%`

## Verification Gate

- [x] `ingame_mod\build.ps1`
- [x] `python -m unittest discover -s tests`
- [x] `python -m frontier_rpg ui --check`
- [x] `python .\scripts\workspace_smoke_test.py`
- [x] `.\preflight.ps1`

## Remaining Polish

The checklist above means the three workbench categories are functionally past the 70% target.
What remains is presentation polish and cleanup:

- final live in-game readability signoff after one more capture pass
- any last spacing/text-density tweaks that only show up in real footage

## Category Status Snapshot

- `Craft`: functional `100%`, current Korean UI cleanup confidence `85%`
- `Archive`: functional `100%`, current Korean UI cleanup confidence `80%`
- `Result`: functional `100%`, current Korean UI cleanup confidence `85%`
- renderer / helper cleanup: `75%+`

## Implemented In The Latest Cleanup Pass

- [x] workbench overlay now uses one active locale-aware render path
- [x] workbench confirm now uses one active locale-aware action path
- [x] dead compact/render helper branches were removed from `frontier_ledger.c`
- [x] active Korean list/detail/footer strings were added to bitmap runtime coverage
- [x] build / unittest / ui-check / smoke / preflight were re-run after the cleanup

## Still Pending

- [ ] fresh live gameplay capture review after the rebuilt bitmap atlas
- [ ] last-mile Korean spacing polish based on that live review
