from __future__ import annotations

import unittest
from copy import deepcopy

from frontier_rpg.features.combat import build_combat_activity
from frontier_rpg.systems import (
    build_boss_rows,
    build_collection_rows,
    build_combat_rows,
    meets_gate,
    resolve_combat_encounter,
    simulate_first_ten_hours,
)


class FrontierCombatTests(unittest.TestCase):
    def test_combat_catalog_surfaces_projection_and_spawn_plan(self) -> None:
        player, _ = simulate_first_ten_hours()

        row_map = {row["encounter"]: row for row in build_combat_rows(player)}
        silas_row = row_map["Silas Redd, The Fence of Ash"]

        self.assertIn(silas_row["projection"], {"Favored", "Contested", "Barely Clear", "Already Cleared"})
        self.assertIn("Test Spawn:", silas_row["detail"])
        self.assertIn("Combat: HP", silas_row["detail"])

    def test_resolving_broker_cell_marks_individual_boss_defeat(self) -> None:
        player, _ = simulate_first_ten_hours()
        self.assertNotIn("boss.blackwater_broker_cell", player.defeated_bosses)

        timeline = resolve_combat_encounter(player, "encounter.blackwater_broker_cell")

        self.assertIn("boss.blackwater_broker_cell", player.defeated_bosses)
        boss_states = {row["boss"]: row["state"] for row in build_boss_rows(player)}
        collection_progress = {row["collection"]: row["progress"] for row in build_collection_rows(player)}

        self.assertEqual(boss_states["Blackwater Broker Cell"], "Defeated")
        self.assertEqual(collection_progress["House Trophies"], "2/3")
        self.assertTrue(any("Boss defeats -> Blackwater Broker Cell" in line for line in timeline))

    def test_grave_luck_increases_boss_material_yield(self) -> None:
        player, _ = simulate_first_ten_hours()
        base_activity, _ = build_combat_activity(deepcopy(player), "encounter.blackwater_broker_cell", meets_gate)

        boosted_player = deepcopy(player)
        boosted_player.unlocked_perks.add("relic_hunter.grave_luck")
        boosted_activity, _ = build_combat_activity(boosted_player, "encounter.blackwater_broker_cell", meets_gate)

        self.assertGreater(
            boosted_activity.materials["engraved silver"],
            base_activity.materials["engraved silver"],
        )


if __name__ == "__main__":
    unittest.main()
