from __future__ import annotations

from collections.abc import Callable

from frontier_rpg.models import PlayerProfile
from frontier_rpg.ui_state import (
    build_affinity_table_rows,
    build_armory_table_rows,
    build_collection_table_rows,
    build_crafting_rows,
    build_dead_ledger_rows,
    build_encyclopedia_rows,
    build_ending_table_rows,
    build_farming_table_rows,
    build_field_overview_lines,
    build_frontier_map_table_rows,
    build_gear_table_rows,
    build_homefront_rows,
    build_network_rows,
    build_perk_rows,
    build_skill_rows,
    build_stats_rows,
    build_trail_rows,
    build_upgrade_rows,
    build_world_state_table_rows,
)
from ingame_mod.scripts.export_ledger_localization import sanitize_text
from ingame_mod.scripts.export_ledger_types import ExportEntry, ExportPage

PAGE_SUMMARIES = {
    "overview": "Project status, world pressure, and active trails.",
    "trails": "Main story, contracts, boss hunts, and repeatable frontier objectives.",
    "frontier_map": "Region danger, boss clue lanes, and frontier route pressure.",
    "homefront": "Family-linked support, ranch pressure, and home defense readiness.",
    "affinity": "Relationship tracks, stage quests, and support routes for Abigail, Jack, and Uncle.",
    "stats": "Core combat and survival attributes for John Marston.",
    "gear": "Grounded accessories, charms, and slot-based build identity.",
    "armory": "Weapon condition, familiarity, mods, and archetype path.",
    "skills": "Lifestyle progression that feeds the frontier economy.",
    "perks": "Unlocked, ready, and locked perk lines.",
    "crafting": "Crafting tables, costs, and immediate unlock paths.",
    "upgrades": "Beecher's Hope, workshop, weapon, and relic upgrade tracks.",
    "dead": "Boss archive with known threats, silhouettes, and territory notes.",
    "collection": "Long-term relic, trophy, archive, and accessory progress.",
    "farming": "Repeatable farming lanes that feed materials, recipes, and long-term route identity.",
    "network": "Integrated subsystem tracks for lawful work, outlaw profit, and frontier support routes.",
    "world": "World tier escalation tables for enemies, loot, bosses, story pressure, and route viability.",
    "ending": "Ending and post-ending route phases, outcomes, and readiness checks.",
    "encyclopedia": "Bosses, regions, world tiers, and frontier lore entries.",
}

def build_overview_entries(player: PlayerProfile) -> list[ExportEntry]:
    entries: list[ExportEntry] = []
    for line in build_field_overview_lines(player):
        line = line.strip()
        if not line:
            continue
        entries.append({"label": line, "detail": line})
    return entries


def build_trail_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['trail']} | {row['category']} | {row['state']}",
            "detail": sanitize_text(row["detail"]),
        }
        for row in build_trail_rows(player)
    ]


def build_frontier_map_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['region']} | {row['state']} | {row['danger']}",
            "detail": sanitize_text(row["detail"]),
        }
        for row in build_frontier_map_table_rows(player)
    ]


def build_homefront_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['member']} | {row['focus']} | {row['request']} | {row['state']}",
            "detail": sanitize_text(row["detail"]),
        }
        for row in build_homefront_rows(player)
    ]


def build_affinity_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['npc']} | {row['stage']} | {row['quest_state']} | {row['repeatable_bonus']}",
            "detail": sanitize_text(row["detail"]),
        }
        for row in build_affinity_table_rows(player)
    ]


def build_stats_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['stat']} | Rank {row['value']} | {row['focus']}",
            "detail": sanitize_text(row["detail"]),
        }
        for row in build_stats_rows(player)
    ]


def build_gear_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['slot']} | {row['item']} | {row['state']}",
            "detail": sanitize_text(row["detail"]),
        }
        for row in build_gear_table_rows(player)
    ]


def build_armory_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['weapon']} | {row['condition']} | Familiarity {row['familiarity']}",
            "detail": sanitize_text(row["detail"]),
        }
        for row in build_armory_table_rows(player)
    ]


