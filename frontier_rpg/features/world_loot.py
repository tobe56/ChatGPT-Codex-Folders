from __future__ import annotations

from collections.abc import Callable

from ..content import REGIONS
from ..data import WORLD_NODES
from ..models import ActivityDefinition, PlayerProfile
from ..platform.localization import gameplay_text
from .combat import build_combat_activity, player_combat_profile
from .growth_effects import build_growth_effects
from .world_loot_inventory import (
    consume_recipe_materials,
    consume_required_items,
    deposit_field_supplies,
    effective_recipe_cost,
    total_item_count,
    total_material_count,
)


def build_world_loot_activity(
    player: PlayerProfile,
    node_id: str,
    gate_evaluator: Callable[[PlayerProfile, object], tuple[bool, list[str]]],
) -> tuple[ActivityDefinition, list[str], list[str]]:
    locale = player.preferred_locale
    node = WORLD_NODES[node_id]
    ready, missing = gate_evaluator(player, node.unlock_gate)
    if not ready:
        raise ValueError(f"World node '{node.name}' locked: {', '.join(missing)}")

    combat_activity, combat_lines = build_combat_activity(player, node.encounter_id, gate_evaluator)
    profile = player_combat_profile(player)
    effects = build_growth_effects(player)
    yield_bonus = max(profile["loot_bonus_percent"] // 35, int(effects["drop_quality_bonus_percent"]) // 28)

    materials = dict(combat_activity.materials)
    items = dict(combat_activity.items)
    for material_name, amount in node.drop_materials.items():
        materials[material_name] = materials.get(material_name, 0) + amount + yield_bonus
    for item_name, amount in node.drop_items.items():
        items[item_name] = items.get(item_name, 0) + amount
    if node.node_id not in player.cleared_world_nodes:
        for item_name, amount in node.first_clear_items.items():
            items[item_name] = items.get(item_name, 0) + amount

    activity = ActivityDefinition(
        activity_id=node.node_id,
        name=node.name,
        xp=combat_activity.xp + 120,
        materials=materials,
        items=items,
        relics=combat_activity.relics,
        boss_kills=combat_activity.boss_kills,
        home_upgrades=combat_activity.home_upgrades,
        region_progress=tuple(sorted(set(combat_activity.region_progress + (node.region,)))),
        completed_trails=combat_activity.completed_trails,
        discovered_regions=tuple(sorted(set(combat_activity.discovered_regions + (node.region,)))),
        discovered_bosses=combat_activity.discovered_bosses,
        defeated_bosses=combat_activity.defeated_bosses,
        farming_gains=combat_activity.farming_gains,
        network_gains=combat_activity.network_gains,
        affinity_changes=combat_activity.affinity_changes,
        ending_phases=combat_activity.ending_phases,
        lifestyle_gains=combat_activity.lifestyle_gains,
        notes=(
            f"{node.summary} "
            f"Route note: {node.route_note or 'Field loot now feeds stash growth.'}"
        ),
    )

    flow_lines = [
        gameplay_text(locale, f"World Node: {node.name}"),
        gameplay_text(locale, f"- Region: {REGIONS[node.region]['name']} | Category: {node.category}"),
        gameplay_text(locale, f"- Encounter won -> {node.encounter_id}"),
        gameplay_text(locale, f"- Drop table -> materials {sum(node.drop_materials.values()) + yield_bonus} | items {sum(node.drop_items.values())}"),
        gameplay_text(locale, f"- Picked and routed toward {node.stash_destination}"),
    ]
    if node.node_id not in player.cleared_world_nodes and node.first_clear_items:
        flow_lines.append(
            gameplay_text(locale, "- First clear package -> " + ", ".join(f"{name} x{amount}" for name, amount in node.first_clear_items.items()))
        )
    return activity, combat_lines, flow_lines


def build_world_loot_rows(
    player: PlayerProfile,
    gate_evaluator: Callable[[PlayerProfile, object], tuple[bool, list[str]]],
) -> list[dict[str, str]]:
    locale = player.preferred_locale
    rows: list[dict[str, str]] = []
    for node in sorted(WORLD_NODES.values(), key=lambda item: (REGIONS[item.region]["name"], item.category, item.name)):
        ready, missing = gate_evaluator(player, node.unlock_gate)
        cleared = node.node_id in player.cleared_world_nodes
        rows.append(
            {
                "node": node.name,
                "region": REGIONS[node.region]["name"],
                "category": node.category,
                "state": "Cleared" if cleared else "Ready" if ready else "Locked",
                "encounter": node.encounter_id,
                "stash": node.stash_destination,
                "detail": gameplay_text(locale, "\n".join(
                    [
                        node.summary,
                        "",
                        f"Encounter: {node.encounter_id}",
                        "Drops: "
                        + ", ".join(
                            [f"{name} x{amount}" for name, amount in node.drop_materials.items()]
                            + [f"{name} x{amount}" for name, amount in node.drop_items.items()]
                        ),
                        "First Clear: "
                        + (", ".join(f"{name} x{amount}" for name, amount in node.first_clear_items.items()) or "None"),
                        f"Stash Route: {node.stash_destination}",
                        f"Route Note: {node.route_note or 'Field loot is routed home after pickup.'}",
                    ]
                    + ([] if ready else ["Gate: " + ", ".join(missing)])
                )),
            }
        )
    return rows
