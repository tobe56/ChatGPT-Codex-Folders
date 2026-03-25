static void frontier_render_workbench_overlay(void)
{
    int korean = frontier_dataset_is_korean();
    const char *current_label;
    float left = korean ? 0.17f : 0.20f;
    float top = 0.08f;
    float width = korean ? 0.72f : 0.60f;
    float height = korean ? 0.75f : 0.73f;
    float header_h = korean ? 0.050f : 0.040f;
    float tab_h = korean ? 0.044f : 0.038f;
    float footer_h = korean ? 0.105f : 0.090f;
    float tabs_top = top + header_h + 0.010f;
    float list_left = left + 0.018f;
    float list_top = tabs_top + tab_h + 0.012f;
    float list_width = korean ? 0.265f : 0.205f;
    float list_height = 0.515f;
    float detail_left = list_left + list_width + 0.020f;
    float detail_top = list_top;
    float detail_width = width - (detail_left - left) - 0.018f;
    float detail_height = list_height;
    float footer_top = detail_top + detail_height + 0.018f;
    float tab_width = (width - 0.036f) / 3.0f;
    int tab_index;
    int current_tab = frontier_clamp_int(g_runtime.workbench_tab, 0, 2);
    int entry_index = frontier_workbench_clamp_entry_index(current_tab, g_runtime.workbench_selected_entry[current_tab]);
    int entry_count = frontier_workbench_entry_count(current_tab);
    int draw_index;
    int start_index;
    int visible_entries;
    float row_height = korean ? 0.082f : 0.054f;
    float row_box_height = korean ? 0.064f : 0.044f;
    float row_top = list_top + 0.070f;
    FrontierColor overlay_scrim = {8, 7, 5, 190};
    FrontierColor board_color = {20, 17, 13, 225};
    FrontierColor board_header = {106, 81, 48, 235};
    FrontierColor board_panel = {34, 28, 22, 230};
    FrontierColor board_row = {49, 40, 31, 230};
    FrontierColor board_selected = {122, 84, 35, 235};
    FrontierColor board_divider = {168, 139, 88, 220};

    visible_entries = frontier_min_int(korean ? 5 : 8, entry_count);
    start_index = frontier_clamp_int(entry_index - (visible_entries / 2), 0, frontier_max_int(0, entry_count - visible_entries));
    current_label = frontier_workbench_current_label();

    frontier_draw_rect_tl(0.0f, 0.0f, 1.0f, 1.0f, overlay_scrim);
    frontier_draw_rect_tl(left, top, width, height, board_color);
    frontier_draw_rect_tl(left, top, width, header_h, board_header);
    frontier_draw_text_short(frontier_ui_text("Workbench", "FL_WB_TITLE"), left + 0.018f, top + 0.010f, korean ? 0.34f : 0.32f, kColorText);
    frontier_draw_text_short(frontier_runtime_context_label(), left + width - (korean ? 0.180f : 0.180f), top + 0.012f, korean ? 0.14f : 0.14f, kColorAccent);

    for (tab_index = 0; tab_index < 3; ++tab_index) {
        FrontierColor tab_color = (tab_index == current_tab) ? board_selected : board_row;
        float tab_left = left + 0.018f + (tab_width * (float)tab_index);
        frontier_draw_rect_tl(tab_left, tabs_top, tab_width - (korean ? 0.010f : 0.008f), tab_h, tab_color);
        frontier_draw_text_short(
            frontier_workbench_tab_name(tab_index),
            tab_left + 0.010f,
            tabs_top + (korean ? 0.012f : 0.010f),
            korean ? 0.18f : 0.22f,
            (tab_index == current_tab) ? kColorText : kColorTextDim
        );
    }

    frontier_draw_rect_tl(list_left, list_top, list_width, list_height, board_panel);
    frontier_draw_rect_tl(detail_left, detail_top, detail_width, detail_height, board_panel);
    frontier_draw_rect_tl(left + 0.018f, footer_top, width - 0.036f, footer_h, board_panel);
    frontier_draw_rect_tl(list_left + list_width + 0.010f, list_top, 0.002f, list_height, board_divider);
    frontier_draw_text_short(frontier_ui_text("Entries", "FL_WB_LIST"), list_left + 0.014f, list_top + 0.015f, korean ? 0.17f : 0.16f, kColorAccent);
    frontier_draw_text_short(frontier_ui_text("Detail", "FL_WB_DETAIL"), detail_left + 0.015f, detail_top + 0.015f, korean ? 0.17f : 0.16f, kColorAccent);
    frontier_draw_rect_tl(list_left + 0.012f, list_top + 0.050f, list_width - 0.024f, 0.0018f, board_divider);
    frontier_draw_rect_tl(detail_left + 0.012f, detail_top + 0.080f, detail_width - 0.024f, 0.0018f, board_divider);
    for (draw_index = 0; draw_index < visible_entries; ++draw_index) {
        const char *label;
        const char *state_text = NULL;
        FrontierColor row_color;
        int actual_index = start_index + draw_index;
        float block_top = row_top + (row_height * (float)draw_index);

        row_color = (actual_index == entry_index) ? board_selected : board_row;

        if (current_tab == FRONTIER_WORKBENCH_TAB_CRAFT) {
            label = frontier_workbench_list_label(current_tab, actual_index);
        } else if (korean) {
            label = frontier_workbench_list_label(current_tab, actual_index);
        } else if (current_tab == FRONTIER_WORKBENCH_TAB_ARCHIVE) {
            label = frontier_workbench_archive_name(actual_index);
        } else {
            label = frontier_workbench_result_name(actual_index);
        }

        if (current_tab == FRONTIER_WORKBENCH_TAB_CRAFT) {
            state_text = korean ? frontier_workbench_recipe_state_badge(actual_index) : frontier_workbench_recipe_state(actual_index);
        }

        frontier_draw_rect_tl(list_left + 0.008f, block_top, list_width - 0.016f, row_box_height, row_color);
        if (actual_index == entry_index) {
            frontier_draw_rect_tl(list_left + 0.010f, block_top + 0.004f, 0.0045f, korean ? 0.050f : 0.030f, kColorAccent);
        }
        if (korean) {
            float state_left = list_left + list_width - 0.084f;
            if (state_text != NULL) {
                FrontierColor state_color = (actual_index == entry_index) ? board_header : board_color;
                frontier_draw_rect_tl(state_left, block_top + 0.013f, 0.058f, 0.030f, state_color);
                frontier_draw_text_short(state_text, state_left + 0.008f, block_top + 0.017f, 0.11f, kColorText);
            }
            frontier_draw_text_short(
                label,
                list_left + 0.022f,
                block_top + 0.017f,
                0.16f,
                kColorText
            );
        } else {
            int label_chars = (state_text != NULL) ? 18 : 24;
            frontier_draw_wrapped_text(label, list_left + 0.020f, block_top + 0.010f, 0.25f, 0.016f, label_chars, 2, kColorText);
        }
        if (!korean && state_text != NULL) {
            float state_left = list_left + list_width - 0.078f;
            FrontierColor state_color = (actual_index == entry_index) ? board_header : board_color;
            frontier_draw_rect_tl(state_left, block_top + 0.010f, 0.060f, 0.022f, state_color);
            frontier_draw_text_short(
                state_text,
                state_left + 0.006f,
                block_top + 0.012f,
                0.11f,
                kColorText
            );
        }
    }

    frontier_draw_text_short(current_label, detail_left + 0.015f, detail_top + 0.046f, korean ? 0.18f : 0.27f, kColorText);
    frontier_draw_wrapped_text(
        frontier_workbench_overlay_detail(current_tab, entry_index),
        detail_left + 0.015f,
        detail_top + 0.104f,
        korean ? 0.15f : 0.27f,
        korean ? 0.034f : 0.020f,
        korean ? 24 : 48,
        korean ? 8 : 18,
        kColorText
    );

    frontier_draw_text_short(frontier_ui_text("Run", "FL_WB_RUN"), left + 0.030f, footer_top + 0.014f, korean ? 0.16f : 0.16f, kColorAccent);
    frontier_draw_text_short(frontier_runtime_progress_summary(), left + 0.030f, footer_top + 0.043f, korean ? 0.16f : 0.14f, kColorTextDim);
    frontier_draw_text_short(frontier_ui_text("Left/Right tabs  Up/Down entries  E confirm", "FL_WB_CONTROLS"), left + 0.250f, footer_top + 0.014f, korean ? 0.12f : 0.13f, kColorTextDim);
    frontier_draw_text_short(frontier_ui_text("Crafted", "FL_WB_CRAFTED"), left + width - 0.200f, footer_top + 0.014f, korean ? 0.12f : 0.13f, kColorTextDim);
    {
        char crafted_buffer[64];
        snprintf(crafted_buffer, sizeof(crafted_buffer), "%d / %d", frontier_live_crafted_recipe_count(), FRONTIER_WORKBENCH_CRAFT_COUNT);
        frontier_draw_text_short(crafted_buffer, left + width - 0.090f, footer_top + 0.014f, korean ? 0.14f : 0.13f, kColorText);
    }
}

