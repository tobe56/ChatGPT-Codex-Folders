from __future__ import annotations

from ..data import AFFINITY_TRACKS
from ..models import AffinityDefinition, AffinityReward, PlayerProfile, ProgressGate

DEFAULT_AFFINITY_POINTS = {
    "affinity.abigail": 12,
    "affinity.jack": 10,
    "affinity.uncle": 8,
}

GIFT_VALUE_BY_TAG = {
    "medicine crate": 8,
    "ranch keepsake": 6,
    "defense tools": 7,
    "sketchbook pages": 6,
    "book": 5,
    "relic notes": 8,
    "liquor": 5,
    "rumor payment": 7,
    "salvage bundle": 6,
}

AFFINITY_REWARD_PAYLOADS: dict[str, dict[str, object]] = {
    "affinity.abigail.stage1": {
        "materials": {"treated leather": 2, "gun oil concentrate": 1},
        "items": {"Medicine Bundle": 1},
        "collection": ("Abigail Watch Rota",),
    },
    "affinity.abigail.stage2": {
        "materials": {"hardwood": 2, "silver fittings": 1},
        "items": {"Ranch Defense Ledger": 1},
        "collection": ("Abigail Supply Cache",),
    },
    "affinity.abigail.stage3": {
        "materials": {"treated leather": 3, "preserved herbs": 2},
        "items": {"Quiet Night Token": 1},
        "collection": ("Abigail Night Watch",),
    },
    "affinity.abigail.stage4": {
        "materials": {"hardwood": 4, "engraved silver": 1},
        "items": {"Ranch Holdout Kit": 1},
        "collection": ("Beecher's Hope Lockdown",),
    },
    "affinity.abigail.stage5": {
        "materials": {"tempered springs": 2, "grave silver": 1},
        "items": {"Peace Bought In Blood": 1},
        "collection": ("Abigail's Last Defense",),
    },
    "affinity.jack.stage1": {
        "materials": {"saint cards": 1},
        "items": {"Boss Sketch Pack": 1},
        "collection": ("Sketch: Frontier Signs",),
    },
    "affinity.jack.stage2": {
        "materials": {"collector bones": 1, "silver fittings": 1},
        "items": {"Archive Margin Notes": 1},
        "collection": ("Jack's Archive Notes",),
    },
    "affinity.jack.stage3": {
        "materials": {"grave silver": 1},
        "items": {"Relic Reading Charm": 1},
        "collection": ("Jack's Relic Theory",),
    },
    "affinity.jack.stage4": {
        "materials": {"grave silver": 2, "meteor splinters": 1},
        "items": {"Frontier Myth Index": 1},
        "collection": ("Jack's Myth Route",),
        "relics": 1,
    },
    "affinity.jack.stage5": {
        "materials": {"meteor splinters": 2, "boss cores": 1},
        "items": {"The Last Archive": 1},
        "collection": ("Jack's Final Archive",),
        "relics": 1,
    },
    "affinity.uncle.stage1": {
        "materials": {"iron scrap": 2, "black powder bundles": 1},
        "items": {"Rumor Payment Ledger": 1},
        "collection": ("Uncle's Rumor Mill",),
    },
    "affinity.uncle.stage2": {
        "materials": {"brass fittings": 2, "iron scrap": 2},
        "items": {"Fence Contact Token": 1},
        "collection": ("Uncle's Easy Score Map",),
    },
    "affinity.uncle.stage3": {
        "materials": {"tempered springs": 1, "engraved silver": 1},
        "items": {"Dirty Work Cache": 1},
        "collection": ("Uncle's Better Bad Idea",),
    },
    "affinity.uncle.stage4": {
        "materials": {"black powder bundles": 2, "engraved silver": 1},
        "items": {"Black Market Route Book": 1},
        "collection": ("Uncle's War Room",),
    },
    "affinity.uncle.stage5": {
        "materials": {"tempered springs": 2, "boss cores": 1},
        "items": {"King Of The Bad Road": 1},
        "collection": ("Uncle's Final Payout",),
    },
}


def ensure_affinity_state(player: PlayerProfile) -> None:
    for affinity_id, default_points in DEFAULT_AFFINITY_POINTS.items():
        player.affinity_points.setdefault(affinity_id, default_points)


def _clamp_affinity_score(value: int) -> int:
    return max(0, min(220, value))


