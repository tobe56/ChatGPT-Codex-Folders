from __future__ import annotations

from dataclasses import dataclass, field
from typing import Mapping

STAT_NAMES = ("strength", "grit", "deadeye", "survival", "cunning")
LIFESTYLE_NAMES = (
    "hunting",
    "gunsmithing",
    "field_medicine",
    "trapping",
    "salvaging",
    "trade",
    "horse_handling",
    "homestead",
)


@dataclass(slots=True)
class Stats:
    strength: int = 2
    grit: int = 2
    deadeye: int = 2
    survival: int = 2
    cunning: int = 2

    def as_dict(self) -> dict[str, int]:
        return {name: getattr(self, name) for name in STAT_NAMES}

    def add_points(self, allocations: Mapping[str, int]) -> int:
        spent = 0
        for name, value in allocations.items():
            if name not in STAT_NAMES:
                raise ValueError(f"Unknown stat '{name}'")
            if value < 0:
                raise ValueError(f"Negative allocation for '{name}'")
            setattr(self, name, getattr(self, name) + value)
            spent += value
        return spent

    def meets(self, thresholds: Mapping[str, int]) -> bool:
        return all(getattr(self, name) >= value for name, value in thresholds.items())


@dataclass(slots=True)
class Lifestyle:
    hunting: int = 1
    gunsmithing: int = 1
    field_medicine: int = 1
    trapping: int = 1
    salvaging: int = 1
    trade: int = 1
    horse_handling: int = 1
    homestead: int = 1

    def as_dict(self) -> dict[str, int]:
        return {name: getattr(self, name) for name in LIFESTYLE_NAMES}

    def add_points(self, gains: Mapping[str, int]) -> None:
        for name, value in gains.items():
            if name not in LIFESTYLE_NAMES:
                raise ValueError(f"Unknown lifestyle skill '{name}'")
            if value < 0:
                raise ValueError(f"Negative lifestyle gain for '{name}'")
            setattr(self, name, getattr(self, name) + value)


@dataclass(slots=True, frozen=True)
class ProgressGate:
    minimum_level: int = 1
    stat_thresholds: dict[str, int] = field(default_factory=dict)
    minimum_workbench_tier: int = 1
    minimum_world_tier: int = 1
    minimum_relic_count: int = 0
    minimum_boss_kills: int = 0
    minimum_home_upgrades: int = 0
    required_perks: tuple[str, ...] = ()
    required_regions: tuple[str, ...] = ()
    required_affinity_stages: dict[str, int] = field(default_factory=dict)
    required_affinity_quests: tuple[str, ...] = ()
    required_items: dict[str, int] = field(default_factory=dict)
    required_trails: tuple[str, ...] = ()


@dataclass(slots=True, frozen=True)
class PerkDefinition:
    perk_id: str
    name: str
    tree: str
    description: str
    requirement: ProgressGate = field(default_factory=ProgressGate)


@dataclass(slots=True, frozen=True)
class RecipeDefinition:
    recipe_id: str
    name: str
    category: str
    station: str
    tier: int
    description: str
    material_cost: dict[str, int] = field(default_factory=dict)
    required_items: dict[str, int] = field(default_factory=dict)
    result_items: dict[str, int] = field(default_factory=dict)
    workbench_requirement: int = 1
    blueprint: str | None = None
    grants_workbench_tier: int | None = None
    grants_home_upgrade: int = 0


@dataclass(slots=True, frozen=True)
class WorldTierDefinition:
    tier: int
    name: str
    description: str
    gate: ProgressGate


@dataclass(slots=True, frozen=True)
class StoryChapterDefinition:
    chapter_id: str
    name: str
    description: str
    gate: ProgressGate


@dataclass(slots=True, frozen=True)
class TrailDefinition:
    trail_id: str
    name: str
    category: str
    summary: str
    detail: str
    activation_gate: ProgressGate = field(default_factory=ProgressGate)
    completion_gate: ProgressGate = field(default_factory=ProgressGate)
    region: str | None = None
    repeatable: bool = False
    priority: int = 100
    rewards: tuple[str, ...] = ()


@dataclass(slots=True, frozen=True)
class RegionIntelDefinition:
    region_id: str
    name: str
    role: str
    recommended_world_tier: int
    danger_by_tier: Mapping[int, str]
    pressure_sources: tuple[str, ...] = ()
    node_types: tuple[str, ...] = ()
    boss_clues: tuple[str, ...] = ()
    faction_routes: tuple[str, ...] = ()


@dataclass(slots=True, frozen=True)
class HubFeatureDefinition:
    feature_id: str
    name: str
    station: str
    category: str
    description: str
    unlock_gate: ProgressGate = field(default_factory=ProgressGate)
    outputs: tuple[str, ...] = ()
    priority: int = 100


