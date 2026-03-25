from __future__ import annotations

from collections.abc import Callable

from ..content import FAMILY_SUPPORT
from ..models import PlayerProfile
from ..platform.localization import gameplay_text
from .affinity import build_affinity_rows


def _current_role(player: PlayerProfile, family_member: str) -> str:
    if family_member == "Abigail":
        return (
            "Homefront upgrades and medicine prep are active."
            if player.home_upgrades >= 1
            else "She is still warning John before the ranch fully mobilizes."
        )
    if family_member == "Jack":
        return (
            "He is sketching bosses and expanding the ledger archive."
            if player.relic_count >= 1
            else "He has not yet turned strange finds into a real archive."
        )
    return (
        "Rumor lanes and dirty jobs are flowing through Uncle's contacts."
        if player.level >= 4
        else "His best leads are still out of reach."
    )


def _homefront_request(family_member: str, affinity_row: dict[str, str] | None) -> str:
    if affinity_row is None:
        return "No active request yet."
    if affinity_row["quest_state"] == "Quest Ready":
        return affinity_row["interaction"]
    if family_member == "Abigail":
        return "Bring medicine stock, repair goods, and calm reports back to the ranch."
    if family_member == "Jack":
        return "Deliver clue scraps, sketches, and relic notes to the archive table."
    return "Turn salvage and rumor money into a cleaner black-market route."


def build_family_rows(
    player: PlayerProfile,
    gate_evaluator: Callable[[PlayerProfile, object], tuple[bool, list[str]]],
) -> list[dict[str, str]]:
    locale = player.preferred_locale
    affinity_rows = {row["npc"]: row for row in build_affinity_rows(player)}
    rows: list[dict[str, str]] = []
    for support in FAMILY_SUPPORT.values():
        unlocked, missing = gate_evaluator(player, support.unlock_gate)
        affinity_row = affinity_rows.get(support.family_member)
        request = _homefront_request(support.family_member, affinity_row)
        rows.append(
            {
                "member": support.family_member,
                "focus": support.focus,
                "request": request,
                "state": "Active" if unlocked else "Locked",
                "detail": gameplay_text(locale, "\n".join(
                    [
                        support.description,
                        "",
                        "Outputs: " + ", ".join(support.outputs),
                        "Current Role: " + _current_role(player, support.family_member),
                        (
                            f"Affinity: {affinity_row['stage']} | Score {affinity_row['score']} | "
                            f"{affinity_row['quest_state']} | Repeatable bonus {affinity_row['repeatable_bonus']}"
                            if affinity_row
                            else "Affinity: not tracked yet"
                        ),
                        (
                            "Storage: "
                            f"field mats {sum(player.materials.values())} | stash mats {sum(player.stash_materials.values())} | "
                            f"stash items {sum(player.stash_inventory.values())} | routes {len(player.cleared_world_nodes)}"
                        ),
                        "Homefront Request: " + request,
                        "Risk: " + support.risk_note,
                    ]
                    + ([] if unlocked else ["Gate: " + ", ".join(missing)])
                )),
            }
        )
    return rows