static void frontier_render_tabs(float left, float top, float width)
{
    int korean = frontier_dataset_is_korean();
    int visible_tabs;
    int start_index;
    int end_index;
    int page_index;
    int draw_index;
    int row_count = 1;
    int visible_per_row;
    int row_index;
    int column_index;
    float tab_width;
    float tab_height;
    float row_gap;
    float indicator_width;
    float tab_area_width;
    int page_count = frontier_page_count();
    char indicator_text[32];

    if (page_count <= 0) {
        return;
    }

    if (korean && page_count > 6) {
        row_count = 2;
    }

    if (row_count == 2) {
        visible_per_row = 4;
        visible_tabs = frontier_min_int(page_count, visible_per_row * row_count);
        start_index = frontier_tab_window_start(page_count, g_runtime.current_page, visible_tabs);
        end_index = frontier_min_int(page_count, start_index + visible_tabs);
        tab_area_width = width;
        tab_width = tab_area_width / (float)visible_per_row;
        tab_height = 0.022f;
        row_gap = 0.004f;

        for (draw_index = 0; draw_index < (end_index - start_index); ++draw_index) {
            page_index = start_index + draw_index;
            row_index = draw_index / visible_per_row;
            column_index = draw_index % visible_per_row;
            {
                FrontierColor tab_color = (page_index == g_runtime.current_page) ? kColorSelection : kColorPanelLight;
                float tab_left = left + (tab_width * (float)column_index);
                float tab_top = top + ((tab_height + row_gap) * (float)row_index);
                frontier_draw_rect_tl(tab_left, tab_top, tab_width - 0.004f, tab_height, tab_color);
                frontier_draw_text_short(
                    frontier_page_title_display(&g_runtime.dataset->pages[page_index]),
                    tab_left + 0.008f,
                    tab_top + 0.005f,
                    0.25f,
                    (page_index == g_runtime.current_page) ? kColorText : kColorTextDim
                );
            }
        }

        if (page_count > visible_tabs) {
            snprintf(indicator_text, sizeof(indicator_text), "%d/%d", g_runtime.current_page + 1, page_count);
            frontier_draw_text_short(indicator_text, left + width - 0.05f, top + 0.050f, 0.10f, kColorTextDim);
        }
        return;
    }

    visible_tabs = frontier_hud_visible_tab_count(width, page_count);
    start_index = frontier_tab_window_start(page_count, g_runtime.current_page, visible_tabs);
    end_index = frontier_min_int(page_count, start_index + visible_tabs);
    indicator_width = (page_count > visible_tabs) ? 0.06f : 0.0f;
    tab_area_width = width - indicator_width;
    tab_width = tab_area_width / (float)frontier_max_int(1, visible_tabs);

    for (draw_index = 0; draw_index < (end_index - start_index); ++draw_index) {
        page_index = start_index + draw_index;
        FrontierColor tab_color = (page_index == g_runtime.current_page) ? kColorSelection : kColorPanelLight;
        frontier_draw_rect_tl(left + (tab_width * (float)draw_index), top, tab_width - 0.004f, korean ? 0.050f : 0.04f, tab_color);
        frontier_draw_text_short(
            frontier_page_title_display(&g_runtime.dataset->pages[page_index]),
            left + (tab_width * (float)draw_index) + 0.01f,
            top + (korean ? 0.013f : 0.010f),
            korean ? 0.31f : 0.33f,
            (page_index == g_runtime.current_page) ? kColorText : kColorTextDim
        );
    }

    if (page_count > visible_tabs) {
        snprintf(indicator_text, sizeof(indicator_text), "%d/%d", g_runtime.current_page + 1, page_count);
        frontier_draw_text_short(indicator_text, left + tab_area_width + 0.008f, top + 0.010f, 0.10f, kColorTextDim);
    }
}

