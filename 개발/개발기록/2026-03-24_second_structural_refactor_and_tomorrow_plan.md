# 2026-03-24 Second Structural Refactor And Tomorrow Plan

## End-Of-Day State

Today did not turn into a feature day.
It stayed aligned with the user's request: no new systems, no content expansion, no behavior rewrite.

The actual outcome is that the project is now better built than `개발 (27).zip` in a structural sense, not because it suddenly does more gameplay work, but because the same validated behavior now sits on top of a cleaner and more modular code layout.

The strongest summary from today is:

- `개발 (27).zip` was already functionally healthy
- the current workspace now preserves that health with lower navigation cost and lower edit blast radius
- the biggest remaining risk is no longer "does the workspace basically work?"
- the biggest remaining risk is "which still-large structural hotspots should be split next before they slow future work down again?"

## What Was Confirmed

### 1. This pass did not regress the workspace behavior

The current workspace passed:

- unit tests
- UI check
- simulation
- native build
- workspace smoke
- preflight

That matters because today's work touched both the Python export path and the native include tree.

### 2. `개발 (27).zip` and the current workspace are not separated by basic stability

The comparison run against `개발 (27).zip` also passed the same workspace loop:

- unit tests
- UI check
- simulation
- native build
- workspace smoke
- preflight

So today's value is not "the old zip was broken and now it works."
Today's value is "the old zip worked, and now the same proof sits on a cleaner structure."

### 3. The biggest gains were structural and future-facing

Compared with `개발 (27).zip`, the current workspace now has:

- thin facade files where the biggest `.inl` and export entrypoints used to be giant implementation files
- dedicated sub-includes for live profile, persistence, and workbench responsibilities
- dedicated helper modules for export localization, page building, render assembly, and typed export data
- dedicated helper modules for reward packages, combat math, combat projection, and world-loot inventory handling
- more regression coverage than the comparison zip

## What Changed Under The Hood

### Native

- `frontier_ledger_live_profile.inl` became a facade over progression/string/crafting/rows/context slices
- `frontier_ledger_persistence.inl` became a facade over io/session/profile/event slices
- `frontier_ledger_workbench.inl` became a facade over prompt-zone/catalog/actions slices

### Export

- `export_ledger_data.py` became a facade
- localization, page-row building, render assembly, and export typing are now separated

### Feature Layer

- farming and network milestone reward logic now share one package-helper path
- combat math and combat projection are separated from row/activity presentation
- world-loot inventory/cost/deposit helpers are separated from activity/row builders

### Regression Guardrails

- export rendering got an additional regression check
- farming/network milestone idempotence now has direct test coverage
- the scoped `ruff` and `mypy` surface is wider than it was in the comparison zip

## Why Tomorrow Should Start From Here

Tomorrow should not restart from "is the workspace stable enough?"
That question is answered.

Tomorrow should start from:

1. the workspace is stable
2. the structure is better than `개발 (27).zip`
3. the remaining problems are concentrated in a few still-large files
4. the next best move is another no-feature structural pass on those hotspots

## Tomorrow Only Needs A Few Focused Tasks

1. split `ingame_mod/src/frontier_ledger_live_profile_context.inl` again so page summary, entry detail, and context focus stop sharing one oversized file
2. remove the remaining duplicated or dead workbench detail paths that survived this pass
3. extend type/test coverage around export-native summary builders before another native cleanup round

## Practical Rule

Tomorrow should keep the same discipline as today:

- no new feature work unless it is required by a cleanup
- preserve behavior first
- split by responsibility, not by arbitrary file count
- validate once narrowly and once broadly after each coherent cleanup block
