from __future__ import annotations

from collections.abc import Callable
from copy import deepcopy

from ..content import COMBAT_ENCOUNTERS, ENEMY_ARCHETYPES, ENEMY_TRAITS, REGIONS
from ..models import ActivityDefinition, CombatEncounterDefinition, PlayerProfile
from ..platform.localization import gameplay_text
from .growth_effects import build_growth_effects


def _encounter_sort_key(encounter: CombatEncounterDefinition) -> tuple[int, str, str]:
    priority = 1 if encounter.encounter_type == "boss" else 0
    return (priority, REGIONS[encounter.region]["name"], encounter.name)


def _find_encounter(source_id: str) -> CombatEncounterDefinition | None:
    for encounter in COMBAT_ENCOUNTERS.values():
        if encounter.source_id == source_id:
            return encounter
    return None


def _player_has_perk(player: PlayerProfile, perk_id: str) -> bool:
    return perk_id in player.unlocked_perks


def _all_weapons_clean(player: PlayerProfile) -> bool:
    if not player.weapons:
        return False
    return all(weapon.condition in {"Pristine", "Clean"} for weapon in player.weapons)


def _average_familiarity(player: PlayerProfile) -> int:
    if not player.weapons:
        return 0
    return round(sum(weapon.familiarity for weapon in player.weapons) / len(player.weapons))


def _player_attack_tags(player: PlayerProfile) -> set[str]:
    tags: set[str] = set()
    categories = {weapon.category for weapon in player.weapons}
    if categories & {"Revolver", "Pistol"}:
        tags.add("revolver")
    if player.stats.deadeye >= 4 or _player_has_perk(player, "gunslinger.front_sight_faith"):
        tags.add("precision")
    if player.stats.strength >= 4 or _player_has_perk(player, "survivor.ranch_resolve"):
        tags.add("stagger")
    if player.stats.survival >= 4 or _player_has_perk(player, "frontiersman.sharp_tracker"):
        tags.add("tracking")
    if player.stats.cunning >= 4 or _player_has_perk(player, "outlaw.fast_pockets"):
        tags.add("flank")
    if player.relic_count >= 2 or _player_has_perk(player, "relic_hunter.grave_luck") or _player_has_perk(player, "relic_hunter.ash_mark"):
        tags.add("relic")
    if _player_has_perk(player, "outlaw.dirty_trigger"):
        tags.add("fear")
    return tags


def player_combat_profile(player: PlayerProfile) -> dict[str, object]:
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
    if _player_has_perk(player, "gunslinger.quick_draw"):
        attack += 4
    if _player_has_perk(player, "gunslinger.iron_wrist"):
        attack += 5
    if _player_has_perk(player, "gunslinger.front_sight_faith") and _all_weapons_clean(player):
        attack += 4
    if _player_has_perk(player, "relic_hunter.ash_mark"):
        attack += 8
    if _player_has_perk(player, "survivor.ranch_resolve") and player.home_upgrades >= 1:
        attack += 3

    armor_break = 8 + (player.stats.strength * 2) + (player.workbench_tier * 2)
    if _player_has_perk(player, "gunslinger.iron_wrist"):
        armor_break += 4
    if _player_has_perk(player, "relic_hunter.ash_mark"):
        armor_break += 6

    mitigation = 12 + (player.stats.grit * 4) + (player.stats.survival * 2) + (player.home_upgrades * 2)
    if _player_has_perk(player, "survivor.thick_skin"):
        mitigation += 7
    if _player_has_perk(player, "survivor.ranch_resolve"):
        mitigation += 4
    if "Saint Medal" in player.inventory:
        mitigation += 2

    max_health = 120 + (player.stats.grit * 18) + (player.stats.survival * 6) + (player.home_upgrades * 8)
    if _player_has_perk(player, "survivor.thick_skin"):
        max_health += 18
    if "Saint Medal" in player.inventory:
        max_health += 8

    pressure_guard = 8 + (player.stats.cunning * 2) + (player.stats.survival * 2) + player.lifestyle.horse_handling
    if _player_has_perk(player, "frontiersman.long_trail"):
        pressure_guard += 3
    if _player_has_perk(player, "frontiersman.sharp_tracker"):
        pressure_guard += 4

    attack += int(effects["combat_damage_bonus_percent"]) // 6
    mitigation += int(effects["damage_resist_percent"]) // 4
    pressure_guard += int(effects["travel_stamina_bonus_percent"]) // 8
    loot_bonus_percent = int(effects["loot_yield_bonus_percent"])

    return {
        "attack": attack,
        "armor_break": armor_break,
        "mitigation": mitigation,
        "max_health": max_health,
        "pressure_guard": pressure_guard,
        "boss_damage": int(effects["boss_damage_bonus_percent"]),
        "loot_bonus_percent": loot_bonus_percent,
        "average_familiarity": average_familiarity,
        "tags": tags,
    }


