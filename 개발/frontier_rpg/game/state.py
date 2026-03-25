from __future__ import annotations

from copy import deepcopy

from ..content import (
    BOSSES,
    COLLECTIONS,
    GEAR_ITEMS,
    PERKS,
    RECIPES,
    REGIONS,
    TRAILS,
    WEAPON_LOADOUTS,
)
from ..data import AFFINITY_TRACKS
from ..features.affinity import affinity_quest_label, affinity_stage, ensure_affinity_state
from ..features.world_loot import total_item_count, total_material_count
from ..features.world_state import claim_available_world_tier_rewards
from ..models import (
    GearDefinition,
    PlayerProfile,
    ProgressGate,
    RecipeDefinition,
    TrailDefinition,
)


def create_baseline_player() -> PlayerProfile:
    player = PlayerProfile()
    player.weapons = [deepcopy(weapon) for weapon in WEAPON_LOADOUTS]
    player.completed_chapters.add("chapter_1")
    player.equipped_gear.update(
        {
            "Major Accessory 1": "gear.ranch_band",
            "Satchel Mod": "gear.field_satchel_tools",
        }
    )
    player.materials.update(
        {
            "iron scrap": 2,
            "treated leather": 2,
            "gun oil concentrate": 1,
            "hardwood": 1,
            "saint cards": 1,
        }
    )
    ensure_affinity_state(player)
    claim_available_world_tier_rewards(player)
    sync_frontier_state(player)
    return player


def has_blueprint(player: PlayerProfile, blueprint_name: str) -> bool:
    return blueprint_name in player.known_blueprints


def meets_gate(player: PlayerProfile, gate: ProgressGate) -> tuple[bool, list[str]]:
    missing: list[str] = []
    if player.level < gate.minimum_level:
        missing.append(f"Level {gate.minimum_level}")
    if player.workbench_tier < gate.minimum_workbench_tier:
        missing.append(f"Workbench Tier {gate.minimum_workbench_tier}")
    if player.world_tier < gate.minimum_world_tier:
        missing.append(f"World Tier {gate.minimum_world_tier}")
    if player.relic_count < gate.minimum_relic_count:
        missing.append(f"Relics {gate.minimum_relic_count}")
    if player.boss_kills < gate.minimum_boss_kills:
        missing.append(f"Boss Kills {gate.minimum_boss_kills}")
    if player.home_upgrades < gate.minimum_home_upgrades:
        missing.append(f"Home Upgrades {gate.minimum_home_upgrades}")
    for perk_id in gate.required_perks:
        if perk_id not in player.unlocked_perks:
            missing.append(PERKS[perk_id].name)
    for region in gate.required_regions:
        if region not in player.stabilized_regions:
            missing.append(REGIONS[region]["name"])
    for stat_name, value in gate.stat_thresholds.items():
        current = player.stats.as_dict()[stat_name]
        if current < value:
            missing.append(f"{stat_name.title()} {value}")
    for affinity_id, required_stage in gate.required_affinity_stages.items():
        definition = AFFINITY_TRACKS[affinity_id]
        current_stage = affinity_stage(player, definition)
        if current_stage < required_stage:
            missing.append(f"{definition.npc_name} Affinity {required_stage}")
    for quest_id in gate.required_affinity_quests:
        if quest_id not in player.completed_affinity_quests:
            missing.append(affinity_quest_label(quest_id))
    for item_name, amount in gate.required_items.items():
        current = total_item_count(player, item_name)
        if current < amount:
            missing.append(f"{item_name} {amount}")
    for trail_id in gate.required_trails:
        if trail_id not in player.completed_trails:
            missing.append(TRAILS[trail_id].name)
    return not missing, missing


def trail_state(player: PlayerProfile, trail: TrailDefinition) -> tuple[str, list[str]]:
    active, active_missing = meets_gate(player, trail.activation_gate)
    if trail.repeatable:
        return ("repeatable", []) if active else ("locked", active_missing)
    if trail.trail_id in player.completed_trails:
        return "completed", []
    if active:
        return "active", []
    return "locked", active_missing


def sync_frontier_state(player: PlayerProfile) -> list[str]:
    player.discovered_materials.update(material for material, amount in player.materials.items() if amount > 0)
    player.discovered_regions.update(player.stabilized_regions)
    if not player.discovered_regions:
        player.discovered_regions.add("west_elizabeth")

    completed_now: list[str] = []
    for trail in sorted(TRAILS.values(), key=lambda item: (item.priority, item.name)):
        if trail.repeatable or trail.trail_id in player.completed_trails:
            continue
        ready, _ = meets_gate(player, trail.completion_gate)
        if ready:
            player.completed_trails.add(trail.trail_id)
            completed_now.append(trail.name)

    active_summaries: list[str] = []
    for trail in sorted(TRAILS.values(), key=lambda item: (item.priority, item.name)):
        state, _ = trail_state(player, trail)
        if state not in {"active", "repeatable"}:
            continue
        prefix = f"[{trail.category}] "
        active_summaries.append(prefix + trail.summary)
    player.active_trails = active_summaries[:3] or [
        "Return to Beecher's Hope and check the Frontier Ledger for the next frontier lead."
    ]
    sync_weapon_loadout(player)
    sync_equipped_gear(player)
    sync_collection_entries(player)
    return completed_now


