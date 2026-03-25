from __future__ import annotations

from typing import cast

from ..content import (
    BOSSES,
    COLLECTIONS,
    ENEMY_ARCHETYPES,
    ENEMY_TRAITS,
    GEAR_ITEMS,
    HUB_FEATURES,
    REGION_INTEL,
    REGIONS,
    TRAILS,
    WORLD_TIERS,
)
from ..data import WORLD_NODES
from ..features.combat import boss_combat_brief
from ..features.combat import build_combat_rows as build_combat_feature_rows
from ..features.ending import build_ending_rows as build_ending_feature_rows
from ..features.family import build_family_rows as build_family_feature_rows
from ..features.farming import build_farming_rows as build_farming_feature_rows
from ..features.network import build_integration_rows as build_network_feature_rows
from ..features.world_loot import (
    build_world_loot_rows as build_world_loot_feature_rows,
)
from ..features.world_loot import (
    total_item_count,
)
from ..features.world_state import build_world_state_rows as build_world_state_feature_rows
from ..models import (
    BossDefinition,
    CollectionDefinition,
    GearDefinition,
    HubFeatureDefinition,
    PlayerProfile,
    RegionIntelDefinition,
)
from .progression import can_promote_world_tier
from .state import has_blueprint, meets_gate, sync_frontier_state, trail_state


def build_active_trail_rows(player: PlayerProfile) -> list[dict[str, str]]:
    sync_frontier_state(player)
    rows: list[dict[str, str]] = []
    for trail in sorted(TRAILS.values(), key=lambda item: (item.priority, item.name)):
        state, missing = trail_state(player, trail)
        if state == "active":
            state_label = "Active"
        elif state == "repeatable":
            state_label = "Repeatable"
        elif state == "completed":
            state_label = "Completed"
        else:
            state_label = "Locked"
        region_name = REGIONS[trail.region]["name"] if trail.region else "Frontier-wide"
        rewards = ", ".join(trail.rewards) if trail.rewards else "Progression only"
        detail_lines = [
            trail.summary,
            "",
            trail.detail,
            "",
            f"Region: {region_name}",
            f"Rewards: {rewards}",
        ]
        if missing:
            detail_lines.append("Missing: " + ", ".join(missing))
        rows.append(
            {
                "category": trail.category,
                "trail": trail.name,
                "state": state_label,
                "region": region_name,
                "detail": "\n".join(detail_lines),
            }
        )
    return rows


def _danger_for_tier(intel: RegionIntelDefinition, tier: int) -> str:
    if tier in intel.danger_by_tier:
        return cast(str, intel.danger_by_tier[tier])
    supported_tiers = sorted(intel.danger_by_tier)
    fallback_tier = max((value for value in supported_tiers if value <= tier), default=supported_tiers[0])
    return cast(str, intel.danger_by_tier[fallback_tier])


def build_frontier_map_rows(player: PlayerProfile) -> list[dict[str, str]]:
    sync_frontier_state(player)
    rows: list[dict[str, str]] = []
    for intel in REGION_INTEL.values():
        if intel.region_id in player.stabilized_regions:
            state = "Stabilized"
        elif intel.region_id in player.discovered_regions:
            state = "Known"
        elif intel.recommended_world_tier <= player.world_tier + 1:
            state = "Rumored"
        else:
            state = "Unknown"
        focus = ", ".join(intel.node_types[:2]) if intel.node_types else "No node notes"
        region_nodes = [node.name for node in WORLD_NODES.values() if node.region == intel.region_id]
        detail = "\n".join(
            [
                intel.role,
                "",
                "Danger: " + _danger_for_tier(intel, player.world_tier),
                "Pressure: " + (", ".join(intel.pressure_sources) if intel.pressure_sources else "None"),
                "Nodes: " + (", ".join(intel.node_types) if intel.node_types else "None"),
                "World Loot Routes: " + (", ".join(region_nodes) if region_nodes else "None"),
                "Faction Routes: " + (", ".join(intel.faction_routes) if intel.faction_routes else "None"),
                "Boss Clues: " + (", ".join(intel.boss_clues) if intel.boss_clues else "None"),
            ]
        )
        rows.append(
            {
                "region": intel.name,
                "state": state,
                "danger": _danger_for_tier(intel, player.world_tier),
                "focus": focus,
                "detail": detail,
            }
        )
    return rows


