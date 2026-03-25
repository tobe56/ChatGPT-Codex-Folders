from __future__ import annotations

import argparse

from .content import (
    BOSSES,
    COLLECTIONS,
    COMBAT_ENCOUNTERS,
    ENDING_PHASES,
    ENEMY_ARCHETYPES,
    ENEMY_TRAITS,
    FAMILY_SUPPORT,
    FARMING_LANES,
    FINAL_MASTERMIND,
    FIRST_NAMED_BOSS,
    FIRST_TEN_HOUR_SCRIPT,
    GEAR_ITEMS,
    HUB_FEATURES,
    INTEGRATION_TRACKS,
    PERK_TREES,
    PERKS,
    PRIMARY_HUB,
    PROJECT_NAME,
    RECIPES,
    REGION_INTEL,
    REGIONS,
    STORY_CHAPTERS,
    TRAILS,
    WORLD_STATE,
    WORLD_TIERS,
)
from .data import AFFINITY_TRACKS, WORLD_NODES
from .features.affinity import build_affinity_rows
from .features.growth_effects import build_growth_effects
from .systems import (
    build_active_trail_rows,
    build_armory_rows,
    build_boss_rows,
    build_collection_rows,
    build_combat_rows,
    build_endgame_profile,
    build_ending_rows,
    build_family_rows,
    build_farming_rows,
    build_frontier_map_rows,
    build_gear_rows,
    build_hub_feature_rows,
    build_integration_rows,
    build_ledger_snapshot,
    build_world_loot_rows,
    build_world_state_rows,
    create_baseline_player,
    resolve_combat_encounter,
    resolve_world_node,
    simulate_first_ten_hours,
)
from .ui import launch_ui


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="RDR2 epilogue RPG systems prototype")
    subparsers = parser.add_subparsers(dest="command", required=True)

    subparsers.add_parser("overview", help="Show implemented content summary")
    subparsers.add_parser("ledger", help="Show a baseline Frontier Ledger snapshot")
    subparsers.add_parser("simulate", help="Run the first-10-hours style simulation")
    subparsers.add_parser("trails", help="Show the Active Trails board state")
    subparsers.add_parser("intel", help="Show the Frontier Map intel state")
    subparsers.add_parser("hub", help="Show Beecher's Hope hub feature state")
    subparsers.add_parser("gear", help="Show grounded equipment slot state")
    subparsers.add_parser("armory", help="Show weapon condition and familiarity state")
    subparsers.add_parser("dead", help="Show Ledger of the Dead boss archive state")
    subparsers.add_parser("collection", help="Show long-term collection progress")
    subparsers.add_parser("homefront", help="Show family support and ranch support state")
    combat_parser = subparsers.add_parser("combat", help="Show encounter readiness or resolve a combat test pass")
    combat_parser.add_argument("--encounter", help="Encounter id from the combat catalog")
    combat_parser.add_argument("--simulate", action="store_true", help="Resolve the encounter on the selected profile")
    combat_parser.add_argument("--simulated-profile", action="store_true", help="Use the simulated midgame profile")
    loot_parser = subparsers.add_parser("loot", help="Show world loot nodes or resolve a pickup -> stash route")
    loot_parser.add_argument("--node", help="World node id from the loot catalog")
    loot_parser.add_argument("--simulate", action="store_true", help="Resolve the node on the selected profile")
    loot_parser.add_argument("--simulated-profile", action="store_true", help="Use the simulated midgame profile")
    effects_parser = subparsers.add_parser("effects", help="Show actual gameplay modifiers derived from stats, skills, and perks")
    effects_parser.add_argument("--simulated-profile", action="store_true", help="Use the simulated midgame profile")
    subparsers.add_parser("farming", help="Show farming lane progression state")
    subparsers.add_parser("network", help="Show integrated subsystem progression state")
    subparsers.add_parser("world", help="Show world escalation and tier state")
    subparsers.add_parser("ending", help="Show ending and post-ending route state")
    subparsers.add_parser("affinity", help="Show affinity and relationship progression state")
    subparsers.add_parser("endgame", help="Show a projected endgame prototype state")
    ui_parser = subparsers.add_parser("ui", help="Launch the Frontier Ledger UI prototype")
    ui_parser.add_argument("--simulate", action="store_true", help="Launch the UI with the simulated profile")
    ui_parser.add_argument("--check", action="store_true", help="Create and destroy the UI once for validation")
    return parser


