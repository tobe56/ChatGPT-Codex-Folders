from __future__ import annotations

from collections.abc import Callable

from ..content import FARMING_LANES, REGIONS
from ..models import PlayerProfile

_LANE_OWNER = {
    "lane.hunter": "Jack",
    "lane.raider": "Uncle",
    "lane.contract": "Uncle",
    "lane.frontier_work": "Abigail",
    "lane.boss": "Jack",
}

FARMING_MILESTONE_PACKAGES: dict[str, tuple[dict[str, object], ...]] = {
    "lane.hunter": (
        {"threshold": 1, "label": "Field Dressing Kit", "materials": {"treated leather": 2}, "items": {"Hunter Route Kit": 1}, "collection": ("Hunter Lane Starter Cache",)},
        {"threshold": 3, "label": "Trapper Supply Stack", "materials": {"collector bones": 1, "preserved herbs": 2}, "items": {"Trapper Payout Tag": 1}, "collection": ("Hunter Lane Midgame Route",)},
        {"threshold": 5, "label": "Myth Hunt Locker", "materials": {"grave silver": 1}, "items": {"Myth Hunt Locker": 1}, "collection": ("Hunter Lane Master Route",)},
    ),
    "lane.raider": (
        {"threshold": 1, "label": "Scrap Wagon Split", "materials": {"iron scrap": 3}, "items": {"Raider Lockbox": 1}, "collection": ("Raider Lane Starter Cache",)},
        {"threshold": 3, "label": "Convoy Break Cache", "materials": {"black powder bundles": 1, "brass fittings": 1}, "items": {"Convoy Route Note": 1}, "collection": ("Raider Lane Midgame Route",)},
        {"threshold": 5, "label": "Reinforced Route Haul", "materials": {"tempered springs": 1}, "items": {"Raider War Crate": 1}, "collection": ("Raider Lane Master Route",)},
    ),
    "lane.contract": (
        {"threshold": 1, "label": "Clean Contract Payout", "materials": {"saint cards": 1}, "items": {"Broker Pay Chit": 1}, "collection": ("Contract Lane Starter Cache",)},
        {"threshold": 3, "label": "Broker Mark Packet", "materials": {"engraved silver": 1}, "items": {"Named Target Slip": 1}, "collection": ("Contract Lane Midgame Route",)},
        {"threshold": 5, "label": "Blackwater Priority Route", "materials": {"elite insignias": 1}, "items": {"Priority Contract Packet": 1}, "collection": ("Contract Lane Master Route",)},
    ),
    "lane.frontier_work": (
        {"threshold": 1, "label": "House Supply Return", "materials": {"hardwood": 2}, "items": {"Homefront Work Order": 1}, "collection": ("Frontier Work Starter Cache",)},
        {"threshold": 3, "label": "Stable Wage Bundle", "materials": {"gun oil concentrate": 1, "treated leather": 1}, "items": {"Ranch Wage Token": 1}, "collection": ("Frontier Work Midgame Route",)},
        {"threshold": 5, "label": "Beecher's Hope Work Ledger", "materials": {"hardwood": 3}, "items": {"Beecher Work Ledger": 1}, "collection": ("Frontier Work Master Route",)},
    ),
    "lane.boss": (
        {"threshold": 1, "label": "Boss Prep Crate", "materials": {"silver fittings": 1}, "items": {"Boss Route Note": 1}, "collection": ("Boss Lane Starter Cache",)},
        {"threshold": 3, "label": "Named Trophy Bundle", "materials": {"elite insignias": 1}, "items": {"Named Trophy Bundle": 1}, "collection": ("Boss Lane Midgame Route",)},
        {"threshold": 5, "label": "Legend Hunt Locker", "materials": {"boss cores": 1}, "items": {"Legend Hunt Locker": 1}, "collection": ("Boss Lane Master Route",)},
    ),
}


def _lane_owner(lane_id: str) -> str:
    return _LANE_OWNER.get(lane_id, "Beecher's Hope")


def _lane_state(progress: int, unlocked: bool) -> str:
    if progress >= 5:
        return "Mastered"
    if unlocked:
        return "Active"
    return "Locked"


