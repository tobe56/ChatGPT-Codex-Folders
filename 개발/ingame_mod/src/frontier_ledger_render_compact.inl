static void frontier_render_compact_overlay(void)
{
    if (frontier_dataset_is_korean()) {
        char line_a[64];
        char line_b[64];
        char line_c[96];
        char line_d[96];
        char line_e[96];
        char line_f[96];
        float left = 0.52f;
        float top = 0.048f;
        float width = 0.42f;
        float card_top = top + 0.046f;
        int bonus_tier = frontier_live_bonus_tier();

        snprintf(line_a, sizeof(line_a), "ENEMY %d", g_runtime.live_profile.enemy_kills);
        snprintf(line_b, sizeof(line_b), "BONUS T%d", bonus_tier);
        snprintf(line_c, sizeof(line_c), "ANIMAL %d", g_runtime.live_profile.animal_kills);
        snprintf(line_d, sizeof(line_d), "LOOT %d", g_runtime.live_profile.loot_count);
        snprintf(line_e, sizeof(line_e), "REWARD $%d | HUB %d", g_runtime.live_profile.reward_earned, g_runtime.live_profile.hub_returns);
        snprintf(
            line_f,
            sizeof(line_f),
            "STAGE %d | REG %d/6 | DIST %.1fKM",
            g_runtime.live_profile.trail_stage,
            g_runtime.live_profile.discovered_region_count,
            g_runtime.live_profile.field_distance_total / 1000.0f
        );

        frontier_draw_rect_tl(left, top, width, 0.255f, kColorBackground);
        frontier_draw_rect_tl(left, top, width, 0.040f, kColorAccentSoft);
        frontier_draw_text_short("프론티어 레저", left + 0.015f, top + 0.008f, 0.42f, kColorText);
        frontier_draw_text_short(frontier_runtime_context_label(), left + 0.230f, top + 0.011f, 0.21f, kColorAccent);
        frontier_draw_text_short("F10 / ESC", left + width - 0.105f, top + 0.011f, 0.16f, kColorTextDim);

        frontier_draw_rect_tl(left + 0.015f, card_top, 0.185f, 0.082f, kColorPanel);
        frontier_draw_text_short("전투", left + 0.026f, card_top + 0.012f, 0.25f, kColorAccent);
        frontier_draw_text_short(line_a, left + 0.026f, card_top + 0.041f, 0.18f, kColorText);
        frontier_draw_text_short(line_b, left + 0.026f, card_top + 0.064f, 0.16f, kColorTextDim);

        frontier_draw_rect_tl(left + 0.220f, card_top, 0.185f, 0.082f, kColorPanel);
        frontier_draw_text_short("사냥", left + 0.231f, card_top + 0.012f, 0.25f, kColorAccent);
        frontier_draw_text_short(line_c, left + 0.231f, card_top + 0.041f, 0.18f, kColorText);
        frontier_draw_text_short(line_d, left + 0.231f, card_top + 0.064f, 0.16f, kColorTextDim);

        frontier_draw_rect_tl(left + 0.015f, card_top + 0.098f, width - 0.030f, 0.115f, kColorPanel);
        frontier_draw_text_short("진행", left + 0.026f, card_top + 0.111f, 0.25f, kColorAccent);
        frontier_draw_text_short(frontier_live_trail_stage_compact_label(g_runtime.live_profile.trail_stage), left + 0.026f, card_top + 0.140f, 0.19f, kColorText);
        frontier_draw_text_short(line_e, left + 0.026f, card_top + 0.164f, 0.16f, kColorTextDim);
        frontier_draw_text_short(line_f, left + 0.026f, card_top + 0.188f, 0.15f, kColorTextDim);
        return;
    }

    const FrontierLedgerSection *section;
    int korean = frontier_dataset_is_korean();
    int compact_chars = frontier_korean_compact_chars(42);
    float compact_line_height = korean ? 0.019f : 0.018f;
    float left = korean ? 0.58f : 0.68f;
    float top = 0.055f;
    float width = korean ? 0.38f : 0.28f;
    float panel_height = korean ? 0.13f : 0.13f;
    float y = top + 0.02f;
    int wrapped_lines;
    int section_index;
    int line_index;

    if (g_runtime.dataset == NULL) {
        return;
    }

    for (section_index = 0; section_index < g_runtime.dataset->compact_section_count; ++section_index) {
        panel_height += 0.031f;
        for (line_index = 0; line_index < g_runtime.dataset->compact_sections[section_index].line_count; ++line_index) {
            wrapped_lines = frontier_count_wrapped_lines(
                g_runtime.dataset->compact_sections[section_index].lines[line_index],
                compact_chars,
                korean ? 2 : 2
            );
            panel_height += (compact_line_height * (float)wrapped_lines) + 0.008f;
        }
    }
    panel_height += 0.015f;
    panel_height += korean ? 0.100f : 0.128f;

    frontier_draw_rect_tl(left, top, width, panel_height, kColorBackground);
    frontier_draw_rect_tl(left, top, width, 0.032f, kColorAccentSoft);
    frontier_draw_text_short(frontier_ui_text("Frontier Ledger", "FL_UI_TITLE"), left + 0.012f, top + 0.006f, korean ? 0.38f : 0.38f, kColorText);

    y += 0.03f;
    for (section_index = 0; section_index < g_runtime.dataset->compact_section_count; ++section_index) {
        section = &g_runtime.dataset->compact_sections[section_index];
        frontier_draw_text_short(section->title, left + 0.012f, y, korean ? 0.34f : 0.34f, kColorAccent);
        y += korean ? 0.025f : 0.021f;
        for (line_index = 0; line_index < section->line_count; ++line_index) {
            frontier_draw_wrapped_text(
                section->lines[line_index],
                left + 0.016f,
                y,
                korean ? 0.31f : 0.31f,
                compact_line_height,
                compact_chars,
                korean ? 2 : 2,
                kColorText
            );
            wrapped_lines = frontier_count_wrapped_lines(section->lines[line_index], compact_chars, korean ? 2 : 2);
            y += (compact_line_height * (float)wrapped_lines) + 0.008f;
        }
        y += 0.01f;
    }

    frontier_draw_rect_tl(left + 0.012f, top + panel_height - (korean ? 0.075f : 0.094f), width - 0.024f, 0.0016f, kColorAccentSoft);
    frontier_draw_text_short(frontier_live_trail_stage_compact_label(g_runtime.live_profile.trail_stage), left + 0.012f, top + panel_height - (korean ? 0.093f : 0.117f), korean ? 0.31f : 0.30f, kColorAccent);
    frontier_draw_text_short(frontier_runtime_progress_summary_compact(), left + 0.012f, top + panel_height - (korean ? 0.068f : 0.088f), korean ? 0.27f : 0.24f, kColorTextDim);
    frontier_draw_wrapped_text(
        korean ? frontier_runtime_objective_compact() : frontier_runtime_objective(),
        left + 0.016f,
        top + panel_height - (korean ? 0.043f : 0.056f),
        korean ? 0.30f : 0.30f,
        korean ? 0.018f : 0.017f,
        compact_chars,
        korean ? 2 : 2,
        kColorText
    );
    frontier_draw_text_short(frontier_runtime_context_label(), left + 0.012f, top + panel_height - 0.020f, korean ? 0.25f : 0.22f, kColorAccentSoft);
    if (!korean) {
        frontier_draw_text_short(frontier_runtime_dataset_label(), left + 0.16f, top + panel_height - 0.024f, 0.17f, kColorTextDim);
    }
}