static void frontier_render_entry_list(float left, float top, float width, float height)
{
    const FrontierLedgerPage *page = frontier_current_page();
    int korean = frontier_dataset_is_korean();
    int label_chars = korean ? 20 : 44;
    int visible_count;
    int start_index;
    int draw_index;
    float row_top = top + (korean ? 0.080f : 0.052f);
    float row_height = korean ? 0.071f : 0.048f;

    frontier_draw_rect_tl(left, top, width, height, kColorPanel);
    if (page == NULL) {
        return;
    }

    frontier_draw_text_short(
        frontier_text("Entries", "항목"),
        left + 0.015f,
        top + (korean ? 0.018f : 0.016f),
        korean ? 0.23f : 0.20f,
        kColorAccent
    );
    frontier_draw_text_short(
        frontier_text("Up/Down", "상하"),
        left + width - (korean ? 0.065f : 0.055f),
        top + (korean ? 0.019f : 0.016f),
        korean ? 0.14f : 0.12f,
        kColorTextDim
    );
    frontier_draw_rect_tl(left + 0.012f, top + (korean ? 0.060f : 0.045f), width - 0.024f, 0.0018f, kColorAccentSoft);

    start_index = g_runtime.top_entry[g_runtime.current_page];
    visible_count = frontier_min_int(frontier_page_entry_capacity(), page->entry_count - start_index);
    for (draw_index = 0; draw_index < visible_count; ++draw_index) {
        int entry_index = start_index + draw_index;
        const char *label = frontier_live_entry_label(page, entry_index, &page->entries[entry_index]);
        FrontierColor row_color = (entry_index == g_runtime.selected_entry[g_runtime.current_page]) ? kColorSelection : kColorPanelLight;
        float block_top = row_top + (row_height * (float)draw_index);
        frontier_draw_rect_tl(left + 0.008f, block_top, width - 0.016f, korean ? 0.056f : 0.040f, row_color);
        if (entry_index == g_runtime.selected_entry[g_runtime.current_page]) {
            frontier_draw_rect_tl(left + 0.011f, block_top + (korean ? 0.005f : 0.004f), 0.0045f, korean ? 0.046f : 0.032f, kColorAccent);
        }
        frontier_draw_wrapped_text(
            label,
            left + (korean ? 0.020f : 0.015f),
            block_top + (korean ? 0.010f : 0.007f),
            korean ? 0.35f : 0.29f,
            korean ? 0.022f : 0.016f,
            label_chars,
            korean ? 1 : 2,
            kColorText
        );
    }
}