@dataclass(slots=True, frozen=True)
class GearDefinition:
    gear_id: str
    name: str
    slot: str
    category: str
    bonuses: tuple[str, ...]
    source: str
    description: str
    requirement: ProgressGate = field(default_factory=ProgressGate)


@dataclass(slots=True, frozen=True)
class BossDefinition:
    boss_id: str
    name: str
    region: str
    threat_rank: str
    territory: str
    summary: str
    rewards: tuple[str, ...]
    lore_note: str
    gate: ProgressGate = field(default_factory=ProgressGate)
    silhouette: str = "Unknown Silhouette"


@dataclass(slots=True, frozen=True)
class BossPhaseDefinition:
    phase_id: str
    name: str
    trigger_health_percent: int
    summary: str
    exposed_weaknesses: tuple[str, ...] = ()
    support_archetypes: tuple[str, ...] = ()
    pressure_bonus: int = 0


@dataclass(slots=True, frozen=True)
class CombatEncounterDefinition:
    encounter_id: str
    name: str
    encounter_type: str
    region: str
    source_id: str
    role: str
    summary: str
    max_health: int
    armor: int
    damage: int
    accuracy: int
    aggression: int
    pressure: int
    traits: tuple[str, ...] = ()
    resistances: Mapping[str, int] = field(default_factory=dict)
    weaknesses: Mapping[str, int] = field(default_factory=dict)
    loot_materials: dict[str, int] = field(default_factory=dict)
    loot_items: dict[str, int] = field(default_factory=dict)
    xp_reward: int = 0
    relics: int = 0
    boss_kills: int = 0
    region_progress: tuple[str, ...] = ()
    completed_trails: tuple[str, ...] = ()
    discovered_regions: tuple[str, ...] = ()
    discovered_bosses: tuple[str, ...] = ()
    defeated_bosses: tuple[str, ...] = ()
    farming_gains: dict[str, int] = field(default_factory=dict)
    network_gains: dict[str, int] = field(default_factory=dict)
    lifestyle_gains: dict[str, int] = field(default_factory=dict)
    gate: ProgressGate = field(default_factory=ProgressGate)
    support_archetypes: tuple[str, ...] = ()
    phases: tuple[BossPhaseDefinition, ...] = ()
    test_spawn_label: str = ""
    spawn_region_note: str = ""


@dataclass(slots=True, frozen=True)
class CollectionDefinition:
    collection_id: str
    name: str
    category: str
    description: str
    entries: tuple[str, ...]
    reward: str
    gate: ProgressGate = field(default_factory=ProgressGate)


@dataclass(slots=True, frozen=True)
class FamilySupportDefinition:
    support_id: str
    family_member: str
    focus: str
    description: str
    outputs: tuple[str, ...]
    unlock_gate: ProgressGate = field(default_factory=ProgressGate)
    risk_note: str = ""


@dataclass(slots=True, frozen=True)
class AffinityReward:
    stage: int
    quest_id: str
    quest_name: str
    rewards: tuple[str, ...]
    minimum_world_tier: int = 1
    repeatable_bonus_percent: int = 0
    quest_gate: ProgressGate = field(default_factory=ProgressGate)
    support_route: str = ""
    recommended_interaction: str = ""
    turn_in_items: tuple[str, ...] = ()


@dataclass(slots=True, frozen=True)
class AffinityDefinition:
    affinity_id: str
    npc_name: str
    role: str
    preferred_stats: dict[str, int]
    preferred_skills: dict[str, int]
    gift_items: tuple[str, ...]
    stage_thresholds: Mapping[int, int]
    stage_rewards: tuple[AffinityReward, ...]
    dialogue_notes: tuple[str, ...] = ()
    repeatable_reward_rule: str = ""


@dataclass(slots=True, frozen=True)
class FarmingLaneDefinition:
    lane_id: str
    name: str
    focus: str
    best_regions: tuple[str, ...]
    rewards: tuple[str, ...]
    unlock_gate: ProgressGate = field(default_factory=ProgressGate)
    recommended_world_tier: int = 1
    route_note: str = ""


@dataclass(slots=True, frozen=True)
class IntegrationTrackDefinition:
    track_id: str
    name: str
    category: str
    role: str
    rewards: tuple[str, ...]
    unlock_gate: ProgressGate = field(default_factory=ProgressGate)
    risk_note: str = ""


@dataclass(slots=True, frozen=True)
class WorldStateDefinition:
    tier: int
    title: str
    enemy_change: str
    loot_change: str
    boss_change: str
    recipe_change: str
    story_change: str
    route_change: str