def _stat_ready(player: PlayerProfile, thresholds: dict[str, int]) -> list[str]:
    ready: list[str] = []
    values = player.stats.as_dict()
    for stat_name, threshold in thresholds.items():
        if values[stat_name] >= threshold:
            ready.append(f"{stat_name.title()} {threshold}")
    return ready


def _skill_ready(player: PlayerProfile, thresholds: dict[str, int]) -> list[str]:
    ready: list[str] = []
    values = player.lifestyle.as_dict()
    for skill_name, threshold in thresholds.items():
        if values[skill_name] >= threshold:
            ready.append(f"{skill_name.replace('_', ' ').title()} {threshold}")
    return ready


def _quests_for_definition(definition: AffinityDefinition) -> tuple[AffinityReward, ...]:
    return definition.stage_rewards


def reward_for_stage(definition: AffinityDefinition, stage: int) -> AffinityReward:
    return next(reward for reward in _quests_for_definition(definition) if reward.stage == stage)


def completed_affinity_stage(player: PlayerProfile, definition: AffinityDefinition) -> int:
    stage = 0
    for reward in _quests_for_definition(definition):
        if reward.quest_id in player.completed_affinity_quests:
            stage = reward.stage
    return stage


def affinity_quest_label(quest_id: str) -> str:
    for definition in AFFINITY_TRACKS.values():
        for reward in definition.stage_rewards:
            if reward.quest_id == quest_id:
                return reward.quest_name
    return quest_id


def _frontier_evidence_bonus(player: PlayerProfile, definition: AffinityDefinition) -> int:
    affinity_id = definition.affinity_id
    if affinity_id == "affinity.abigail":
        return (
            (2 if player.home_upgrades >= 1 else 0)
            + (2 if player.workbench_tier >= 2 else 0)
            + (2 if "Saint Medal" in player.inventory or "Trapper Sigil" in player.inventory else 0)
        )
    if affinity_id == "affinity.jack":
        return (
            (2 if player.relic_count >= 1 else 0)
            + (2 if len(player.known_bosses) >= 2 else 0)
            + (2 if any(entry.startswith("Sketch:") for entry in player.collection_entries) else 0)
        )
    return (
        (2 if player.network_progress.get("track.contracts", 0) >= 2 else 0)
        + (2 if player.network_progress.get("track.stagecoaches", 0) >= 1 else 0)
        + (2 if player.materials.get("black powder bundles", 0) >= 1 or player.materials.get("brass fittings", 0) >= 1 else 0)
    )


def affinity_score_breakdown(player: PlayerProfile, definition: AffinityDefinition) -> dict[str, int]:
    ensure_affinity_state(player)
    stat_matches = _stat_ready(player, definition.preferred_stats)
    skill_matches = _skill_ready(player, definition.preferred_skills)
    stored = player.affinity_points.get(definition.affinity_id, 0)
    compatibility = (len(stat_matches) * 2) + len(skill_matches)
    frontier_evidence = _frontier_evidence_bonus(player, definition)
    quest_bonus = completed_affinity_stage(player, definition) * 4
    return {
        "stored": stored,
        "compatibility": compatibility,
        "frontier_evidence": frontier_evidence,
        "quest_bonus": quest_bonus,
        "total": stored + compatibility + frontier_evidence + quest_bonus,
    }


def affinity_score(player: PlayerProfile, definition: AffinityDefinition) -> int:
    return affinity_score_breakdown(player, definition)["total"]


def affinity_stage(player: PlayerProfile, definition: AffinityDefinition) -> int:
    score = affinity_score(player, definition)
    current_stage = 0
    for reward in _quests_for_definition(definition):
        threshold = definition.stage_thresholds[reward.stage]
        if score >= threshold and player.world_tier >= reward.minimum_world_tier:
            current_stage = reward.stage
    return current_stage


def repeatable_reward_bonus_percent(player: PlayerProfile, definition: AffinityDefinition) -> int:
    reward_stage = completed_affinity_stage(player, definition)
    if reward_stage <= 0:
        return 0
    return reward_for_stage(definition, reward_stage).repeatable_bonus_percent


def repeatable_bonus_for_system(player: PlayerProfile, system_name: str) -> tuple[str, int]:
    ensure_affinity_state(player)
    if system_name in {"Jobs"}:
        definition = AFFINITY_TRACKS["affinity.abigail"]
    elif system_name in {"Contracts", "Stagecoaches And Money Wagons", "Bank Robberies"}:
        definition = AFFINITY_TRACKS["affinity.uncle"]
    elif system_name in {"Bounties Expansion", "Duels", "Companion System"}:
        definition = AFFINITY_TRACKS["affinity.jack"]
    else:
        definition = AFFINITY_TRACKS["affinity.abigail"]
    return definition.npc_name, repeatable_reward_bonus_percent(player, definition)


