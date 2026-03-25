from __future__ import annotations

import unittest

from frontier_rpg.systems import create_baseline_player, simulate_first_ten_hours
from frontier_rpg.ui_state import (
    build_compact_sections,
    build_crafting_rows,
    build_encyclopedia_rows,
    build_perk_rows,
    build_skill_rows,
    build_stats_rows,
    build_upgrade_rows,
)


class FrontierUiStateTests(unittest.TestCase):
    def test_compact_sections_include_required_panels(self) -> None:
        player = create_baseline_player()
        sections = build_compact_sections(player)

        self.assertEqual(set(sections), {"Frontier Status", "Quick Intel", "Ready Now"})
        self.assertTrue(sections["Frontier Status"])
        self.assertTrue(any("Affinity:" in line for line in sections["Quick Intel"]))

    def test_full_tabs_have_rows(self) -> None:
        player = simulate_first_ten_hours()[0]
        encyclopedia_rows = build_encyclopedia_rows(player)
        crafting_rows = build_crafting_rows(player)
        upgrade_rows = build_upgrade_rows(player)

        self.assertGreater(len(build_stats_rows(player)), 0)
        self.assertGreater(len(build_skill_rows(player)), 0)
        self.assertGreater(len(build_perk_rows(player)), 0)
        self.assertEqual(len(crafting_rows), 13)
        self.assertGreater(len(upgrade_rows), 0)
        self.assertGreater(len(encyclopedia_rows), 0)
        self.assertEqual(crafting_rows[0]["recipe"], "Reliable Grip Wrap")
        self.assertEqual(crafting_rows[-1]["recipe"], "Relic Marked Rounds")
        self.assertTrue(any(row["recipe"] == "Field Support Cabinet" for row in crafting_rows))
        self.assertTrue(any(row["recipe"] == "Ash-Marked Repeater" for row in crafting_rows))
        self.assertTrue(any(row["category"] == "World Route" for row in encyclopedia_rows))
        self.assertTrue(any("Blueprint:" in row["detail"] for row in crafting_rows))
        self.assertTrue(any(row["category"] == "Ammo Load" for row in upgrade_rows))
        self.assertTrue(any(row["category"] == "Blueprint" for row in encyclopedia_rows))


if __name__ == "__main__":
    unittest.main()
