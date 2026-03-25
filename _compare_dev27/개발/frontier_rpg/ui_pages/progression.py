from __future__ import annotations

from ..content import PERKS
from ..features.growth_effects import build_growth_effects
from ..models import PlayerProfile
from ..platform.localization import gameplay_text
from ..systems import available_perks, meets_gate

STAT_DETAILS = {
    "strength": "Melee power, forced entry, carry and handling.",
    "grit": "Health, resistance, recovery, and pressure survival.",
    "deadeye": "Accuracy, crits, reload flow, and weak-point play.",
    "survival": "Tracking, hunting yield, environment reading, and field sense.",
    "cunning": "Trade leverage, clue reading, stash detection, and ambush warning.",
}

SKILL_DETAILS = {
    "hunting": "Improves pelt yield and rare animal drops.",
    "gunsmithing": "Lowers tuning cost and unlocks deeper weapon work.",
    "field_medicine": "Improves recovery tools and cuts downtime.",
    "trapping": "Boosts bait efficiency and wilderness recipes.",
    "salvaging": "Finds more parts in camps, wagons, and enemy gear.",
    "trade": "Improves pricing and black-market outcomes.",
    "horse_handling": "Stabilizes mounts under pressure and supports mounted builds.",
    "homestead": "Accelerates Beecher's Hope growth and support systems.",
}


def build_stats_rows(player: PlayerProfile) -> list[dict[str, str]]:
    effects = build_growth_effects(player)
    locale = player.preferred_locale
    rows: list[dict[str, str]] = []
    for stat_name, value in player.stats.as_dict().items():
        if stat_name == "strength":
            impact = f"Actual: combat damage +{effects['combat_damage_bonus_percent']}%"
        elif stat_name == "grit":
            impact = f"Actual: damage resist +{effects['damage_resist_percent']}%"
        elif stat_name == "deadeye":
            impact = f"Actual: boss damage +{effects['boss_damage_bonus_percent']}%"
        elif stat_name == "survival":
            impact = f"Actual: travel stamina +{effects['travel_stamina_bonus_percent']}%"
        else:
            impact = f"Actual: loot yield +{effects['loot_yield_bonus_percent']}%"
        rows.append(
            {
                "stat": stat_name.title(),
                "value": str(value),
                "focus": STAT_DETAILS[stat_name].split(",")[0],
                "detail": gameplay_text(locale, f"{STAT_DETAILS[stat_name]}\n\n{impact}"),
            }
        )
    return rows


def build_skill_rows(player: PlayerProfile) -> list[dict[str, str]]:
    effects = build_growth_effects(player)
    locale = player.preferred_locale
    rows: list[dict[str, str]] = []
    for skill_name, value in player.lifestyle.as_dict().items():
        if skill_name in {"gunsmithing", "homestead"}:
            impact = f"Actual: crafting cost -{effects['craft_cost_reduction_percent']}%"
        elif skill_name in {"hunting", "trapping"}:
            impact = f"Actual: drop quality +{effects['drop_quality_bonus_percent']}%"
        elif skill_name == "field_medicine":
            impact = f"Actual: medicine efficiency +{effects['medicine_efficiency_percent']}%"
        elif skill_name == "horse_handling":
            impact = f"Actual: mounted control +{effects['mounted_control_bonus_percent']}%"
        else:
            impact = f"Actual: loot yield +{effects['loot_yield_bonus_percent']}%"
        rows.append(
            {
                "skill": skill_name.replace("_", " ").title(),
                "rank": str(value),
                "focus": SKILL_DETAILS[skill_name].split(".")[0],
                "detail": gameplay_text(locale, f"{SKILL_DETAILS[skill_name]}\n\n{impact}"),
            }
        )
    return rows


def build_perk_rows(player: PlayerProfile) -> list[dict[str, str]]:
    ready_ids = {perk.perk_id for perk in available_perks(player)}
    rows: list[dict[str, str]] = []
    for perk in sorted(PERKS.values(), key=lambda item: (item.tree, item.name)):
        if perk.perk_id in player.unlocked_perks:
            state = "Unlocked"
            requirement = "Already active"
        elif perk.perk_id in ready_ids:
            state = "Ready"
            requirement = "Spend 1 perk point"
        else:
            state = "Locked"
            _, missing = meets_gate(player, perk.requirement)
            requirement = ", ".join(missing) if missing else "Locked"
        rows.append(
            {
                "tree": perk.tree,
                "perk": perk.name,
                "state": state,
                "requirement": requirement,
                "detail": perk.description,
            }
        )
    return rows
