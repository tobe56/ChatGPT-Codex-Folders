static const char *frontier_live_entry_label(
    const FrontierLedgerPage *page,
    int entry_index,
    const FrontierLedgerEntry *entry
)
{
    if (!frontier_runtime_uses_live_dataset() || page == NULL || entry == NULL) {
        return entry != NULL ? entry->label : "";
    }

    if (frontier_dataset_is_korean()) {
        if (entry_index == 0 && strcmp(page->page_id, "overview") == 0) {
            snprintf(
                g_runtime.runtime_entry_label,
                sizeof(g_runtime.runtime_entry_label),
                "실시간 런 | %s",
                frontier_live_trail_stage_compact_label(g_runtime.live_profile.trail_stage)
            );
            return g_runtime.runtime_entry_label;
        }

        if (entry_index == 0 && strcmp(page->page_id, "trails") == 0) {
            snprintf(
                g_runtime.runtime_entry_label,
                sizeof(g_runtime.runtime_entry_label),
                "목표 | %s | %s",
                frontier_live_trail_stage_compact_label(g_runtime.live_profile.trail_stage),
                frontier_runtime_context_label()
            );
            return g_runtime.runtime_entry_label;
        }

        if (entry_index == 0 && strcmp(page->page_id, "world") == 0) {
            snprintf(
                g_runtime.runtime_entry_label,
                sizeof(g_runtime.runtime_entry_label),
                "압력 %d | 지역 %d/6",
                frontier_live_activity_points(),
                g_runtime.live_profile.discovered_region_count
            );
            return g_runtime.runtime_entry_label;
        }

        if (entry_index == 0 && strcmp(page->page_id, "farming") == 0) {
            snprintf(
                g_runtime.runtime_entry_label,
                sizeof(g_runtime.runtime_entry_label),
                "사냥 %d | 루팅 %d | %.0fm",
                g_runtime.live_profile.animal_kills,
                g_runtime.live_profile.loot_count,
                g_runtime.live_profile.field_distance_since_hub
            );
            return g_runtime.runtime_entry_label;
        }

        if (entry_index == 0 && strcmp(page->page_id, "network") == 0) {
            snprintf(
                g_runtime.runtime_entry_label,
                sizeof(g_runtime.runtime_entry_label),
                "보상 $%d | 복귀 %d",
                g_runtime.live_profile.reward_earned,
                g_runtime.live_profile.hub_returns
            );
            return g_runtime.runtime_entry_label;
        }

        if (entry_index == 0 && strcmp(page->page_id, "ending") == 0) {
            snprintf(
                g_runtime.runtime_entry_label,
                sizeof(g_runtime.runtime_entry_label),
                "엔딩 | %s",
                frontier_live_trail_stage_compact_label(g_runtime.live_profile.trail_stage)
            );
            return g_runtime.runtime_entry_label;
        }
    }

    if (strcmp(page->page_id, "stats") == 0) {
        return frontier_live_stat_action_label(entry_index);
    }
    if (strcmp(page->page_id, "skills") == 0) {
        return frontier_live_skill_action_label(entry_index);
    }
    if (strcmp(page->page_id, "perks") == 0) {
        return frontier_live_perk_label(entry_index);
    }
    if (strcmp(page->page_id, "crafting") == 0) {
        return frontier_live_crafting_label_safe(entry_index);
    }
    if (strcmp(page->page_id, "armory") == 0) {
        return frontier_live_armory_label(entry_index);
    }
    if (strcmp(page->page_id, "dead") == 0) {
        return frontier_live_dead_label_synced(entry_index);
    }
    if (strcmp(page->page_id, "collection") == 0) {
        return frontier_live_collection_label(entry_index);
    }
    if (strcmp(page->page_id, "gear") == 0) {
        return frontier_live_gear_label(entry_index);
    }

    if (entry_index == 0 && strcmp(page->page_id, "overview") == 0) {
        snprintf(
            g_runtime.runtime_entry_label,
            sizeof(g_runtime.runtime_entry_label),
            "%s | %s",
            frontier_text("Live Frontier Run", "실시간 프론티어 런"),
            frontier_runtime_phase_label()
        );
        return g_runtime.runtime_entry_label;
    }

    if (entry_index == 0 && strcmp(page->page_id, "trails") == 0) {
        snprintf(
            g_runtime.runtime_entry_label,
            sizeof(g_runtime.runtime_entry_label),
            "%s | %s | %s",
            frontier_text("Live Objective", "실시간 목표"),
            frontier_live_trail_stage_label(g_runtime.live_profile.trail_stage),
            frontier_runtime_context_label()
        );
        return g_runtime.runtime_entry_label;
    }

    if (entry_index == 0 && strcmp(page->page_id, "homefront") == 0) {
        snprintf(
            g_runtime.runtime_entry_label,
            sizeof(g_runtime.runtime_entry_label),
            "%s | %s %d / %s %d",
            frontier_text("Homefront", "거점"),
            frontier_text("Threat", "위협"),
            g_runtime.live_profile.ranch_threat_level,
            frontier_text("Defense", "방어"),
            g_runtime.live_profile.ranch_defense_level
        );
        return g_runtime.runtime_entry_label;
    }

    if (entry_index == 0 && strcmp(page->page_id, "world") == 0) {
        snprintf(
            g_runtime.runtime_entry_label,
            sizeof(g_runtime.runtime_entry_label),
            "%s | %s %d | %s %d/6",
            frontier_text("Live World State", "실시간 월드 상태"),
            frontier_text("Pressure", "압력"),
            frontier_live_activity_points(),
            frontier_text("Regions", "지역"),
            g_runtime.live_profile.discovered_region_count
        );
        return g_runtime.runtime_entry_label;
    }

    if (entry_index == 0 && strcmp(page->page_id, "farming") == 0) {
        snprintf(
            g_runtime.runtime_entry_label,
            sizeof(g_runtime.runtime_entry_label),
            "%s | %s | %s %d",
            frontier_text("Live Farming Sweep", "실시간 농장 스윕"),
            frontier_farming_lane_label(g_runtime.live_profile.farming_lane),
            frontier_text("Hunts", "사냥"),
            g_runtime.live_profile.animal_kills
        );
        return g_runtime.runtime_entry_label;
    }

    if (entry_index == 0 && strcmp(page->page_id, "network") == 0) {
        snprintf(
            g_runtime.runtime_entry_label,
            sizeof(g_runtime.runtime_entry_label),
            "%s | %s $%d",
            frontier_text("Live Network Intake", "실시간 네트워크 수입"),
            frontier_text("Reward", "보상"),
            g_runtime.live_profile.reward_earned
        );
        return g_runtime.runtime_entry_label;
    }

    if (entry_index == 0 && strcmp(page->page_id, "ending") == 0) {
        snprintf(
            g_runtime.runtime_entry_label,
            sizeof(g_runtime.runtime_entry_label),
            "%s | %s | %s",
            frontier_text("Live Ending Route", "실시간 엔딩 루트"),
            frontier_story_clue_label(g_runtime.live_profile.surveyor_clue_count),
            frontier_sky_devil_label(g_runtime.live_profile.sky_devil_stage)
        );
        return g_runtime.runtime_entry_label;
    }

    return entry->label;
}

