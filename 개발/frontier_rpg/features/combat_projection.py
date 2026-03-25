from __future__ import annotations

from collections.abc import Callable

from ..models import CombatEncounterDefinition, PlayerProfile
from .combat_math import combat_has_perk, player_combat_profile
from .combat_types import CombatPreview


def preview_combat_encounter(
    player: PlayerProfile,
    encounter: CombatEncounterDefinition,
    gate_evaluator: Callable[[PlayerProfile, object], tuple[bool, list[str]]],
) -> CombatPreview:
    ready, missing = gate_evaluator(player, encounter.gate)
    stats = player_combat_profile(player)
    if not ready:
        return {
            "state": "Locked",
            "projection": "Blocked",
            "missing": missing,
            "profile": stats,
            "player_health_remaining": stats["max_health"],
            "enemy_health_remaining": encounter.max_health,
            "rounds": 0,
            "phases_triggered": [],
        }

    enemy_health = encounter.max_health
    player_health = stats["max_health"]
    rounds = 0
    phases_triggered: list[str] = []
    active_weaknesses = dict(encounter.weaknesses)
    active_resistances = dict(encounter.resistances)
    active_pressure = encounter.pressure + (len(encounter.support_archetypes) * 2)
    tags = stats["tags"]
    phase_queue = sorted(encounter.phases, key=lambda item: item.trigger_health_percent, reverse=True)

    while enemy_health > 0 and player_health > 0 and rounds < 8:
        rounds += 1
        weakness_bonus = sum(value for key, value in active_weaknesses.items() if key in tags)
        resistance_penalty = sum(value for key, value in active_resistances.items() if key in tags)
        effective_attack = stats["attack"] + stats["boss_damage"] + weakness_bonus - resistance_penalty
        armor_gap = max(0, encounter.armor - stats["armor_break"])
        effective_attack -= armor_gap // 4
        if encounter.encounter_type == "boss":
            effective_attack += 4
        effective_attack = max(12, effective_attack)

        enemy_attack = encounter.damage + (encounter.accuracy // 8) + (encounter.aggression // 10) + (active_pressure // 6)
        enemy_attack -= stats["mitigation"] // 5
        enemy_attack -= stats["pressure_guard"] // 6
        if "stagger" in tags and "stagger" in active_weaknesses:
            enemy_attack -= 3
        enemy_attack = max(6, enemy_attack)

        enemy_health -= effective_attack
        player_health -= enemy_attack

        current_percent = max(0, int((enemy_health / encounter.max_health) * 100))
        for phase in phase_queue:
            if phase.name in phases_triggered:
                continue
            if current_percent <= phase.trigger_health_percent:
                phases_triggered.append(phase.name)
                active_pressure += phase.pressure_bonus
                for weakness in phase.exposed_weaknesses:
                    active_weaknesses[weakness] = active_weaknesses.get(weakness, 0) + 8

    player_percent = max(0, int((player_health / stats["max_health"]) * 100))
    if enemy_health <= 0 and player_percent >= 45:
        projection = "Favored"
    elif enemy_health <= 0 and player_percent >= 18:
        projection = "Contested"
    elif enemy_health <= 0:
        projection = "Barely Clear"
    else:
        projection = "Overrun"

    return {
        "state": "Ready",
        "projection": projection,
        "missing": [],
        "profile": stats,
        "player_health_remaining": max(0, player_health),
        "enemy_health_remaining": max(0, enemy_health),
        "rounds": rounds,
        "phases_triggered": phases_triggered,
    }


def scaled_rewards(player: PlayerProfile, encounter: CombatEncounterDefinition) -> tuple[dict[str, int], dict[str, int]]:
    stats = player_combat_profile(player)
    materials = dict(encounter.loot_materials)
    items = dict(encounter.loot_items)
    loot_bonus = stats["loot_bonus_percent"]
    for material_name, amount in list(materials.items()):
        bonus = amount * loot_bonus // 100
        if encounter.encounter_type == "boss" and combat_has_perk(player, "relic_hunter.grave_luck"):
            bonus += 1
        materials[material_name] = amount + bonus
    if encounter.encounter_type == "boss" and combat_has_perk(player, "outlaw.greased_palm") and "Broker" in encounter.name:
        items["Broker Cut"] = items.get("Broker Cut", 0) + 1
    return materials, items
