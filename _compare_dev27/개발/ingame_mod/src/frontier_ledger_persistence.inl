static int frontier_runtime_file_has_content(const char *path)
{
    HANDLE handle;
    LARGE_INTEGER size;

    if (path == NULL || path[0] == '\0') {
        return 0;
    }

    handle = CreateFileA(
        path,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (handle == INVALID_HANDLE_VALUE) {
        return 0;
    }

    size.QuadPart = 0;
    if (!GetFileSizeEx(handle, &size)) {
        CloseHandle(handle);
        return 0;
    }

    CloseHandle(handle);
    return size.QuadPart > 0;
}

static int frontier_write_ini_string_retry(
    const char *file_name,
    const char *section,
    const char *key,
    const char *value,
    char *path,
    size_t path_size
)
{
    frontier_ensure_runtime_ini_path(path, path_size, file_name);
    if (path == NULL || path[0] == '\0') {
        return 0;
    }
    frontier_touch_runtime_file(path);
    if (WritePrivateProfileStringA(section, key, value, path)) {
        return 1;
    }
    frontier_build_neighbor_path(g_runtime.module_path, file_name, path, path_size);
    frontier_touch_runtime_file(path);
    return WritePrivateProfileStringA(section, key, value, path) != 0;
}

static int frontier_flush_ini_retry(const char *file_name, char *path, size_t path_size)
{
    int flush_ok = 0;

    frontier_ensure_runtime_ini_path(path, path_size, file_name);
    if (path == NULL || path[0] == '\0') {
        return 0;
    }
    frontier_touch_runtime_file(path);
    if (WritePrivateProfileStringA(NULL, NULL, NULL, path)) {
        flush_ok = 1;
    } else {
        frontier_build_neighbor_path(g_runtime.module_path, file_name, path, path_size);
        frontier_touch_runtime_file(path);
        flush_ok = WritePrivateProfileStringA(NULL, NULL, NULL, path) != 0;
    }
    if (flush_ok) {
        return 1;
    }
    return frontier_runtime_file_has_content(path);
}

static void frontier_session_save(void)
{
    char buffer[32];
    int index;
    int write_failed = 0;

    frontier_ensure_runtime_ini_path(g_runtime.session_path, sizeof(g_runtime.session_path), "FrontierLedger.session.ini");
    if (g_runtime.session_path[0] == '\0') {
        return;
    }

    if (g_runtime.requested_dataset_id[0] != '\0') {
        if (!frontier_write_ini_string_retry(
                "FrontierLedger.session.ini",
                "Session",
                "Dataset",
                g_runtime.requested_dataset_id,
                g_runtime.session_path,
                sizeof(g_runtime.session_path))) {
            write_failed = 1;
        }
    } else if (g_runtime.dataset != NULL && g_runtime.dataset->dataset_id != NULL) {
        if (!frontier_write_ini_string_retry(
                "FrontierLedger.session.ini",
                "Session",
                "Dataset",
                g_runtime.dataset->dataset_id,
                g_runtime.session_path,
                sizeof(g_runtime.session_path))) {
            write_failed = 1;
        }
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.current_page);
    if (!frontier_write_ini_string_retry(
            "FrontierLedger.session.ini",
            "Session",
            "CurrentPage",
            buffer,
            g_runtime.session_path,
            sizeof(g_runtime.session_path))) {
        write_failed = 1;
    }

    for (index = 0; index < FRONTIER_MAX_PAGES; ++index) {
        char key[24];
        snprintf(key, sizeof(key), "SelectedEntry%d", index);
        snprintf(buffer, sizeof(buffer), "%d", g_runtime.selected_entry[index]);
        if (!frontier_write_ini_string_retry(
                "FrontierLedger.session.ini",
                "Selection",
                key,
                buffer,
                g_runtime.session_path,
                sizeof(g_runtime.session_path))) {
            write_failed = 1;
        }
    }

    snprintf(buffer, sizeof(buffer), "%d", frontier_clamp_int(g_runtime.workbench_tab, 0, 2));
    if (!frontier_write_ini_string_retry(
            "FrontierLedger.session.ini",
            "Workbench",
            "Tab",
            buffer,
            g_runtime.session_path,
            sizeof(g_runtime.session_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", frontier_max_int(0, g_runtime.workbench_selected_entry[FRONTIER_WORKBENCH_TAB_CRAFT]));
    if (!frontier_write_ini_string_retry(
            "FrontierLedger.session.ini",
            "Workbench",
            "EntryCraft",
            buffer,
            g_runtime.session_path,
            sizeof(g_runtime.session_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", frontier_max_int(0, g_runtime.workbench_selected_entry[FRONTIER_WORKBENCH_TAB_ARCHIVE]));
    if (!frontier_write_ini_string_retry(
            "FrontierLedger.session.ini",
            "Workbench",
            "EntryArchive",
            buffer,
            g_runtime.session_path,
            sizeof(g_runtime.session_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", frontier_max_int(0, g_runtime.workbench_selected_entry[FRONTIER_WORKBENCH_TAB_RESULT]));
    if (!frontier_write_ini_string_retry(
            "FrontierLedger.session.ini",
            "Workbench",
            "EntryResult",
            buffer,
            g_runtime.session_path,
            sizeof(g_runtime.session_path))) {
        write_failed = 1;
    }

    if (!frontier_flush_ini_retry("FrontierLedger.session.ini", g_runtime.session_path, sizeof(g_runtime.session_path))) {
        write_failed = 1;
    }

    if (write_failed && GetTickCount() - g_runtime.last_session_write_failure_at >= kFrontierWriteFailureLogCooldownMs) {
        g_runtime.last_session_write_failure_at = GetTickCount();
        frontier_log("Session save encountered a write failure. Path=%s", g_runtime.session_path);
    } else if (!write_failed &&
               GetTickCount() - g_runtime.last_session_save_log_at >= kFrontierSaveLogCooldownMs) {
        g_runtime.last_session_save_log_at = GetTickCount();
        frontier_log(
            "Session saved. Dataset=%s Page=%d",
            g_runtime.requested_dataset_id[0] != '\0'
                ? g_runtime.requested_dataset_id
                : (g_runtime.dataset != NULL ? g_runtime.dataset->dataset_id : "null"),
            g_runtime.current_page
        );
    }
}

static void frontier_session_load(void)
{
    char dataset_buffer[32];
    int index;
    int page_count;

    frontier_ensure_runtime_ini_path(g_runtime.session_path, sizeof(g_runtime.session_path), "FrontierLedger.session.ini");
    if (g_runtime.session_path[0] == '\0') {
        return;
    }

    GetPrivateProfileStringA("Session", "Dataset", g_runtime.config.default_dataset, dataset_buffer, (DWORD)sizeof(dataset_buffer), g_runtime.session_path);
    if (dataset_buffer[0] != '\0') {
        frontier_select_dataset(dataset_buffer);
    }

    g_runtime.current_page = (int)GetPrivateProfileIntA("Session", "CurrentPage", 0, g_runtime.session_path);
    for (index = 0; index < FRONTIER_MAX_PAGES; ++index) {
        char key[24];
        snprintf(key, sizeof(key), "SelectedEntry%d", index);
        g_runtime.selected_entry[index] = (int)GetPrivateProfileIntA("Selection", key, 0, g_runtime.session_path);
    }

    g_runtime.workbench_tab = frontier_clamp_int(
        (int)GetPrivateProfileIntA("Workbench", "Tab", 0, g_runtime.session_path),
        0,
        2
    );
    g_runtime.workbench_selected_entry[FRONTIER_WORKBENCH_TAB_CRAFT] =
        frontier_clamp_int((int)GetPrivateProfileIntA("Workbench", "EntryCraft", 0, g_runtime.session_path), 0, FRONTIER_WORKBENCH_CRAFT_COUNT - 1);
    g_runtime.workbench_selected_entry[FRONTIER_WORKBENCH_TAB_ARCHIVE] =
        frontier_clamp_int((int)GetPrivateProfileIntA("Workbench", "EntryArchive", 0, g_runtime.session_path), 0, FRONTIER_WORKBENCH_ARCHIVE_COUNT - 1);
    g_runtime.workbench_selected_entry[FRONTIER_WORKBENCH_TAB_RESULT] =
        frontier_clamp_int((int)GetPrivateProfileIntA("Workbench", "EntryResult", 0, g_runtime.session_path), 0, FRONTIER_WORKBENCH_RESULT_COUNT - 1);

    page_count = frontier_page_count();
    for (index = 0; index < page_count; ++index) {
        frontier_clamp_selection(index);
    }

    frontier_log(
        "Session loaded. SavedDataset=%s ActiveDataset=%s Page=%d Locale=%s",
        dataset_buffer[0] != '\0' ? dataset_buffer : g_runtime.config.default_dataset,
        g_runtime.requested_dataset_id[0] != '\0' ? g_runtime.requested_dataset_id : g_runtime.config.default_dataset,
        g_runtime.current_page,
        g_runtime.config.preferred_locale
    );
}

static int frontier_tab_window_start(int page_count, int current_page, int visible_tabs)
{
    int start_index;

    if (page_count <= 0 || visible_tabs >= page_count) {
        return 0;
    }

    if (visible_tabs < 1) {
        visible_tabs = 1;
    }

    start_index = current_page - (visible_tabs / 2);
    if (start_index < 0) {
        start_index = 0;
    }
    if (start_index + visible_tabs > page_count) {
        start_index = page_count - visible_tabs;
    }
    return start_index;
}

static int frontier_overlay_visible_tab_count(int width, int page_count)
{
    int visible_tabs;

    if (page_count <= 0) {
        return 0;
    }

    visible_tabs = width / 160;
    visible_tabs = frontier_max_int(4, visible_tabs);
    visible_tabs = frontier_min_int(visible_tabs, 6);
    return frontier_min_int(page_count, visible_tabs);
}

static int frontier_hud_visible_tab_count(float width, int page_count)
{
    int visible_tabs;

    if (page_count <= 0) {
        return 0;
    }

    visible_tabs = (width >= 0.56f) ? 6 : 5;
    visible_tabs = frontier_max_int(4, visible_tabs);
    visible_tabs = frontier_min_int(visible_tabs, 6);
    return frontier_min_int(page_count, visible_tabs);
}

static int frontier_count_wrapped_lines(const char *text, int max_chars, int max_lines)
{
    int line_count = 0;
    char line_buffer[128];
    const char *cursor;

    if (text == NULL || text[0] == '\0') {
        return 0;
    }
    cursor = frontier_resolve_text(text);

    if (frontier_dataset_is_korean()) {
        return frontier_count_bitmap_lines(text, max_chars, max_lines);
    }

    while (cursor != NULL && *cursor != '\0' && line_count < max_lines) {
        cursor = frontier_next_wrapped_line(cursor, max_chars, line_buffer, sizeof(line_buffer));
        if (line_buffer[0] != '\0' || (cursor != NULL && *cursor != '\0')) {
            ++line_count;
        }
    }

    if (line_count <= 0) {
        line_count = 1;
    }
    return line_count;
}

static const FrontierLedgerPage *frontier_current_page(void)
{
    int page_count = frontier_page_count();

    if (page_count <= 0) {
        return NULL;
    }
    if (g_runtime.current_page < 0) {
        g_runtime.current_page = 0;
    }
    if (g_runtime.current_page >= page_count) {
        g_runtime.current_page = page_count - 1;
    }
    return &g_runtime.dataset->pages[g_runtime.current_page];
}

static void frontier_clamp_selection(int page_index)
{
    int entry_count;

    if (g_runtime.dataset == NULL || page_index < 0 || page_index >= frontier_page_count()) {
        return;
    }

    entry_count = g_runtime.dataset->pages[page_index].entry_count;
    if (entry_count <= 0) {
        g_runtime.selected_entry[page_index] = 0;
        g_runtime.top_entry[page_index] = 0;
        return;
    }

    if (g_runtime.selected_entry[page_index] < 0) {
        g_runtime.selected_entry[page_index] = 0;
    }
    if (g_runtime.selected_entry[page_index] >= entry_count) {
        g_runtime.selected_entry[page_index] = entry_count - 1;
    }
    if (g_runtime.top_entry[page_index] < 0) {
        g_runtime.top_entry[page_index] = 0;
    }
    if (g_runtime.selected_entry[page_index] < g_runtime.top_entry[page_index]) {
        g_runtime.top_entry[page_index] = g_runtime.selected_entry[page_index];
    }
    if (g_runtime.selected_entry[page_index] >= g_runtime.top_entry[page_index] + frontier_page_entry_capacity()) {
        g_runtime.top_entry[page_index] = g_runtime.selected_entry[page_index] - frontier_page_entry_capacity() + 1;
    }
    if (g_runtime.top_entry[page_index] > entry_count - 1) {
        g_runtime.top_entry[page_index] = entry_count - 1;
    }
}

static const FrontierLedgerDataset *frontier_find_dataset_by_id(const char *dataset_id)
{
    int index;

    if (dataset_id == NULL || dataset_id[0] == '\0') {
        return &kFrontierLedgerDatasets[0];
    }

    for (index = 0; index < kFrontierLedgerDatasetCount; ++index) {
        if (strcmp(kFrontierLedgerDatasets[index].dataset_id, dataset_id) == 0) {
            return &kFrontierLedgerDatasets[index];
        }
    }

    return &kFrontierLedgerDatasets[0];
}

static void frontier_resolve_dataset_pointer(void)
{
    const char *resolved_id = g_runtime.requested_dataset_id;

    if (frontier_runtime_uses_live_dataset()) {
        resolved_id = frontier_live_phase_dataset_id();
    }

    g_runtime.dataset = frontier_find_dataset_by_id(resolved_id);
}

static void frontier_select_dataset(const char *dataset_id)
{
    const char *normalized_id = frontier_dataset_id_for_preferred_locale(dataset_id);

    if (dataset_id == NULL || dataset_id[0] == '\0') {
        frontier_copy_string(
            g_runtime.requested_dataset_id,
            sizeof(g_runtime.requested_dataset_id),
            frontier_dataset_id_for_preferred_locale(g_runtime.config.default_dataset)
        );
        frontier_resolve_dataset_pointer();
        frontier_mark_ui_dirty();
        return;
    }

    frontier_copy_string(g_runtime.requested_dataset_id, sizeof(g_runtime.requested_dataset_id), normalized_id);
    frontier_resolve_dataset_pointer();
    frontier_mark_ui_dirty();
}

static void frontier_set_status(const char *message)
{
    frontier_copy_string(g_runtime.status_message, sizeof(g_runtime.status_message), message);
    g_runtime.status_expires_at = (DWORD)(frontier_game_timer() + g_runtime.config.status_duration_ms);
    frontier_mark_ui_dirty();
}

static int frontier_status_active(void)
{
    if (g_runtime.status_message[0] == '\0') {
        return 0;
    }
    return frontier_game_timer() < (int)g_runtime.status_expires_at;
}

static void frontier_render_status(void)
{
    if (!frontier_status_active()) {
        return;
    }
    frontier_draw_rect_tl(0.32f, 0.90f, 0.36f, 0.055f, kColorToast);
    frontier_draw_wrapped_text(g_runtime.status_message, 0.34f, 0.913f, 0.37f, 0.021f, 58, 2, kColorText);
}

static void frontier_live_profile_save(int force_now)
{
    char buffer[64];
    DWORD now = force_now ? GetTickCount() : (DWORD)frontier_game_timer();
    int write_failed = 0;

    frontier_ensure_runtime_ini_path(g_runtime.profile_path, sizeof(g_runtime.profile_path), "FrontierLedger.profile.ini");
    if (g_runtime.profile_path[0] == '\0') {
        return;
    }

    if (!force_now && !g_runtime.live_profile.dirty) {
        return;
    }

    if (!force_now && now - g_runtime.live_profile.last_saved_at < kFrontierLiveSaveIntervalMs) {
        return;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.phase);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "Phase", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.trail_stage);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "TrailStage", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.discovered_region_mask);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "DiscoveredRegionMask", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.hub_returns);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "HubReturns", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.enemy_kills);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "EnemyKills", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.loot_count);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "LootCount", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.animal_kills);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "AnimalKills", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.reward_earned);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "RewardEarned", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%u", g_runtime.live_profile.crafted_mask);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "CraftedMask", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.stat_bonus_strength);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "StatBonusStrength", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.stat_bonus_grit);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "StatBonusGrit", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.stat_bonus_deadeye);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "StatBonusDeadeye", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.stat_bonus_survival);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "StatBonusSurvival", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.stat_bonus_cunning);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "StatBonusCunning", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.skill_bonus_hunting);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "SkillBonusHunting", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.skill_bonus_gunsmithing);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "SkillBonusGunsmithing", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.skill_bonus_field_medicine);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "SkillBonusFieldMedicine", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.skill_bonus_trapping);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "SkillBonusTrapping", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.skill_bonus_salvaging);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "SkillBonusSalvaging", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.skill_bonus_trade);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "SkillBonusTrade", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.skill_bonus_horse_handling);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "SkillBonusHorseHandling", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.skill_bonus_homestead);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "SkillBonusHomestead", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%.2f", g_runtime.live_profile.field_distance_total);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "FieldDistanceTotal", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%.2f", g_runtime.live_profile.field_distance_since_hub);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "FieldDistanceSinceHub", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    if (!frontier_flush_ini_retry("FrontierLedger.profile.ini", g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    if (write_failed && now - g_runtime.last_profile_write_failure_at >= kFrontierWriteFailureLogCooldownMs) {
        g_runtime.last_profile_write_failure_at = now;
        frontier_log("Profile save encountered a write failure. Path=%s", g_runtime.profile_path);
    } else if (!write_failed &&
               (force_now || now - g_runtime.last_profile_save_log_at >= kFrontierSaveLogCooldownMs)) {
        g_runtime.last_profile_save_log_at = now;
        frontier_log(
            "Profile saved. Phase=%d Stage=%d Enemy=%d Loot=%d Hunt=%d Reward=%d CraftedMask=0x%X StatPts=%d SkillPts=%d",
            g_runtime.live_profile.phase,
            g_runtime.live_profile.trail_stage,
            g_runtime.live_profile.enemy_kills,
            g_runtime.live_profile.loot_count,
            g_runtime.live_profile.animal_kills,
            g_runtime.live_profile.reward_earned,
            g_runtime.live_profile.crafted_mask,
            frontier_live_stat_points_available(),
            frontier_live_skill_points_available()
        );
    }

    g_runtime.live_profile.last_saved_at = now;
    g_runtime.live_profile.dirty = 0;
}

static void frontier_live_profile_load(void)
{
    char buffer[64];
    int max_region_mask = (1 << kFrontierRegionAnchorCount) - 1;
    int raw_phase;
    int raw_stage;
    int raw_mask;
    int raw_hub_returns;
    int raw_enemy_kills;
    int raw_loot_count;
    int raw_animal_kills;
    int raw_reward_earned;
    int raw_crafted_mask;
    int raw_stat_bonus_strength;
    int raw_stat_bonus_grit;
    int raw_stat_bonus_deadeye;
    int raw_stat_bonus_survival;
    int raw_stat_bonus_cunning;
    int raw_skill_bonus_hunting;
    int raw_skill_bonus_gunsmithing;
    int raw_skill_bonus_field_medicine;
    int raw_skill_bonus_trapping;
    int raw_skill_bonus_salvaging;
    int raw_skill_bonus_trade;
    int raw_skill_bonus_horse_handling;
    int raw_skill_bonus_homestead;
    int sanitized = 0;
    float raw_field_distance_total;
    float raw_field_distance_since_hub;

    ZeroMemory(&g_runtime.live_profile, sizeof(g_runtime.live_profile));
    g_runtime.applied_bonus_tier = -1;

    frontier_ensure_runtime_ini_path(g_runtime.profile_path, sizeof(g_runtime.profile_path), "FrontierLedger.profile.ini");
    if (g_runtime.profile_path[0] == '\0') {
        frontier_live_profile_refresh_strings();
        return;
    }

    raw_phase = (int)GetPrivateProfileIntA("Run", "Phase", FRONTIER_PHASE_SETUP, g_runtime.profile_path);
    raw_stage = (int)GetPrivateProfileIntA("Run", "TrailStage", 0, g_runtime.profile_path);
    raw_mask = (int)GetPrivateProfileIntA("Run", "DiscoveredRegionMask", 0, g_runtime.profile_path);
    raw_hub_returns = (int)GetPrivateProfileIntA("Run", "HubReturns", 0, g_runtime.profile_path);
    raw_enemy_kills = (int)GetPrivateProfileIntA("Run", "EnemyKills", 0, g_runtime.profile_path);
    raw_loot_count = (int)GetPrivateProfileIntA("Run", "LootCount", 0, g_runtime.profile_path);
    raw_animal_kills = (int)GetPrivateProfileIntA("Run", "AnimalKills", 0, g_runtime.profile_path);
    raw_reward_earned = (int)GetPrivateProfileIntA("Run", "RewardEarned", 0, g_runtime.profile_path);
    raw_crafted_mask = (int)GetPrivateProfileIntA("Run", "CraftedMask", 0, g_runtime.profile_path);
    raw_stat_bonus_strength = (int)GetPrivateProfileIntA("Growth", "StatBonusStrength", 0, g_runtime.profile_path);
    raw_stat_bonus_grit = (int)GetPrivateProfileIntA("Growth", "StatBonusGrit", 0, g_runtime.profile_path);
    raw_stat_bonus_deadeye = (int)GetPrivateProfileIntA("Growth", "StatBonusDeadeye", 0, g_runtime.profile_path);
    raw_stat_bonus_survival = (int)GetPrivateProfileIntA("Growth", "StatBonusSurvival", 0, g_runtime.profile_path);
    raw_stat_bonus_cunning = (int)GetPrivateProfileIntA("Growth", "StatBonusCunning", 0, g_runtime.profile_path);
    raw_skill_bonus_hunting = (int)GetPrivateProfileIntA("Growth", "SkillBonusHunting", 0, g_runtime.profile_path);
    raw_skill_bonus_gunsmithing = (int)GetPrivateProfileIntA("Growth", "SkillBonusGunsmithing", 0, g_runtime.profile_path);
    raw_skill_bonus_field_medicine = (int)GetPrivateProfileIntA("Growth", "SkillBonusFieldMedicine", 0, g_runtime.profile_path);
    raw_skill_bonus_trapping = (int)GetPrivateProfileIntA("Growth", "SkillBonusTrapping", 0, g_runtime.profile_path);
    raw_skill_bonus_salvaging = (int)GetPrivateProfileIntA("Growth", "SkillBonusSalvaging", 0, g_runtime.profile_path);
    raw_skill_bonus_trade = (int)GetPrivateProfileIntA("Growth", "SkillBonusTrade", 0, g_runtime.profile_path);
    raw_skill_bonus_horse_handling = (int)GetPrivateProfileIntA("Growth", "SkillBonusHorseHandling", 0, g_runtime.profile_path);
    raw_skill_bonus_homestead = (int)GetPrivateProfileIntA("Growth", "SkillBonusHomestead", 0, g_runtime.profile_path);

    g_runtime.live_profile.phase = frontier_clamp_int(
        raw_phase,
        FRONTIER_PHASE_SETUP,
        FRONTIER_PHASE_ENDGAME
    );
    g_runtime.live_profile.trail_stage = frontier_clamp_int(
        raw_stage,
        0,
        5
    );
    g_runtime.live_profile.discovered_region_mask = frontier_clamp_int(
        raw_mask,
        0,
        max_region_mask
    ) & max_region_mask;
    g_runtime.live_profile.hub_returns = frontier_clamp_int(
        raw_hub_returns,
        0,
        99
    );
    g_runtime.live_profile.enemy_kills = frontier_clamp_int(
        raw_enemy_kills,
        0,
        99999
    );
    g_runtime.live_profile.loot_count = frontier_clamp_int(
        raw_loot_count,
        0,
        99999
    );
    g_runtime.live_profile.animal_kills = frontier_clamp_int(
        raw_animal_kills,
        0,
        99999
    );
    g_runtime.live_profile.reward_earned = frontier_clamp_int(
        raw_reward_earned,
        0,
        999999
    );
    g_runtime.live_profile.crafted_mask = (uint32_t)frontier_clamp_int(
        raw_crafted_mask,
        0,
        0x1FFF
    );
    g_runtime.live_profile.stat_bonus_strength = frontier_clamp_int(raw_stat_bonus_strength, 0, 3);
    g_runtime.live_profile.stat_bonus_grit = frontier_clamp_int(raw_stat_bonus_grit, 0, 3);
    g_runtime.live_profile.stat_bonus_deadeye = frontier_clamp_int(raw_stat_bonus_deadeye, 0, 3);
    g_runtime.live_profile.stat_bonus_survival = frontier_clamp_int(raw_stat_bonus_survival, 0, 3);
    g_runtime.live_profile.stat_bonus_cunning = frontier_clamp_int(raw_stat_bonus_cunning, 0, 3);
    g_runtime.live_profile.skill_bonus_hunting = frontier_clamp_int(raw_skill_bonus_hunting, 0, 3);
    g_runtime.live_profile.skill_bonus_gunsmithing = frontier_clamp_int(raw_skill_bonus_gunsmithing, 0, 3);
    g_runtime.live_profile.skill_bonus_field_medicine = frontier_clamp_int(raw_skill_bonus_field_medicine, 0, 3);
    g_runtime.live_profile.skill_bonus_trapping = frontier_clamp_int(raw_skill_bonus_trapping, 0, 3);
    g_runtime.live_profile.skill_bonus_salvaging = frontier_clamp_int(raw_skill_bonus_salvaging, 0, 3);
    g_runtime.live_profile.skill_bonus_trade = frontier_clamp_int(raw_skill_bonus_trade, 0, 3);
    g_runtime.live_profile.skill_bonus_horse_handling = frontier_clamp_int(raw_skill_bonus_horse_handling, 0, 3);
    g_runtime.live_profile.skill_bonus_homestead = frontier_clamp_int(raw_skill_bonus_homestead, 0, 3);

    GetPrivateProfileStringA("Run", "FieldDistanceTotal", "0", buffer, (DWORD)sizeof(buffer), g_runtime.profile_path);
    raw_field_distance_total = (float)atof(buffer);
    g_runtime.live_profile.field_distance_total = frontier_clamp_float(raw_field_distance_total, 0.0f, 250000.0f);

    GetPrivateProfileStringA("Run", "FieldDistanceSinceHub", "0", buffer, (DWORD)sizeof(buffer), g_runtime.profile_path);
    raw_field_distance_since_hub = (float)atof(buffer);
    g_runtime.live_profile.field_distance_since_hub = frontier_clamp_float(raw_field_distance_since_hub, 0.0f, 50000.0f);

    sanitized =
        raw_phase != g_runtime.live_profile.phase ||
        raw_stage != g_runtime.live_profile.trail_stage ||
        raw_mask != g_runtime.live_profile.discovered_region_mask ||
        raw_hub_returns != g_runtime.live_profile.hub_returns ||
        raw_enemy_kills != g_runtime.live_profile.enemy_kills ||
        raw_loot_count != g_runtime.live_profile.loot_count ||
        raw_animal_kills != g_runtime.live_profile.animal_kills ||
        raw_reward_earned != g_runtime.live_profile.reward_earned ||
        raw_crafted_mask != (int)g_runtime.live_profile.crafted_mask ||
        raw_stat_bonus_strength != g_runtime.live_profile.stat_bonus_strength ||
        raw_stat_bonus_grit != g_runtime.live_profile.stat_bonus_grit ||
        raw_stat_bonus_deadeye != g_runtime.live_profile.stat_bonus_deadeye ||
        raw_stat_bonus_survival != g_runtime.live_profile.stat_bonus_survival ||
        raw_stat_bonus_cunning != g_runtime.live_profile.stat_bonus_cunning ||
        raw_skill_bonus_hunting != g_runtime.live_profile.skill_bonus_hunting ||
        raw_skill_bonus_gunsmithing != g_runtime.live_profile.skill_bonus_gunsmithing ||
        raw_skill_bonus_field_medicine != g_runtime.live_profile.skill_bonus_field_medicine ||
        raw_skill_bonus_trapping != g_runtime.live_profile.skill_bonus_trapping ||
        raw_skill_bonus_salvaging != g_runtime.live_profile.skill_bonus_salvaging ||
        raw_skill_bonus_trade != g_runtime.live_profile.skill_bonus_trade ||
        raw_skill_bonus_horse_handling != g_runtime.live_profile.skill_bonus_horse_handling ||
        raw_skill_bonus_homestead != g_runtime.live_profile.skill_bonus_homestead ||
        raw_field_distance_total != g_runtime.live_profile.field_distance_total ||
        raw_field_distance_since_hub != g_runtime.live_profile.field_distance_since_hub;

    if (sanitized) {
        g_runtime.live_profile.dirty = 1;
        if (GetTickCount() - g_runtime.last_profile_recovery_log_at >= kFrontierWriteFailureLogCooldownMs) {
            g_runtime.last_profile_recovery_log_at = GetTickCount();
            frontier_log(
                "Profile recovery applied. Phase=%d Stage=%d Regions=%d Hub=%d Enemy=%d Loot=%d Hunt=%d Reward=%d CraftedMask=0x%X",
                g_runtime.live_profile.phase,
                g_runtime.live_profile.trail_stage,
                g_runtime.live_profile.discovered_region_mask,
                g_runtime.live_profile.hub_returns,
                g_runtime.live_profile.enemy_kills,
                g_runtime.live_profile.loot_count,
                g_runtime.live_profile.animal_kills,
                g_runtime.live_profile.reward_earned,
                g_runtime.live_profile.crafted_mask
            );
        }
    }

    g_runtime.live_profile.discovered_region_count = frontier_count_region_bits(g_runtime.live_profile.discovered_region_mask);
    frontier_live_profile_refresh_progression_layers();
    frontier_sanitize_growth_allocations();
    frontier_live_profile_refresh_progression_layers();
    frontier_live_profile_refresh_strings();
    frontier_log(
        "Profile loaded. Phase=%d Stage=%d Enemy=%d Loot=%d Hunt=%d Reward=%d Regions=%d Hub=%d CraftedMask=0x%X StatPts=%d SkillPts=%d",
        g_runtime.live_profile.phase,
        g_runtime.live_profile.trail_stage,
        g_runtime.live_profile.enemy_kills,
        g_runtime.live_profile.loot_count,
        g_runtime.live_profile.animal_kills,
        g_runtime.live_profile.reward_earned,
        g_runtime.live_profile.discovered_region_count,
        g_runtime.live_profile.hub_returns,
        g_runtime.live_profile.crafted_mask,
        frontier_live_stat_points_available(),
        frontier_live_skill_points_available()
    );
}

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
