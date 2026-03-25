from __future__ import annotations

from ..content import PRIMARY_HUB, PROJECT_NAME, WORLD_TIERS
from ..core.progression import xp_progress
from ..features.affinity import build_affinity_rows
from ..features.growth_effects import build_growth_effects
from ..models import PlayerProfile
from ..systems import (
    available_perks,
    available_recipes,
    build_active_trail_rows,
    build_farming_rows,
    build_frontier_map_rows,
    can_promote_world_tier,
    chapter_status,
    next_world_tier,
    sync_frontier_state,
)


def build_header_state(player: PlayerProfile) -> dict[str, str]:
    sync_frontier_state(player)
    xp_current, xp_needed = xp_progress(player)
    world = WORLD_TIERS[player.world_tier]
    return {
        "project": PROJECT_NAME,
        "hub": PRIMARY_HUB,
        "level": f"Level {player.level}",
        "xp": f"XP {xp_current}/{xp_needed}",
        "world_tier": f"World Tier {player.world_tier}: {world.name}",
        "home": f"Workbench {player.workbench_tier} | Home Upgrades {player.home_upgrades}",
    }

def build_field_overview_lines(player: PlayerProfile) -> list[str]:
    sync_frontier_state(player)
    effects = build_growth_effects(player)
    candidate = next_world_tier(player)
    active_rows = [row for row in build_active_trail_rows(player) if row["state"] in {"Active", "Repeatable"}]
    region_rows = [row for row in build_frontier_map_rows(player) if row["state"] in {"Known", "Stabilized", "Rumored"}]
    farming_rows = [row for row in build_farming_rows(player) if row["state"] in {"Active", "Mastered"}]
    affinity_rows = build_affinity_rows(player)
    lines = [
        f"Project: {PROJECT_NAME}",
        f"Current hub anchor: {PRIMARY_HUB}",
        "F10 opens the Compact Frontier Ledger anywhere in the field.",
        "Beecher's Hope opens the full management ledger.",
        "",
        f"Level {player.level} | World Tier {player.world_tier}",
        f"Relics {player.relic_count} | Boss Kills {player.boss_kills}",
        f"Blueprints {len(player.known_blueprints)} | Stash returns {player.stash_returns} | Field mats {sum(player.materials.values())} | Stash mats {sum(player.stash_materials.values())}",
        f"Live effect shell: DMG +{effects['combat_damage_bonus_percent']}% | LOOT +{effects['loot_yield_bonus_percent']}% | CRAFT -{effects['craft_cost_reduction_percent']}%",
    ]
    if candidate is not None:
        ready, missing = can_promote_world_tier(player, candidate)
        if ready:
            lines.append(f"Next World Tier ready: {WORLD_TIERS[candidate].name}")
        else:
            lines.append(f"Next World Tier blocked by: {', '.join(missing)}")
    lines.extend(["", "Active Trails"])
    lines.extend(f"- {row['trail']} ({row['category']})" for row in active_rows[:5])
    lines.extend(["", "Frontier Map"])
    lines.extend(f"- {row['region']}: {row['danger']}" for row in region_rows[:4])
    lines.extend(["", "Farming Lanes"])
    lines.extend(f"- {row['lane']}: {row['focus']}" for row in farming_rows[:3])
    lines.extend(["", "Affinity Routes"])
    lines.extend(f"- {row['npc']}: {row['quest_state']} | {row['interaction']}" for row in affinity_rows[:3])
    return lines

def build_compact_sections(player: PlayerProfile) -> dict[str, list[str]]:
    sync_frontier_state(player)
    effects = build_growth_effects(player)
    ready_perk_names = [perk.name for perk in available_perks(player)[:4]]
    ready_recipes = [recipe.name for recipe, missing in available_recipes(player) if not missing][:4]
    chapter_lines = []
    for chapter_name, state, missing in chapter_status(player)[:3]:
        if missing:
            chapter_lines.append(f"{chapter_name}: {state} ({', '.join(missing[:2])})")
        else:
            chapter_lines.append(f"{chapter_name}: {state}")
    trail_rows = [row for row in build_active_trail_rows(player) if row["state"] in {"Active", "Repeatable"}]
    intel_rows = [row for row in build_frontier_map_rows(player) if row["state"] in {"Known", "Stabilized", "Rumored"}]
    farming_rows = [row for row in build_farming_rows(player) if row["state"] in {"Active", "Mastered"}]
    affinity_rows = build_affinity_rows(player)
    return {
        "Frontier Status": [
            build_header_state(player)["level"],
            build_header_state(player)["xp"],
            build_header_state(player)["world_tier"],
            build_header_state(player)["home"],
            f"DMG +{effects['combat_damage_bonus_percent']} | LOOT +{effects['loot_yield_bonus_percent']} | CRAFT -{effects['craft_cost_reduction_percent']}",
        ],
        "Quick Intel": [f"{row['trail']} [{row['region']}]" for row in trail_rows[:2]]
        + [f"{row['region']}: {row['danger']}" for row in intel_rows[:1]]
        + [f"Farm: {row['lane']}" for row in farming_rows[:1]]
        + [f"Affinity: {affinity_rows[0]['npc']} {affinity_rows[0]['quest_state']}" if affinity_rows else "Affinity: none"],
        "Ready Now": [
            "Perks: " + (", ".join(ready_perk_names) if ready_perk_names else "None"),
            "Crafting: " + (", ".join(ready_recipes) if ready_recipes else "None"),
            "Blueprints: " + (", ".join(sorted(player.known_blueprints)[:2]) if player.known_blueprints else "None"),
            f"Stash: {sum(player.stash_materials.values())} mats | {sum(player.stash_inventory.values())} items",
        ]
        + chapter_lines,
    }


