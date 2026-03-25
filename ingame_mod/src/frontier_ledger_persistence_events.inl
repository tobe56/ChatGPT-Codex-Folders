static int frontier_live_profile_phase_from_state(void)
{
    if (g_runtime.live_profile.discovered_region_count >= 5 &&
        g_runtime.live_profile.hub_returns >= 3 &&
        g_runtime.live_profile.field_distance_total >= kFrontierPhaseTwoDistance) {
        return FRONTIER_PHASE_ENDGAME;
    }

    if (g_runtime.live_profile.discovered_region_count >= 2 &&
        g_runtime.live_profile.hub_returns >= 1 &&
        g_runtime.live_profile.field_distance_total >= kFrontierPhaseOneDistance) {
        return FRONTIER_PHASE_ESCALATION;
    }

    return FRONTIER_PHASE_SETUP;
}

static int frontier_live_recent_event_exists(int kind, int entity, int value, DWORD now, DWORD window_ms)
{
    int index;

    for (index = 0; index < FRONTIER_LIVE_RECENT_EVENT_SLOTS; ++index) {
        FrontierRecentEvent *event = &g_runtime.recent_events[index];
        if (event->kind != kind) {
            continue;
        }
        if (event->entity != entity) {
            continue;
        }
        if (kind == FRONTIER_EVENT_KIND_REWARD && event->value != value) {
            continue;
        }
        if (event->recorded_at != 0 && now - event->recorded_at <= window_ms) {
            return 1;
        }
    }

    return 0;
}

static void frontier_live_record_recent_event(int kind, int entity, int value, DWORD now)
{
    int index;
    int oldest_index = 0;
    DWORD oldest_at = 0xFFFFFFFFUL;

    for (index = 0; index < FRONTIER_LIVE_RECENT_EVENT_SLOTS; ++index) {
        FrontierRecentEvent *event = &g_runtime.recent_events[index];
        if (event->recorded_at == 0) {
            oldest_index = index;
            oldest_at = 0;
            break;
        }
        if (event->recorded_at < oldest_at) {
            oldest_at = event->recorded_at;
            oldest_index = index;
        }
    }

    g_runtime.recent_events[oldest_index].kind = kind;
    g_runtime.recent_events[oldest_index].entity = entity;
    g_runtime.recent_events[oldest_index].value = value;
    g_runtime.recent_events[oldest_index].recorded_at = now;
}

static FrontierTrackedPed *frontier_live_find_tracked_ped(int entity, int create_slot, DWORD now)
{
    int index;
    int stale_index = -1;
    DWORD ttl_ms = (DWORD)(g_runtime.config.tracked_ped_ttl_ms > 0 ? g_runtime.config.tracked_ped_ttl_ms : (int)kFrontierTrackedPedTtlMs);
    ttl_ms = (DWORD)frontier_max_int(10000, (int)ttl_ms);

    if (entity == 0) {
        return NULL;
    }

    for (index = 0; index < FRONTIER_LIVE_TRACKED_PED_SLOTS; ++index) {
        FrontierTrackedPed *slot = &g_runtime.tracked_peds[index];
        if (slot->entity == entity) {
            slot->last_seen_at = now;
            return slot;
        }
        if (stale_index < 0 &&
            (slot->entity == 0 ||
             (slot->last_seen_at != 0 && now - slot->last_seen_at > ttl_ms))) {
            stale_index = index;
        }
    }

    if (!create_slot || stale_index < 0) {
        return NULL;
    }

    ZeroMemory(&g_runtime.tracked_peds[stale_index], sizeof(g_runtime.tracked_peds[stale_index]));
    g_runtime.tracked_peds[stale_index].entity = entity;
    g_runtime.tracked_peds[stale_index].last_seen_at = now;
    return &g_runtime.tracked_peds[stale_index];
}