def build_hub_feature_rows(player: PlayerProfile) -> list[dict[str, str]]:
    sync_frontier_state(player)
    rows: list[dict[str, str]] = []
    for feature in sorted(HUB_FEATURES.values(), key=lambda item: (item.priority, item.name)):
        unlocked, missing = meets_gate(player, feature.unlock_gate)
        outputs = ", ".join(feature.outputs) if feature.outputs else "No output listed"
        rows.append(
            {
                "category": feature.category,
                "feature": feature.name,
                "station": feature.station,
                "state": "Installed" if unlocked else "Locked",
                "output": outputs,
                "detail": "\n".join(
                    [
                        feature.description,
                        "",
                        f"Station: {feature.station}",
                        f"Outputs: {outputs}",
                    ]
                    + ([] if unlocked else ["Missing: " + ", ".join(missing)])
                ),
            }
        )
    return rows


def build_gear_rows(player: PlayerProfile) -> list[dict[str, str]]:
    sync_frontier_state(player)
    rows: list[dict[str, str]] = []
    ordered_slots = [
        "Major Accessory 1",
        "Major Accessory 2",
        "Minor Charm 1",
        "Minor Charm 2",
        "Holster Mod",
        "Satchel Mod",
        "Coat Lining",
        "Saddle Token",
    ]
    for slot in ordered_slots:
        gear_id = player.equipped_gear.get(slot)
        if gear_id:
            gear = GEAR_ITEMS[gear_id]
            bonuses = ", ".join(gear.bonuses)
            source = gear.source
            name = gear.name
            detail = "\n".join(
                [
                    gear.description,
                    "",
                    f"Source: {source}",
                    f"Bonuses: {bonuses}",
                ]
            )
            state = "Equipped"
        else:
            name = "Empty Slot"
            bonuses = "No bonus"
            source = "Not assigned"
            detail = "This slot is part of the planned grounded gear structure but is not equipped yet."
            state = "Open"
        rows.append(
            {
                "slot": slot,
                "item": name,
                "category": GEAR_ITEMS[gear_id].category if gear_id else "Unassigned",
                "state": state,
                "detail": detail,
                "bonuses": bonuses,
                "source": source,
            }
        )
    return rows


def build_armory_rows(player: PlayerProfile) -> list[dict[str, str]]:
    sync_frontier_state(player)
    rows: list[dict[str, str]] = []
    for weapon in player.weapons:
        mod_summary = ", ".join(f"{slot}: {value}" for slot, value in weapon.mods.items())
        ammo_tune = weapon.mods.get("ammo preference", "No tuned ammo path yet.")
        named_status = "Named route active" if weapon.name not in {"Marston Repeater", "Beecher's Sidearm"} else "Frontier base"
        if weapon.weapon_id == "weapon.beechers_sidearm":
            if total_item_count(player, "Blackwater Peacebreaker") > 0:
                next_focus = "Named broker sidearm route is active and ready for harder contract work."
            elif has_blueprint(player, "Blackwater Peacebreaker Diagram"):
                next_focus = "Broker diagram secured; finish the named sidearm once mid-tier materials are stocked."
            elif has_blueprint(player, "Blackwater Ballistics Notes"):
                next_focus = "Precision ammo prep is unlocked, but the broker diagram is still missing."
            elif player.workbench_tier >= 2:
                next_focus = "Eligible for deeper sidearm tuning once Blackwater diagrams are recovered."
            else:
                next_focus = "Needs workbench growth before the sidearm can become a real signature weapon."
        else:
            if total_item_count(player, "Ash-Marked Repeater") > 0:
                next_focus = "Named repeater route is active and feeding the boss lane."
            elif has_blueprint(player, "Silas Repeater Schematic"):
                next_focus = "Silas schematic recovered; convert the captured receiver into a named repeater."
            elif weapon.familiarity >= 4:
                next_focus = "Familiar enough for deeper tuning once the schematic route is recovered."
            elif player.workbench_tier >= 2:
                next_focus = "Eligible for deeper tuning once route pressure yields better parts."
            else:
                next_focus = "Needs workbench growth before deeper armory routing opens."
        rows.append(
            {
                "weapon": weapon.name,
                "category": weapon.category,
                "condition": weapon.condition,
                "familiarity": str(weapon.familiarity),
                "detail": "\n".join(
                    [
                        f"Archetype Path: {weapon.archetype}",
                        f"Installed Mods: {mod_summary}",
                        f"Ammo Tune: {ammo_tune}",
                        f"Named Status: {named_status}",
                        f"Next Focus: {next_focus}",
                    ]
                ),
            }
        )
    return rows