def command_overview() -> str:
    lines = [
        PROJECT_NAME,
        "",
        f"Primary hub: {PRIMARY_HUB}",
        f"First named boss: {FIRST_NAMED_BOSS}",
        f"Late-game mastermind: {FINAL_MASTERMIND}",
        "",
        "Implemented content",
        f"- Regions: {len(REGIONS)}",
        f"- Perk trees: {len(PERK_TREES)}",
        f"- Perks: {len(PERKS)}",
        f"- Recipes: {len(RECIPES)}",
        f"- Trail definitions: {len(TRAILS)}",
        f"- Region intel definitions: {len(REGION_INTEL)}",
        f"- Hub features: {len(HUB_FEATURES)}",
        f"- Gear entries: {len(GEAR_ITEMS)}",
        f"- Boss archive entries: {len(BOSSES)}",
        f"- Collection tracks: {len(COLLECTIONS)}",
        f"- Family support tracks: {len(FAMILY_SUPPORT)}",
        f"- Affinity tracks: {len(AFFINITY_TRACKS)}",
        f"- Farming lanes: {len(FARMING_LANES)}",
        f"- Integration tracks: {len(INTEGRATION_TRACKS)}",
        f"- World state tables: {len(WORLD_STATE)}",
        f"- Ending phases: {len(ENDING_PHASES)}",
        f"- World tiers: {len(WORLD_TIERS)}",
        f"- Story chapters: {len(STORY_CHAPTERS)}",
        f"- Enemy traits: {len(ENEMY_TRAITS)}",
        f"- Enemy archetypes: {len(ENEMY_ARCHETYPES)}",
        f"- Combat encounters: {len(COMBAT_ENCOUNTERS)}",
        f"- World loot nodes: {len(WORLD_NODES)}",
        f"- First 10 hour scripted activities: {len(FIRST_TEN_HOUR_SCRIPT)}",
        "",
        "Perk trees",
    ]
    for tree_name, perks in PERK_TREES.items():
        lines.append(f"- {tree_name}: {len(perks)} perks")
    return "\n".join(lines)


def command_ledger() -> str:
    player = create_baseline_player()
    return build_ledger_snapshot(player)


def command_simulate() -> str:
    player, timeline = simulate_first_ten_hours()
    sections = ["Simulation Timeline"]
    sections.extend(f"- {entry}" for entry in timeline)
    sections.extend(["", "Final Ledger", build_ledger_snapshot(player)])
    return "\n".join(sections)


def command_trails() -> str:
    player = create_baseline_player()
    lines = ["Active Trails Board", ""]
    for row in build_active_trail_rows(player):
        lines.append(f"- {row['category']} | {row['trail']} | {row['state']} | {row['region']}")
    return "\n".join(lines)


def command_intel() -> str:
    player = create_baseline_player()
    lines = ["Frontier Map Intel", ""]
    for row in build_frontier_map_rows(player):
        lines.append(f"- {row['region']} | {row['state']} | {row['danger']}")
    return "\n".join(lines)


def command_hub() -> str:
    player = create_baseline_player()
    lines = ["Beecher's Hope Hub", ""]
    for row in build_hub_feature_rows(player):
        lines.append(f"- {row['feature']} | {row['state']} | {row['output']}")
    return "\n".join(lines)


def command_gear() -> str:
    player = create_baseline_player()
    lines = ["Gear Sheet", ""]
    for row in build_gear_rows(player):
        lines.append(f"- {row['slot']} | {row['item']} | {row['state']}")
    return "\n".join(lines)