def _quest_gate_missing(player: PlayerProfile, gate: ProgressGate) -> list[str]:
    missing: list[str] = []
    if player.level < gate.minimum_level:
        missing.append(f"Level {gate.minimum_level}")
    if player.workbench_tier < gate.minimum_workbench_tier:
        missing.append(f"Workbench Tier {gate.minimum_workbench_tier}")
    if player.world_tier < gate.minimum_world_tier:
        missing.append(f"World Tier {gate.minimum_world_tier}")
    if player.relic_count < gate.minimum_relic_count:
        missing.append(f"Relics {gate.minimum_relic_count}")
    if player.boss_kills < gate.minimum_boss_kills:
        missing.append(f"Boss Kills {gate.minimum_boss_kills}")
    if player.home_upgrades < gate.minimum_home_upgrades:
        missing.append(f"Home Upgrades {gate.minimum_home_upgrades}")
    return missing


def next_affinity_reward(player: PlayerProfile, definition: AffinityDefinition) -> tuple[AffinityReward | None, list[str]]:
    trust_stage = affinity_stage(player, definition)
    cleared_stage = completed_affinity_stage(player, definition)
    next_stage = cleared_stage + 1
    if next_stage > len(definition.stage_rewards):
        return None, []

    reward = reward_for_stage(definition, next_stage)
    score = affinity_score(player, definition)
    missing: list[str] = []
    threshold = definition.stage_thresholds[next_stage]
    if score < threshold:
        missing.append(f"Score {threshold}")
    if player.world_tier < reward.minimum_world_tier:
        missing.append(f"World Tier {reward.minimum_world_tier}")
    if trust_stage < next_stage:
        return reward, missing

    missing.extend(_quest_gate_missing(player, reward.quest_gate))
    return reward, missing


def affinity_quest_state(player: PlayerProfile, definition: AffinityDefinition) -> tuple[str, AffinityReward | None, list[str]]:
    reward, missing = next_affinity_reward(player, definition)
    if reward is None:
        return "Quest Secured", None, []
    if not missing:
        return "Quest Ready", reward, []
    if any(item.startswith("Score ") or item.startswith("World Tier ") for item in missing):
        return "Building Trust", reward, missing
    return "Quest Blocked", reward, missing


def apply_affinity_gift(player: PlayerProfile, affinity_id: str, gift_item: str) -> int:
    ensure_affinity_state(player)
    definition = AFFINITY_TRACKS[affinity_id]
    if gift_item not in definition.gift_items:
        return 0
    gain = GIFT_VALUE_BY_TAG[gift_item]
    player.affinity_points[affinity_id] = _clamp_affinity_score(player.affinity_points[affinity_id] + gain)
    return gain


def apply_affinity_dialogue_choice(player: PlayerProfile, affinity_id: str, delta: int) -> int:
    ensure_affinity_state(player)
    current = player.affinity_points[affinity_id]
    updated = _clamp_affinity_score(current + delta)
    player.affinity_points[affinity_id] = updated
    return updated - current


def apply_affinity_activity(player: PlayerProfile, affinity_changes: dict[str, int]) -> list[str]:
    ensure_affinity_state(player)
    entries: list[str] = []
    for affinity_id, delta in affinity_changes.items():
        if affinity_id not in AFFINITY_TRACKS or delta == 0:
            continue
        definition = AFFINITY_TRACKS[affinity_id]
        current = player.affinity_points.get(affinity_id, 0)
        updated = _clamp_affinity_score(current + delta)
        actual = updated - current
        player.affinity_points[affinity_id] = updated
        if actual:
            entries.append(f"Affinity -> {definition.npc_name} {actual:+d}")
    return entries


def _apply_reward_payload(player: PlayerProfile, reward: AffinityReward) -> None:
    payload = AFFINITY_REWARD_PAYLOADS.get(reward.quest_id, {})
    for material_name, amount in payload.get("materials", {}).items():
        player.materials[material_name] = player.materials.get(material_name, 0) + amount
    for item_name, amount in payload.get("items", {}).items():
        player.inventory[item_name] = player.inventory.get(item_name, 0) + amount
    for entry in payload.get("collection", ()):
        player.collection_entries.add(entry)
    player.relic_count += int(payload.get("relics", 0))


