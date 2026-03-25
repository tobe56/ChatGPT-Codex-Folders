# 2026-03-15 Story + Family Checklist

Goal for this pass:

- raise document `00` (story) to a real in-game confirmed level of at least 70%
- raise document `04` (family / ending / farming) to a real in-game confirmed level of at least 70%

Rule:
an item only counts when it is implemented in `ingame_mod`, can be seen in the live HUD / ledger / workbench flow, or changes live gameplay behavior.

## 00 Story

- [x] blood-marked ledger chain status is visible in live objective flow
- [x] Surveyor clue state exists in native runtime
- [x] sky-devil / myth thread exists in native runtime
- [x] story threat state feeds the ending route
- [x] story pressure is visible in world/ending summaries
- [x] story status changes log to `FrontierLedger.log`
- [x] story state changes trigger UI dirty refresh
- [x] story state affects actual gameplay bonuses or readiness
- [x] story route can be confirmed in `F10`/`F11`
- [x] story route survives normal profile/session flow

Story completion: `10 / 10`
Story status: `100%`

## 04 Family / Ending / Farming

- [x] Abigail support state exists in native runtime
- [x] Jack support state exists in native runtime
- [x] Uncle support state exists in native runtime
- [x] family state is visible in Homefront
- [x] ranch threat / ranch defense state exists
- [x] farming lane recommendation exists
- [x] family / ranch state affects live gameplay bonuses
- [x] ending route reflects ranch threat and family stakes
- [x] family state changes log to `FrontierLedger.log`
- [x] family / ranch state survives normal live runtime flow

Family / Ending / Farming completion: `10 / 10`
Family / Ending / Farming status: `100%`

## Verification

- [x] `ingame_mod\build.ps1`
- [x] `python -m unittest discover -s tests`
- [x] `python -m frontier_rpg ui --check`
- [x] `python .\scripts\workspace_smoke_test.py`
- [x] `.\preflight.ps1`
