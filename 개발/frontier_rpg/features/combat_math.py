from __future__ import annotations

from ..models import PlayerProfile
from .combat_types import CombatProfile
from .growth_effects import build_growth_effects


def combat_has_perk(player: PlayerProfile, perk_id: str) -> bool:
    return perk_id in player.unlocked_perks


def _all_weapons_clean(player: PlayerProfile) -> bool:
    if not player.weapons:
        return False
    return all(weapon.condition in {"Pristine", "Clean"} for weapon in player.weapons)


def _average_familiarity(player: PlayerProfile) -> int:
    if not player.weapons:
        return 0
    return int(round(sum(weapon.familiarity for weapon in player.weapons) / len(player.weapons)))


def _player_attack_tags(player: PlayerProfile) -> set[str]:
    tags: set[str] = set()
    categories = {weapon.category for weapon in player.weapons}
    if categories & {"Revolver", "Pistol"}:
        tags.add("revolver")
    if player.stats.deadeye >= 4 or combat_has_perk(player, "gunslinger.front_sight_faith"):
        tags.add("precision")
    if player.stats.strength >= 4 or combat_has_perk(player, "survivor.ranch_resolve"):
        tags.add("stagger")
    if player.stats.survival >= 4 or combat_has_perk(player, "frontiersman.sharp_tracker"):
        tags.add("tracking")
    if player.stats.cunning >= 4 or combat_has_perk(player, "outlaw.fast_pockets"):
        tags.add("flank")
    if player.relic_count >= 2 or combat_has_perk(player, "relic_hunter.grave_luck") or combat_has_perk(player, "relic_hunter.ash_mark"):
        tags.add("relic")
    if combat_has_perk(player, "outlaw.dirty_trigger"):
        tags.add("fear")
    return tags


def player_combat_profile(player: PlayerProfile) -> CombatProfile:
    effects = build_growth_effects(player)
    average_familiarity = _average_familiarity(player)
    tags = _player_attack_tags(player)
    attack = (
        26
        + (player.stats.strength * 3)
        + (player.stats.deadeye * 3)
        + average_familiarity
        + player.lifestyle.gunsmithing
    )
    if combat_has_perk(player, "gunslinger.quick_draw"):
        attack += 4
    if combat_has_perk(player, "gunslinger.iron_wrist"):
        attack += 5
    if combat_has_perk(player, "gunslinger.front_sight_faith") and _all_weapons_clean(player):
        attack += 4
    if combat_has_perk(player, "relic_hunter.ash_mark"):
        attack += 8
    if combat_has_perk(player, "survivor.ranch_resolve") and player.home_upgrades >= 1:
        attack += 3

    armor_break = 8 + (player.stats.strength * 2) + (player.workbench_tier * 2)
    if combat_has_perk(player, "gunslinger.iron_wrist"):
        armor_break += 4
    if combat_has_perk(player, "relic_hunter.ash_mark"):
        armor_break += 6

    mitigation = 12 + (player.stats.grit * 4) + (player.stats.survival * 2) + (player.home_upgrades * 2)
    if combat_has_perk(player, "survivor.thick_skin"):
        mitigation += 7
    if combat_has_perk(player, "survivor.ranch_resolve"):
        mitigation += 4
    if "Saint Medal" in player.inventory:
        mitigation += 2

    max_health = 120 + (player.stats.grit * 18) + (player.stats.survival * 6) + (player.home_upgrades * 8)
    if combat_has_perk(player, "survivor.thick_skin"):
        max_health += 18
    if "Saint Medal" in player.inventory:
        max_health += 8

    pressure_guard = 8 + (player.stats.cunning * 2) + (player.stats.survival * 2) + player.lifestyle.horse_handling
    if combat_has_perk(player, "frontiersman.long_trail"):
        pressure_guard += 3
    if combat_has_perk(player, "frontiersman.sharp_tracker"):
        pressure_guard += 4

    attack += int(effects["combat_damage_bonus_percent"]) // 6
    mitigation += int(effects["damage_resist_percent"]) // 4
    pressure_guard += int(effects["travel_stamina_bonus_percent"]) // 8

    return {
        "attack": attack,
        "armor_break": armor_break,
        "mitigation": mitigation,
        "max_health": max_health,
        "pressure_guard": pressure_guard,
        "boss_damage": int(effects["boss_damage_bonus_percent"]),
        "loot_bonus_percent": int(effects["loot_yield_bonus_percent"]),
        "average_familiarity": average_familiarity,
        "tags": tags,
    }
