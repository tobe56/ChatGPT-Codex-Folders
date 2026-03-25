static void frontier_draw_token_lines(
    const char *text,
    float x,
    float y,
    float scale,
    float line_height,
    int max_lines,
    FrontierColor color
)
{
    char buffer[192];
    const char *cursor = text;
    int line_index = 0;

    if (text == NULL || text[0] == '\0') {
        return;
    }

    while (*cursor != '\0' && line_index < max_lines) {
        const char *break_at = strstr(cursor, "~n~");
        size_t length = break_at != NULL ? (size_t)(break_at - cursor) : strlen(cursor);
        if (length >= sizeof(buffer)) {
            length = sizeof(buffer) - 1U;
        }
        memcpy(buffer, cursor, length);
        buffer[length] = '\0';
        if (frontier_dataset_is_korean() && !frontier_text_is_ascii(buffer)) {
            float compact_line_height = line_height * 0.92f;
            float pixel_height = compact_line_height / (float)FRONTIER_BITMAP_FONT_LINE_ADVANCE;
            float pixel_width = frontier_bitmap_pixel_width_for_height(pixel_height) * 0.86f;
            frontier_draw_bitmap_text_internal(
                buffer,
                x,
                y + (line_height * (float)line_index),
                pixel_width,
                pixel_height,
                line_height,
                24,
                1,
                color
            );
        } else {
            frontier_draw_text_short(buffer, x, y + (line_height * (float)line_index), scale, color);
        }
        ++line_index;
        if (break_at == NULL) {
            break;
        }
        cursor = break_at + 3;
    }
}

static void frontier_open_workbench_overlay(void)
{
    g_runtime.workbench_tab = frontier_clamp_int(g_runtime.workbench_tab, 0, 2);
    g_runtime.workbench_selected_entry[g_runtime.workbench_tab] =
        frontier_workbench_clamp_entry_index(
            g_runtime.workbench_tab,
            g_runtime.workbench_selected_entry[g_runtime.workbench_tab]
        );
    g_runtime.workbench_visible = 1;
    g_runtime.compact_visible = 0;
    g_runtime.full_visible = 0;
    frontier_refresh_context_state(1);
    frontier_refresh_live_profile(1);
    frontier_session_save();
    frontier_mark_ui_dirty();
    frontier_set_status(frontier_text("Workbench opened.", "작업대를 열었습니다."));
    frontier_log("Workbench opened. Tab=%d Entry=%d", g_runtime.workbench_tab, g_runtime.workbench_selected_entry[g_runtime.workbench_tab]);
}

static void frontier_close_workbench_overlay(int save_profile)
{
    if (!g_runtime.workbench_visible) {
        return;
    }

    g_runtime.workbench_visible = 0;
    frontier_session_save();
    if (save_profile) {
        frontier_live_profile_save(1);
    }
    frontier_mark_ui_dirty();
    frontier_set_status(frontier_text("Workbench closed.", "작업대를 닫았습니다."));
    frontier_log("Workbench closed.");
}

static void frontier_change_workbench_tab(int delta)
{
    g_runtime.workbench_tab += delta;
    if (g_runtime.workbench_tab < FRONTIER_WORKBENCH_TAB_CRAFT) {
        g_runtime.workbench_tab = FRONTIER_WORKBENCH_TAB_RESULT;
    }
    if (g_runtime.workbench_tab > FRONTIER_WORKBENCH_TAB_RESULT) {
        g_runtime.workbench_tab = FRONTIER_WORKBENCH_TAB_CRAFT;
    }
    g_runtime.workbench_selected_entry[g_runtime.workbench_tab] =
        frontier_workbench_clamp_entry_index(
            g_runtime.workbench_tab,
            g_runtime.workbench_selected_entry[g_runtime.workbench_tab]
        );
    frontier_session_save();
    frontier_mark_ui_dirty();
    frontier_log("Workbench tab changed to %d.", g_runtime.workbench_tab);
}