def build_boss_rows(player: PlayerProfile) -> list[dict[str, str]]:
    sync_frontier_state(player)
    rows: list[dict[str, str]] = []
    for boss in BOSSES.values():
        discovered = boss.boss_id in player.known_bosses
        defeated = boss.boss_id in player.defeated_bosses
        gate_ready, missing = meets_gate(player, boss.gate)
        combat_brief = boss_combat_brief(player, boss.boss_id, meets_gate)
        if defeated:
            state = "Defeated"
        elif discovered:
            state = "Known"
        elif gate_ready:
            state = "Clue Ready"
        else:
            state = "Hidden"
        display_name = boss.name if discovered or defeated else boss.silhouette
        rewards = ", ".join(boss.rewards)
        detail_lines = [
            boss.summary if discovered or defeated else "Only partial notes and silhouettes exist for this threat.",
            "",
            f"Territory: {boss.territory if discovered or defeated else 'Unknown frontier territory'}",
            f"Rewards: {rewards if discovered or defeated else 'Unknown'}",
            f"Lore: {boss.lore_note if discovered or defeated else 'Jack needs more clues before the ledger can resolve this entry.'}",
        ]
        if combat_brief is not None and (discovered or defeated or gate_ready):
            detail_lines.extend(
                [
                    f"Combat: {combat_brief['combat']}",
                    f"Resistances: {combat_brief['resistances']}",
                    f"Weaknesses: {combat_brief['weaknesses']}",
                    f"Projection: {combat_brief['projection']}",
                    f"Phases: {combat_brief['phases']}",
                    f"Test Spawn: {combat_brief['test_spawn']}",
                ]
            )
        if missing and state in {"Hidden", "Clue Ready"}:
            detail_lines.append("Gate: " + ", ".join(missing))
        rows.append(
            {
                "boss": display_name,
                "region": REGIONS[boss.region]["name"],
                "rank": boss.threat_rank,
                "state": state,
                "detail": "\n".join(detail_lines),
            }
        )
    return rows


def build_collection_rows(player: PlayerProfile) -> list[dict[str, str]]:
    sync_frontier_state(player)
    rows: list[dict[str, str]] = []
    for collection in COLLECTIONS.values():
        gate_ready, missing = meets_gate(player, collection.gate)
        current = 0
        for entry in collection.entries:
            if entry.startswith("boss."):
                if entry in player.defeated_bosses:
                    current += 1
            elif entry in player.collection_entries:
                current += 1
        total = len(collection.entries)
        if current == total and total > 0:
            state = "Complete"
        elif gate_ready:
            state = "In Progress"
        else:
            state = "Locked"
        detail_lines = [
            collection.description,
            "",
            f"Progress: {current}/{total}",
            "Entries: " + ", ".join(collection.entries),
            f"Reward: {collection.reward}",
        ]
        if missing and state == "Locked":
            detail_lines.append("Gate: " + ", ".join(missing))
        rows.append(
            {
                "collection": collection.name,
                "category": collection.category,
                "progress": f"{current}/{total}",
                "state": state,
                "detail": "\n".join(detail_lines),
            }
        )
    return rows


def build_family_rows(player: PlayerProfile) -> list[dict[str, str]]:
    sync_frontier_state(player)
    return cast(list[dict[str, str]], build_family_feature_rows(player, meets_gate))


def build_combat_rows(player: PlayerProfile) -> list[dict[str, str]]:
    sync_frontier_state(player)
    return cast(list[dict[str, str]], build_combat_feature_rows(player, meets_gate))


def build_world_loot_rows(player: PlayerProfile) -> list[dict[str, str]]:
    sync_frontier_state(player)
    return cast(list[dict[str, str]], build_world_loot_feature_rows(player, meets_gate))


def build_farming_rows(player: PlayerProfile) -> list[dict[str, str]]:
    sync_frontier_state(player)
    return cast(list[dict[str, str]], build_farming_feature_rows(player, meets_gate))


def build_integration_rows(player: PlayerProfile) -> list[dict[str, str]]:
    sync_frontier_state(player)
    return cast(list[dict[str, str]], build_network_feature_rows(player, meets_gate))


def build_world_state_rows(player: PlayerProfile) -> list[dict[str, str]]:
    sync_frontier_state(player)
    return cast(list[dict[str, str]], build_world_state_feature_rows(player, can_promote_world_tier))


def build_ending_rows(player: PlayerProfile) -> list[dict[str, str]]:
    sync_frontier_state(player)
    return cast(list[dict[str, str]], build_ending_feature_rows(player, meets_gate))


