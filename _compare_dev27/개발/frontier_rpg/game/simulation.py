from __future__ import annotations

from typing import cast

from ..content import (
    BOSSES,
    COMBAT_ENCOUNTERS,
    ENEMY_ARCHETYPES,
    ENEMY_TRAITS,
    FARMING_LANES,
    FIRST_NAMED_BOSS,
    FIRST_TEN_HOUR_SCRIPT,
    INTEGRATION_TRACKS,
    PERK_TREES,
    PERKS,
    PRIMARY_HUB,
    PROJECT_NAME,
    RECIPES,
    REGIONS,
    STORY_CHAPTERS,
    WORLD_TIERS,
)
from ..core.progression import total_xp_required_for_level, xp_progress
from ..data import WORLD_NODES
from ..features.affinity import apply_affinity_activity, build_affinity_rows, complete_available_affinity_quests
from ..features.combat import build_combat_activity
from ..features.ending import complete_available_ending_phases
from ..features.farming import claim_available_farming_milestones
from ..features.network import claim_available_network_milestones
from ..features.world_loot import build_world_loot_activity, deposit_field_supplies
from ..features.world_state import claim_available_world_tier_rewards
from ..models import ActivityDefinition, PlayerProfile
from .progression import (
    allocate_stat_points,
    available_perks,
    available_recipes,
    award_xp,
    can_promote_world_tier,
    craft,
    next_world_tier,
    promote_world_tier,
    unlock_perk,
)
from .rows import (
    build_active_trail_rows,
    build_armory_rows,
    build_boss_rows,
    build_collection_rows,
    build_ending_rows,
    build_family_rows,
    build_farming_rows,
    build_frontier_map_rows,
    build_gear_rows,
    build_hub_feature_rows,
    build_integration_rows,
    build_world_state_rows,
)
from .state import create_baseline_player, meets_gate, sync_frontier_state


def apply_activity(player: PlayerProfile, activity: ActivityDefinition) -> list[str]:
    entries = [f"{activity.name}: +{activity.xp} XP"]
    levels = award_xp(player, activity.xp)
    if levels:
        entries.append("Level ups -> " + ", ".join(str(level) for level in levels))
    for material, amount in activity.materials.items():
        player.materials[material] = player.materials.get(material, 0) + amount
    for item_name, amount in activity.items.items():
        player.inventory[item_name] = player.inventory.get(item_name, 0) + amount
    player.relic_count += activity.relics
    player.boss_kills += activity.boss_kills
    player.home_upgrades += activity.home_upgrades
    player.stabilized_regions.update(activity.region_progress)
    player.discovered_regions.update(activity.discovered_regions)
    player.completed_trails.update(activity.completed_trails)
    player.known_bosses.update(activity.discovered_bosses)
    player.defeated_bosses.update(activity.defeated_bosses)
    for lane_id, amount in activity.farming_gains.items():
        player.farming_progress[lane_id] = player.farming_progress.get(lane_id, 0) + amount
    for track_id, amount in activity.network_gains.items():
        player.network_progress[track_id] = player.network_progress.get(track_id, 0) + amount
    affinity_entries = apply_affinity_activity(player, activity.affinity_changes)
    player.completed_ending_phases.update(activity.ending_phases)
    player.completed_chapters.update(activity.completed_chapters)
    player.lifestyle.add_points(activity.lifestyle_gains)
    player.known_blueprints.update(activity.blueprints)
    if activity.relics:
        entries.append(f"Relics found -> +{activity.relics}")
    if activity.boss_kills:
        entries.append(f"Boss kills -> +{activity.boss_kills}")
    if activity.defeated_bosses:
        defeated_names = ", ".join(BOSSES[boss_id].name for boss_id in activity.defeated_bosses if boss_id in BOSSES)
        if defeated_names:
            entries.append("Boss defeats -> " + defeated_names)
    if activity.region_progress:
        names = ", ".join(REGIONS[region]["name"] for region in activity.region_progress)
        entries.append(f"Regional pressure tracked in {names}")
    if activity.farming_gains:
        entries.append(
            "Farming lanes -> "
            + ", ".join(f"{FARMING_LANES[lane_id].name} +{amount}" for lane_id, amount in activity.farming_gains.items())
        )
    if activity.network_gains:
        entries.append(
            "Network tracks -> "
            + ", ".join(f"{INTEGRATION_TRACKS[track_id].name} +{amount}" for track_id, amount in activity.network_gains.items())
        )
    if activity.completed_chapters:
        chapter_names = ", ".join(STORY_CHAPTERS[chapter_id].name for chapter_id in activity.completed_chapters)
        entries.append("Story chapters -> " + chapter_names)
    if activity.blueprints:
        entries.append("Blueprints -> " + ", ".join(activity.blueprints))
    entries.extend(affinity_entries)
    farming_claims = claim_available_farming_milestones(player)
    network_claims = claim_available_network_milestones(player)
    if farming_claims:
        entries.extend(farming_claims)
    if network_claims:
        entries.extend(network_claims)
    completed_now = sync_frontier_state(player)
    if completed_now:
        entries.append("Trail completions -> " + ", ".join(completed_now))
    affinity_quests = complete_available_affinity_quests(player)
    if affinity_quests:
        entries.extend(affinity_quests)
        sync_frontier_state(player)
    ending_updates = complete_available_ending_phases(player, meets_gate)
    if ending_updates:
        entries.extend(ending_updates)
        sync_frontier_state(player)
    if activity.notes:
        entries.append(activity.notes)
    return entries


