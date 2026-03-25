from __future__ import annotations

import unittest

from frontier_rpg.content import PERKS, RECIPES, STORY_CHAPTERS
from frontier_rpg.systems import (
    allocate_stat_points,
    available_perks,
    award_xp,
    build_active_trail_rows,
    build_armory_rows,
    build_boss_rows,
    build_collection_rows,
    build_endgame_profile,
    build_ending_rows,
    build_family_rows,
    build_farming_rows,
    build_frontier_map_rows,
    build_gear_rows,
    build_hub_feature_rows,
    build_integration_rows,
    build_ledger_snapshot,
    build_world_state_rows,
    can_promote_world_tier,
    craft,
    create_baseline_player,
    meets_gate,
    recipe_status,
    simulate_first_ten_hours,
)


class FrontierSystemsTests(unittest.TestCase):
    def test_award_xp_grants_level_rewards(self) -> None:
        player = create_baseline_player()
        levels = award_xp(player, 260)

        self.assertEqual(levels, [2, 3])
        self.assertEqual(player.level, 3)
        self.assertEqual(player.unspent_stat_points, 4)
        self.assertEqual(player.unspent_perk_points, 1)

    def test_perk_unlocks_after_stat_investment(self) -> None:
        player = create_baseline_player()
        award_xp(player, 450)
        allocate_stat_points(player, ["deadeye", "deadeye", "grit", "survival"])

        perk_names = {perk.name for perk in available_perks(player)}
        self.assertIn(PERKS["gunslinger.quick_draw"].name, perk_names)
        self.assertIn(PERKS["gunslinger.iron_wrist"].name, perk_names)

    def test_crafting_consumes_materials_and_applies_upgrade(self) -> None:
        player = create_baseline_player()
        player.materials.update(
            {"iron scrap": 8, "treated leather": 4, "spring assemblies": 2, "hardwood": 4}
        )

        craft(player, "bench.expanded_workshop")

        self.assertEqual(player.workbench_tier, 2)
        self.assertEqual(player.home_upgrades, 1)
        self.assertEqual(player.materials["iron scrap"], 0)

    def test_story_gate_requires_key_item_and_growth(self) -> None:
        player = create_baseline_player()
        ready, missing = meets_gate(player, STORY_CHAPTERS["chapter_2"].gate)

        self.assertFalse(ready)
        self.assertIn("Level 2", missing)
        self.assertIn("Courier Satchel Seal 1", missing)

    def test_world_tier_two_becomes_available(self) -> None:
        player = create_baseline_player()
        player.level = 8
        player.boss_kills = 1
        player.home_upgrades = 1

        ready, missing = can_promote_world_tier(player, 2)

        self.assertTrue(ready)
        self.assertEqual(missing, [])

    def test_ledger_snapshot_mentions_project_state(self) -> None:
        player = create_baseline_player()
        snapshot = build_ledger_snapshot(player)

        self.assertIn("World Tier 1", snapshot)
        self.assertIn("Beecher's Sidearm", snapshot)
        self.assertIn("No recipes ready", snapshot)
        self.assertIn("Beecher's Hope Hub", snapshot)

    def test_doc_led_trails_are_exposed_in_baseline(self) -> None:
        player = create_baseline_player()

        rows = build_active_trail_rows(player)
        active_names = {row["trail"] for row in rows if row["state"] == "Active"}

        self.assertIn("Blood-Marked Ledger", active_names)
        self.assertIn("Dead Courier Satchel", active_names)
        self.assertIn("First Workbench Expansion", active_names)

    def test_simulation_unlocks_hub_features(self) -> None:
        player, _ = simulate_first_ten_hours()

        rows = build_hub_feature_rows(player)
        installed = {row["feature"] for row in rows if row["state"] == "Installed"}

        self.assertIn("Map Intelligence Board", installed)
        self.assertIn("Relic Stash Chest", installed)
        self.assertIn("Support Station", installed)

    def test_frontier_map_reflects_discovered_regions(self) -> None:
        player, _ = simulate_first_ten_hours()

        rows = build_frontier_map_rows(player)
        state_by_region = {row["region"]: row["state"] for row in rows}

        self.assertEqual(state_by_region["Great Plains / Tall Trees"], "Stabilized")
        self.assertIn(state_by_region["Lemoyne"], {"Known", "Stabilized"})

    def test_simulation_equips_doc_led_gear_slots(self) -> None:
        player, _ = simulate_first_ten_hours()

        rows = build_gear_rows(player)
        equipped = {row["slot"]: row["item"] for row in rows if row["state"] == "Equipped"}

        self.assertEqual(equipped["Major Accessory 2"], "Saint Medal")
        self.assertEqual(equipped["Minor Charm 1"], "Trapper Sigil")
        self.assertIn("Saddle Token", equipped)

    def test_simulation_collects_blueprints_and_named_weapon_routes(self) -> None:
        player, _ = simulate_first_ten_hours()

        self.assertIn("Blackwater Peacebreaker Diagram", player.known_blueprints)
        self.assertIn("Silas Repeater Schematic", player.known_blueprints)
        self.assertIn("Field Cabinet Plan", player.known_blueprints)
        self.assertIn("gear.saint_medal", player.crafted_recipes)
        self.assertIn("ammo.blackwater_precision_loads", player.crafted_recipes)
        armory_names = {row["weapon"] for row in build_armory_rows(player)}
        self.assertIn("Ash-Marked Repeater", armory_names)
        self.assertEqual(player.inventory.get("Saint Medal", 0), 1)
        self.assertEqual(player.inventory.get("Blackwater Precision Loads", 0), 1)

    def test_blueprint_recipe_route_unlocks_with_key_item(self) -> None:
        player, _ = simulate_first_ten_hours()
        player.materials.update({"rifled steel": 3, "tempered springs": 3, "engraved silver": 2, "elite insignias": 1})
        player.inventory["Blackwater Safehouse Ledger"] = 1

        ready, missing = recipe_status(player, RECIPES["weapon.blackwater_peacebreaker"])

        self.assertTrue(ready, msg=f"unexpected missing requirements: {missing}")
        craft(player, "weapon.blackwater_peacebreaker")
        self.assertEqual(player.inventory["Blackwater Peacebreaker"], 1)

    def test_crafted_recipe_cannot_be_recrafted_for_free_progression(self) -> None:
        player = create_baseline_player()
        player.materials.update({"saint cards": 2, "silver fittings": 2, "treated leather": 1})

        craft(player, "gear.saint_medal")
        ready, missing = recipe_status(player, RECIPES["gear.saint_medal"])

        self.assertFalse(ready)
        self.assertIn("already secured", missing)
        self.assertEqual(player.inventory["Saint Medal"], 1)

    def test_boss_archive_and_collection_progress_expand_after_simulation(self) -> None:
        player, _ = simulate_first_ten_hours()

        boss_rows = build_boss_rows(player)
        boss_states = {row["boss"]: row["state"] for row in boss_rows}
        collection_rows = build_collection_rows(player)
        collection_states = {row["collection"]: row["state"] for row in collection_rows}

        self.assertEqual(boss_states["Silas Redd, The Fence of Ash"], "Defeated")
        self.assertEqual(boss_states["Lemoyne Relic Keeper"], "Known")
        self.assertEqual(collection_states["Frontier Records"], "Complete")
        self.assertEqual(collection_states["Frontier Charms"], "Complete")

    def test_family_support_progresses_with_frontier_state(self) -> None:
        player, _ = simulate_first_ten_hours()

        rows = build_family_rows(player)
        states = {row["member"]: row["state"] for row in rows}
        requests = {row["member"]: row["request"] for row in rows}

        self.assertEqual(states["Abigail"], "Active")
        self.assertEqual(states["Jack"], "Active")
        self.assertEqual(states["Uncle"], "Active")
        self.assertIn("medicine", requests["Abigail"].lower())

    def test_farming_and_network_lanes_open_in_simulation(self) -> None:
        player, _ = simulate_first_ten_hours()

        farming_rows = build_farming_rows(player)
        farming_states = {row["lane"]: row["state"] for row in farming_rows}
        farming_owners = {row["lane"]: row["owner"] for row in farming_rows}
        farming_rewards = {row["lane"]: row["next_reward"] for row in farming_rows}
        network_rows = build_integration_rows(player)
        network_states = {row["system"]: row["state"] for row in network_rows}
        network_details = {row["system"]: row["detail"] for row in network_rows}
        network_hooks = {row["system"]: row["hook"] for row in network_rows}
        network_milestones = {row["system"]: row["milestone"] for row in network_rows}

        self.assertEqual(farming_states["Contract Lane"], "Active")
        self.assertEqual(farming_states["Boss Lane"], "Mastered")
        self.assertEqual(farming_owners["Contract Lane"], "Uncle")
        self.assertEqual(farming_rewards["Frontier Work Lane"], "All route packages secured")
        self.assertEqual(network_states["Contracts"], "Integrated")
        self.assertEqual(network_states["Stagecoaches And Money Wagons"], "Integrated")
        self.assertIn("Repeatable payout bonus", network_details["Contracts"])
        self.assertIn("Blackwater", network_hooks["Contracts"])
        self.assertEqual(network_milestones["Contracts"], "Route packages secured")
        self.assertIn("Ranch Wage Token", player.inventory)
        self.assertIn("Blackwater Broker File", player.inventory)

    def test_affinity_gated_perks_unlock_into_ready_pool(self) -> None:
        player, _ = simulate_first_ten_hours()

        perk_names = {perk.name for perk in available_perks(player)}
        unlocked_names = {PERKS[perk_id].name for perk_id in player.unlocked_perks}

        self.assertIn("Household Reserve", unlocked_names)
        self.assertIn("Greased Palm", perk_names)
        self.assertIn("Margin Notes", perk_names)

    def test_world_state_rows_track_current_and_ready_tiers(self) -> None:
        player, _ = simulate_first_ten_hours()

        rows = build_world_state_rows(player)
        state_by_tier = {row["tier"]: row["state"] for row in rows}
        checkpoints = {row["tier"]: row["checkpoint"] for row in rows}
        package_states = {row["tier"]: row["package_state"] for row in rows}

        self.assertEqual(state_by_tier["WT 3"], "Current")
        self.assertEqual(state_by_tier["WT 4"], "Locked")
        self.assertIn("Level 22", checkpoints["WT 4"])
        self.assertEqual(package_states["WT 3"], "Secured")
        self.assertIn("Relic War Dossier", player.inventory)

    def test_endgame_profile_completes_campaign_projection(self) -> None:
        player, timeline = build_endgame_profile()

        self.assertGreaterEqual(player.level, 30)
        self.assertEqual(player.world_tier, 5)
        self.assertEqual(player.workbench_tier, 4)
        self.assertEqual(player.home_upgrades, 3)
        self.assertIn("World state forced to After The Ledger for prototype validation.", timeline)

        ending_states = {row["phase"]: row["state"] for row in build_ending_rows(player)}
        ending_focus = {row["phase"]: row["focus"] for row in build_ending_rows(player)}
        ending_rewards = {row["phase"]: row["reward_state"] for row in build_ending_rows(player)}
        collection_states = {row["collection"]: row["state"] for row in build_collection_rows(player)}
        self.assertEqual(ending_states["Close The Ledger"], "Completed")
        self.assertEqual(ending_states["After The Ledger"], "Completed")
        self.assertIn("campaign record", ending_focus["Close The Ledger"])
        self.assertEqual(ending_rewards["After The Ledger"], "Secured")
        self.assertIn("Sandbox Charter", player.inventory)
        self.assertEqual(collection_states["Frontier Charms"], "Complete")


if __name__ == "__main__":
    unittest.main()