static void frontier_render_detail_panel(float left, float top, float width, float height)
{
    const FrontierLedgerPage *page = frontier_current_page();
    int korean = frontier_dataset_is_korean();
    const FrontierLedgerEntry *entry;
    const char *summary_text;
    const char *selected_label;
    const char *detail_text;
    int summary_used_lines;
    int selected_used_lines;
    int detail_available_lines;
    float summary_top;
    float summary_bottom;
    float summary_divider_y;
    float selected_header_y;
    float selected_top;
    float selected_bottom;
    float detail_header_y;
    float detail_top;
    float detail_divider_y;
    float summary_scale = korean ? 0.35f : 0.28f;
    float summary_line_height = korean ? 0.022f : 0.017f;
    int summary_chars = korean ? 22 : 50;
    int summary_lines = korean ? 2 : 2;
    float selected_scale = korean ? 0.37f : 0.31f;
    float selected_line_height = korean ? 0.024f : 0.018f;
    int selected_chars = korean ? 22 : 50;
    int selected_lines = korean ? 2 : 2;
    float detail_scale = korean ? 0.35f : 0.28f;
    float detail_line_height = korean ? 0.024f : 0.017f;
    int detail_chars = korean ? 24 : 56;
    int detail_lines = korean ? 7 : 10;

    frontier_draw_rect_tl(left, top, width, height, kColorPanel);
    if (page == NULL || page->entry_count <= 0) {
        frontier_draw_text_short(frontier_ui_text("No entries available.", "FL_UI_EMPTY"), left + 0.015f, top + 0.02f, 0.36f, kColorText);
        return;
    }

    entry = &page->entries[g_runtime.selected_entry[g_runtime.current_page]];
    summary_text = frontier_live_page_summary(page);
    selected_label = frontier_live_entry_label(page, g_runtime.selected_entry[g_runtime.current_page], entry);
    detail_text = frontier_live_entry_detail(page, g_runtime.selected_entry[g_runtime.current_page], entry);
    summary_used_lines = frontier_count_wrapped_lines(summary_text, summary_chars, summary_lines);
    selected_used_lines = frontier_count_wrapped_lines(selected_label, selected_chars, selected_lines);
    summary_top = top + 0.052f;
    summary_bottom = summary_top + (summary_line_height * (float)frontier_max_int(1, summary_used_lines));
    summary_divider_y = summary_bottom + 0.014f;
    if (summary_divider_y < top + 0.112f) {
        summary_divider_y = top + 0.112f;
    }
    selected_header_y = summary_divider_y + 0.013f;
    selected_top = selected_header_y + 0.030f;
    selected_bottom = selected_top + (selected_line_height * (float)frontier_max_int(1, selected_used_lines));
    detail_header_y = selected_bottom + 0.022f;
    detail_top = detail_header_y + 0.030f;
    detail_divider_y = top + height - 0.118f;
    detail_available_lines = frontier_min_int(
        detail_lines,
        (int)((detail_divider_y - detail_top - 0.012f) / detail_line_height)
    );
    if (detail_available_lines < 1) {
        detail_available_lines = 1;
    }

    frontier_draw_text_short(frontier_page_title_display(page), left + 0.015f, top + 0.018f, korean ? 0.42f : 0.40f, kColorAccent);
    frontier_draw_text_short(frontier_text("Summary", "요약"), left + width - (korean ? 0.072f : 0.062f), top + 0.020f, korean ? 0.16f : 0.14f, kColorTextDim);
    frontier_draw_wrapped_text(
        summary_text,
        left + 0.015f,
        summary_top,
        summary_scale,
        summary_line_height,
        summary_chars,
        summary_lines,
        kColorText
    );
    frontier_draw_rect_tl(left + 0.012f, summary_divider_y, width - 0.024f, 0.0018f, kColorAccentSoft);
    frontier_draw_text_short(frontier_text("Selected", "선택"), left + 0.015f, selected_header_y, korean ? 0.18f : 0.15f, kColorTextDim);
    frontier_draw_wrapped_text(
        selected_label,
        left + 0.015f,
        selected_top,
        selected_scale,
        selected_line_height,
        selected_chars,
        selected_lines,
        kColorText
    );
    frontier_draw_text_short(frontier_text("Detail", "세부"), left + 0.015f, detail_header_y, korean ? 0.18f : 0.15f, kColorTextDim);
    frontier_draw_wrapped_text(
        detail_text,
        left + 0.015f,
        detail_top,
        detail_scale,
        detail_line_height,
        detail_chars,
        detail_available_lines,
        kColorText
    );
    frontier_draw_rect_tl(left + 0.012f, detail_divider_y, width - 0.024f, 0.0018f, kColorAccentSoft);
    frontier_draw_text_short(frontier_text("Current frontier", "현재 프론티어"), left + 0.015f, top + height - 0.105f, korean ? 0.18f : 0.15f, kColorTextDim);
    frontier_draw_text_short(frontier_runtime_context_label(), left + 0.015f, top + height - 0.078f, korean ? 0.31f : 0.27f, kColorAccent);
    frontier_draw_wrapped_text(frontier_runtime_route_hint(), left + 0.015f, top + height - 0.046f, korean ? 0.31f : 0.31f, korean ? 0.020f : 0.017f, korean ? 24 : 44, korean ? 2 : 2, kColorTextDim);
}

