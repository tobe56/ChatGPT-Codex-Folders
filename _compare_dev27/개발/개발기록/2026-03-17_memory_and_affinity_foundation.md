# 2026-03-17 Memory And Affinity Foundation

## Purpose

Prepare the workspace for faster follow-up implementation by reducing rediscovery cost and starting the affinity system through the target architecture instead of the old monolithic path.

## Landed Today

- `Memory/TroubleShootings.md`
- `Memory/API_Index.md`
- `Memory/Frontier_Ledger_Domain_Knowledge.md`
- `Memory/Architecture_Domain_Knowledge.md`
- `.codex/skills/frontier-ledger-current-request-focus/SKILL.md`
- `기획문서/09_affinity_relationships_draft.md`
- first prototype split folders under `frontier_rpg`
- standalone affinity feature/data modules

## Why This Matters

The project is already beyond the point where raw recall and ad hoc search are efficient.
Memory docs and a direct-scope skill reduce wasted turns.
The affinity feature is also the first deliberate example of the new rule:

- feature logic in `features`
- authored definitions in `data`
- reusable helpers in `core`
- old aggregation files shrink over time instead of growing forever

## Recommended Follow-Up

1. continue extracting new work into the target structure
2. avoid broad native monolith rewrites unless absolutely necessary
3. wire affinity rewards into real perk, item, and quest outputs next