static void frontier_change_workbench_entry(int delta)
{
    int tab_index = frontier_clamp_int(g_runtime.workbench_tab, 0, 2);
    int entry_count = frontier_workbench_entry_count(tab_index);

    if (entry_count <= 0) {
        return;
    }

    g_runtime.workbench_selected_entry[tab_index] += delta;
    if (g_runtime.workbench_selected_entry[tab_index] < 0) {
        g_runtime.workbench_selected_entry[tab_index] = 0;
    }
    if (g_runtime.workbench_selected_entry[tab_index] >= entry_count) {
        g_runtime.workbench_selected_entry[tab_index] = entry_count - 1;
    }
    frontier_session_save();
    frontier_mark_ui_dirty();
    frontier_log("Workbench entry changed. Tab=%d Entry=%d", tab_index, g_runtime.workbench_selected_entry[tab_index]);
}

static int frontier_try_craft_recipe_entry(int entry_index, int update_workbench_result, const char *source_text)
{
    int korean = frontier_dataset_is_korean();
    uint32_t flag = frontier_craft_flag_for_entry(entry_index);
    const char *gate_text = frontier_workbench_recipe_gate_display(entry_index);
    const char *effect_text = frontier_workbench_recipe_effect_display(entry_index);
    const char *materials_text = frontier_workbench_recipe_materials_display(entry_index);
    const char *detail_text = frontier_workbench_recipe_export_detail(entry_index);
    const char *source = source_text != NULL ? source_text : "Ledger";

    if (frontier_live_recipe_crafted(entry_index)) {
        if (update_workbench_result) {
            snprintf(g_runtime.workbench_result_title, sizeof(g_runtime.workbench_result_title), "%s", frontier_workbench_recipe_primary_name(entry_index));
            snprintf(g_runtime.workbench_result_body, sizeof(g_runtime.workbench_result_body), "%s~n~%s", detail_text, gate_text);
        }
        frontier_session_save();
        frontier_mark_ui_dirty();
        frontier_set_status(frontier_text("This recipe is already secured.", "This recipe is already secured."));
        frontier_log("%s craft skipped because recipe is already secured. Entry=%d", source, entry_index);
        return 0;
    }

    if (!frontier_live_recipe_ready(entry_index)) {
        if (update_workbench_result) {
            snprintf(g_runtime.workbench_result_title, sizeof(g_runtime.workbench_result_title), "%s", frontier_workbench_recipe_primary_name(entry_index));
            snprintf(g_runtime.workbench_result_body, sizeof(g_runtime.workbench_result_body), "%s~n~%s", detail_text, gate_text);
        }
        frontier_session_save();
        frontier_mark_ui_dirty();
        frontier_set_status(frontier_text("Recipe is not ready yet.", "Recipe is not ready yet."));
        frontier_log("%s craft blocked because recipe is not ready. Entry=%d", source, entry_index);
        return -1;
    }

    g_runtime.live_profile.crafted_mask |= flag;
    frontier_live_profile_refresh_progression_layers();
    frontier_live_profile_refresh_strings();
    g_runtime.live_profile.dirty = 1;

    if (update_workbench_result) {
        snprintf(g_runtime.workbench_result_title, sizeof(g_runtime.workbench_result_title), "%s", frontier_workbench_recipe_primary_name(entry_index));
        snprintf(
            g_runtime.workbench_result_body,
            sizeof(g_runtime.workbench_result_body),
            korean ? "%s~n~%s~n~%s" : "%s~n~%s~n~%s",
            detail_text,
            frontier_text("LIVE BONUS UPDATED", "LIVE BONUS UPDATED"),
            g_runtime.live_profile.bonus_summary
        );
    }

    frontier_session_save();
    frontier_mark_ui_dirty();
    frontier_set_status(frontier_text("Recipe secured.", "Recipe secured."));
    frontier_log("%s craft committed. Entry=%d CraftedMask=0x%X", source, entry_index, g_runtime.live_profile.crafted_mask);
    frontier_live_profile_save(1);
    return 1;
}