def command_armory() -> str:
    player = create_baseline_player()
    lines = ["Armory", ""]
    for row in build_armory_rows(player):
        lines.append(f"- {row['weapon']} | {row['condition']} | Familiarity {row['familiarity']}")
    return "\n".join(lines)


def command_dead() -> str:
    player = create_baseline_player()
    lines = ["Ledger of the Dead", ""]
    for row in build_boss_rows(player):
        lines.append(f"- {row['boss']} | {row['state']} | {row['region']}")
    return "\n".join(lines)


def command_collection() -> str:
    player = create_baseline_player()
    lines = ["Collection", ""]
    for row in build_collection_rows(player):
        lines.append(f"- {row['collection']} | {row['progress']} | {row['state']}")
    return "\n".join(lines)


def command_homefront() -> str:
    player = create_baseline_player()
    lines = ["Homefront", ""]
    for row in build_family_rows(player):
        lines.append(f"- {row['member']} | {row['focus']} | {row['request']} | {row['state']}")
    return "\n".join(lines)


def command_combat(encounter_id: str | None = None, simulate: bool = False, simulated_profile: bool = False) -> str:
    player = simulate_first_ten_hours()[0] if simulated_profile else create_baseline_player()
    rows = build_combat_rows(player)
    if encounter_id is None:
        lines = ["Combat Catalog", ""]
        for row in rows:
            lines.append(
                f"- {row['encounter']} | {row['type']} | {row['state']} | "
                f"{row['projection']} | {row['spawn_state']}"
            )
        return "\n".join(lines)

    if encounter_id not in COMBAT_ENCOUNTERS:
        raise ValueError(f"Unknown combat encounter '{encounter_id}'")

    row = next(item for item in rows if item["encounter"] == COMBAT_ENCOUNTERS[encounter_id].name)
    if not simulate:
        return "\n".join(
            [
                "Combat Encounter",
                "",
                f"- Encounter: {row['encounter']}",
                f"- Type: {row['type']}",
                f"- Region: {row['region']}",
                f"- State: {row['state']}",
                f"- Projection: {row['projection']}",
                f"- Spawn: {row['spawn_state']}",
                "",
                row["detail"],
            ]
        )

    timeline = resolve_combat_encounter(player, encounter_id)
    return "\n".join(["Combat Resolution", ""] + timeline + ["", "Updated Ledger", build_ledger_snapshot(player)])


def command_loot(node_id: str | None = None, simulate: bool = False, simulated_profile: bool = False) -> str:
    player = simulate_first_ten_hours()[0] if simulated_profile else create_baseline_player()
    rows = build_world_loot_rows(player)
    if node_id is None:
        lines = ["World Loot Catalog", ""]
        for row in rows:
            lines.append(
                f"- {row['node']} | {row['category']} | {row['state']} | "
                f"{row['region']} | {row['stash']}"
            )
        return "\n".join(lines)

    if node_id not in WORLD_NODES:
        raise ValueError(f"Unknown world node '{node_id}'")

    row = next(item for item in rows if item["node"] == WORLD_NODES[node_id].name)
    if not simulate:
        return "\n".join(
            [
                "World Loot Route",
                "",
                f"- Node: {row['node']}",
                f"- Category: {row['category']}",
                f"- Region: {row['region']}",
                f"- State: {row['state']}",
                f"- Stash: {row['stash']}",
                "",
                row["detail"],
            ]
        )

    timeline = resolve_world_node(player, node_id)
    return "\n".join(["World Loot Resolution", ""] + timeline + ["", "Updated Ledger", build_ledger_snapshot(player)])


