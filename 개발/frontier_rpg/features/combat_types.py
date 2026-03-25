from __future__ import annotations

from typing import TypedDict


class CombatProfile(TypedDict):
    attack: int
    armor_break: int
    mitigation: int
    max_health: int
    pressure_guard: int
    boss_damage: int
    loot_bonus_percent: int
    average_familiarity: int
    tags: set[str]


class CombatPreview(TypedDict):
    state: str
    projection: str
    missing: list[str]
    profile: CombatProfile
    player_health_remaining: int
    enemy_health_remaining: int
    rounds: int
    phases_triggered: list[str]