def _modifier_lines(mapping: dict[str, int] | object) -> str:
    if not mapping:
        return "None"
    if isinstance(mapping, dict):
        items = mapping.items()
    else:
        items = []
    return ", ".join(f"{key.replace('_', ' ').title()} {value}%" for key, value in items)


def _support_labels(encounter: CombatEncounterDefinition, phases_triggered: list[str]) -> str:
    labels: list[str] = []
    for archetype_id in encounter.support_archetypes:
        labels.append(ENEMY_ARCHETYPES[archetype_id].name)
    for phase_name in phases_triggered:
        labels.append(phase_name)
    return ", ".join(labels) if labels else "None"


def preview_combat_encounter(
    player: PlayerProfile,
    encounter: CombatEncounterDefinition,
    gate_evaluator: Callable[[PlayerProfile, object], tuple[bool, list[str]]],
) -> dict[str, object]:
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
    player_health = int(stats["max_health"])
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
        effective_attack = int(stats["attack"]) + int(stats["boss_damage"]) + weakness_bonus - resistance_penalty
        armor_gap = max(0, encounter.armor - int(stats["armor_break"]))
        effective_attack -= armor_gap // 4
        if encounter.encounter_type == "boss":
            effective_attack += 4
        effective_attack = max(12, effective_attack)

        enemy_attack = encounter.damage + (encounter.accuracy // 8) + (encounter.aggression // 10) + (active_pressure // 6)
        enemy_attack -= int(stats["mitigation"]) // 5
        enemy_attack -= int(stats["pressure_guard"]) // 6
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

    player_percent = max(0, int((player_health / int(stats["max_health"])) * 100))
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


def _scaled_rewards(player: PlayerProfile, encounter: CombatEncounterDefinition) -> tuple[dict[str, int], dict[str, int]]:
    stats = player_combat_profile(player)
    materials = dict(encounter.loot_materials)
    items = dict(encounter.loot_items)
    loot_bonus = int(stats["loot_bonus_percent"])
    for material_name, amount in list(materials.items()):
        bonus = amount * loot_bonus // 100
        if encounter.encounter_type == "boss" and _player_has_perk(player, "relic_hunter.grave_luck"):
            bonus += 1
        materials[material_name] = amount + bonus
    if encounter.encounter_type == "boss" and _player_has_perk(player, "outlaw.greased_palm") and "Broker" in encounter.name:
        items["Broker Cut"] = items.get("Broker Cut", 0) + 1
    return materials, items


def build_combat_activity(
    player: PlayerProfile,
    encounter_id: str,
    gate_evaluator: Callable[[PlayerProfile, object], tuple[bool, list[str]]],
) -> tuple[ActivityDefinition, list[str]]:
    locale = player.preferred_locale
    encounter = COMBAT_ENCOUNTERS[encounter_id]
    preview = preview_combat_encounter(player, encounter, gate_evaluator)
    if preview["state"] != "Ready":
        raise ValueError(f"Encounter '{encounter.name}' locked: {', '.join(preview['missing'])}")
    if preview["projection"] == "Overrun":
        raise ValueError(f"Encounter '{encounter.name}' is still projected as an overrun.")

    materials, items = _scaled_rewards(player, encounter)
    notes = [
        encounter.summary,
        f"Projection: {preview['projection']} after {preview['rounds']} rounds.",
        "Traits: " + ", ".join(ENEMY_TRAITS[trait_id].name for trait_id in encounter.traits),
        "Test Spawn: " + encounter.test_spawn_label,
    ]
    if preview["phases_triggered"]:
        notes.append("Triggered phases -> " + ", ".join(preview["phases_triggered"]))
    if encounter.support_archetypes:
        notes.append("Support pressure -> " + ", ".join(ENEMY_ARCHETYPES[archetype_id].name for archetype_id in encounter.support_archetypes))

    return (
        ActivityDefinition(
            activity_id=encounter.encounter_id,
            name=encounter.name,
            xp=encounter.xp_reward,
            materials=materials,
            items=items,
            relics=encounter.relics,
            boss_kills=encounter.boss_kills,
            region_progress=encounter.region_progress,
            completed_trails=encounter.completed_trails,
            discovered_regions=encounter.discovered_regions,
            discovered_bosses=encounter.discovered_bosses,
            defeated_bosses=encounter.defeated_bosses,
            farming_gains=encounter.farming_gains,
            network_gains=encounter.network_gains,
            lifestyle_gains=encounter.lifestyle_gains,
            notes=" ".join(notes),
        ),
        [
            gameplay_text(locale, f"Combat Encounter: {encounter.name}"),
            gameplay_text(locale, f"- Type: {encounter.encounter_type.title()} | Region: {REGIONS[encounter.region]['name']}"),
            gameplay_text(locale, f"- Projection: {preview['projection']} | Rounds: {preview['rounds']}"),
            gameplay_text(locale, f"- Player HP left: {preview['player_health_remaining']} | Enemy HP left: {preview['enemy_health_remaining']}"),
            gameplay_text(locale, f"- Tags in play: {', '.join(sorted(preview['profile']['tags'])) if preview['profile']['tags'] else 'none'}"),
            gameplay_text(locale, f"- Resistances: {_modifier_lines(encounter.resistances)}"),
            gameplay_text(locale, f"- Weaknesses: {_modifier_lines(encounter.weaknesses)}"),
            gameplay_text(locale, f"- Support / phases: {_support_labels(encounter, preview['phases_triggered'])}"),
            gameplay_text(locale, f"- Test Spawn: {encounter.test_spawn_label} | {encounter.spawn_region_note}"),
        ],
    )


def build_combat_rows(
    player: PlayerProfile,
    gate_evaluator: Callable[[PlayerProfile, object], tuple[bool, list[str]]],
) -> list[dict[str, str]]:
    locale = player.preferred_locale
    rows: list[dict[str, str]] = []
    for encounter in sorted(COMBAT_ENCOUNTERS.values(), key=_encounter_sort_key):
        preview = preview_combat_encounter(player, encounter, gate_evaluator)
        defeated = encounter.encounter_type == "boss" and encounter.source_id in player.defeated_bosses
        state = "Defeated" if defeated else preview["state"]
        rows.append(
            {
                "encounter": encounter.name,
                "type": encounter.encounter_type.title(),
                "region": REGIONS[encounter.region]["name"],
                "state": state,
                "projection": "Already Cleared" if defeated else str(preview["projection"]),
                "spawn_state": "Summon Ready" if preview["state"] == "Ready" else "Summon Locked",
                "detail": gameplay_text(locale, "\n".join(
                    [
                        encounter.summary,
                        "",
                        f"Combat Role: {encounter.role}",
                        f"Combat: HP {encounter.max_health} | Armor {encounter.armor} | Damage {encounter.damage} | Accuracy {encounter.accuracy} | Pressure {encounter.pressure}",
                        "Traits: " + ", ".join(ENEMY_TRAITS[trait_id].name for trait_id in encounter.traits),
                        "Resistances: " + _modifier_lines(encounter.resistances),
                        "Weaknesses: " + _modifier_lines(encounter.weaknesses),
                        f"Projection: {preview['projection']} | Rounds: {preview['rounds']}",
                        "Support: " + (", ".join(ENEMY_ARCHETYPES[archetype_id].name for archetype_id in encounter.support_archetypes) if encounter.support_archetypes else "None"),
                        "Phases: " + (", ".join(phase.name for phase in encounter.phases) if encounter.phases else "Single phase"),
                        f"Test Spawn: {encounter.test_spawn_label}",
                        f"Spawn Note: {encounter.spawn_region_note}",
                    ]
                    + ([] if not preview["missing"] else ["Gate: " + ", ".join(preview["missing"])])
                )),
            }
        )
    return rows


def boss_combat_brief(
    player: PlayerProfile,
    boss_id: str,
    gate_evaluator: Callable[[PlayerProfile, object], tuple[bool, list[str]]],
) -> dict[str, str] | None:
    encounter = _find_encounter(boss_id)
    if encounter is None:
        return None
    preview = preview_combat_encounter(player, encounter, gate_evaluator)
    return {
        "combat": f"HP {encounter.max_health} | Armor {encounter.armor} | Damage {encounter.damage} | Pressure {encounter.pressure}",
        "resistances": _modifier_lines(encounter.resistances),
        "weaknesses": _modifier_lines(encounter.weaknesses),
        "projection": str(preview["projection"]),
        "test_spawn": f"{encounter.test_spawn_label} | {encounter.spawn_region_note}",
        "phases": ", ".join(phase.name for phase in encounter.phases) if encounter.phases else "Single phase",
    }


def archetype_combat_brief(archetype_id: str) -> dict[str, str] | None:
    encounter = _find_encounter(archetype_id)
    if encounter is None:
        return None
    return {
        "combat": f"HP {encounter.max_health} | Armor {encounter.armor} | Damage {encounter.damage} | Accuracy {encounter.accuracy} | Pressure {encounter.pressure}",
        "resistances": _modifier_lines(encounter.resistances),
        "weaknesses": _modifier_lines(encounter.weaknesses),
        "spawn": f"{encounter.test_spawn_label} | {encounter.spawn_region_note}",
    }


def simulate_combat_rows(
    player: PlayerProfile,
    gate_evaluator: Callable[[PlayerProfile, object], tuple[bool, list[str]]],
) -> list[dict[str, str]]:
    simulated_player = deepcopy(player)
    return build_combat_rows(simulated_player, gate_evaluator)
