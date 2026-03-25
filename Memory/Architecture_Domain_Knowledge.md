# Frontier Ledger Architecture Domain Knowledge

## Current Pain Point

The workspace already proves many systems, but several files still act like aggregation bottlenecks:

- `개발/frontier_rpg/content.py`
- `개발/frontier_rpg/systems.py`
- `개발/ingame_mod/src/frontier_ledger.c`

Large edits inside those files slow down search, increase regression risk, and make focused feature work harder.

## Target Architecture

### Layer 1

- Prototype
- Real mod
- Generators

### Layer 2 inside each layer

- `core`
- `platform`
- `game`
- `ui`
- `features`
- `data`

## Practical Rule For This Repo

- Do not try to split the entire monolith in one pass.
- Every new feature should enter through the target structure first.
- Then existing monolith code should shrink by extraction, not by giant move-all refactors.

## First Extraction Targets

### Prototype

- progression helpers -> `frontier_rpg/core`
- affinity -> `frontier_rpg/features`
- affinity authored data -> `frontier_rpg/data`

### Native mod

- leave `frontier_ledger.c` stable unless a small, testable extraction is clearly worth it
- prefer new feature seams, headers, and helper modules over broad string-heavy rewrites

## Anti-Pattern

- embedding feature rules directly into UI/page text code

## Preferred Pattern

- feature logic in `features/*`
- authored rewards and thresholds in `data/*`
- page rendering only reads feature state
