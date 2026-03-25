from __future__ import annotations

from collections.abc import Callable

from ..content import REGIONS
from ..data import WORLD_NODES
from ..models import ActivityDefinition, PlayerProfile, RecipeDefinition
from ..platform.localization import gameplay_text
from .combat import build_combat_activity, player_combat_profile
from .growth_effects import build_growth_effects


def total_material_count(player: PlayerProfile, material_name: str) -> int:
    return player.materials.get(material_name, 0) + player.stash_materials.get(material_name, 0)


def total_item_count(player: PlayerProfile, item_name: str) -> int:
    return player.inventory.get(item_name, 0) + player.stash_inventory.get(item_name, 0)


def effective_recipe_cost(player: PlayerProfile, recipe: RecipeDefinition) -> dict[str, int]:
    cost = dict(recipe.material_cost)
    if not cost:
        return cost

    effects = build_growth_effects(player)
    general_discount = 0
    if recipe.category == "Hub Upgrade":
        general_discount += player.lifestyle.homestead // 4
        general_discount += 1 if "frontiersman.camp_mechanic" in player.unlocked_perks else 0
    elif recipe.category in {"Weapon Mod", "Named Weapon", "Ammo Load"}:
        general_discount += player.lifestyle.gunsmithing // 4
        general_discount += 1 if "survivor.patch_job" in player.unlocked_perks else 0
    elif recipe.category == "Accessory":
        general_discount += player.lifestyle.trade // 5
        general_discount += 1 if "outlaw.greased_palm" in player.unlocked_perks else 0
    elif recipe.category == "Relic Upgrade":
        general_discount += player.lifestyle.trade // 6
        general_discount += 1 if "relic_hunter.grave_luck" in player.unlocked_perks else 0
    general_discount += int(effects["craft_cost_reduction_percent"]) // 18

    for material_name, amount in list(cost.items()):
        reduction = general_discount
        if material_name in {"treated leather", "collector bones"}:
            reduction += player.lifestyle.hunting // 5
            reduction += player.lifestyle.trapping // 5
        if material_name in {"iron scrap", "spring assemblies", "brass fittings"}:
            reduction += player.lifestyle.salvaging // 5
        if material_name in {"grave silver", "boss cores"} and "relic_hunter.grave_luck" in player.unlocked_perks:
            reduction += 1
        if reduction > 0:
            cost[material_name] = max(1, amount - reduction)
    return cost


def consume_recipe_materials(player: PlayerProfile, costs: dict[str, int]) -> None:
    for material_name, amount in costs.items():
        remaining = amount
        field_amount = player.materials.get(material_name, 0)
        if field_amount > 0:
            consumed = min(field_amount, remaining)
            player.materials[material_name] = field_amount - consumed
            remaining -= consumed
        if remaining > 0:
            stash_amount = player.stash_materials.get(material_name, 0)
            consumed = min(stash_amount, remaining)
            player.stash_materials[material_name] = stash_amount - consumed
            remaining -= consumed
        if remaining > 0:
            raise ValueError(f"Material underflow while consuming '{material_name}'")


def consume_required_items(player: PlayerProfile, required_items: dict[str, int]) -> None:
    for item_name, amount in required_items.items():
        remaining = amount
        field_amount = player.inventory.get(item_name, 0)
        if field_amount > 0:
            consumed = min(field_amount, remaining)
            player.inventory[item_name] = field_amount - consumed
            remaining -= consumed
        if remaining > 0:
            stash_amount = player.stash_inventory.get(item_name, 0)
            consumed = min(stash_amount, remaining)
            player.stash_inventory[item_name] = stash_amount - consumed
            remaining -= consumed
        if remaining > 0:
            raise ValueError(f"Item underflow while consuming '{item_name}'")


def deposit_field_supplies(player: PlayerProfile, destination: str = "Beecher's Hope Stash") -> list[str]:
    moved_materials: list[str] = []
    moved_items: list[str] = []

    for material_name, amount in list(player.materials.items()):
        if amount <= 0:
            continue
        player.stash_materials[material_name] = player.stash_materials.get(material_name, 0) + amount
        moved_materials.append(f"{material_name} x{amount}")
        del player.materials[material_name]

    for item_name, amount in list(player.inventory.items()):
        if amount <= 0:
            continue
        player.stash_inventory[item_name] = player.stash_inventory.get(item_name, 0) + amount
        moved_items.append(f"{item_name} x{amount}")
        del player.inventory[item_name]

    if moved_materials or moved_items:
        player.stash_returns += 1

    lines: list[str] = []
    if moved_materials:
        lines.append(f"Deposited materials -> {destination}: {', '.join(moved_materials)}")
    if moved_items:
        lines.append(f"Deposited picked loot -> {destination}: {', '.join(moved_items)}")
    if not lines:
        lines.append(f"No field cargo needed deposit at {destination}.")
    return lines


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
    yield_bonus = max(int(profile["loot_bonus_percent"]) // 35, int(effects["drop_quality_bonus_percent"]) // 28)

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
