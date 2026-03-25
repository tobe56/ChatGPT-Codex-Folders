from __future__ import annotations

from collections.abc import Callable

from ..content import INTEGRATION_TRACKS
from ..models import PlayerProfile
from .affinity import repeatable_bonus_for_system
from .reward_packages import RewardPackage, claim_threshold_packages, next_unclaimed_package

NETWORK_MILESTONE_PACKAGES: dict[str, tuple[RewardPackage, ...]] = {
    "track.jobs": (
        {"threshold": 1, "label": "Ranch Wage Cache", "materials": {"hardwood": 1}, "items": {"Job Wage Envelope": 1}, "collection": ("Jobs Route Starter",)},
        {"threshold": 3, "label": "House Support Contract", "materials": {"treated leather": 1}, "items": {"Home Support Contract": 1}, "collection": ("Jobs Route Integrated",)},
    ),
    "track.contracts": (
        {"threshold": 1, "label": "Broker Entry Mark", "materials": {"saint cards": 1}, "items": {"Broker Entry Mark": 1}, "collection": ("Contracts Route Starter",)},
        {"threshold": 3, "label": "Blackwater Broker File", "materials": {"engraved silver": 1}, "items": {"Blackwater Broker File": 1}, "collection": ("Contracts Route Integrated",)},
    ),
    "track.stagecoaches": (
        {"threshold": 1, "label": "Convoy Tip Sheet", "materials": {"iron scrap": 2}, "items": {"Convoy Tip Sheet": 1}, "collection": ("Stagecoach Route Starter",)},
        {"threshold": 3, "label": "Reinforced Cargo Map", "materials": {"black powder bundles": 1}, "items": {"Reinforced Cargo Map": 1}, "collection": ("Stagecoach Route Integrated",)},
    ),
    "track.bounties": (
        {"threshold": 1, "label": "Lawful Hunt Docket", "materials": {"silver fittings": 1}, "items": {"Lawful Hunt Docket": 1}, "collection": ("Bounty Route Starter",)},
    ),
    "track.duels": (
        {"threshold": 1, "label": "Duelist Calling Card", "materials": {"engraved silver": 1}, "items": {"Duelist Calling Card": 1}, "collection": ("Duels Route Starter",)},
        {"threshold": 3, "label": "Prestige Duel Ledger", "materials": {"silver fittings": 1}, "items": {"Prestige Duel Ledger": 1}, "collection": ("Duels Route Integrated",)},
    ),
    "track.companions": (
        {"threshold": 1, "label": "Support Hand Contract", "materials": {"gun oil concentrate": 1}, "items": {"Support Hand Contract": 1}, "collection": ("Companion Route Starter",)},
    ),
    "track.properties": (
        {"threshold": 1, "label": "Safehouse Ledger", "materials": {"hardwood": 2}, "items": {"Safehouse Ledger": 1}, "collection": ("Properties Route Starter",)},
    ),
    "track.banks": (
        {"threshold": 1, "label": "Bank Raid Sketch", "materials": {"black powder bundles": 1}, "items": {"Bank Raid Sketch": 1}, "collection": ("Banks Route Starter",)},
        {"threshold": 3, "label": "Vault Route Charter", "materials": {"boss cores": 1}, "items": {"Vault Route Charter": 1}, "collection": ("Banks Route Integrated",)},
    ),
}


def claim_available_network_milestones(player: PlayerProfile) -> list[str]:
    return claim_threshold_packages(
        player,
        progress_by_id=player.network_progress,
        packages_by_id=NETWORK_MILESTONE_PACKAGES,
        claimed_milestones=player.claimed_network_milestones,
        label_by_id={track_id: track.name for track_id, track in INTEGRATION_TRACKS.items()},
        prefix="Network milestone",
    )


def _network_hook(track_id: str, progress: int, state: str) -> str:
    if state == "Locked":
        return "The route gate is still closed."
    if track_id == "track.contracts":
        return "Keep Blackwater contracts flowing into rarer marks."
    if track_id == "track.jobs":
        return "Turn steady work into cleaner ranch support."
    if track_id == "track.stagecoaches":
        return "Convert convoy hits into reinforced supply lines."
    if track_id == "track.banks":
        return "Late-game burst payouts are the next escalation point."
    if progress >= 3:
        return "This route is already feeding the wider frontier loop."
    return "One more clean push will stabilize this category."


def _network_next_package(player: PlayerProfile, track_id: str) -> RewardPackage | None:
    return next_unclaimed_package(track_id, NETWORK_MILESTONE_PACKAGES.get(track_id, ()), player.claimed_network_milestones)


def build_integration_rows(
    player: PlayerProfile,
    gate_evaluator: Callable[[PlayerProfile, object], tuple[bool, list[str]]],
) -> list[dict[str, str]]:
    rows: list[dict[str, str]] = []
    for track in INTEGRATION_TRACKS.values():
        unlocked, missing = gate_evaluator(player, track.unlock_gate)
        bonus_npc, bonus_percent = repeatable_bonus_for_system(player, track.name)
        progress = player.network_progress.get(track.track_id, 0)
        if progress >= 3:
            state = "Integrated"
        elif unlocked:
            state = "Open"
        else:
            state = "Locked"
        hook = _network_hook(track.track_id, progress, state)
        next_package = _network_next_package(player, track.track_id)
        rows.append(
            {
                "system": track.name,
                "category": track.category,
                "owner": bonus_npc,
                "progress": str(progress),
                "hook": hook,
                "milestone": next_package["label"] if next_package is not None else "Route packages secured",
                "state": state,
                "detail": "\n".join(
                    [
                        track.role,
                        "",
                        f"Route Owner: {bonus_npc}",
                        "Rewards: " + ", ".join(track.rewards),
                        f"Repeatable payout bonus: +{bonus_percent}% via {bonus_npc}",
                        "Current Hook: " + hook,
                        "Next Package: " + (str(next_package["label"]) if next_package is not None else "Route packages secured"),
                        "Risk: " + track.risk_note,
                    ]
                    + ([] if unlocked else ["Gate: " + ", ".join(missing)])
                ),
            }
        )
    return rows
