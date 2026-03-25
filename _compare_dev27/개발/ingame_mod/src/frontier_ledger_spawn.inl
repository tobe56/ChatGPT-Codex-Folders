static uint32_t frontier_resolve_valid_model_hash(const char *const *candidates, int candidate_count)
{
    int index;

    if (candidates == NULL || candidate_count <= 0) {
        return 0U;
    }

    for (index = 0; index < candidate_count; ++index) {
        uint32_t model_hash = frontier_model_hash_override_for_name(candidates[index]);
        if (model_hash == 0U) {
            model_hash = frontier_get_hash_key(candidates[index]);
        }
        if (frontier_is_model_valid(model_hash)) {
            return model_hash;
        }
    }

    return 0U;
}

static int frontier_find_spawn_donor_ped(int player_ped)
{
    int nearby_peds[FRONTIER_LIVE_NEARBY_PED_SCAN_MAX];
    int nearby_count;
    int index;

    if (player_ped == 0) {
        return 0;
    }

    ZeroMemory(nearby_peds, sizeof(nearby_peds));
    nearby_count = frontier_get_ped_nearby_peds(player_ped, nearby_peds, FRONTIER_LIVE_NEARBY_PED_SCAN_MAX);
    for (index = 0; index < nearby_count; ++index) {
        int ped = nearby_peds[index];
        if (ped == 0 || ped == player_ped) {
            continue;
        }
        if (!frontier_is_ped_human(ped) || frontier_is_ped_a_player(ped) || frontier_is_ped_dead_or_dying(ped)) {
            continue;
        }
        if (!frontier_does_entity_exist(ped)) {
            continue;
        }
        return ped;
    }

    return 0;
}

static int frontier_request_model_blocking(uint32_t model_hash)
{
    int attempt;

    if (model_hash == 0U) {
        return 0;
    }

    frontier_request_model(model_hash);
    for (attempt = 0; attempt < 40; ++attempt) {
        if (frontier_has_model_loaded(model_hash)) {
            return 1;
        }
        FrontierScriptHook_Wait(50);
        frontier_request_model(model_hash);
    }

    return frontier_has_model_loaded(model_hash);
}