static int frontier_live_should_credit_player_kill(int ped, int player_ped, FrontierTrackedPed *tracked)
{
    int source;
    int attached_to_source;
    int damaged_by_player;

    if (ped == 0 || player_ped == 0) {
        return 0;
    }

    source = frontier_get_ped_source_of_death(ped);
    if (source == player_ped) {
        return 1;
    }

    attached_to_source = frontier_get_entity_attached_to(source);
    if (attached_to_source == player_ped) {
        return 1;
    }

    damaged_by_player = frontier_has_entity_been_damaged_by_entity(ped, player_ped);
    if (damaged_by_player) {
        return 1;
    }

    if (tracked != NULL && tracked->spawned_by_frontier && frontier_is_trackable_enemy_ped(ped)) {
        frontier_log(
            "Kill credit fallback used for spawned frontier ped=%d source=%d attached=%d damagedByPlayer=%d",
            ped,
            source,
            attached_to_source,
            damaged_by_player
        );
        return 1;
    }

    frontier_log(
        "Kill not credited. ped=%d source=%d attached=%d damagedByPlayer=%d spawned=%d human=%d animal=%d",
        ped,
        source,
        attached_to_source,
        damaged_by_player,
        tracked != NULL ? tracked->spawned_by_frontier : 0,
        frontier_is_trackable_enemy_ped(ped),
        frontier_is_trackable_animal_ped(ped)
    );
    return 0;
}

static void frontier_live_profile_register_enemy_kill(int ped)
{
    DWORD now = (DWORD)frontier_game_timer();
    if (frontier_live_recent_event_exists(FRONTIER_EVENT_KIND_ENEMY_KILL, ped, 0, now, 15000U)) {
        return;
    }
    frontier_live_record_recent_event(FRONTIER_EVENT_KIND_ENEMY_KILL, ped, 0, now);
    g_runtime.live_profile.enemy_kills = frontier_clamp_int(g_runtime.live_profile.enemy_kills + 1, 0, 99999);
    g_runtime.live_profile.dirty = 1;
    frontier_log("Live counter updated: enemy_kills=%d ped=%d", g_runtime.live_profile.enemy_kills, ped);
    frontier_mark_ui_dirty();
}

static void frontier_live_profile_register_animal_kill(int ped)
{
    DWORD now = (DWORD)frontier_game_timer();
    if (frontier_live_recent_event_exists(FRONTIER_EVENT_KIND_ANIMAL_KILL, ped, 0, now, 15000U)) {
        return;
    }
    frontier_live_record_recent_event(FRONTIER_EVENT_KIND_ANIMAL_KILL, ped, 0, now);
    g_runtime.live_profile.animal_kills = frontier_clamp_int(g_runtime.live_profile.animal_kills + 1, 0, 99999);
    g_runtime.live_profile.dirty = 1;
    frontier_log("Live counter updated: animal_kills=%d ped=%d", g_runtime.live_profile.animal_kills, ped);
    frontier_mark_ui_dirty();
}

static void frontier_live_profile_register_loot(int entity)
{
    DWORD now = (DWORD)frontier_game_timer();
    if (frontier_live_recent_event_exists(FRONTIER_EVENT_KIND_LOOT, entity, 0, now, 20000U)) {
        return;
    }
    frontier_live_record_recent_event(FRONTIER_EVENT_KIND_LOOT, entity, 0, now);
    g_runtime.live_profile.loot_count = frontier_clamp_int(g_runtime.live_profile.loot_count + 1, 0, 99999);
    g_runtime.live_profile.dirty = 1;
    frontier_log("Live counter updated: loot_count=%d entity=%d", g_runtime.live_profile.loot_count, entity);
    frontier_mark_ui_dirty();
}

static void frontier_live_profile_register_reward(int delta)
{
    DWORD now = (DWORD)frontier_game_timer();
    if (delta <= 0) {
        return;
    }
    if (g_runtime.last_reward_delta == delta &&
        g_runtime.last_reward_counted_at != 0 &&
        now - g_runtime.last_reward_counted_at <= 1500U) {
        return;
    }
    if (frontier_live_recent_event_exists(FRONTIER_EVENT_KIND_REWARD, 0, delta, now, 1200U)) {
        return;
    }
    frontier_live_record_recent_event(FRONTIER_EVENT_KIND_REWARD, 0, delta, now);
    g_runtime.last_reward_delta = delta;
    g_runtime.last_reward_counted_at = now;
    g_runtime.live_profile.reward_earned = frontier_clamp_int(g_runtime.live_profile.reward_earned + delta, 0, 999999);
    g_runtime.live_profile.dirty = 1;
    frontier_log("Live counter updated: reward_earned=%d delta=%d", g_runtime.live_profile.reward_earned, delta);
    frontier_mark_ui_dirty();
}