def command_effects(simulated_profile: bool = False) -> str:
    player = simulate_first_ten_hours()[0] if simulated_profile else create_baseline_player()
    effects = build_growth_effects(player)
    return "\n".join(
        [
            "Growth Effects",
            "",
            f"- Combat damage bonus: +{effects['combat_damage_bonus_percent']}%",
            f"- Boss damage bonus: +{effects['boss_damage_bonus_percent']}%",
            f"- Damage resist: +{effects['damage_resist_percent']}%",
            f"- Loot yield bonus: +{effects['loot_yield_bonus_percent']}%",
            f"- Drop quality bonus: +{effects['drop_quality_bonus_percent']}%",
            f"- Travel stamina bonus: +{effects['travel_stamina_bonus_percent']}%",
            f"- Mounted control bonus: +{effects['mounted_control_bonus_percent']}%",
            f"- Craft cost reduction: -{effects['craft_cost_reduction_percent']}%",
            f"- Medicine efficiency: +{effects['medicine_efficiency_percent']}%",
        ]
    )


def command_farming() -> str:
    player = create_baseline_player()
    lines = ["Farming Lanes", ""]
    for row in build_farming_rows(player):
        lines.append(
            f"- {row['lane']} | {row['owner']} | {row['tier']} | {row['state']} | "
            f"Next {row['next_reward']}"
        )
    return "\n".join(lines)


def command_network() -> str:
    player = create_baseline_player()
    lines = ["Frontier Network", ""]
    for row in build_integration_rows(player):
        lines.append(
            f"- {row['system']} | {row['owner']} | {row['state']} | "
            f"Progress {row['progress']} | {row['milestone']}"
        )
    return "\n".join(lines)


def command_world() -> str:
    player = create_baseline_player()
    lines = ["World State", ""]
    for row in build_world_state_rows(player):
        lines.append(
            f"- {row['tier']} | {row['title']} | {row['state']} | "
            f"{row['package_state']} | {row['package']}"
        )
    return "\n".join(lines)


def command_ending() -> str:
    player = create_baseline_player()
    lines = ["Ending Route", ""]
    for row in build_ending_rows(player):
        lines.append(
            f"- {row['phase']} | {row['type']} | {row['state']} | "
            f"{row['reward_state']} | {row['reward_package']}"
        )
    return "\n".join(lines)


def command_affinity() -> str:
    player = create_baseline_player()
    lines = ["Affinity Tracks", ""]
    for row in build_affinity_rows(player):
        lines.append(
            f"- {row['npc']} | {row['stage']} | Score {row['score']} | "
            f"{row['quest_state']} | Repeatable {row['repeatable_bonus']}"
        )
    return "\n".join(lines)


def command_endgame() -> str:
    player, timeline = build_endgame_profile()
    sections = ["Endgame Projection"]
    sections.extend(f"- {entry}" for entry in timeline)
    sections.extend(["", "Endgame Ledger", build_ledger_snapshot(player)])
    return "\n".join(sections)


def main(argv: list[str] | None = None) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)

    if args.command == "overview":
        print(command_overview())
    elif args.command == "ledger":
        print(command_ledger())
    elif args.command == "simulate":
        print(command_simulate())
    elif args.command == "trails":
        print(command_trails())
    elif args.command == "intel":
        print(command_intel())
    elif args.command == "hub":
        print(command_hub())
    elif args.command == "gear":
        print(command_gear())
    elif args.command == "armory":
        print(command_armory())
    elif args.command == "dead":
        print(command_dead())
    elif args.command == "collection":
        print(command_collection())
    elif args.command == "homefront":
        print(command_homefront())
    elif args.command == "combat":
        print(command_combat(args.encounter, args.simulate, args.simulated_profile))
    elif args.command == "loot":
        print(command_loot(args.node, args.simulate, args.simulated_profile))
    elif args.command == "effects":
        print(command_effects(args.simulated_profile))
    elif args.command == "farming":
        print(command_farming())
    elif args.command == "network":
        print(command_network())
    elif args.command == "world":
        print(command_world())
    elif args.command == "ending":
        print(command_ending())
    elif args.command == "affinity":
        print(command_affinity())
    elif args.command == "endgame":
        print(command_endgame())
    elif args.command == "ui":
        result = launch_ui(simulated=args.simulate, check=args.check)
        if result:
            print(result)
    else:
        parser.error(f"Unknown command {args.command}")
    return 0
