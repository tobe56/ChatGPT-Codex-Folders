# 2026-03-15 Story + Family Push

## Purpose

Raise document `00` and document `04` by adding native in-game runtime state instead of only prototype/document alignment.

## What Was Added

- Surveyor clue tracking
- sky-devil myth stage
- Abigail support rank
- Jack support rank
- Uncle support rank
- ranch threat level
- ranch defense level
- farming lane recommendation

## Where It Now Shows Up

- live objective flow
- `Homefront`
- `Farming`
- `World`
- `Ending`
- live bonus layer
- live status/log output

## Why This Counts As Real In-Game Progress

These additions are not just static text.
They are derived from the live native run state and feed back into:

- actual page summaries/details
- actual status messages
- actual UI refresh conditions
- actual gameplay bonuses

## Verification

- `ingame_mod\build.ps1`
- `python -m unittest discover -s tests`
- `python -m frontier_rpg ui --check`
- `python .\scripts\workspace_smoke_test.py`
- `.\preflight.ps1`

All passed from the development workspace.