@dataclass(slots=True, frozen=True)
class EndingPhaseDefinition:
    phase_id: str
    name: str
    summary: str
    gate: ProgressGate = field(default_factory=ProgressGate)
    outcomes: tuple[str, ...] = ()
    post_ending: bool = False


@dataclass(slots=True, frozen=True)
class EnemyTraitDefinition:
    trait_id: str
    name: str
    family: str
    description: str
    minimum_world_tier: int = 1


@dataclass(slots=True, frozen=True)
class EnemyArchetypeDefinition:
    archetype_id: str
    name: str
    rank: str
    region: str
    traits: tuple[str, ...]
    notes: str


@dataclass(slots=True, frozen=True)
class WorldNodeDefinition:
    node_id: str
    name: str
    region: str
    category: str
    summary: str
    encounter_id: str
    drop_materials: dict[str, int] = field(default_factory=dict)
    drop_items: dict[str, int] = field(default_factory=dict)
    first_clear_items: dict[str, int] = field(default_factory=dict)
    stash_destination: str = "Beecher's Hope Stash"
    unlock_gate: ProgressGate = field(default_factory=ProgressGate)
    route_note: str = ""
    auto_deposit: bool = True


@dataclass(slots=True)
class WeaponState:
    weapon_id: str
    name: str
    category: str
    condition: str
    familiarity: int
    archetype: str
    mods: dict[str, str] = field(default_factory=dict)


@dataclass(slots=True, frozen=True)
class ActivityDefinition:
    activity_id: str
    name: str
    xp: int
    materials: dict[str, int] = field(default_factory=dict)
    items: dict[str, int] = field(default_factory=dict)
    relics: int = 0
    boss_kills: int = 0
    home_upgrades: int = 0
    region_progress: tuple[str, ...] = ()
    completed_trails: tuple[str, ...] = ()
    discovered_regions: tuple[str, ...] = ()
    discovered_bosses: tuple[str, ...] = ()
    defeated_bosses: tuple[str, ...] = ()
    farming_gains: dict[str, int] = field(default_factory=dict)
    network_gains: dict[str, int] = field(default_factory=dict)
    affinity_changes: dict[str, int] = field(default_factory=dict)
    ending_phases: tuple[str, ...] = ()
    lifestyle_gains: dict[str, int] = field(default_factory=dict)
    blueprints: tuple[str, ...] = ()
    completed_chapters: tuple[str, ...] = ()
    notes: str = ""


@dataclass(slots=True)
class PlayerProfile:
    name: str = "John Marston"
    frontier_title: str = "Ashes of the Frontier"
    preferred_locale: str = "en"
    level: int = 1
    total_xp: int = 0
    unspent_stat_points: int = 0
    unspent_perk_points: int = 0
    stats: Stats = field(default_factory=Stats)
    lifestyle: Lifestyle = field(default_factory=Lifestyle)
    world_tier: int = 1
    workbench_tier: int = 1
    home_upgrades: int = 0
    relic_count: int = 0
    boss_kills: int = 0
    materials: dict[str, int] = field(default_factory=dict)
    stash_materials: dict[str, int] = field(default_factory=dict)
    inventory: dict[str, int] = field(default_factory=dict)
    stash_inventory: dict[str, int] = field(default_factory=dict)
    unlocked_perks: set[str] = field(default_factory=set)
    stabilized_regions: set[str] = field(default_factory=set)
    completed_chapters: set[str] = field(default_factory=set)
    known_bosses: set[str] = field(default_factory=lambda: {"boss.silas_redd"})
    defeated_bosses: set[str] = field(default_factory=set)
    discovered_regions: set[str] = field(default_factory=lambda: {"west_elizabeth"})
    discovered_materials: set[str] = field(default_factory=set)
    completed_trails: set[str] = field(default_factory=set)
    active_trails: list[str] = field(default_factory=list)
    equipped_gear: dict[str, str] = field(default_factory=dict)
    collection_entries: set[str] = field(default_factory=set)
    cleared_world_nodes: set[str] = field(default_factory=set)
    farming_progress: dict[str, int] = field(default_factory=dict)
    claimed_farming_milestones: set[str] = field(default_factory=set)
    network_progress: dict[str, int] = field(default_factory=dict)
    claimed_network_milestones: set[str] = field(default_factory=set)
    affinity_points: dict[str, int] = field(default_factory=dict)
    completed_affinity_quests: set[str] = field(default_factory=set)
    claimed_world_tier_rewards: set[int] = field(default_factory=set)
    completed_ending_phases: set[str] = field(default_factory=set)
    known_blueprints: set[str] = field(default_factory=set)
    crafted_recipes: set[str] = field(default_factory=set)
    weapons: list[WeaponState] = field(default_factory=list)
    stash_returns: int = 0
    notes: list[str] = field(default_factory=list)
