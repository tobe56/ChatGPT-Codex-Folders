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

