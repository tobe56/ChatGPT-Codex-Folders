from __future__ import annotations

from collections.abc import Callable

from ..content import ENDING_PHASES
from ..models import PlayerProfile

ENDING_PHASE_PACKAGES: dict[str, dict[str, object]] = {
    "ending.ranch_under_threat": {"label": "Ranch Threat Briefing", "materials": {"hardwood": 2}, "items": {"Ranch Threat Briefing": 1}, "collection": ("Ending Phase: Ranch Under Threat",)},
    "ending.surveyor_trail": {"label": "Surveyor Hunt Packet", "materials": {"engraved silver": 1}, "items": {"Surveyor Hunt Packet": 1}, "collection": ("Ending Phase: Surveyor Trail",)},
    "ending.myth_breaks_open": {"label": "Sky Devil Case File", "materials": {"grave silver": 1}, "items": {"Sky Devil Case File": 1}, "collection": ("Ending Phase: Myth Breaks Open",)},
    "ending.last_ride": {"label": "Last Ride Campaign Map", "materials": {"tempered springs": 1}, "items": {"Last Ride Campaign Map": 1}, "collection": ("Ending Phase: Last Ride",)},
    "ending.close_the_ledger": {"label": "Closed Ledger Seal", "materials": {"boss cores": 1}, "items": {"Closed Ledger Seal": 1}, "collection": ("Ending Phase: Close The Ledger",)},
    "ending.after_the_ledger": {"label": "After The Ledger Sandbox Charter", "materials": {"meteor splinters": 1}, "items": {"Sandbox Charter": 1}, "collection": ("Ending Phase: After The Ledger",)},
}


def _phase_focus(phase_id: str, state: str) -> str:
    if state == "Completed":
        return "This phase is already folded into the campaign record."
    if phase_id == "ending.ranch_under_threat":
        return "Stabilize Beecher's Hope before the war spills into the house."
    if phase_id == "ending.surveyor_trail":
        return "Turn clue density into a clean Surveyor route."
    if phase_id == "ending.myth_breaks_open":
        return "Push relic pressure until the sky-devil thread resolves."
    if phase_id == "ending.last_ride":
        return "Convert full frontier readiness into the final ride."
    if phase_id == "ending.close_the_ledger":
        return "Bring the campaign home instead of letting power win."
    return "Keep the post-ending sandbox tied back to Beecher's Hope."


def complete_available_ending_phases(
    player: PlayerProfile,
    gate_evaluator: Callable[[PlayerProfile, object], tuple[bool, list[str]]],
) -> list[str]:
    entries: list[str] = []
    made_progress = True
    while made_progress:
        made_progress = False
        for phase in ENDING_PHASES.values():
            if phase.phase_id in player.completed_ending_phases:
                continue
            ready, _ = gate_evaluator(player, phase.gate)
            if not ready:
                continue
            player.completed_ending_phases.add(phase.phase_id)
            package = ENDING_PHASE_PACKAGES.get(phase.phase_id)
            if package is not None:
                for material_name, amount in package.get("materials", {}).items():
                    player.materials[material_name] = player.materials.get(material_name, 0) + amount
                for item_name, amount in package.get("items", {}).items():
                    player.inventory[item_name] = player.inventory.get(item_name, 0) + amount
                for entry in package.get("collection", ()):
                    player.collection_entries.add(entry)
            entries.append(f"Ending phase cleared -> {phase.name}")
            made_progress = True
    return entries


def build_ending_rows(
    player: PlayerProfile,
    gate_evaluator: Callable[[PlayerProfile, object], tuple[bool, list[str]]],
) -> list[dict[str, str]]:
    rows: list[dict[str, str]] = []
    for phase in ENDING_PHASES.values():
        ready, missing = gate_evaluator(player, phase.gate)
        if phase.phase_id in player.completed_ending_phases:
            state = "Completed"
        elif ready:
            state = "Ready"
        else:
            state = "Locked"
        focus = _phase_focus(phase.phase_id, state)
        package = ENDING_PHASE_PACKAGES.get(phase.phase_id)
        rows.append(
            {
                "phase": phase.name,
                "state": state,
                "type": "Post-Ending" if phase.post_ending else "Ending",
                "focus": focus,
                "reward_package": package["label"] if package is not None else "No package",
                "reward_state": "Secured" if phase.phase_id in player.completed_ending_phases else "Pending",
                "detail": "\n".join(
                    [
                        phase.summary,
                        "",
                        "Focus: " + focus,
                        "Outcomes: " + ", ".join(phase.outcomes),
                        "Reward Package: " + (package["label"] if package is not None else "No package"),
                        "Reward State: " + ("Secured" if phase.phase_id in player.completed_ending_phases else "Pending"),
                    ]
                    + ([] if ready or phase.phase_id in player.completed_ending_phases else ["Gate: " + ", ".join(missing)])
                ),
            }
        )
    return rows