def complete_affinity_quest(player: PlayerProfile, affinity_id: str, stage: int) -> list[str]:
    definition = AFFINITY_TRACKS[affinity_id]
    reward = reward_for_stage(definition, stage)
    if reward.quest_id in player.completed_affinity_quests:
        return []

    current_reward, missing = next_affinity_reward(player, definition)
    if current_reward is None or current_reward.quest_id != reward.quest_id:
        raise ValueError(f"Affinity quest '{reward.quest_name}' is not the next reward stage")
    if missing:
        raise ValueError(f"Affinity quest '{reward.quest_name}' blocked: {', '.join(missing)}")

    player.completed_affinity_quests.add(reward.quest_id)
    _apply_reward_payload(player, reward)
    player.notes.append(f"Affinity quest cleared: {reward.quest_name}")
    return [
        f"Affinity quest cleared -> {reward.quest_name}",
        "Affinity rewards -> " + ", ".join(reward.rewards),
    ]


def complete_available_affinity_quests(player: PlayerProfile) -> list[str]:
    completed: list[str] = []
    made_progress = True
    while made_progress:
        made_progress = False
        for definition in AFFINITY_TRACKS.values():
            quest_state, reward, missing = affinity_quest_state(player, definition)
            if reward is None or quest_state != "Quest Ready" or missing:
                continue
            completed.extend(complete_affinity_quest(player, definition.affinity_id, reward.stage))
            made_progress = True
    return completed


def build_affinity_rows(player: PlayerProfile) -> list[dict[str, str]]:
    ensure_affinity_state(player)
    rows: list[dict[str, str]] = []
    for definition in AFFINITY_TRACKS.values():
        breakdown = affinity_score_breakdown(player, definition)
        score = breakdown["total"]
        trust_stage = affinity_stage(player, definition)
        reward_stage = completed_affinity_stage(player, definition)
        quest_state, next_reward, missing = affinity_quest_state(player, definition)
        stat_matches = _stat_ready(player, definition.preferred_stats)
        skill_matches = _skill_ready(player, definition.preferred_skills)
        last_reward = reward_for_stage(definition, reward_stage) if reward_stage > 0 else None
        detail_lines = [
            definition.role,
            "",
            f"Trust stage: {trust_stage}",
            f"Reward stage secured: {reward_stage}",
            f"Stored score: {breakdown['stored']}",
            f"Compatibility bonus: +{breakdown['compatibility']}",
            f"Frontier evidence: +{breakdown['frontier_evidence']}",
            f"Quest bonus: +{breakdown['quest_bonus']}",
            f"Repeatable bonus: +{repeatable_reward_bonus_percent(player, definition)}%",
            "Affinity items: " + ", ".join(definition.gift_items),
            "Stat matches: " + (", ".join(stat_matches) if stat_matches else "None yet"),
            "Skill matches: " + (", ".join(skill_matches) if skill_matches else "None yet"),
            "Dialogue rule: " + " / ".join(definition.dialogue_notes),
            "Repeatable scaling: " + definition.repeatable_reward_rule,
        ]
        if last_reward is not None:
            detail_lines.extend(
                [
                    "",
                    f"Last secured quest: {last_reward.quest_name}",
                    "Last secured rewards: " + ", ".join(last_reward.rewards),
                ]
            )
        if next_reward is not None:
            detail_lines.extend(
                [
                    "",
                    f"Quest state: {quest_state}",
                    f"Next quest: {next_reward.quest_name}",
                    "Support route: " + next_reward.support_route,
                    "Recommended interaction: " + next_reward.recommended_interaction,
                    "Turn-ins: " + (", ".join(next_reward.turn_in_items) if next_reward.turn_in_items else "Field proof only"),
                    "Next rewards: " + ", ".join(next_reward.rewards),
                ]
            )
            if missing:
                detail_lines.append("Missing: " + ", ".join(missing))
        else:
            detail_lines.extend(["", "Quest state: Quest Secured", "All authored affinity stages are currently secured."])
        rows.append(
            {
                "npc": definition.npc_name,
                "score": str(score),
                "stage": f"Stage {trust_stage}",
                "quest_state": quest_state,
                "repeatable_bonus": f"+{repeatable_reward_bonus_percent(player, definition)}%",
                "interaction": next_reward.recommended_interaction if next_reward is not None else "Hold the current support route.",
                "detail": "\n".join(detail_lines),
            }
        )
    return rows
