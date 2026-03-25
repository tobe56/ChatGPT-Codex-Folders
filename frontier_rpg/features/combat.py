from __future__ import annotations

from collections.abc import Callable
from copy import deepcopy

from ..content import COMBAT_ENCOUNTERS, ENEMY_ARCHETYPES, ENEMY_TRAITS, REGIONS
from ..models import ActivityDefinition, CombatEncounterDefinition, PlayerProfile
from ..platform.localization import gameplay_text
from .combat_math import player_combat_profile
from .combat_projection import preview_combat_encounter, scaled_rewards


def _encounter_sort_key(encounter: CombatEncounterDefinition) -> tuple[int, str, str]:
    priority = 1 if encounter.encounter_type == "boss" else 0
    return (priority, REGIONS[encounter.region]["name"], encounter.name)


def _find_encounter(source_id: str) -> CombatEncounterDefinition | None:
    for encounter in COMBAT_ENCOUNTERS.values():
        if encounter.source_id == source_id:
            return encounter
    return None


def _modifier_lines(mapping: dict[str, int] | object) -> str:
    if not mapping or not isinstance(mapping, dict):
        return "None"
    return ", ".join(f"{key.replace('_', ' ').title()} {value}%" for key, value in mapping.items())


def _support_labels(encounter: CombatEncounterDefinition, phases_triggered: list[str]) -> str:
    labels: list[str] = []
    for archetype_id in encounter.support_archetypes:
        labels.append(ENEMY_ARCHETYPES[archetype_id].name)
    for phase_name in phases_triggered:
        labels.append(phase_name)
    return ", ".join(labels) if labels else "None"


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

    materials, items = scaled_rewards(player, encounter)
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