def resolve_combat_encounter(player: PlayerProfile, encounter_id: str) -> list[str]:
    activity, combat_lines = build_combat_activity(player, encounter_id, meets_gate)
    return cast(list[str], combat_lines + apply_activity(player, activity))


def resolve_world_node(player: PlayerProfile, node_id: str) -> list[str]:
    activity, combat_lines, flow_lines = build_world_loot_activity(player, node_id, meets_gate)
    applied_lines = apply_activity(player, activity)
    player.cleared_world_nodes.add(node_id)
    stash_lines = deposit_field_supplies(player, WORLD_NODES[node_id].stash_destination)
    return cast(list[str], combat_lines + flow_lines + applied_lines + stash_lines)


def unlock_recommended_perks(player: PlayerProfile) -> list[str]:
    priorities = [
        "survivor.thick_skin",
        "gunslinger.quick_draw",
        "frontiersman.long_trail",
        "outlaw.fast_pockets",
        "frontiersman.camp_mechanic",
        "survivor.patch_job",
        "gunslinger.iron_wrist",
        "survivor.household_reserve",
        "outlaw.greased_palm",
        "outlaw.dirty_trigger",
        "relic_hunter.strange_pull",
        "relic_hunter.margin_notes",
        "survivor.ranch_resolve",
        "gunslinger.front_sight_faith",
        "relic_hunter.grave_luck",
        "relic_hunter.ash_mark",
    ]
    unlocked: list[str] = []
    while player.unspent_perk_points > 0:
        before = player.unspent_perk_points
        for perk_id in priorities:
            if perk_id in player.unlocked_perks:
                continue
            try:
                unlock_perk(player, perk_id)
            except ValueError:
                continue
            unlocked.append(PERKS[perk_id].name)
            break
        if player.unspent_perk_points == before:
            break
    return unlocked


def auto_craft_recommended_path(player: PlayerProfile) -> list[str]:
    crafted: list[str] = []
    for recipe_id in (
        "bench.expanded_workshop",
        "gear.saint_medal",
        "weapon.reliable_grip_wrap",
        "weapon.long_barrel_walker_kit",
        "weapon.reinforced_frame",
        "bench.field_support_cabinet",
        "ammo.blackwater_precision_loads",
        "gear.trapper_sigil",
        "bench.trophy_board",
        "weapon.blackwater_peacebreaker",
        "weapon.ash_marked_repeater",
        "ammo.relic_marked_rounds",
    ):
        try:
            craft(player, recipe_id)
        except ValueError:
            continue
        crafted.append(RECIPES[recipe_id].name)
    return crafted


def chapter_status(player: PlayerProfile) -> list[tuple[str, str, list[str]]]:
    status: list[tuple[str, str, list[str]]] = []
    for chapter in STORY_CHAPTERS.values():
        ready, missing = meets_gate(player, chapter.gate)
        if chapter.chapter_id in player.completed_chapters:
            state = "completed"
        elif ready:
            state = "ready"
        else:
            state = "locked"
        status.append((chapter.name, state, missing))
    return status