static int frontier_spawn_profile_near_player(const FrontierSpawnProfile *profile)
{
    int player_ped = frontier_player_ped();
    int donor_ped = 0;
    int requested_count;
    uint32_t model_hash;
    uint32_t weapon_hash;
    float player_heading;
    int spawned = 0;
    int index;

    if (profile == NULL || player_ped == 0) {
        return 0;
    }

    requested_count = g_runtime.debug_spawn_single_ped
        ? (profile->count > 1 ? 1 : profile->count)
        : profile->count;
    frontier_log("Spawn start: profile=%s count=%d playerPed=%d", profile->label, profile->count, player_ped);
    donor_ped = frontier_find_spawn_donor_ped(player_ped);
    if (donor_ped != 0) {
        frontier_log("Spawn donor ped selected: %d", donor_ped);
        frontier_log("Spawn donor clone path disabled for stability. Using direct CREATE_PED.");
        donor_ped = 0;
    }
    model_hash = frontier_get_entity_model(player_ped);
    if (model_hash == 0U) {
        model_hash = frontier_resolve_valid_model_hash(profile->model_candidates, profile->model_candidate_count);
    }
    frontier_log("Spawn candidate model resolved: 0x%X", (unsigned int)model_hash);
    if (model_hash == frontier_get_entity_model(player_ped)) {
        frontier_log("Spawn model selected from player ped: 0x%X", (unsigned int)model_hash);
    } else if (model_hash != 0U && frontier_request_model_blocking(model_hash)) {
        frontier_log("Spawn model selected from candidate set: 0x%X", (unsigned int)model_hash);
    } else {
        frontier_log("Spawn failed: candidate model could not be loaded for profile=%s", profile->label);
        frontier_set_status(frontier_text("Spawn failed because the NPC model did not load.", "NPC 모델이 로드되지 않아 소환에 실패했습니다."));
        return 0;
    }
    if (model_hash == 0U) {
        frontier_log("Spawn failed: no valid model hash for %s.", profile->label);
        frontier_set_status(frontier_text("Spawn failed because no valid model was found.", "유효한 모델을 찾지 못해 소환에 실패했습니다."));
        return 0;
    }

    weapon_hash = frontier_get_hash_key(profile->weapon_name);
    frontier_log("Spawn weapon hash resolved: 0x%X for %s", (unsigned int)weapon_hash, profile->weapon_name);
    player_heading = frontier_get_entity_heading(player_ped);
    for (index = 0; index < requested_count; ++index) {
        FrontierVector3 spawn_pos = frontier_get_offset_from_entity_coords(
            player_ped,
            profile->first_offset_x + (profile->spacing_x * (float)index),
            profile->first_offset_y + (profile->spacing_y * (float)index),
            0.0f
        );
        int ped;

        frontier_log("Creating ped for %s index=%d at %.2f %.2f %.2f", profile->label, index, spawn_pos.x, spawn_pos.y, spawn_pos.z);
        if (donor_ped != 0) {
            ped = frontier_clone_ped(donor_ped);
            if (ped != 0) {
                frontier_set_entity_coords_no_offset(ped, spawn_pos.x, spawn_pos.y, spawn_pos.z);
                frontier_set_entity_heading(ped, player_heading + 180.0f);
                frontier_set_entity_alpha(ped, 255);
                frontier_set_entity_visible(ped, 1);
                frontier_freeze_entity_position(ped, 0);
                frontier_set_entity_invincible(ped, 0);
                frontier_resurrect_ped(ped);
            }
        } else {
            ped = frontier_create_ped(model_hash, spawn_pos.x, spawn_pos.y, spawn_pos.z, player_heading + 180.0f);
            if (ped != 0) {
                frontier_set_entity_alpha(ped, 255);
                frontier_set_entity_visible(ped, 1);
                frontier_set_entity_coords_no_offset(ped, spawn_pos.x, spawn_pos.y, spawn_pos.z);
                frontier_set_entity_heading(ped, player_heading + 180.0f);
            }
        }
        if (ped == 0) {
            frontier_log("Spawn failed: CREATE_PED returned 0 for %s index=%d.", profile->label, index);
            continue;
        }

        {
            FrontierTrackedPed *tracked = frontier_live_find_tracked_ped(ped, 1, (DWORD)frontier_game_timer());
            if (tracked != NULL) {
                tracked->spawned_by_frontier = 1;
                tracked->spawn_label = profile->label;
                tracked->spawn_label_ko = profile->label_ko;
            }
        }
        frontier_log("Spawned ped handle=%d for %s index=%d.", ped, profile->label, index);
        frontier_set_entity_max_health(ped, profile->health);
        frontier_set_ped_max_health(ped, profile->health);
        frontier_set_entity_health(ped, profile->health, 0);
        frontier_set_ped_accuracy(ped, frontier_clamp_int(profile->accuracy, 1, 100));
        frontier_set_ped_combat_ability(ped, frontier_clamp_int(profile->combat_ability, 0, 2));
        frontier_set_ped_combat_range(ped, frontier_clamp_int(profile->combat_range, 0, 3));
        frontier_set_ped_combat_movement(ped, frontier_clamp_int(profile->combat_movement, 0, 3));
        frontier_set_ped_keep_task(ped, 1);
        frontier_set_blocking_of_non_temporary_events(ped, 1);
        frontier_set_ped_can_be_targetted(ped, 1);
        frontier_set_ped_can_ragdoll(ped, 1);
        frontier_set_ped_combat_attribute(ped, 5, 1);
        frontier_set_ped_combat_attribute(ped, 13, 1);
        frontier_set_ped_combat_attribute(ped, 46, 1);
        if (weapon_hash != 0U) {
            frontier_log("Giving weapon hash=0x%X to ped=%d.", (unsigned int)weapon_hash, ped);
            frontier_give_weapon_to_ped(ped, weapon_hash, 120);
        }
        frontier_log("Assigning combat task to ped=%d target=%d.", ped, player_ped);
        frontier_task_combat_ped(ped, player_ped);
        spawned += 1;
    }

    frontier_set_model_as_no_longer_needed(model_hash);
    return spawned;
}

static int frontier_label_contains(const char *label, const char *needle)
{
    return label != NULL && needle != NULL && strstr(label, needle) != NULL;
}

