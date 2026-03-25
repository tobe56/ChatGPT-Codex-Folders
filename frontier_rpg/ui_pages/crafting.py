from __future__ import annotations

from ..content import RECIPES
from ..features.world_loot import effective_recipe_cost, total_item_count, total_material_count
from ..models import PlayerProfile
from ..platform.localization import gameplay_text
from ..systems import (
    available_recipes,
    build_hub_feature_rows,
    has_blueprint,
    recipe_secured,
    recipe_status,
)


def build_crafting_rows(player: PlayerProfile) -> list[dict[str, str]]:
    locale = player.preferred_locale
    rows: list[dict[str, str]] = []
    for recipe, missing in available_recipes(player):
        secured = recipe_secured(player, recipe)
        status = "Secured" if secured else ("Ready" if not missing else "Locked")
        effective_costs = effective_recipe_cost(player, recipe)
        cost = ", ".join(f"{name} x{amount}" for name, amount in effective_costs.items()) or "No material cost"
        source_lines = ", ".join(f"{name} {total_material_count(player, name)} owned" for name in effective_costs) or "No material stock needed"
        blueprint_line = "Blueprint: not required"
        if recipe.blueprint:
            blueprint_state = "owned" if has_blueprint(player, recipe.blueprint) else "missing"
            blueprint_line = f"Blueprint: {recipe.blueprint} ({blueprint_state})"
        required_item_line = "Key items: none"
        if recipe.required_items:
            required_item_line = "Key items: " + ", ".join(
                f"{name} {total_item_count(player, name)}/{amount}"
                for name, amount in recipe.required_items.items()
            )
        result = ", ".join(f"{name} x{amount}" for name, amount in recipe.result_items.items()) or "Progression upgrade"
        detail = gameplay_text(
            locale,
            f"{recipe.description}\n\n{blueprint_line}\n{required_item_line}\nCost: {cost}\nStorage: {source_lines}\nResult: {result}",
        )
        if secured:
            detail += "\nState: already secured"
        rows.append(
            {
                "recipe": recipe.name,
                "station": recipe.station,
                "tier": str(recipe.tier),
                "status": status,
                "detail": detail + ("" if not missing or secured else f"\nMissing: {', '.join(missing)}"),
            }
        )
    return rows

def build_upgrade_rows(player: PlayerProfile) -> list[dict[str, str]]:
    rows: list[dict[str, str]] = []
    hub_rows = build_hub_feature_rows(player)
    for row in hub_rows:
        rows.append(
            {
                "category": row["category"],
                "upgrade": row["feature"],
                "effect": row["output"],
                "state": row["state"],
                "detail": row["detail"],
            }
        )
    tracked_categories = {"Weapon Mod", "Named Weapon", "Relic Upgrade", "Ammo Load"}
    for recipe in sorted(RECIPES.values(), key=lambda item: (item.tier, item.category, item.name)):
        if recipe.category not in tracked_categories:
            continue
        ready, missing = recipe_status(player, recipe)
        effect = []
        if recipe.grants_workbench_tier is not None:
            effect.append(f"Workbench -> {recipe.grants_workbench_tier}")
        if recipe.grants_home_upgrade:
            effect.append(f"Home upgrades +{recipe.grants_home_upgrade}")
        if recipe.result_items:
            effect.append(", ".join(recipe.result_items.keys()))
        if recipe.blueprint:
            effect.append(f"Blueprint {recipe.blueprint}")
        if recipe.required_items:
            effect.append("Key item " + ", ".join(recipe.required_items.keys()))
        rows.append(
            {
                "category": recipe.category,
                "upgrade": recipe.name,
                "effect": " | ".join(effect) if effect else recipe.description,
                "state": "Ready" if ready and not missing else "Locked",
                "detail": recipe.description + ("" if not missing else "\nMissing: " + ", ".join(missing)),
            }
        )
    return rows


