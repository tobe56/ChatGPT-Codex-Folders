---
name: frontier-ledger-doc-sync
description: Align Frontier Ledger design docs, Python prototype content, native export pages, and regression tests. Use when Codex changes progression, trails, family systems, workbench recipes, world tiers, page order, or any feature driven by `기획문서/*.md` that must propagate through `frontier_rpg` and `ingame_mod`.
---

# Frontier Ledger Doc Sync

Read `references/alignment-map.md` before editing multiple files.

## Follow The Source-Of-Truth Order

1. Start from the narrowest relevant design doc in `기획문서`.
2. Update `frontier_rpg\models.py` only if the data shape changes.
3. Update `frontier_rpg\content.py` for new definitions and content tables.
4. Update `frontier_rpg\systems.py` for progression and gate logic.
5. Update `frontier_rpg\ui_state.py` and `frontier_rpg\cli.py` if the visible ledger output changes.
6. Update `ingame_mod\scripts\export_ledger_data.py` when the native export, page order, or dataset text needs to reflect the change.
7. Update tests under `tests\`.

## Keep Context Tight

- Read only the design docs that match the requested feature area instead of loading every planning file.
- Prefer `07_master_design_consolidated.md` or `08_large_scale_implementation_blueprint.md` as consistency checks after the focused edit, not as the first file every time.

## Respect Generated Boundaries

- Do not hand-edit files under `ingame_mod\generated`.
- Regenerate native outputs through the build loop after changing export logic or page content.

## Validate The Sync

- Use narrow CLI commands from `frontier_rpg` to inspect the changed system quickly.
- Run the relevant regression tests.
- Use `$frontier-ledger-build-loop` after any change that affects native export data, generated strings, or packaged assets.
