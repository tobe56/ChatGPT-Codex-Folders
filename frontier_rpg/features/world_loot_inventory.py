from __future__ import annotations

from ..models import PlayerProfile, RecipeDefinition
from .growth_effects import build_growth_effects


def total_material_count(player: PlayerProfile, material_name: str) -> int:
    field_count = int(player.materials.get(material_name, 0))
    stash_count = int(player.stash_materials.get(material_name, 0))
    return field_count + stash_count


def total_item_count(player: PlayerProfile, item_name: str) -> int:
    field_count = int(player.inventory.get(item_name, 0))
    stash_count = int(player.stash_inventory.get(item_name, 0))
    return field_count + stash_count


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