static void frontier_render_full_overlay(void)
{
    if (frontier_dataset_is_korean()) {
        char card_overview_1[64];
        char card_overview_2[64];
        char card_progress_1[64];
        char card_progress_2[64];
        char card_combat_1[64];
        char card_combat_2[64];
        char card_hunt_1[64];
        char card_hunt_2[64];
        char card_stats_1[64];
        char card_stats_2[64];
        char card_stats_3[64];
        char card_skills_1[64];
        char card_skills_2[64];
        char card_skills_3[64];
        char card_craft_1[64];
        char card_craft_2[64];
        char card_bonus_1[64];
        char card_bonus_2[64];
        char card_bonus_3[64];
        char footer_1[96];
        char footer_2[96];
        float left = 0.20f;
        float top = 0.05f;
        float width = 0.66f;
        float col_w = 0.31f;
        float gap = 0.02f;
        float row_h = 0.108f;
        float row1 = 0.13f;
        float row2 = row1 + 0.125f;
        float row3 = row2 + 0.125f;
        float row4 = row3 + 0.125f;
        float right = left + col_w + gap;
        int bonus_tier = frontier_live_bonus_tier();
        int weapon_percent = frontier_clamp_int((int)(g_runtime.applied_weapon_degradation_modifier * 100.0f), 0, 200);
        int sprint_percent = frontier_clamp_int((int)(g_runtime.applied_stamina_depletion_multiplier * 100.0f), 0, 200);
        int deadeye_percent = frontier_clamp_int((int)(g_runtime.applied_deadeye_delay * 100.0f), 0, 300);

        snprintf(card_overview_1, sizeof(card_overview_1), "STAGE %d", g_runtime.live_profile.trail_stage);
        snprintf(card_overview_2, sizeof(card_overview_2), "PHASE %d | REG %d/6", g_runtime.live_profile.phase, g_runtime.live_profile.discovered_region_count);

        snprintf(card_progress_1, sizeof(card_progress_1), "DIST %.1fKM", g_runtime.live_profile.field_distance_total / 1000.0f);
        snprintf(card_progress_2, sizeof(card_progress_2), "HUB %d | CONTRACT %d", g_runtime.live_profile.hub_returns, g_runtime.live_profile.contract_count);

        snprintf(card_combat_1, sizeof(card_combat_1), "ENEMY %d | REWARD $%d", g_runtime.live_profile.enemy_kills, g_runtime.live_profile.reward_earned);
        snprintf(card_combat_2, sizeof(card_combat_2), "BONUS T%d | SILAS %s", bonus_tier, g_runtime.live_profile.boss_silas_ready ? "READY" : "LOCK");

        snprintf(card_hunt_1, sizeof(card_hunt_1), "ANIMAL %d | LOOT %d", g_runtime.live_profile.animal_kills, g_runtime.live_profile.loot_count);
        snprintf(card_hunt_2, sizeof(card_hunt_2), "HIDE %d | SALV %d", g_runtime.live_profile.hide_stock, g_runtime.live_profile.salvage_stock);

        snprintf(card_stats_1, sizeof(card_stats_1), "STR %d | GRIT %d", g_runtime.live_profile.stat_strength, g_runtime.live_profile.stat_grit);
        snprintf(card_stats_2, sizeof(card_stats_2), "DE %d | SURV %d", g_runtime.live_profile.stat_deadeye, g_runtime.live_profile.stat_survival);
        snprintf(card_stats_3, sizeof(card_stats_3), "CUN %d", g_runtime.live_profile.stat_cunning);

        snprintf(card_skills_1, sizeof(card_skills_1), "HUNT %d | SMITH %d", g_runtime.live_profile.skill_hunting, g_runtime.live_profile.skill_gunsmithing);
        snprintf(card_skills_2, sizeof(card_skills_2), "TRADE %d | HOME %d", g_runtime.live_profile.skill_trade, g_runtime.live_profile.skill_homestead);
        snprintf(card_skills_3, sizeof(card_skills_3), "MED %d | HORSE %d", g_runtime.live_profile.skill_field_medicine, g_runtime.live_profile.skill_horse_handling);

        snprintf(card_craft_1, sizeof(card_craft_1), "WORK T%d | ARM T%d", g_runtime.live_profile.workshop_tier, g_runtime.live_profile.armory_tier);
        snprintf(card_craft_2, sizeof(card_craft_2), "MED T%d | RELIC %d", g_runtime.live_profile.medicine_tier, g_runtime.live_profile.relic_stock);

        snprintf(card_bonus_1, sizeof(card_bonus_1), "SPR %d%% | WEAR %d%%", sprint_percent, weapon_percent);
        snprintf(card_bonus_2, sizeof(card_bonus_2), "DEAD %d%% | BONUS T%d", deadeye_percent, bonus_tier);
        snprintf(card_bonus_3, sizeof(card_bonus_3), "PROFILE LIVE");

        snprintf(footer_1, sizeof(footer_1), "REGION %s", frontier_runtime_context_label());
        snprintf(footer_2, sizeof(footer_2), "NEXT %s", frontier_runtime_progress_summary());

        frontier_draw_rect_tl(left, top, width, 0.79f, kColorBackground);
        frontier_draw_rect_tl(left, top, width, 0.040f, kColorAccentSoft);
        frontier_draw_text_short("프론티어 레저", left + 0.015f, top + 0.008f, 0.36f, kColorText);
        frontier_draw_text_short("F11 / ESC", left + width - 0.09f, top + 0.010f, 0.14f, kColorTextDim);

        frontier_draw_rect_tl(left + 0.015f, row1, col_w, row_h, kColorPanel);
        frontier_draw_text_short("개요", left + 0.025f, row1 + 0.010f, 0.22f, kColorAccent);
        frontier_draw_text_short(card_overview_1, left + 0.025f, row1 + 0.038f, 0.17f, kColorText);
        frontier_draw_text_short(card_overview_2, left + 0.025f, row1 + 0.061f, 0.15f, kColorTextDim);

        frontier_draw_rect_tl(right, row1, col_w, row_h, kColorPanel);
        frontier_draw_text_short("진행", right + 0.010f, row1 + 0.010f, 0.22f, kColorAccent);
        frontier_draw_text_short(card_progress_1, right + 0.010f, row1 + 0.038f, 0.17f, kColorText);
        frontier_draw_text_short(card_progress_2, right + 0.010f, row1 + 0.061f, 0.15f, kColorTextDim);

        frontier_draw_rect_tl(left + 0.015f, row2, col_w, row_h, kColorPanel);
        frontier_draw_text_short("전투", left + 0.025f, row2 + 0.010f, 0.22f, kColorAccent);
        frontier_draw_text_short(card_combat_1, left + 0.025f, row2 + 0.038f, 0.15f, kColorText);
        frontier_draw_text_short(card_combat_2, left + 0.025f, row2 + 0.061f, 0.15f, kColorTextDim);

        frontier_draw_rect_tl(right, row2, col_w, row_h, kColorPanel);
        frontier_draw_text_short("사냥", right + 0.010f, row2 + 0.010f, 0.22f, kColorAccent);
        frontier_draw_text_short(card_hunt_1, right + 0.010f, row2 + 0.038f, 0.15f, kColorText);
        frontier_draw_text_short(card_hunt_2, right + 0.010f, row2 + 0.061f, 0.15f, kColorTextDim);

        frontier_draw_rect_tl(left + 0.015f, row3, col_w, row_h, kColorPanel);
        frontier_draw_text_short("능력", left + 0.025f, row3 + 0.010f, 0.22f, kColorAccent);
        frontier_draw_text_short(card_stats_1, left + 0.025f, row3 + 0.036f, 0.15f, kColorText);
        frontier_draw_text_short(card_stats_2, left + 0.025f, row3 + 0.058f, 0.15f, kColorTextDim);
        frontier_draw_text_short(card_stats_3, left + 0.025f, row3 + 0.080f, 0.15f, kColorTextDim);

        frontier_draw_rect_tl(right, row3, col_w, row_h, kColorPanel);
        frontier_draw_text_short("기술", right + 0.010f, row3 + 0.010f, 0.22f, kColorAccent);
        frontier_draw_text_short(card_skills_1, right + 0.010f, row3 + 0.036f, 0.15f, kColorText);
        frontier_draw_text_short(card_skills_2, right + 0.010f, row3 + 0.058f, 0.15f, kColorTextDim);
        frontier_draw_text_short(card_skills_3, right + 0.010f, row3 + 0.080f, 0.15f, kColorTextDim);

        frontier_draw_rect_tl(left + 0.015f, row4, col_w, row_h, kColorPanel);
        frontier_draw_text_short("제작", left + 0.025f, row4 + 0.010f, 0.22f, kColorAccent);
        frontier_draw_text_short(card_craft_1, left + 0.025f, row4 + 0.038f, 0.15f, kColorText);
        frontier_draw_text_short(card_craft_2, left + 0.025f, row4 + 0.061f, 0.15f, kColorTextDim);

        frontier_draw_rect_tl(right, row4, col_w, row_h, kColorPanel);
        frontier_draw_text_short("보너스", right + 0.010f, row4 + 0.010f, 0.22f, kColorAccent);
        frontier_draw_text_short(card_bonus_1, right + 0.010f, row4 + 0.036f, 0.15f, kColorText);
        frontier_draw_text_short(card_bonus_2, right + 0.010f, row4 + 0.058f, 0.15f, kColorTextDim);
        frontier_draw_text_short(card_bonus_3, right + 0.010f, row4 + 0.080f, 0.15f, kColorTextDim);

        frontier_draw_rect_tl(left + 0.015f, 0.66f, width - 0.030f, 0.110f, kColorPanel);
        frontier_draw_text_short("현재", left + 0.025f, 0.672f, 0.22f, kColorAccent);
        frontier_draw_text_short(footer_1, left + 0.025f, 0.700f, 0.15f, kColorText);
        frontier_draw_text_short(footer_2, left + 0.025f, 0.724f, 0.14f, kColorTextDim);
        frontier_draw_text_short("좌우 / 상하 / ESC", left + width - 0.18f, 0.726f, 0.13f, kColorTextDim);
        return;
    }

    const FrontierLedgerPage *page = frontier_current_page();
    int korean = frontier_dataset_is_korean();
    float header_left = korean ? 0.24f : 0.38f;
    float header_top = 0.05f;
    float header_width = korean ? 0.69f : 0.54f;
    float left_panel_left = korean ? 0.24f : 0.38f;
    float right_panel_left = korean ? 0.50f : 0.59f;
    float content_top = korean ? 0.205f : 0.16f;
    float content_height = korean ? 0.52f : 0.54f;
    float bottom_left = korean ? 0.24f : 0.38f;
    float bottom_top = korean ? 0.745f : 0.73f;
    float bottom_width = korean ? 0.69f : 0.54f;

    frontier_draw_rect_tl(header_left, header_top, header_width, 0.055f, kColorAccentSoft);
    frontier_draw_text_short(frontier_main_title_display(), header_left + 0.02f, header_top + 0.012f, korean ? 0.39f : 0.39f, kColorText);
    frontier_draw_text_short(frontier_runtime_context_label(), header_left + (korean ? 0.28f : 0.31f), header_top + 0.014f, korean ? 0.19f : 0.18f, kColorAccent);
    if (!korean) {
        frontier_draw_text_short(frontier_runtime_dataset_label(), header_left + 0.43f, header_top + 0.014f, 0.18f, kColorTextDim);
    }

    frontier_draw_rect_tl(header_left, 0.115f, header_width, korean ? 0.078f : 0.038f, kColorPanel);
    frontier_render_tabs(header_left + 0.01f, 0.115f, header_width - 0.02f);

    frontier_render_entry_list(left_panel_left, content_top, korean ? 0.24f : 0.19f, content_height);
    frontier_render_detail_panel(right_panel_left, content_top, korean ? 0.43f : 0.33f, content_height);

    frontier_draw_rect_tl(bottom_left, bottom_top, bottom_width, korean ? 0.100f : 0.075f, kColorPanel);
    if (page != NULL) {
        frontier_draw_text_short(frontier_live_trail_stage_compact_label(g_runtime.live_profile.trail_stage), bottom_left + 0.015f, bottom_top + 0.012f, korean ? 0.26f : 0.23f, kColorAccent);
        frontier_draw_wrapped_text(frontier_live_page_summary(page), bottom_left + 0.015f, bottom_top + 0.041f, korean ? 0.24f : 0.19f, korean ? 0.019f : 0.016f, korean ? 22 : 42, korean ? 2 : 2, kColorTextDim);
    }
    frontier_draw_text_short(frontier_runtime_progress_summary(), bottom_left + (korean ? 0.26f : 0.22f), bottom_top + 0.015f, korean ? 0.22f : 0.18f, kColorTextDim);
    frontier_draw_wrapped_text(korean ? frontier_runtime_objective_compact() : frontier_runtime_objective(), bottom_left + (korean ? 0.26f : 0.22f), bottom_top + 0.041f, korean ? 0.24f : 0.20f, korean ? 0.019f : 0.016f, korean ? 24 : 52, korean ? 2 : 2, kColorText);
    frontier_draw_text_short(
        frontier_footer_controls_display(),
        bottom_left + (korean ? 0.50f : 0.43f),
        bottom_top + (korean ? 0.060f : 0.045f),
        korean ? 0.17f : 0.16f,
        kColorTextDim
    );
    frontier_draw_text_short(frontier_runtime_context_label(), bottom_left + (korean ? 0.55f : 0.47f), bottom_top + 0.015f, korean ? 0.13f : 0.09f, kColorAccent);
}
