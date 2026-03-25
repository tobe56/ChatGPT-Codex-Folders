from __future__ import annotations

import unittest

from frontier_rpg.data import AFFINITY_TRACKS
from frontier_rpg.features.affinity import (
    affinity_stage,
    apply_affinity_dialogue_choice,
    apply_affinity_gift,
    build_affinity_rows,
    ensure_affinity_state,
    repeatable_bonus_for_system,
    repeatable_reward_bonus_percent,
)
from frontier_rpg.systems import create_baseline_player, simulate_first_ten_hours


class FrontierAffinityTests(unittest.TestCase):
    def test_baseline_affinity_state_is_seeded_for_main_family_npcs(self) -> None:
        player = create_baseline_player()
        ensure_affinity_state(player)

        self.assertIn("affinity.abigail", player.affinity_points)
        self.assertIn("affinity.jack", player.affinity_points)
        self.assertIn("affinity.uncle", player.affinity_points)

    def test_world_tier_gates_late_affinity_stages(self) -> None:
        player = create_baseline_player()
        ensure_affinity_state(player)
        player.affinity_points["affinity.abigail"] = 200

        self.assertEqual(affinity_stage(player, AFFINITY_TRACKS["affinity.abigail"]), 3)

        player.world_tier = 2
        self.assertEqual(affinity_stage(player, AFFINITY_TRACKS["affinity.abigail"]), 4)

    def test_simulation_rows_show_repeatable_bonus_and_next_rewards(self) -> None:
        player, _ = simulate_first_ten_hours()

        rows = build_affinity_rows(player)
        row_map = {row["npc"]: row for row in rows}

        self.assertIn("Repeatable bonus", row_map["Uncle"]["detail"])
        self.assertIn("Next quest", row_map["Abigail"]["detail"])
        self.assertIn("Quest state", row_map["Jack"]["detail"])
        self.assertGreaterEqual(repeatable_reward_bonus_percent(player, AFFINITY_TRACKS["affinity.uncle"]), 0)

    def test_repeatable_bonus_can_be_mapped_to_network_systems(self) -> None:
        player, _ = simulate_first_ten_hours()

        npc_name, bonus = repeatable_bonus_for_system(player, "Contracts")

        self.assertEqual(npc_name, "Uncle")
        self.assertGreaterEqual(bonus, 0)

    def test_gifts_and_dialogue_can_raise_or_lower_affinity(self) -> None:
        player = create_baseline_player()
        ensure_affinity_state(player)

        gift_gain = apply_affinity_gift(player, "affinity.abigail", "medicine crate")
        dialogue_delta = apply_affinity_dialogue_choice(player, "affinity.abigail", -4)

        self.assertEqual(gift_gain, 8)
        self.assertEqual(dialogue_delta, -4)
        self.assertGreaterEqual(player.affinity_points["affinity.abigail"], 0)

    def test_simulation_completes_midgame_affinity_quests(self) -> None:
        player, _ = simulate_first_ten_hours()

        self.assertIn("affinity.abigail.stage2", player.completed_affinity_quests)
        self.assertIn("affinity.jack.stage2", player.completed_affinity_quests)
        self.assertIn("affinity.uncle.stage2", player.completed_affinity_quests)