static void frontier_confirm_workbench_action(void)
{
    int korean = frontier_dataset_is_korean();
    int entry_index = frontier_workbench_clamp_entry_index(
        g_runtime.workbench_tab,
        g_runtime.workbench_selected_entry[g_runtime.workbench_tab]
    );

    if (g_runtime.workbench_tab == FRONTIER_WORKBENCH_TAB_CRAFT) {
        frontier_try_craft_recipe_entry(entry_index, 1, "Workbench");
        return;
        uint32_t flag = frontier_craft_flag_for_entry(entry_index);
        const char *gate_text = frontier_workbench_recipe_gate_display(entry_index);
        const char *effect_text = frontier_workbench_recipe_effect_display(entry_index);
        const char *materials_text = frontier_workbench_recipe_materials_display(entry_index);

        if (frontier_live_recipe_crafted(entry_index)) {
            snprintf(g_runtime.workbench_result_title, sizeof(g_runtime.workbench_result_title), "%s", frontier_workbench_recipe_name(entry_index));
            snprintf(
                g_runtime.workbench_result_body,
                sizeof(g_runtime.workbench_result_body),
                "%s~n~%s",
                gate_text,
                effect_text
            );
            frontier_session_save();
            frontier_mark_ui_dirty();
            frontier_set_status(frontier_text("This recipe is already secured.", "이미 확보된 제작입니다."));
            frontier_log("Workbench craft skipped because recipe is already secured. Entry=%d", entry_index);
            return;
        }

        if (!frontier_live_recipe_ready(entry_index)) {
            snprintf(g_runtime.workbench_result_title, sizeof(g_runtime.workbench_result_title), "%s", frontier_workbench_recipe_name(entry_index));
            snprintf(
                g_runtime.workbench_result_body,
                sizeof(g_runtime.workbench_result_body),
                "%s~n~%s",
                materials_text,
                gate_text
            );
            frontier_session_save();
            frontier_mark_ui_dirty();
            frontier_set_status(frontier_text("Recipe is not ready yet.", "아직 준비되지 않은 제작입니다."));
            frontier_log("Workbench craft blocked because recipe is not ready. Entry=%d", entry_index);
            return;
        }

        g_runtime.live_profile.crafted_mask |= flag;
        frontier_live_profile_refresh_progression_layers();
        frontier_live_profile_refresh_strings();
        g_runtime.live_profile.dirty = 1;
        snprintf(g_runtime.workbench_result_title, sizeof(g_runtime.workbench_result_title), "%s", frontier_workbench_recipe_name(entry_index));
            snprintf(
                g_runtime.workbench_result_body,
                sizeof(g_runtime.workbench_result_body),
                korean ? "%s~n~%s~n~%s" : "%s~n~%s~n~%s~n~%s",
                materials_text,
                effect_text,
                frontier_text("LIVE BONUS UPDATED", "현재 런 보너스에 바로 반영됩니다."),
                g_runtime.live_profile.bonus_summary
            );
        frontier_session_save();
        frontier_mark_ui_dirty();
        frontier_set_status(frontier_text("Recipe secured at the workbench.", "작업대에서 제작을 확정했습니다."));
        frontier_log("Workbench craft committed. Entry=%d CraftedMask=0x%X", entry_index, g_runtime.live_profile.crafted_mask);
        frontier_live_profile_save(1);
        return;
    }

    if (g_runtime.workbench_tab == FRONTIER_WORKBENCH_TAB_ARCHIVE) {
        snprintf(g_runtime.workbench_result_title, sizeof(g_runtime.workbench_result_title), "%s", frontier_workbench_archive_name(entry_index));
        snprintf(
            g_runtime.workbench_result_body,
            sizeof(g_runtime.workbench_result_body),
            "%s",
            korean ? frontier_workbench_archive_detail_korean(entry_index) : frontier_workbench_archive_detail(entry_index)
        );
        frontier_session_save();
        frontier_mark_ui_dirty();
        frontier_set_status(frontier_text("Archive note pinned to results.", "백과 내용을 결과 패널에 고정했습니다."));
        frontier_log("Workbench archive note pinned. Entry=%d", entry_index);
        return;
    }

    frontier_set_status(frontier_text("Result tab is read-only.", "결과 탭은 읽기 전용입니다."));
    frontier_log("Workbench confirm ignored on result tab.");
}

