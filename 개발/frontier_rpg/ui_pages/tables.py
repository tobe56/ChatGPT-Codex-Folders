from __future__ import annotations

from typing import cast

from ..features.affinity import build_affinity_rows
from ..models import PlayerProfile
from ..systems import (
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


def build_trail_rows(player: PlayerProfile) -> list[dict[str, str]]:
    return build_active_trail_rows(player)


def build_frontier_map_table_rows(player: PlayerProfile) -> list[dict[str, str]]:
    return build_frontier_map_rows(player)


def build_hub_rows(player: PlayerProfile) -> list[dict[str, str]]:
    return build_hub_feature_rows(player)


def build_gear_table_rows(player: PlayerProfile) -> list[dict[str, str]]:
    return build_gear_rows(player)


def build_armory_table_rows(player: PlayerProfile) -> list[dict[str, str]]:
    return build_armory_rows(player)


def build_dead_ledger_rows(player: PlayerProfile) -> list[dict[str, str]]:
    return build_boss_rows(player)


def build_collection_table_rows(player: PlayerProfile) -> list[dict[str, str]]:
    return build_collection_rows(player)


def build_homefront_rows(player: PlayerProfile) -> list[dict[str, str]]:
    return build_family_rows(player)


def build_affinity_table_rows(player: PlayerProfile) -> list[dict[str, str]]:
    return cast(list[dict[str, str]], build_affinity_rows(player))


def build_farming_table_rows(player: PlayerProfile) -> list[dict[str, str]]:
    return build_farming_rows(player)


def build_network_rows(player: PlayerProfile) -> list[dict[str, str]]:
    return build_integration_rows(player)


def build_world_state_table_rows(player: PlayerProfile) -> list[dict[str, str]]:
    return build_world_state_rows(player)


def build_ending_table_rows(player: PlayerProfile) -> list[dict[str, str]]:
    return build_ending_rows(player)

