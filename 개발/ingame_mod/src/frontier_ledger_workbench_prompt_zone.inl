static void frontier_render_zone_prompt(void)
{
    if (!g_runtime.config.show_zone_prompt) {
        return;
    }
    frontier_draw_rect_tl(0.33f, 0.835f, 0.31f, 0.058f, kColorBackground);
    frontier_draw_rect_tl(0.33f, 0.835f, 0.31f, 0.008f, kColorAccentSoft);
    frontier_draw_text_short(
        frontier_text("Beecher's Hope [F11] full ledger", "거점 [F11] 레저"),
        0.350f,
        0.852f,
        frontier_dataset_is_korean() ? 0.40f : 0.34f,
        kColorText
    );
}

static const FrontierTrackedPed *frontier_active_frontier_corpse(void)
{
    return NULL;
    int player_ped = frontier_player_ped();
    FrontierVector3 player_position;
    const FrontierTrackedPed *best = NULL;
    float best_distance_sq = 64.0f;
    int index;

    if (player_ped == 0) {
        return NULL;
    }

    player_position = frontier_get_entity_coords(player_ped);
    for (index = 0; index < FRONTIER_LIVE_TRACKED_PED_SLOTS; ++index) {
        const FrontierTrackedPed *tracked = &g_runtime.tracked_peds[index];
        FrontierVector3 corpse_position;
        float dx;
        float dy;
        float dz;
        float distance_sq;

        if (!tracked->spawned_by_frontier || tracked->loot_resolved || tracked->entity == 0) {
            continue;
        }
        if (!frontier_does_entity_exist(tracked->entity) || !frontier_is_ped_dead_or_dying(tracked->entity)) {
            continue;
        }
        if (frontier_is_entity_fully_looted(tracked->entity)) {
            continue;
        }

        corpse_position = frontier_get_entity_coords(tracked->entity);
        dx = corpse_position.x - player_position.x;
        dy = corpse_position.y - player_position.y;
        dz = frontier_abs_float(corpse_position.z - player_position.z);
        distance_sq = (dx * dx) + (dy * dy);
        if (dz > 3.0f || distance_sq > best_distance_sq) {
            continue;
        }

        best_distance_sq = distance_sq;
        best = tracked;
    }

    return best;
}

static void frontier_render_frontier_corpse_prompt(void)
{
    const FrontierTrackedPed *tracked = frontier_active_frontier_corpse();
    const char *label;

    if (tracked == NULL) {
        return;
    }

    label = frontier_dataset_is_korean()
        ? (tracked->spawn_label_ko != NULL && tracked->spawn_label_ko[0] != '\0' ? tracked->spawn_label_ko : tracked->spawn_label)
        : tracked->spawn_label;
    if (label == NULL || label[0] == '\0') {
        label = "Authored frontier target";
    }

    frontier_draw_rect_tl(0.29f, 0.830f, 0.42f, 0.065f, kColorBackground);
    frontier_draw_rect_tl(0.29f, 0.830f, 0.42f, 0.009f, kColorAccentSoft);
    snprintf(
        g_runtime.runtime_entry_label,
        sizeof(g_runtime.runtime_entry_label),
        frontier_dataset_is_korean() ? "Authored corpse | %s" : "Authored corpse | %s",
        label
    );
    frontier_draw_text_short(
        g_runtime.runtime_entry_label,
        0.312f,
        0.852f,
        frontier_dataset_is_korean() ? 0.30f : 0.28f,
        kColorText
    );
}

static int frontier_inside_workbench_zone(void)
{
    float dx;
    float dy;
    float radius_sq;
    float dz;

    frontier_refresh_zone_state(0);
    if (!g_runtime.zone_state.valid) {
        return 0;
    }

    dx = g_runtime.zone_state.position.x - g_runtime.config.workbench_x;
    dy = g_runtime.zone_state.position.y - g_runtime.config.workbench_y;
    dz = frontier_abs_float(g_runtime.zone_state.position.z - g_runtime.config.workbench_z);
    radius_sq = g_runtime.config.workbench_radius * g_runtime.config.workbench_radius;

    return g_runtime.zone_state.inside_zone &&
        ((dx * dx) + (dy * dy)) <= radius_sq &&
        dz <= 6.0f;
}

static const char *frontier_workbench_tab_name(int tab_index)
{
    switch (tab_index) {
    case FRONTIER_WORKBENCH_TAB_CRAFT:
        return frontier_ui_text("Craft", "FL_WB_TAB_CRAFT");
    case FRONTIER_WORKBENCH_TAB_ARCHIVE:
        return frontier_ui_text("Archive", "FL_WB_TAB_ARCHIVE");
    default:
        return frontier_ui_text("Result", "FL_WB_TAB_RESULT");
    }
}

static int frontier_workbench_entry_count(int tab_index)
{
    switch (tab_index) {
    case FRONTIER_WORKBENCH_TAB_CRAFT:
        return FRONTIER_WORKBENCH_CRAFT_COUNT;
    case FRONTIER_WORKBENCH_TAB_ARCHIVE:
        return FRONTIER_WORKBENCH_ARCHIVE_COUNT;
    default:
        return FRONTIER_WORKBENCH_RESULT_COUNT;
    }
}

static int frontier_workbench_clamp_entry_index(int tab_index, int entry_index)
{
    int entry_count = frontier_workbench_entry_count(tab_index);
    if (entry_count <= 0) {
        return 0;
    }
    return frontier_clamp_int(entry_index, 0, entry_count - 1);
}

