# Alignment Map

## Planning Docs

- `00_epilogue_rpg_story_draft.md`: story framing, opening raid, late-game mastermind.
- `01_world_ui_hub_draft.md`: ledger surfaces, Beecher's Hope hub, UI priorities.
- `02_first_10_hours_content_loop.md`: early-game activity flow and simulation beats.
- `03_progression_systems_draft.md`: levels, stats, perks, gates, world tiers.
- `04_family_integration_ending_farming_draft.md`: family support, homefront, ending, farming lanes.
- `05_weapon_modding_crafting_perks_draft.md`: recipes, workbench, armory, perk/crafting relationships.
- `06_enemy_traits_world_tier_story_gating_draft.md`: enemy traits, archetypes, world-tier escalation.
- `07_master_design_consolidated.md`: cross-check for consistency after a focused change.
- `08_large_scale_implementation_blueprint.md`: higher-level implementation blueprint and sequencing.

## Code Map

- `frontier_rpg\models.py`: shared schemas and typed data structures.
- `frontier_rpg\content.py`: canonical authored content tables.
- `frontier_rpg\systems.py`: simulation, gates, progression, and unlock logic.
- `frontier_rpg\ui_state.py`: row builders for compact/full ledger surfaces.
- `frontier_rpg\cli.py`: fast inspection entry points.
- `ingame_mod\scripts\export_ledger_data.py`: bridge from Python state to native datasets/pages.

## Tests

- `tests\test_systems.py`: progression and simulation rules.
- `tests\test_ui_state.py`: compact/full row coverage.
- `tests\test_ingame_export.py`: native page order, dataset coverage, and packaged INI defaults.

## Common Sync Risks

- Adding content in `content.py` without updating the row builders that expose it.
- Changing page order or page ids without updating export tests.
- Editing generated native headers instead of the export script.