static int frontier_page_entry_contains_text(const FrontierLedgerPage *page, int entry_index, const char *needle)
{
    const FrontierLedgerEntry *entry;
    const char *resolved_label;
    const char *resolved_detail;

    if (page == NULL || needle == NULL || needle[0] == '\0' || entry_index < 0 || entry_index >= page->entry_count) {
        return 0;
    }

    entry = &page->entries[entry_index];
    resolved_label = frontier_resolve_text(entry->label);
    resolved_detail = frontier_resolve_text(entry->detail);
    return frontier_label_contains(resolved_label, needle) || frontier_label_contains(resolved_detail, needle);
}

static void frontier_log_selected_entry_context(const FrontierLedgerPage *page, int entry_index, const char *reason)
{
    const FrontierLedgerEntry *entry;
    const char *resolved_label;

    if (page == NULL || entry_index < 0 || entry_index >= page->entry_count) {
        frontier_log("Selected-entry context unavailable for %s.", reason != NULL ? reason : "unknown");
        return;
    }

    entry = &page->entries[entry_index];
    resolved_label = frontier_resolve_text(entry->label);
    frontier_log(
        "Selected-entry context for %s: page=%s entry=%d label=%s",
        reason != NULL ? reason : "unknown",
        page->page_id != NULL ? page->page_id : "null",
        entry_index,
        resolved_label != NULL ? resolved_label : "null"
    );
}

static const FrontierSpawnProfile *frontier_region_spawn_profile(void)
{
    int region_index = g_runtime.context.valid ? g_runtime.context.region_index : 0;
    int profile_count = (int)(sizeof(kFrontierRegionSpawnProfiles) / sizeof(kFrontierRegionSpawnProfiles[0]));

    if (region_index < 0 || region_index >= profile_count) {
        region_index = 0;
    }

    switch (region_index) {
    case 0:
        if (g_runtime.live_profile.trail_stage <= 1 || g_runtime.live_profile.contract_count <= 0) {
            return &kFrontierCourierSpawnProfile;
        }
        if (!g_runtime.live_profile.boss_blackwater_ready &&
            (g_runtime.live_profile.enemy_kills >= 4 || g_runtime.live_profile.reward_earned >= 80)) {
            return &kFrontierRuskSpawnProfile;
        }
        return &kFrontierRegionSpawnProfiles[0];
    case 1:
        if (!g_runtime.live_profile.boss_silas_ready &&
            (g_runtime.live_profile.trail_stage >= 3 || g_runtime.live_profile.enemy_kills >= 5)) {
            return &kFrontierSilasPosseSpawnProfile;
        }
        return &kFrontierRegionSpawnProfiles[1];
    default:
        return &kFrontierRegionSpawnProfiles[region_index];
    }
}