def simulate_first_ten_hours() -> tuple[PlayerProfile, list[str]]:
    player = create_baseline_player()
    timeline = [
        f"Project: {PROJECT_NAME}",
        f"Hub anchor: {PRIMARY_HUB}",
        f"First boss target: {FIRST_NAMED_BOSS}",
    ]
    stat_plan = ["grit", "deadeye", "survival", "deadeye", "cunning", "grit", "strength"]
    for activity in FIRST_TEN_HOUR_SCRIPT:
        timeline.extend(apply_activity(player, activity))
        if player.unspent_stat_points:
            spent = allocate_stat_points(player, stat_plan)
            if spent:
                timeline.append(f"Auto-assigned stat points -> {spent}")
        unlocked = unlock_recommended_perks(player)
        if unlocked:
            timeline.append("Perks unlocked -> " + ", ".join(unlocked))
        crafted = auto_craft_recommended_path(player)
        if crafted:
            timeline.append("Crafted -> " + ", ".join(crafted))
        promoted = promote_world_tier(player)
        if promoted:
            timeline.append(f"World tier advanced -> {promoted}")
    return player, timeline


def build_endgame_profile() -> tuple[PlayerProfile, list[str]]:
    player, timeline = simulate_first_ten_hours()
    timeline.append("----- Late Frontier Campaign Projection -----")

    target_level = 32
    xp_needed = total_xp_required_for_level(target_level) - player.total_xp
    if xp_needed > 0:
        levels = award_xp(player, xp_needed)
        if levels:
            timeline.append("Late-campaign levels -> " + ", ".join(str(level) for level in levels))
    allocate_stat_points(player, ["deadeye", "grit", "survival", "cunning", "strength"])

    player.workbench_tier = 4
    player.home_upgrades = 3
    player.relic_count = 8
    player.boss_kills = 5
    player.stabilized_regions.update(REGIONS.keys())
    player.discovered_regions.update(REGIONS.keys())
    player.known_bosses.update(BOSSES.keys())
    player.defeated_bosses.update(BOSSES.keys())
    player.completed_chapters.update(STORY_CHAPTERS.keys())

    player.materials.update(
        {
            "rifled steel": 3,
            "meteor splinters": 2,
            "boss cores": 2,
            "grave silver": max(player.materials.get("grave silver", 0), 2),
        }
    )
    player.inventory.update(
        {
            "Blackwater Peacebreaker": 1,
            "Blackwater Precision Loads": 1,
            "Ash-Marked Repeater": 1,
            "Relic Binding Kit": 1,
            "Relic Marked Rounds": 1,
            "Broker Badge": 1,
            "Blackwater Seal": 1,
            "Gunslinger Mark": 1,
        }
    )
    player.known_blueprints.update(
        {
            "Trail Mechanic Sketch",
            "Blackwater Ballistics Notes",
            "Blackwater Peacebreaker Diagram",
            "Field Cabinet Plan",
            "Silas Repeater Schematic",
            "Fence Relic Schematic",
            "Surveyor Ballistics Plate",
        }
    )

    for lane_id in FARMING_LANES:
        player.farming_progress[lane_id] = max(player.farming_progress.get(lane_id, 0), 5)
    for track_id in INTEGRATION_TRACKS:
        player.network_progress[track_id] = max(player.network_progress.get(track_id, 0), 3)
    timeline.extend(claim_available_farming_milestones(player))
    timeline.extend(claim_available_network_milestones(player))

    for weapon in player.weapons:
        weapon.familiarity = max(weapon.familiarity, 5)
        weapon.condition = "Pristine"

    while True:
        ready_perks = available_perks(player)
        if not ready_perks:
            break
        if player.unspent_perk_points <= 0:
            player.unspent_perk_points += len(ready_perks)
        for perk in ready_perks:
            if player.unspent_perk_points <= 0:
                break
            try:
                unlock_perk(player, perk.perk_id)
            except ValueError:
                continue

    player.world_tier = 5
    timeline.extend(claim_available_world_tier_rewards(player))
    player.affinity_points.update(
        {
            "affinity.abigail": 180,
            "affinity.jack": 176,
            "affinity.uncle": 172,
        }
    )
    sync_frontier_state(player)
    while True:
        ready_perks = available_perks(player)
        if not ready_perks:
            break
        if player.unspent_perk_points <= 0:
            player.unspent_perk_points += len(ready_perks)
        for perk in ready_perks:
            if player.unspent_perk_points <= 0:
                break
            try:
                unlock_perk(player, perk.perk_id)
            except ValueError:
                continue
    timeline.extend(complete_available_ending_phases(player, meets_gate))
    sync_frontier_state(player)

    timeline.extend(
        [
            "World state forced to After The Ledger for prototype validation.",
            "War-room hub, post-ending repeatables, and final act phases marked complete.",
        ]
    )
    return player, timeline


