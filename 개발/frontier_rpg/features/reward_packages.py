from __future__ import annotations

from collections.abc import Mapping
from typing import TypedDict

from ..models import PlayerProfile


class RewardPackage(TypedDict, total=False):
    threshold: int
    label: str
    materials: dict[str, int]
    items: dict[str, int]
    collection: tuple[str, ...]


def _milestone_id(source_id: str, package: RewardPackage) -> str:
    return f"{source_id}:{package['threshold']}"


def apply_reward_package(player: PlayerProfile, package: RewardPackage) -> None:
    for material_name, amount in package.get("materials", {}).items():
        player.materials[material_name] = player.materials.get(material_name, 0) + amount
    for item_name, amount in package.get("items", {}).items():
        player.inventory[item_name] = player.inventory.get(item_name, 0) + amount
    for entry in package.get("collection", ()):
        player.collection_entries.add(entry)


def next_unclaimed_package(
    source_id: str,
    packages: tuple[RewardPackage, ...],
    claimed_milestones: set[str],
) -> RewardPackage | None:
    for package in packages:
        if _milestone_id(source_id, package) not in claimed_milestones:
            return package
    return None


def last_claimed_package(
    source_id: str,
    packages: tuple[RewardPackage, ...],
    claimed_milestones: set[str],
) -> RewardPackage | None:
    last: RewardPackage | None = None
    for package in packages:
        if _milestone_id(source_id, package) in claimed_milestones:
            last = package
    return last


def claim_threshold_packages(
    player: PlayerProfile,
    progress_by_id: Mapping[str, int],
    packages_by_id: Mapping[str, tuple[RewardPackage, ...]],
    claimed_milestones: set[str],
    label_by_id: Mapping[str, str],
    prefix: str,
) -> list[str]:
    entries: list[str] = []
    for source_id, packages in packages_by_id.items():
        progress = progress_by_id.get(source_id, 0)
        for package in packages:
            milestone_id = _milestone_id(source_id, package)
            if progress < package["threshold"] or milestone_id in claimed_milestones:
                continue
            claimed_milestones.add(milestone_id)
            apply_reward_package(player, package)
            entries.append(f"{prefix} -> {label_by_id[source_id]}: {package['label']}")
    return entries
