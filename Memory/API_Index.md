# Frontier Ledger API Index

## Purpose

Fast navigation for a large workspace without re-discovering the same entry points every turn.

## Workspace Topology

### Planning Docs

- `기획문서/00_epilogue_rpg_story_draft.md`
- `기획문서/01_world_ui_hub_draft.md`
- `기획문서/02_first_10_hours_content_loop.md`
- `기획문서/03_progression_systems_draft.md`
- `기획문서/04_family_integration_ending_farming_draft.md`
- `기획문서/05_weapon_modding_crafting_perks_draft.md`
- `기획문서/06_enemy_traits_world_tier_story_gating_draft.md`
- `기획문서/07_master_design_consolidated.md`
- `기획문서/08_large_scale_implementation_blueprint.md`
- `기획문서/09_affinity_relationships_draft.md`

### Prototype

- Root: `개발/frontier_rpg`
- Canonical data: `개발/frontier_rpg/content.py`
- Shared models: `개발/frontier_rpg/models.py`
- Main gameplay logic: `개발/frontier_rpg/systems.py`
- UI state rows: `개발/frontier_rpg/ui_state.py`
- CLI entry: `개발/frontier_rpg/cli.py`
- UI launcher: `개발/frontier_rpg/ui.py`

### New Split Targets

- `개발/frontier_rpg/core/`
  - low-level reusable progression helpers
- `개발/frontier_rpg/data/`
  - feature-owned authored data
- `개발/frontier_rpg/features/`
  - independent gameplay feature modules
- `개발/frontier_rpg/ui.py` and `개발/frontier_rpg/ui_state.py`
  - current UI layer
- `개발/frontier_rpg/systems.py`
  - current game aggregation layer that should shrink over time

### Native Mod

- Root: `개발/ingame_mod`
- Main runtime: `개발/ingame_mod/src/frontier_ledger.c`
- Config seam: `개발/ingame_mod/src/ledger_config.c`
- ScriptHook bridge: `개발/ingame_mod/src/script_hook_bindings.c`
- Export generator: `개발/ingame_mod/scripts/export_ledger_data.py`
- Build loop: `개발/ingame_mod/build.ps1`
- Live install: `개발/ingame_mod/install_to_game.ps1`

### Tests

- `개발/tests/test_systems.py`
- `개발/tests/test_ui_state.py`
- `개발/tests/test_ingame_export.py`
- `개발/tests/test_affinity.py`

### Live Reference

- `개발/ingame_mod/live_reference/2026-03-16_live_verified_current`

## Search Index

### Prototype logic

- XP and progression: `rg -n "xp_for_next_level|award_xp|allocate_stat_points"`
- Trail and world sync: `rg -n "build_active_trail_rows|build_world_state_rows|sync_frontier_state"`
- Family and affinity: `rg -n "build_family_rows|affinity|FAMILY_SUPPORT"`
- Crafting and upgrades: `rg -n "available_recipes|craft\\(|build_crafting_rows|build_upgrade_rows"`

### Native runtime

- Live profile persistence: `rg -n "GetPrivateProfileIntA|frontier_live_profile_save|frontier_live_profile_load"`
- Full-ledger input path: `rg -n "frontier_handle_initial_keydown|confirm_key|frontier_confirm_full_ledger_action"`
- Workbench path: `rg -n "frontier_confirm_workbench_action|frontier_workbench_"`
- Runtime summaries: `rg -n "bonus_summary|active_objective|runtime_page_summary"`

### Build and generated boundaries

- Generated-file outputs: `개발/ingame_mod/generated/*`
- Never hand-edit generated native data. Edit:
  - `개발/ingame_mod/scripts/export_ledger_data.py`
  - `개발/ingame_mod/scripts/generate_bitmap_font.py`

## Current Refactor Direction

### High-level split

1. Prototype
2. Real mod
3. Generators

### Inside each layer

1. `core`
2. `platform`
3. `game`
4. `ui`
5. `features`
6. `data`

### Rule

- New feature work should land in a dedicated `features` module first.
- `Affinity` should stay a standalone feature module and not be embedded as page-only logic.