def build_ledger_snapshot(player: PlayerProfile) -> str:
    sync_frontier_state(player)
    xp_current, xp_needed = xp_progress(player)
    trail_rows = build_active_trail_rows(player)
    map_rows = build_frontier_map_rows(player)
    hub_rows = build_hub_feature_rows(player)
    gear_rows = build_gear_rows(player)
    boss_rows = build_boss_rows(player)
    collection_rows = build_collection_rows(player)
    family_rows = build_family_rows(player)
    affinity_rows = build_affinity_rows(player)
    farming_rows = build_farming_rows(player)
    integration_rows = build_integration_rows(player)
    world_rows = build_world_state_rows(player)
    ending_rows = build_ending_rows(player)
    lines: list[str] = [
        f"{player.name} | {player.frontier_title}",
        f"Project: {PROJECT_NAME}",
        "",
        "John",
        f"- Level {player.level} | XP {xp_current}/{xp_needed} inside current level | Total XP {player.total_xp}",
        "- Stats: "
        + ", ".join(f"{name.title()} {value}" for name, value in player.stats.as_dict().items()),
        "- Lifestyle: "
        + ", ".join(f"{name.replace('_', ' ').title()} {value}" for name, value in player.lifestyle.as_dict().items()),
        f"- Unspent points: stats {player.unspent_stat_points}, perks {player.unspent_perk_points}",
        "",
        "Frontier Status",
        f"- World Tier {player.world_tier}: {WORLD_TIERS[player.world_tier].name}",
        f"- Workbench Tier {player.workbench_tier} | Home Upgrades {player.home_upgrades}",
        f"- Relics {player.relic_count} | Boss Kills {player.boss_kills}",
        "- Stabilized regions: "
        + (", ".join(REGIONS[key]["name"] for key in sorted(player.stabilized_regions)) or "none"),
        "",
        "Active Trails",
    ]
    for row in trail_rows:
        if row["state"] in {"Active", "Repeatable"}:
            lines.append(f"- {row['category']} | {row['trail']} | {row['region']}")
    if lines[-1] == "Active Trails":
        lines.append("- No active trails")
    lines.extend(["", "Frontier Map"])
    for row in map_rows:
        if row["state"] in {"Known", "Stabilized", "Rumored"}:
            lines.append(f"- {row['region']}: {row['state']} | {row['danger']}")
    lines.extend(["", "Beecher's Hope Hub"])
    for row in hub_rows:
        if row["state"] == "Installed":
            lines.append(f"- {row['feature']}: {row['output']}")
    lines.extend(["", "Homefront"])
    for row in family_rows:
        lines.append(f"- {row['member']}: {row['state']} | {row['focus']}")
    lines.extend(["", "Affinity"])
    for row in affinity_rows:
        lines.append(
            f"- {row['npc']}: {row['stage']} | Score {row['score']} | "
            f"{row['quest_state']} | Repeatable {row['repeatable_bonus']}"
        )
    lines.extend(["", "Farming Lanes"])
    for row in farming_rows:
        lines.append(f"- {row['lane']}: {row['tier']} | {row['state']} | {row['owner']} | Next {row['next_reward']}")
    lines.extend(["", "Frontier Network"])
    for row in integration_rows:
        lines.append(f"- {row['system']}: {row['state']} | {row['owner']} | {row['milestone']}")
    lines.extend(["", "Perks"])
    if player.unlocked_perks:
        lines.extend(f"- {PERKS[perk_id].tree}: {PERKS[perk_id].name}" for perk_id in sorted(player.unlocked_perks))
    else:
        lines.append("- No perks unlocked yet")
    ready_perks = [perk.name for perk in available_perks(player)]
    lines.append(f"- Ready next: {', '.join(ready_perks[:5]) if ready_perks else 'none'}")
    lines.extend(["", "Gear"])
    for row in gear_rows:
        if row["state"] == "Equipped":
            lines.append(f"- {row['slot']}: {row['item']} | {row['bonuses']}")
    lines.extend(["", "Armory"])
    for row in build_armory_rows(player):
        lines.append(
            f"- {row['weapon']} ({row['category']}) | Condition {row['condition']} | Familiarity {row['familiarity']}"
        )
    lines.extend(["", "Materials"])
    if player.materials:
        for material, amount in sorted(player.materials.items()):
            if amount > 0:
                lines.append(f"- Field {material}: {amount}")
    else:
        lines.append("- No field materials tracked")
    if player.stash_materials:
        for material, amount in sorted(player.stash_materials.items()):
            if amount > 0:
                lines.append(f"- Stash {material}: {amount}")
    else:
        lines.append("- No stash materials stored yet")
    lines.append(
        "- Discovered material knowledge: "
        + (", ".join(sorted(player.discovered_materials)) if player.discovered_materials else "none")
    )
    lines.extend(["", "Inventory"])
    if player.inventory:
        for item_name, amount in sorted(player.inventory.items()):
            lines.append(f"- Field {item_name}: {amount}")
    else:
        lines.append("- No field inventory tracked")
    if player.stash_inventory:
        for item_name, amount in sorted(player.stash_inventory.items()):
            lines.append(f"- Stash {item_name}: {amount}")
    else:
        lines.append("- No stash inventory stored yet")
    lines.extend(["", "Ledger of the Dead"])
    for row in boss_rows:
        if row["state"] in {"Known", "Defeated", "Clue Ready"}:
            lines.append(f"- {row['boss']}: {row['state']} | {row['region']}")
    lines.extend(["", "Collection"])
    for row in collection_rows:
        lines.append(f"- {row['collection']}: {row['progress']} | {row['state']}")
    lines.extend(["", "World State"])
    for row in world_rows:
        if row["state"] in {"Current", "Ready"}:
            lines.append(f"- {row['tier']} {row['title']}: {row['state']} | {row['package_state']} | {row['package']}")
    lines.extend(["", "Ending Route"])
    for row in ending_rows:
        lines.append(f"- {row['phase']}: {row['state']} | {row['reward_state']} | {row['reward_package']}")
    lines.extend(["", "Crafting Opportunities"])
    ready_recipe = False
    for recipe, missing in available_recipes(player):
        if not missing:
            ready_recipe = True
            lines.append(f"- READY | {recipe.name} ({recipe.station}, tier {recipe.tier})")
    if not ready_recipe:
        lines.append("- No recipes ready")
    lines.extend(["", "Story Gates"])
    for chapter_name, state, missing in chapter_status(player):
        if missing:
            lines.append(f"- {chapter_name}: {state} | Missing: {', '.join(missing)}")
        else:
            lines.append(f"- {chapter_name}: {state}")
    candidate = next_world_tier(player)
    lines.extend(["", "World Tier Readiness"])
    if candidate is None:
        lines.append("- Final world tier already reached")
    else:
        ready, missing = can_promote_world_tier(player, candidate)
        if ready:
            lines.append(f"- Ready for Tier {candidate}: {WORLD_TIERS[candidate].name}")
        else:
            lines.append(f"- Tier {candidate} locked: {', '.join(missing)}")
    lines.extend(
        [
            "",
            "Encounter Catalog",
            f"- Enemy archetypes implemented: {len(ENEMY_ARCHETYPES)}",
            f"- Trait definitions implemented: {len(ENEMY_TRAITS)}",
            f"- Combat encounters implemented: {len(COMBAT_ENCOUNTERS)}",
            f"- World loot nodes implemented: {len(WORLD_NODES)}",
            f"- Stash returns tracked: {player.stash_returns}",
            f"- Perk trees implemented: {len(PERK_TREES)}",
        ]
    )
    return "\n".join(lines)