static void frontier_confirm_full_ledger_action(void)
{
    const FrontierLedgerPage *page = frontier_current_page();
    int entry_index;

    if (page == NULL || page->entry_count <= 0) {
        return;
    }

    entry_index = frontier_clamp_int(g_runtime.selected_entry[g_runtime.current_page], 0, frontier_max_int(0, page->entry_count - 1));

    if (strcmp(page->page_id, "stats") == 0) {
        int *slot = frontier_live_stat_bonus_slot(entry_index);
        if (frontier_live_stat_points_available() <= 0) {
            frontier_set_status(frontier_text("No stat points available yet.", "No stat points available yet."));
            return;
        }
        if (*slot >= 3) {
            frontier_set_status(frontier_text("This stat is already fully invested for now.", "This stat is already fully invested for now."));
            return;
        }
        *slot += 1;
        g_runtime.live_profile.dirty = 1;
        frontier_live_profile_refresh_progression_layers();
        frontier_live_profile_refresh_strings();
        frontier_session_save();
        frontier_live_profile_save(1);
        frontier_mark_ui_dirty();
        frontier_set_status(frontier_text("Stat point invested.", "Stat point invested."));
        frontier_log("Stat investment applied. Entry=%d Remaining=%d", entry_index, frontier_live_stat_points_available());
        return;
    }

    if (strcmp(page->page_id, "skills") == 0) {
        int *slot = frontier_live_skill_bonus_slot(entry_index);
        if (frontier_live_skill_points_available() <= 0) {
            frontier_set_status(frontier_text("No skill points available yet.", "No skill points available yet."));
            return;
        }
        if (*slot >= 3) {
            frontier_set_status(frontier_text("This skill is already fully trained for now.", "This skill is already fully trained for now."));
            return;
        }
        *slot += 1;
        g_runtime.live_profile.dirty = 1;
        frontier_live_profile_refresh_progression_layers();
        frontier_live_profile_refresh_strings();
        frontier_session_save();
        frontier_live_profile_save(1);
        frontier_mark_ui_dirty();
        frontier_set_status(frontier_text("Skill point invested.", "Skill point invested."));
        frontier_log("Skill investment applied. Entry=%d Remaining=%d", entry_index, frontier_live_skill_points_available());
        return;
    }

    if (strcmp(page->page_id, "crafting") == 0) {
        frontier_try_craft_recipe_entry(entry_index, 0, "Ledger");
        return;
    }

    if (strcmp(page->page_id, "armory") == 0) {
        int craft_entry = -1;
        if (entry_index == 0) {
            if (!frontier_live_recipe_crafted(7) && frontier_live_recipe_ready(7)) {
                craft_entry = 7;
            } else if (!frontier_live_recipe_crafted(4) && frontier_live_recipe_ready(4)) {
                craft_entry = 4;
            }
        } else {
            if (!frontier_live_recipe_crafted(8) && frontier_live_recipe_ready(8)) {
                craft_entry = 8;
            } else if (!frontier_live_recipe_crafted(5) && frontier_live_recipe_ready(5)) {
                craft_entry = 5;
            } else if (!frontier_live_recipe_crafted(2) && frontier_live_recipe_ready(2)) {
                craft_entry = 2;
            } else if (!frontier_live_recipe_crafted(0) && frontier_live_recipe_ready(0)) {
                craft_entry = 0;
            }
        }

        if (craft_entry >= 0) {
            frontier_try_craft_recipe_entry(craft_entry, 0, "Armory");
        } else {
            frontier_set_status(frontier_text("No armory tuning is ready for this weapon yet.", "No armory tuning is ready for this weapon yet."));
        }
        return;
    }

    if (frontier_selected_boss_spawn_profile() != NULL) {
        frontier_log_selected_entry_context(page, entry_index, "confirm-boss-queue");
        g_runtime.pending_spawn_request = FRONTIER_SPAWN_REQUEST_BOSS;
        frontier_set_status("Boss spawn queued from the selected archive entry.");
        return;
    }

    if (frontier_selected_entry_spawn_profile() != NULL) {
        frontier_log_selected_entry_context(page, entry_index, "confirm-selected-queue");
        g_runtime.pending_spawn_request = FRONTIER_SPAWN_REQUEST_SELECTED;
        frontier_set_status("Selected encounter spawn queued from the full ledger.");
        return;
    }

    frontier_set_status(frontier_text("This page stays read-only for now.", "This page stays read-only for now."));
}

static void frontier_render_workbench_prompt(void)
{
    frontier_draw_rect_tl(0.29f, 0.830f, 0.42f, 0.065f, kColorBackground);
    frontier_draw_rect_tl(0.29f, 0.830f, 0.42f, 0.009f, kColorAccentSoft);
    frontier_draw_text_short(
        frontier_text("Workbench [E] craft  |  Ledger [F11]", "작업대 [E] 제작  |  레저 [F11]"),
        0.312f,
        0.852f,
        frontier_dataset_is_korean() ? 0.36f : 0.33f,
        kColorText
    );
}