static const FrontierSpawnProfile *frontier_selected_entry_spawn_profile(void)
{
    const FrontierLedgerPage *page = frontier_current_page();
    int entry_index;

    if (page == NULL || page->entry_count <= 0) {
        return NULL;
    }

    if (strcmp(page->page_id, "dead") == 0) {
        return NULL;
    }

    entry_index = frontier_clamp_int(g_runtime.selected_entry[g_runtime.current_page], 0, page->entry_count - 1);
    if (frontier_page_entry_contains_text(page, entry_index, "Silas Redd, The Fence of Ash") ||
        frontier_page_entry_contains_text(page, entry_index, "Silas Redd Hunt")) {
        return &kFrontierBossSpawnProfiles[0];
    }
    if (frontier_page_entry_contains_text(page, entry_index, "Blackwater Broker Cell") ||
        frontier_page_entry_contains_text(page, entry_index, "Blackwater Broker")) {
        return &kFrontierBossSpawnProfiles[1];
    }
    if (frontier_page_entry_contains_text(page, entry_index, "Tall Trees Tracker King")) {
        return &kFrontierBossSpawnProfiles[2];
    }
    if (frontier_page_entry_contains_text(page, entry_index, "Lemoyne Relic Keeper") ||
        frontier_page_entry_contains_text(page, entry_index, "Lemoyne Relic Whispers")) {
        return &kFrontierBossSpawnProfiles[3];
    }
    if (frontier_page_entry_contains_text(page, entry_index, "The Surveyor") ||
        frontier_page_entry_contains_text(page, entry_index, "Surveyor's Reckoning")) {
        return &kFrontierBossSpawnProfiles[4];
    }
    if (frontier_page_entry_contains_text(page, entry_index, "Rusk") ||
        frontier_page_entry_contains_text(page, entry_index, "Collector's Dog") ||
        frontier_page_entry_contains_text(page, entry_index, "Rusk Contract Holdout")) {
        return &kFrontierRuskSpawnProfile;
    }
    if (frontier_page_entry_contains_text(page, entry_index, "Dead Courier") ||
        frontier_page_entry_contains_text(page, entry_index, "Courier Reclaimers") ||
        frontier_page_entry_contains_text(page, entry_index, "Courier Satchel")) {
        return &kFrontierCourierSpawnProfile;
    }
    if (frontier_page_entry_contains_text(page, entry_index, "Silas Redd Posse") ||
        frontier_page_entry_contains_text(page, entry_index, "Great Plains Convoy Wreck") ||
        frontier_page_entry_contains_text(page, entry_index, "Silas Redd Hunt")) {
        return &kFrontierSilasPosseSpawnProfile;
    }
    if (frontier_page_entry_contains_text(page, entry_index, "Tall Trees Tracker")) {
        return &kFrontierRegionSpawnProfiles[1];
    }
    if (frontier_page_entry_contains_text(page, entry_index, "Blackwater Mercenary")) {
        return &kFrontierRegionSpawnProfiles[0];
    }
    if (frontier_page_entry_contains_text(page, entry_index, "Armadillo") ||
        frontier_page_entry_contains_text(page, entry_index, "New Austin")) {
        return &kFrontierRegionSpawnProfiles[2];
    }
    if (frontier_page_entry_contains_text(page, entry_index, "New Hanover")) {
        return &kFrontierRegionSpawnProfiles[3];
    }
    if (frontier_page_entry_contains_text(page, entry_index, "Lemoyne")) {
        return &kFrontierRegionSpawnProfiles[4];
    }
    if (frontier_page_entry_contains_text(page, entry_index, "Surveyor") ||
        frontier_page_entry_contains_text(page, entry_index, "Ambarino")) {
        return &kFrontierRegionSpawnProfiles[5];
    }
    return NULL;
}

static const FrontierSpawnProfile *frontier_selected_boss_spawn_profile(void)
{
    const FrontierLedgerPage *page = frontier_current_page();
    int entry_index;

    if (page == NULL || strcmp(page->page_id, "dead") != 0 || page->entry_count <= 0) {
        return NULL;
    }

    entry_index = frontier_clamp_int(g_runtime.selected_entry[g_runtime.current_page], 0, page->entry_count - 1);
    if (frontier_page_entry_contains_text(page, entry_index, "Silas Redd")) {
        return &kFrontierBossSpawnProfiles[0];
    }
    if (frontier_page_entry_contains_text(page, entry_index, "Blackwater Broker")) {
        return &kFrontierBossSpawnProfiles[1];
    }
    if (frontier_page_entry_contains_text(page, entry_index, "Tall Trees Tracker King")) {
        return &kFrontierBossSpawnProfiles[2];
    }
    if (frontier_page_entry_contains_text(page, entry_index, "Lemoyne Relic Keeper")) {
        return &kFrontierBossSpawnProfiles[3];
    }
    if (frontier_page_entry_contains_text(page, entry_index, "Surveyor")) {
        return &kFrontierBossSpawnProfiles[4];
    }
    return NULL;
}

static int frontier_spawn_region_encounter(void)
{
    const FrontierSpawnProfile *profile = frontier_region_spawn_profile();
    frontier_log("Processing regional spawn request for profile=%s", profile != NULL ? profile->label : "null");
    int spawned = frontier_spawn_profile_near_player(profile);
    if (spawned > 0) {
        frontier_set_status(frontier_text("Regional encounter seeded in the field.", "지역 전투 시드가 필드에 소환되었습니다."));
        frontier_log("Spawned regional encounter: %s count=%d regionIndex=%d", profile->label, spawned, g_runtime.context.region_index);
    } else {
        frontier_set_status(frontier_text("Regional encounter spawn failed.", "지역 전투 소환에 실패했습니다."));
        frontier_log("Regional encounter spawn failed: %s", profile != NULL ? profile->label : "null");
    }
    return spawned;
}

