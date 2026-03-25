from __future__ import annotations

from typing import cast

from ..content import LEVEL_CAP, PERKS, RECIPES, WORLD_TIERS
from ..core.progression import total_xp_required_for_level
from ..features.world_loot import (
    consume_recipe_materials,
    consume_required_items,
    effective_recipe_cost,
    total_item_count,
    total_material_count,
)
from ..features.world_state import claim_available_world_tier_rewards
from ..models import PerkDefinition, PlayerProfile, RecipeDefinition
from .state import has_blueprint, meets_gate, sync_frontier_state


def award_xp(player: PlayerProfile, amount: int) -> list[int]:
    player.total_xp += amount
    level_ups: list[int] = []
    while player.level < LEVEL_CAP:
        next_level = player.level + 1
        if player.total_xp < total_xp_required_for_level(next_level):
            break
        player.level = next_level
        player.unspent_stat_points += 2
        if player.level % 2 == 0:
            player.unspent_perk_points += 1
        level_ups.append(player.level)
    return level_ups


def allocate_stat_points(player: PlayerProfile, priorities: list[str]) -> int:
    spent = 0
    while player.unspent_stat_points > 0 and priorities:
        target = priorities[spent % len(priorities)]
        player.stats.add_points({target: 1})
        player.unspent_stat_points -= 1
        spent += 1
    return spent


def available_perks(player: PlayerProfile) -> list[PerkDefinition]:
    perks: list[PerkDefinition] = []
    for perk in PERKS.values():
        if perk.perk_id in player.unlocked_perks:
            continue
        ready, _ = meets_gate(player, perk.requirement)
        if ready:
            perks.append(perk)
    return sorted(perks, key=lambda perk: (perk.tree, perk.name))


def unlock_perk(player: PlayerProfile, perk_id: str) -> None:
    if perk_id in player.unlocked_perks:
        raise ValueError(f"Perk '{perk_id}' already unlocked")
    if player.unspent_perk_points <= 0:
        raise ValueError("No perk points available")
    perk = PERKS[perk_id]
    ready, missing = meets_gate(player, perk.requirement)
    if not ready:
        raise ValueError(f"Perk '{perk.name}' locked: {', '.join(missing)}")
    player.unlocked_perks.add(perk_id)
    player.unspent_perk_points -= 1


def recipe_secured(player: PlayerProfile, recipe: RecipeDefinition) -> bool:
    return recipe.recipe_id in player.crafted_recipes


def recipe_status(player: PlayerProfile, recipe: RecipeDefinition) -> tuple[bool, list[str]]:
    missing: list[str] = []
    if recipe_secured(player, recipe):
        missing.append("already secured")
    costs = effective_recipe_cost(player, recipe)
    if player.workbench_tier < recipe.workbench_requirement:
        missing.append(f"Workbench Tier {recipe.workbench_requirement}")
    if recipe.blueprint and not has_blueprint(player, recipe.blueprint):
        missing.append(recipe.blueprint)
    for item_name, amount in recipe.required_items.items():
        current = total_item_count(player, item_name)
        if current < amount:
            missing.append(f"{item_name} {amount}")
    for material, amount in costs.items():
        current = total_material_count(player, material)
        if current < amount:
            missing.append(f"{material} {amount - current} short")
    return not missing, missing


def craft(player: PlayerProfile, recipe_id: str) -> None:
    recipe = RECIPES[recipe_id]
    ready, missing = recipe_status(player, recipe)
    if not ready:
        raise ValueError(f"Recipe '{recipe.name}' unavailable: {', '.join(missing)}")
    consume_required_items(player, recipe.required_items)
    consume_recipe_materials(player, effective_recipe_cost(player, recipe))
    for item_name, amount in recipe.result_items.items():
        player.inventory[item_name] = player.inventory.get(item_name, 0) + amount
    if recipe.grants_home_upgrade:
        player.home_upgrades += recipe.grants_home_upgrade
    if recipe.grants_workbench_tier is not None:
        player.workbench_tier = max(player.workbench_tier, recipe.grants_workbench_tier)
    player.crafted_recipes.add(recipe.recipe_id)
    completed_now = sync_frontier_state(player)
    player.notes.append(f"Crafted {recipe.name} at {recipe.station}.")
    if completed_now:
        player.notes.append("Trail completions -> " + ", ".join(completed_now))


def available_recipes(player: PlayerProfile) -> list[tuple[RecipeDefinition, list[str]]]:
    results: list[tuple[RecipeDefinition, list[str]]] = []
    for recipe in RECIPES.values():
        ready, missing = recipe_status(player, recipe)
        if ready:
            results.append((recipe, []))
        else:
            results.append((recipe, missing))
    return sorted(results, key=lambda item: (item[0].tier, item[0].name))


def next_world_tier(player: PlayerProfile) -> int | None:
    candidate = player.world_tier + 1
    return candidate if candidate in WORLD_TIERS else None


def can_promote_world_tier(player: PlayerProfile, tier: int) -> tuple[bool, list[str]]:
    definition = WORLD_TIERS[tier]
    return meets_gate(player, definition.gate)


def promote_world_tier(player: PlayerProfile) -> str | None:
    candidate = next_world_tier(player)
    if candidate is None:
        return None
    ready, _ = can_promote_world_tier(player, candidate)
    if not ready:
        return None
    player.world_tier = candidate
    name = cast(str, WORLD_TIERS[candidate].name)
    player.notes.append(f"World Tier promoted to {name}.")
    for entry in claim_available_world_tier_rewards(player):
        player.notes.append(entry)
    return name