def sync_equipped_gear(player: PlayerProfile) -> None:
    equipped = {
        "Major Accessory 1": "gear.ranch_band",
        "Satchel Mod": "gear.field_satchel_tools",
    }
    if player.home_upgrades >= 1:
        equipped["Coat Lining"] = "gear.ranch_coat_lining"
    if total_item_count(player, "Saint Medal") > 0:
        equipped["Major Accessory 2"] = "gear.saint_medal"
    if total_item_count(player, "Trapper Sigil") > 0:
        equipped["Minor Charm 1"] = "gear.trapper_sigil"
    if player.level >= 6 and player.home_upgrades >= 1:
        equipped["Minor Charm 2"] = "gear.gambler_token"
    if player.workbench_tier >= 2:
        equipped["Holster Mod"] = "gear.reinforced_gun_belt_plate"
    if player.level >= 8 and player.home_upgrades >= 1:
        equipped["Major Accessory 1"] = "gear.officers_pocketwatch"
    if "great_plains" in player.discovered_regions:
        equipped["Saddle Token"] = "gear.scout_saddle_token"
    player.equipped_gear = equipped


def sync_weapon_loadout(player: PlayerProfile) -> None:
    for weapon in player.weapons:
        if weapon.weapon_id == "weapon.marston_repeater":
            weapon.name = "Marston Repeater"
            weapon.category = "Repeater"
            weapon.archetype = "Reliable Frontier"
            mods = {"barrel": "Factory", "receiver": "Factory", "sight": "Frontier notch"}
            if total_item_count(player, "Long Barrel Walker Kit") > 0:
                mods["barrel"] = "Long trail barrel"
                mods["stock tune"] = "Saddle-braced stock"
                weapon.archetype = "Ranger Build"
            if total_item_count(player, "Silas Redd's Repeater Receiver") > 0:
                mods["receiver"] = "Silas Redd's Repeater Receiver"
                weapon.archetype = "Outlaw Pressure"
            if total_item_count(player, "Ash-Marked Repeater") > 0:
                weapon.name = "Ash-Marked Repeater"
                mods["receiver"] = "Ash-marked receiver"
                mods["lever tune"] = "Scorched quick-cycle"
                weapon.archetype = "Outlaw Pressure"
            if total_item_count(player, "Relic Binding Kit") > 0:
                mods["receiver"] = "Relic-bound receiver"
            if total_item_count(player, "Relic Marked Rounds") > 0:
                mods["ammo preference"] = "Relic-marked rounds"
                weapon.archetype = "Relic Frontier"
            weapon.mods = mods
            continue

        if weapon.weapon_id == "weapon.beechers_sidearm":
            weapon.name = "Beecher's Sidearm"
            weapon.category = "Revolver"
            weapon.archetype = "Duel Build"
            mods = {"barrel": "Factory", "grip": "Hardwood", "action tune": "Field tuned"}
            if total_item_count(player, "Reliable Grip Wrap") > 0:
                mods["grip"] = "Reliable Grip Wrap"
            if total_item_count(player, "Reinforced Frame") > 0:
                mods["frame"] = "Reinforced Frame"
            if total_item_count(player, "Blackwater Precision Loads") > 0:
                mods["ammo preference"] = "Precision hand-loads"
                weapon.archetype = "Lawman Precision"
            if total_item_count(player, "Blackwater Peacebreaker") > 0:
                weapon.name = "Blackwater Peacebreaker"
                mods["frame"] = "Broker silver frame"
                mods["sight"] = "Blackwater silver sight"
                weapon.archetype = "Lawman Precision"
            weapon.mods = mods


def sync_collection_entries(player: PlayerProfile) -> None:
    entries = set(player.collection_entries)
    for item_name, amount in player.inventory.items():
        if amount > 0:
            entries.add(item_name)
    for item_name, amount in player.stash_inventory.items():
        if amount > 0:
            entries.add(item_name)
    for gear_id in player.equipped_gear.values():
        entries.add(GEAR_ITEMS[gear_id].name)
    for material_name, amount in player.materials.items():
        if amount > 0:
            entries.add(material_name)
    for material_name, amount in player.stash_materials.items():
        if amount > 0:
            entries.add(material_name)
    entries.update(player.known_blueprints)
    if player.level >= 6 and player.home_upgrades >= 1:
        entries.add("Broker Badge")
        entries.add("Gunslinger Mark")
    if "west_elizabeth" in player.stabilized_regions and player.home_upgrades >= 1:
        entries.add("Blackwater Seal")
        entries.add("Route Notes: Blackwater")
    if "boss.silas_redd" in player.known_bosses:
        entries.add("Sketch: Silas")
    if "boss.tall_trees_tracker_king" in player.known_bosses:
        entries.add("Sketch: Tracker King")
    if "boss.lemoyne_relic_keeper" in player.known_bosses:
        entries.add("Sketch: Relic Keeper")
    if "boss.the_surveyor" in player.known_bosses:
        entries.add("Sketch: Surveyor")
    player.collection_entries = entries


