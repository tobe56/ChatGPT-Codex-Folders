from __future__ import annotations

import unittest

from frontier_rpg.content import RECIPES
from frontier_rpg.data import WORLD_NODES
from frontier_rpg.features.growth_effects import build_growth_effects
from frontier_rpg.features.world_loot import effective_recipe_cost
from frontier_rpg.systems import create_baseline_player, resolve_world_node, simulate_first_ten_hours


class FrontierWorldLootTests(unittest.TestCase):
    def test_world_node_routes_field_loot_into_stash(self) -> None:
        player = create_baseline_player()

        timeline = resolve_world_node(player, "node.beechers_supply_drop")

        self.assertIn("node.beechers_supply_drop", player.cleared_world_nodes)
        self.assertEqual(sum(player.materials.values()), 0)
        self.assertGreater(sum(player.stash_materials.values()), 0)
        self.assertGreater(sum(player.stash_inventory.values()), 0)
        self.assertEqual(player.stash_returns, 1)
        self.assertTrue(any("Deposited materials" in line for line in timeline))
        self.assertTrue(any("Deposited picked loot" in line for line in timeline))

    def test_growth_driven_recipe_cost_reduction_uses_skills_and_perks(self) -> None:
        player = simulate_first_ten_hours()[0]
        baseline = effective_recipe_cost(create_baseline_player(), RECIPES["bench.expanded_workshop"])
        reduced = effective_recipe_cost(player, RECIPES["bench.expanded_workshop"])

        self.assertLess(reduced["iron scrap"], baseline["iron scrap"])
        self.assertLess(reduced["hardwood"], baseline["hardwood"])

    def test_growth_effects_expand_after_simulation(self) -> None:
        baseline = build_growth_effects(create_baseline_player())
        simulated = build_growth_effects(simulate_first_ten_hours()[0])

        self.assertGreater(simulated["combat_damage_bonus_percent"], baseline["combat_damage_bonus_percent"])
        self.assertGreater(simulated["loot_yield_bonus_percent"], baseline["loot_yield_bonus_percent"])
        self.assertGreater(simulated["craft_cost_reduction_percent"], baseline["craft_cost_reduction_percent"])

    def test_world_nodes_now_cover_more_regions_and_destinations(self) -> None:
        destinations = {node.stash_destination for node in WORLD_NODES.values()}
        regions = {node.region for node in WORLD_NODES.values()}

        self.assertGreaterEqual(len(WORLD_NODES), 14)
        self.assertGreaterEqual(len(destinations), 6)
        self.assertIn("ambarino", regions)
        self.assertIn("node.blackwater_dead_courier", WORLD_NODES)
        self.assertIn("node.rusk_contract_holdout", WORLD_NODES)

    def test_simulated_profile_can_clear_new_courier_node(self) -> None:
        player = simulate_first_ten_hours()[0]

        timeline = resolve_world_node(player, "node.blackwater_dead_courier")

        self.assertIn("node.blackwater_dead_courier", player.cleared_world_nodes)
        self.assertTrue(any("Courier recovery seed" in line for line in timeline))
        self.assertGreater(player.stash_returns, 0)


if __name__ == "__main__":
    unittest.main()