def _route_tier(progress: int) -> str:
    if progress >= 5:
        return "Master Route"
    if progress >= 3:
        return "Established"
    if progress >= 1:
        return "Seeded"
    return "Dormant"


def _grant_package(player: PlayerProfile, package: dict[str, object]) -> None:
    for material_name, amount in package.get("materials", {}).items():
        player.materials[material_name] = player.materials.get(material_name, 0) + amount
    for item_name, amount in package.get("items", {}).items():
        player.inventory[item_name] = player.inventory.get(item_name, 0) + amount
    for entry in package.get("collection", ()):
        player.collection_entries.add(entry)


def claim_available_farming_milestones(player: PlayerProfile) -> list[str]:
    entries: list[str] = []
    for lane_id, packages in FARMING_MILESTONE_PACKAGES.items():
        progress = player.farming_progress.get(lane_id, 0)
        for package in packages:
            milestone_id = f"{lane_id}:{package['threshold']}"
            if progress < int(package["threshold"]) or milestone_id in player.claimed_farming_milestones:
                continue
            player.claimed_farming_milestones.add(milestone_id)
            _grant_package(player, package)
            entries.append(f"Farming milestone -> {FARMING_LANES[lane_id].name}: {package['label']}")
    return entries


def _next_package(player: PlayerProfile, lane_id: str) -> dict[str, object] | None:
    for package in FARMING_MILESTONE_PACKAGES.get(lane_id, ()):
        milestone_id = f"{lane_id}:{package['threshold']}"
        if milestone_id not in player.claimed_farming_milestones:
            return package
    return None


def _last_package(player: PlayerProfile, lane_id: str) -> dict[str, object] | None:
    last: dict[str, object] | None = None
    for package in FARMING_MILESTONE_PACKAGES.get(lane_id, ()):
        milestone_id = f"{lane_id}:{package['threshold']}"
        if milestone_id in player.claimed_farming_milestones:
            last = package
    return last


def _lane_next_unlock(progress: int, state: str, lane_name: str, next_package: dict[str, object] | None) -> str:
    if state == "Locked":
        return "Open the route gate first."
    if next_package is None:
        return f"{lane_name} is already in the late-game rotation."
    if progress <= 1:
        return f"Secure {next_package['label']}."
    if progress <= 3:
        return f"Push toward {next_package['label']}."
    return f"One more strong sweep will secure {next_package['label']}."


def build_farming_rows(
    player: PlayerProfile,
    gate_evaluator: Callable[[PlayerProfile, object], tuple[bool, list[str]]],
) -> list[dict[str, str]]:
    rows: list[dict[str, str]] = []
    for lane in FARMING_LANES.values():
        unlocked, missing = gate_evaluator(player, lane.unlock_gate)
        progress = player.farming_progress.get(lane.lane_id, 0)
        state = _lane_state(progress, unlocked)
        route_tier = _route_tier(progress)
        regions = ", ".join(REGIONS[region]["name"] for region in lane.best_regions)
        owner = _lane_owner(lane.lane_id)
        next_package = _next_package(player, lane.lane_id)
        last_package = _last_package(player, lane.lane_id)
        next_unlock = _lane_next_unlock(progress, state, lane.name, next_package)
        rows.append(
            {
                "lane": lane.name,
                "focus": lane.focus,
                "owner": owner,
                "tier": route_tier,
                "progress": str(progress),
                "next_unlock": next_unlock,
                "next_reward": next_package["label"] if next_package is not None else "All route packages secured",
                "state": state,
                "detail": "\n".join(
                    [
                        lane.route_note or lane.focus,
                        "",
                        f"Best Regions: {regions}",
                        f"Route Owner: {owner}",
                        f"Route Tier: {route_tier}",
                        "Rewards: " + ", ".join(lane.rewards),
                        "Last Secured Package: " + (str(last_package["label"]) if last_package is not None else "None yet"),
                        "Next Package: " + (str(next_package["label"]) if next_package is not None else "All route packages secured"),
                        f"Next Unlock: {next_unlock}",
                        f"Recommended World Tier: {lane.recommended_world_tier}",
                    ]
                    + ([] if unlocked else ["Gate: " + ", ".join(missing)])
                ),
            }
        )
    return rows
