from __future__ import annotations

import configparser
import unittest
from pathlib import Path

from frontier_rpg.systems import build_endgame_profile, create_baseline_player, simulate_first_ten_hours
from ingame_mod.scripts.export_ledger_data import build_dataset, build_pages


class IngameExportTests(unittest.TestCase):
    def test_native_export_includes_remaining_doc_led_pages(self) -> None:
        player = create_baseline_player()

        pages = build_pages(player)
        page_ids = [page["page_id"] for page in pages]

        self.assertEqual(
            page_ids,
            [
                "overview",
                "trails",
                "frontier_map",
                "homefront",
                "affinity",
                "stats",
                "gear",
                "armory",
                "skills",
                "perks",
                "crafting",
                "upgrades",
                "dead",
                "collection",
                "farming",
                "network",
                "world",
                "ending",
                "encyclopedia",
            ],
        )

    def test_simulated_dataset_keeps_new_pages_populated(self) -> None:
        player, _ = simulate_first_ten_hours()

        dataset = build_dataset("simulated_en", "Simulated Frontier Run", player, "en")
        page_map = {page["page_id"]: page for page in dataset["pages"]}

        self.assertGreater(len(page_map["affinity"]["entries"]), 0)
        self.assertGreater(len(page_map["farming"]["entries"]), 0)
        self.assertGreater(len(page_map["network"]["entries"]), 0)
        self.assertGreater(len(page_map["world"]["entries"]), 0)
        self.assertGreater(len(page_map["ending"]["entries"]), 0)
        self.assertEqual(len(page_map["crafting"]["entries"]), 13)
        self.assertEqual(len(page_map["dead"]["entries"]), 5)
        self.assertIn("Master Route", "".join(entry["label"] for entry in page_map["farming"]["entries"]))
        self.assertIn("Repeatable payout bonus", page_map["network"]["entries"][0]["detail"])
        self.assertIn("Checkpoint", page_map["world"]["entries"][0]["detail"])
        self.assertTrue(any("Blueprint:" in entry["detail"] for entry in page_map["crafting"]["entries"]))
        self.assertTrue(any("Field Support Cabinet" in entry["label"] for entry in page_map["crafting"]["entries"]))
        self.assertTrue(any("Relic Marked Rounds" in entry["label"] for entry in page_map["crafting"]["entries"]))
        self.assertTrue(any("Tall Trees Tracker King" in entry["label"] for entry in page_map["dead"]["entries"]))
        self.assertTrue(any("Surveyor" in entry["label"] for entry in page_map["dead"]["entries"]))
        self.assertTrue(any("Blueprint" in entry["label"] for entry in page_map["encyclopedia"]["entries"]))

    def test_endgame_dataset_carries_post_ending_pages(self) -> None:
        player, _ = build_endgame_profile()

        dataset = build_dataset("endgame_en", "After The Ledger", player, "en")
        page_map = {page["page_id"]: page for page in dataset["pages"]}
        ending_labels = [entry["label"] for entry in page_map["ending"]["entries"]]

        self.assertIn(
            "After The Ledger | Post-Ending | Completed | Secured | After The Ledger Sandbox Charter",
            ending_labels,
        )
        self.assertEqual(dataset["dataset_id"], "endgame_en")

    def test_packaged_ini_defaults_to_english_first_ingame_renderer(self) -> None:
        config = configparser.ConfigParser()
        config.read(Path(__file__).resolve().parents[1] / "ingame_mod" / "FrontierLedger.ini", encoding="utf-8")

        self.assertEqual(config["Renderer"]["UseDesktopOverlay"], "0")
        self.assertEqual(config["Data"]["DefaultDataset"], "live_en")
        self.assertEqual(config["Data"]["PreferredLocale"], "en")


if __name__ == "__main__":
    unittest.main()