def build_skill_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['skill']} | Rank {row['rank']} | {row['focus']}",
            "detail": sanitize_text(row["detail"]),
        }
        for row in build_skill_rows(player)
    ]


def build_perk_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['perk']} | {row['tree']} | {row['state']}",
            "detail": sanitize_text(f"Requirement: {row['requirement']}\n{row['detail']}"),
        }
        for row in build_perk_rows(player)
    ]


def build_crafting_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['recipe']} | {row['station']} | Tier {row['tier']} | {row['status']}",
            "detail": sanitize_text(row["detail"]),
        }
        for row in build_crafting_rows(player)
    ]


def build_upgrade_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['upgrade']} | {row['category']} | {row['state']}",
            "detail": sanitize_text(f"{row['effect']}\n{row['detail']}"),
        }
        for row in build_upgrade_rows(player)
    ]


def build_dead_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['boss']} | {row['state']} | {row['region']}",
            "detail": sanitize_text(row["detail"]),
        }
        for row in build_dead_ledger_rows(player)
    ]


def build_collection_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['collection']} | {row['progress']} | {row['state']}",
            "detail": sanitize_text(row["detail"]),
        }
        for row in build_collection_table_rows(player)
    ]


def build_farming_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['lane']} | {row['owner']} | {row['tier']} | {row['state']} | {row['next_reward']}",
            "detail": sanitize_text(f"{row['focus']}\n{row['detail']}"),
        }
        for row in build_farming_table_rows(player)
    ]


def build_network_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['system']} | {row['owner']} | {row['state']} | {row['milestone']}",
            "detail": sanitize_text(row["detail"]),
        }
        for row in build_network_rows(player)
    ]


def build_world_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['tier']} | {row['title']} | {row['state']} | {row['package_state']} | {row['package']}",
            "detail": sanitize_text(f"{row['focus']}\n{row['detail']}"),
        }
        for row in build_world_state_table_rows(player)
    ]


def build_ending_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['phase']} | {row['type']} | {row['state']} | {row['reward_state']} | {row['reward_package']}",
            "detail": sanitize_text(row["detail"]),
        }
        for row in build_ending_table_rows(player)
    ]


def build_encyclopedia_entries(player: PlayerProfile) -> list[ExportEntry]:
    return [
        {
            "label": f"{row['entry']} | {row['category']} | {row['status']}",
            "detail": sanitize_text(row["detail"]),
        }
        for row in build_encyclopedia_rows(player)
    ]


PageBuilder = Callable[[PlayerProfile], list[ExportEntry]]

PAGE_DEFINITIONS: tuple[tuple[str, str, PageBuilder], ...] = (
    ("overview", "Overview", build_overview_entries),
    ("trails", "Active Trails", build_trail_entries),
    ("frontier_map", "Frontier Map", build_frontier_map_entries),
    ("homefront", "Homefront", build_homefront_entries),
    ("affinity", "Affinity", build_affinity_entries),
    ("stats", "Stats", build_stats_entries),
    ("gear", "Gear", build_gear_entries),
    ("armory", "Armory", build_armory_entries),
    ("skills", "Skills", build_skill_entries),
    ("perks", "Perks", build_perk_entries),
    ("crafting", "Crafting", build_crafting_entries),
    ("upgrades", "Workshop", build_upgrade_entries),
    ("dead", "Ledger of the Dead", build_dead_entries),
    ("collection", "Collection", build_collection_entries),
    ("farming", "Farming", build_farming_entries),
    ("network", "Network", build_network_entries),
    ("world", "World", build_world_entries),
    ("ending", "Ending", build_ending_entries),
    ("encyclopedia", "Encyclopedia", build_encyclopedia_entries),
)


def build_pages(player: PlayerProfile) -> list[ExportPage]:
    return [
        {
            "page_id": page_id,
            "title": title,
            "summary": PAGE_SUMMARIES[page_id],
            "entries": builder(player),
        }
        for page_id, title, builder in PAGE_DEFINITIONS
    ]
