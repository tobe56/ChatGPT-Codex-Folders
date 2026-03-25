static int frontier_find_page_index(const char *page_id)
{
    int index;
    int page_count = frontier_page_count();

    if (page_id == NULL || page_id[0] == '\0' || g_runtime.dataset == NULL) {
        return -1;
    }

    for (index = 0; index < page_count; ++index) {
        if (strcmp(g_runtime.dataset->pages[index].page_id, page_id) == 0) {
            return index;
        }
    }

    return -1;
}

static int frontier_context_entry_for_page(const FrontierLedgerPage *page)
{
    if (page == NULL || page->entry_count <= 0 || !g_runtime.context.valid) {
        return -1;
    }

    if (strcmp(page->page_id, "frontier_map") == 0) {
        return frontier_min_int(g_runtime.context.region_index, page->entry_count - 1);
    }

    if (strcmp(page->page_id, "farming") == 0) {
        static const int kLaneByRegion[] = {3, 1, 1, 0, 4, 4};
        return frontier_min_int(kLaneByRegion[g_runtime.context.region_index], page->entry_count - 1);
    }

    if (strcmp(page->page_id, "network") == 0) {
        static const int kNetworkByRegion[] = {2, 3, 3, 1, 5, 7};
        return frontier_min_int(kNetworkByRegion[g_runtime.context.region_index], page->entry_count - 1);
    }

    if (strcmp(page->page_id, "dead") == 0) {
        static const int kDeadByRegion[] = {1, 0, 0, 4, 3, 4};
        return frontier_min_int(kDeadByRegion[g_runtime.context.region_index], page->entry_count - 1);
    }

    if (strcmp(page->page_id, "world") == 0) {
        if (g_runtime.dataset != NULL) {
            if (strstr(g_runtime.dataset->dataset_id, "baseline") != NULL) {
                return 0;
            }
            if (strstr(g_runtime.dataset->dataset_id, "simulated") != NULL) {
                return frontier_min_int(1, page->entry_count - 1);
            }
            if (strstr(g_runtime.dataset->dataset_id, "endgame") != NULL) {
                return frontier_min_int(4, page->entry_count - 1);
            }
        }
    }

    if (strcmp(page->page_id, "ending") == 0) {
        if (g_runtime.dataset != NULL) {
            if (strstr(g_runtime.dataset->dataset_id, "baseline") != NULL) {
                return 0;
            }
            if (strstr(g_runtime.dataset->dataset_id, "simulated") != NULL) {
                return frontier_min_int(1, page->entry_count - 1);
            }
            if (strstr(g_runtime.dataset->dataset_id, "endgame") != NULL) {
                return frontier_min_int(page->entry_count - 1, page->entry_count - 1);
            }
        }
    }

    return -1;
}

static void frontier_apply_context_focus_for_page(int page_index)
{
    const FrontierLedgerPage *page;
    int target_entry;

    if (page_index < 0 || page_index >= frontier_page_count() || g_runtime.dataset == NULL) {
        return;
    }

    page = &g_runtime.dataset->pages[page_index];
    target_entry = frontier_context_entry_for_page(page);
    if (target_entry < 0) {
        return;
    }

    g_runtime.selected_entry[page_index] = target_entry;
    frontier_clamp_selection(page_index);
}

static void frontier_refresh_context_state(int force_refresh)
{
    int index;
    int best_index = -1;
    float best_distance = 0.0f;
    FrontierVector3 position;
    int game_timer;

    game_timer = frontier_game_timer();
    if (!force_refresh && g_runtime.context.valid && (game_timer - (int)g_runtime.context.refreshed_at) < (int)kFrontierContextRefreshMs) {
        return;
    }

    frontier_refresh_zone_state(force_refresh);
    position = g_runtime.zone_state.position;

    if (!g_runtime.zone_state.valid) {
        return;
    }

    for (index = 0; index < kFrontierRegionAnchorCount; ++index) {
        float dx = position.x - kFrontierRegionAnchors[index].x;
        float dy = position.y - kFrontierRegionAnchors[index].y;
        float distance = (dx * dx) + (dy * dy);

        if (best_index < 0 || distance < best_distance) {
            best_index = index;
            best_distance = distance;
        }
    }

    if (best_index >= 0) {
        g_runtime.context.valid = 1;
        g_runtime.context.region_index = best_index;
        g_runtime.context.distance_sq = best_distance;
        g_runtime.context.refreshed_at = (DWORD)game_timer;
        frontier_copy_string(
            g_runtime.context.region_label,
            sizeof(g_runtime.context.region_label),
            frontier_dataset_is_korean() ? kFrontierRegionAnchors[best_index].label_ko : kFrontierRegionAnchors[best_index].label
        );
        frontier_copy_string(
            g_runtime.context.route_hint,
            sizeof(g_runtime.context.route_hint),
            frontier_dataset_is_korean() ? kFrontierRegionAnchors[best_index].route_hint_ko : kFrontierRegionAnchors[best_index].route_hint
        );
    }
}
