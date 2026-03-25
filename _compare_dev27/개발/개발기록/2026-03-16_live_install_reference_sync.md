# 2026-03-16 Live Install Reference Sync

## Purpose

Pull the exact currently installed Frontier Ledger package out of the real game folder and preserve it inside the development workspace before more content or UI work shifts the baseline again.

## Result

The active live Frontier Ledger package now has a dedicated workspace snapshot:

- `개발/ingame_mod/live_reference/2026-03-16_live_verified_current`

This snapshot preserves:

- the installed ASI
- the installed INI
- the installed LML text pack
- the latest live log
- the latest live session/profile state
- the live removal manifest and removal script

## Verification

- live `FrontierLedger.asi` SHA256 matches workspace `dist/FrontierLedger/FrontierLedger.asi`
- live `strings.gxt2` SHA256 matches workspace `dist/FrontierLedger/lml/Frontier Ledger Korean Text/strings.gxt2`

This confirms that the currently readable live install is not a hidden one-off build.
It is the same package already generated inside the development workspace, now preserved with the real runtime state that was active in the game folder.

## Current State Sync

The planning docs had drifted behind the implementation and were still saying the project was concept-only.
That has now been corrected in:

- `기획문서/07_master_design_consolidated.md`
- `기획문서/08_large_scale_implementation_blueprint.md`

## Recommended Next Content Direction

The next best use of time is not more page-surface work.
The project already has enough ledger, profile, and workbench shell to carry a real vertical slice.

Recommended next build target:

1. turn the existing Blackwater pressure and Blood-Marked Ledger path into a real authored content chain
2. connect that chain to contract-board availability, field triggers, and a first elite/boss escalation
3. land the first true reward loop that feeds materials, boss-state, and workbench unlocks back into the existing UI

In short: move from display-first systems into the first playable frontier story-and-combat slice.
