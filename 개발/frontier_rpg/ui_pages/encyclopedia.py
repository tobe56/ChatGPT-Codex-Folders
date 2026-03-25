from __future__ import annotations

from ..content import (
    ENEMY_ARCHETYPES,
    ENEMY_TRAITS,
    FINAL_MASTERMIND,
    FIRST_NAMED_BOSS,
    PROJECT_NAME,
    RECIPES,
    REGIONS,
    STORY_CHAPTERS,
    WORLD_TIERS,
)
from ..data import WORLD_NODES
from ..features.combat import archetype_combat_brief
from ..models import PlayerProfile
from ..systems import (
    build_boss_rows,
    build_collection_rows,
    build_ending_rows,
    build_farming_rows,
    build_gear_rows,
    build_integration_rows,
    build_world_state_rows,
    has_blueprint,
    meets_gate,
)

FAMILY_ENTRIES = (
    (
        "Family",
        "Abigail",
        "Home stability, medicine, and the emotional cost of escalation.",
    ),
    (
        "Family",
        "Jack",
        "Archive keeper for clues, boss notes, and the Frontier Ledger.",
    ),
    (
        "Family",
        "Uncle",
        "Rumor broker for dirty jobs, shortcuts, and black-market leads.",
    ),
)

def build_encyclopedia_rows(player: PlayerProfile) -> list[dict[str, str]]:
    rows: list[dict[str, str]] = [
        {
            "category": "Project",
            "entry": PROJECT_NAME,
            "status": "Active",
            "detail": "Epilogue-only RPG overhaul structured around the Frontier Ledger, progression, and Beecher's Hope.",
        },
        {
            "category": "Boss",
            "entry": FIRST_NAMED_BOSS,
            "status": "Known",
            "detail": "First named boss chain and the first major pressure point for the frontier war.",
        },
        {
            "category": "Boss",
            "entry": FINAL_MASTERMIND,
            "status": "Late Game",
            "detail": "Late-game mastermind concept for the mythic arc payoff.",
        },
    ]
    for category, entry, detail in FAMILY_ENTRIES:
        rows.append(
            {
                "category": category,
                "entry": entry,
                "status": "Known",
                "detail": detail,
            }
        )
    for row in build_gear_rows(player):
        if row["state"] != "Equipped":
            continue
        rows.append(
            {
                "category": "Gear",
                "entry": row["item"],
                "status": row["state"],
                "detail": row["detail"],
            }
        )
    for row in build_boss_rows(player):
        rows.append(
            {
                "category": "Boss Archive",
                "entry": row["boss"],
                "status": row["state"],
                "detail": row["detail"],
            }
        )
    for row in build_collection_rows(player):
        rows.append(
            {
                "category": "Collection",
                "entry": row["collection"],
                "status": row["progress"],
                "detail": row["detail"],
            }
        )
    for recipe in sorted(RECIPES.values(), key=lambda item: (item.tier, item.name)):
        if not recipe.blueprint:
            continue
        rows.append(
            {
                "category": "Blueprint",
                "entry": recipe.blueprint,
                "status": "Owned" if has_blueprint(player, recipe.blueprint) else "Missing",
                "detail": f"Unlocks {recipe.name} at {recipe.station}.",
            }
        )
    for row in build_farming_rows(player):
        rows.append(
            {
                "category": "Farming Lane",
                "entry": row["lane"],
                "status": row["state"],
                "detail": row["detail"],
            }
        )
    for row in build_integration_rows(player):
        rows.append(
            {
                "category": "Network",
                "entry": row["system"],
                "status": row["state"],
                "detail": row["detail"],
            }
        )
    for row in build_world_state_rows(player):
        rows.append(
            {
                "category": "World State",
                "entry": row["title"],
                "status": row["state"],
                "detail": row["detail"],
            }
        )
    for row in build_ending_rows(player):
        rows.append(
            {
                "category": "Ending",
                "entry": row["phase"],
                "status": row["state"],
                "detail": row["detail"],
            }
        )
    for region in REGIONS.values():
        rows.append(
            {
                "category": "Region",
                "entry": region["name"],
                "status": "Open",
                "detail": region["role"],
            }
        )
    for world_tier in WORLD_TIERS.values():
        rows.append(
            {
                "category": "World Tier",
                "entry": world_tier.name,
                "status": "Current" if world_tier.tier == player.world_tier else "Locked" if world_tier.tier > player.world_tier else "Cleared",
                "detail": world_tier.description,
            }
        )
    for trait in ENEMY_TRAITS.values():
        rows.append(
            {
                "category": "Enemy Trait",
                "entry": trait.name,
                "status": f"WT {trait.minimum_world_tier}+",
                "detail": f"{trait.description}\n\nFamily: {trait.family}\nWorld Tier Floor: {trait.minimum_world_tier}",
            }
        )
    for archetype in ENEMY_ARCHETYPES.values():
        combat_brief = archetype_combat_brief(archetype.archetype_id)
        detail_lines = [
            archetype.notes,
            "",
            "Traits: " + ", ".join(ENEMY_TRAITS[trait_id].name for trait_id in archetype.traits),
        ]
        if combat_brief is not None:
            detail_lines.extend(
                [
                    combat_brief["combat"],
                    "Resistances: " + combat_brief["resistances"],
                    "Weaknesses: " + combat_brief["weaknesses"],
                    "Test Spawn: " + combat_brief["spawn"],
                ]
            )
        rows.append(
            {
                "category": "Enemy Archetype",
                "entry": archetype.name,
                "status": archetype.rank,
                "detail": "\n".join(detail_lines),
            }
        )
    for node in WORLD_NODES.values():
        rows.append(
            {
                "category": "World Route",
                "entry": node.name,
                "status": node.category,
                "detail": "\n".join(
                    [
                        node.summary,
                        "",
                        f"Encounter: {node.encounter_id}",
                        "Drops: "
                        + ", ".join(
                            [f"{name} x{amount}" for name, amount in node.drop_materials.items()]
                            + [f"{name} x{amount}" for name, amount in node.drop_items.items()]
                        ),
                        f"Stash Route: {node.stash_destination}",
                        f"Route Note: {node.route_note or 'Field loot gets routed home after pickup.'}",
                    ]
                ),
            }
        )
    for chapter in STORY_CHAPTERS.values():
        ready, missing = meets_gate(player, chapter.gate)
        chapter_state = "Completed" if chapter.chapter_id in player.completed_chapters else "Ready" if ready else "Planned"
        rows.append(
            {
                "category": "Story Chapter",
                "entry": chapter.name,
                "status": chapter_state,
                "detail": chapter.description + ("" if not missing else "\n\nGate: " + ", ".join(missing)),
            }
        )
    return rows