static int frontier_spawn_selected_entry_encounter(void)
{
    const FrontierLedgerPage *page = frontier_current_page();
    int entry_index = page != NULL && page->entry_count > 0
        ? frontier_clamp_int(g_runtime.selected_entry[g_runtime.current_page], 0, page->entry_count - 1)
        : 0;
    const FrontierSpawnProfile *profile = frontier_selected_entry_spawn_profile();
    int spawned;

    if (profile == NULL) {
        frontier_log_selected_entry_context(page, entry_index, "selected-encounter-miss");
        frontier_set_status("No authored encounter is linked to the selected entry.");
        return 0;
    }

    frontier_log_selected_entry_context(page, entry_index, "selected-encounter");
    frontier_log("Processing selected-entry spawn request for profile=%s", profile->label);
    spawned = frontier_spawn_profile_near_player(profile);
    if (spawned > 0) {
        g_runtime.full_visible = 0;
        frontier_session_save();
        frontier_mark_ui_dirty();
        frontier_set_status("Selected encounter seeded from the full ledger.");
        frontier_log("Spawned selected authored encounter: %s count=%d", profile->label, spawned);
    } else {
        frontier_set_status("Selected encounter spawn failed.");
        frontier_log("Selected authored encounter spawn failed: %s", profile->label);
    }
    return spawned;
}

static int frontier_spawn_selected_boss(void)
{
    const FrontierLedgerPage *page = frontier_current_page();
    int entry_index = page != NULL && page->entry_count > 0
        ? frontier_clamp_int(g_runtime.selected_entry[g_runtime.current_page], 0, page->entry_count - 1)
        : 0;
    const FrontierSpawnProfile *profile = frontier_selected_boss_spawn_profile();
    int spawned;

    if (profile == NULL) {
        frontier_set_status(frontier_text("Open the boss archive and select a boss entry first.", "보스 장부에서 보스 항목을 먼저 선택해 주세요."));
        return 0;
    }

    frontier_log_selected_entry_context(page, entry_index, "boss-encounter");
    frontier_log("Processing boss spawn request for profile=%s", profile->label);
    spawned = frontier_spawn_profile_near_player(profile);
    if (spawned > 0) {
        g_runtime.full_visible = 0;
        frontier_session_save();
        frontier_mark_ui_dirty();
        frontier_set_status(frontier_text("Boss summon seeded from the archive.", "보스 소환이 장부에서 시드되었습니다."));
        frontier_log("Spawned boss encounter: %s count=%d", profile->label, spawned);
    } else {
        frontier_set_status(frontier_text("Boss summon failed.", "보스 소환에 실패했습니다."));
        frontier_log("Boss summon failed: %s", profile->label);
    }
    return spawned;
}

static void frontier_try_spawn_debug_path(void)
{
    DWORD now = (DWORD)frontier_game_timer();
    if (g_runtime.last_spawn_triggered_at != 0 && now - g_runtime.last_spawn_triggered_at < 1200U) {
        return;
    }
    g_runtime.last_spawn_triggered_at = now;
    frontier_log(
        "Spawn debug triggered. Full=%d Compact=%d Workbench=%d Page=%d RegionIndex=%d Region=%s",
        g_runtime.full_visible,
        g_runtime.compact_visible,
        g_runtime.workbench_visible,
        g_runtime.current_page,
        g_runtime.context.region_index,
        frontier_runtime_context_label()
    );

    if (g_runtime.full_visible || g_runtime.compact_visible || g_runtime.workbench_visible) {
        frontier_set_status("Close overlays before using F8 spawn debug. Use E for selected or boss summons.");
        frontier_log("Spawn debug ignored because an overlay is open.");
        if (0) {
            frontier_set_status(frontier_text("Spawn debug works from the field or the boss archive.", "디버그 소환은 필드 또는 보스 장부에서 동작합니다."));
        }
        return;
    }

    if (!g_runtime.compact_visible && !g_runtime.workbench_visible) {
        g_runtime.pending_spawn_request = FRONTIER_SPAWN_REQUEST_REGION;
        g_runtime.debug_spawn_single_ped = 1;
        frontier_set_status("Regional spawn queued.");
        frontier_log("Queued regional spawn request from field state. DebugSinglePed=1");
    }
}
