from __future__ import annotations

import unittest

from frontier_rpg.features.farming import claim_available_farming_milestones
from frontier_rpg.features.network import claim_available_network_milestones
from frontier_rpg.systems import create_baseline_player


class FrontierRewardPackageTests(unittest.TestCase):
    def test_farming_milestones_are_claimed_once_per_threshold(self) -> None:
        player = create_baseline_player()
        player.farming_progress["lane.contract"] = 5

        claimed = claim_available_farming_milestones(player)
        claimed_again = claim_available_farming_milestones(player)

        self.assertEqual(len(claimed), 3)
        self.assertEqual(claimed_again, [])
        self.assertEqual(player.inventory["Priority Contract Packet"], 1)
        self.assertIn("Contract Lane Master Route", player.collection_entries)

    def test_network_milestones_are_claimed_once_per_threshold(self) -> None:
        player = create_baseline_player()
        player.network_progress["track.contracts"] = 3

        claimed = claim_available_network_milestones(player)
        claimed_again = claim_available_network_milestones(player)

        self.assertEqual(len(claimed), 2)
        self.assertEqual(claimed_again, [])
        self.assertEqual(player.inventory["Blackwater Broker File"], 1)
        self.assertIn("Contracts Route Integrated", player.collection_entries)


if __name__ == "__main__":
    unittest.main()
