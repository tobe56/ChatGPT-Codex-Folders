from __future__ import annotations

from ..models import PlayerProfile


def xp_for_next_level(level: int) -> int:
    return 100 + ((level - 1) * 50)


def total_xp_required_for_level(level: int) -> int:
    if level <= 1:
        return 0
    return sum(xp_for_next_level(current) for current in range(1, level))


def xp_progress(player: PlayerProfile) -> tuple[int, int]:
    current_floor = total_xp_required_for_level(player.level)
    next_requirement = xp_for_next_level(player.level)
    return player.total_xp - current_floor, next_requirement
