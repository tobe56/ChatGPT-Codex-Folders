from __future__ import annotations

from collections.abc import Callable

from ..content import WORLD_STATE, WORLD_TIERS
from ..models import PlayerProfile

WORLD_TIER_PACKAGES: dict[int, dict[str, object]] = {
    1: {"label": "Frontier Stirring Field Pack", "materials": {"iron scrap": 1}, "items": {"Frontier Stirring Dossier": 1}, "collection": ("World Tier 1 Package",)},
    2: {"label": "Open Wounds Route Pack", "materials": {"silver fittings": 1, "spring assemblies": 1}, "items": {"Open Wounds Dossier": 1}, "collection": ("World Tier 2 Package",)},
    3: {"label": "Relic War Logistics Pack", "materials": {"grave silver": 1, "elite insignias": 1}, "items": {"Relic War Dossier": 1}, "collection": ("World Tier 3 Package",)},
    4: {"label": "Ash Frontier War Pack", "materials": {"tempered springs": 2}, "items": {"Ash Frontier Dossier": 1}, "collection": ("World Tier 4 Package",)},
    5: {"label": "After The Ledger Master Pack", "materials": {"boss cores": 1, "meteor splinters": 1}, "items": {"After The Ledger Dossier": 1}, "collection": ("World Tier 5 Package",)},
}


def claim_available_world_tier_rewards(player: PlayerProfile) -> list[str]:
    entries: list[str] = []
    for tier, package in sorted(WORLD_TIER_PACKAGES.items()):
        if player.world_tier < tier or tier in player.claimed_world_tier_rewards:
            continue
        player.claimed_world_tier_rewards.add(tier)
        for material_name, amount in package.get("materials", {}).items():
            player.materials[material_name] = player.materials.get(material_name, 0) + amount
        for item_name, amount in package.get("items", {}).items():
            player.inventory[item_name] = player.inventory.get(item_name, 0) + amount
        for entry in package.get("collection", ()):
            player.collection_entries.add(entry)
        entries.append(f"World tier package -> WT {tier}: {package['label']}")
    return entries


def _checkpoint(state: str, missing: list[str], title: str) -> str:
    if state == "Cleared":
        return f"{title} has already been pushed through."
    if state == "Current":
        return "This is the active pressure band."
    if state == "Ready":
        return "Promotion gate is open now."
    return "Next gate: " + ", ".join(missing[:3]) if missing else "Promotion work still remains."


def build_world_state_rows(
    player: PlayerProfile,
    world_ready_evaluator: Callable[[PlayerProfile, int], tuple[bool, list[str]]],
) -> list[dict[str, str]]:
    rows: list[dict[str, str]] = []
    for definition in WORLD_STATE.values():
        missing: list[str] = []
        if definition.tier < player.world_tier:
            state = "Cleared"
        elif definition.tier == player.world_tier:
            state = "Current"
        else:
            ready, missing = world_ready_evaluator(player, definition.tier) if definition.tier in WORLD_TIERS else (False, [])
            state = "Ready" if ready else "Locked"
        checkpoint = _checkpoint(state, missing, definition.title)
        package = WORLD_TIER_PACKAGES.get(definition.tier)
        rows.append(
            {
                "tier": f"WT {definition.tier}",
                "title": definition.title,
                "state": state,
                "focus": definition.enemy_change,
                "checkpoint": checkpoint,
                "package": package["label"] if package is not None else "No package",
                "package_state": "Secured" if definition.tier in player.claimed_world_tier_rewards else "Pending",
                "detail": "\n".join(
                    [
                        f"Enemy: {definition.enemy_change}",
                        f"Loot: {definition.loot_change}",
                        f"Bosses: {definition.boss_change}",
                        f"Recipes: {definition.recipe_change}",
                        f"Story: {definition.story_change}",
                        f"Routes: {definition.route_change}",
                        f"Checkpoint: {checkpoint}",
                        "Unlock Package: " + (package["label"] if package is not None else "No package"),
                        "Package State: " + ("Secured" if definition.tier in player.claimed_world_tier_rewards else "Pending"),
                    ]
                ),
            }
        )
    return rows
