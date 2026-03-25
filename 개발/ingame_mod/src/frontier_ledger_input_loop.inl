static void frontier_change_page(int delta)
{
    int page_count = frontier_page_count();

    if (page_count <= 0) {
        return;
    }

    g_runtime.current_page += delta;
    if (g_runtime.current_page < 0) {
        g_runtime.current_page = page_count - 1;
    }
    if (g_runtime.current_page >= page_count) {
        g_runtime.current_page = 0;
    }
    frontier_apply_context_focus_for_page(g_runtime.current_page);
    frontier_clamp_selection(g_runtime.current_page);
    frontier_session_save();
    frontier_mark_ui_dirty();
}

static void frontier_change_entry(int delta)
{
    const FrontierLedgerPage *page = frontier_current_page();
    if (page == NULL || page->entry_count <= 0) {
        return;
    }

    g_runtime.selected_entry[g_runtime.current_page] += delta;
    if (g_runtime.selected_entry[g_runtime.current_page] < 0) {
        g_runtime.selected_entry[g_runtime.current_page] = 0;
    }
    if (g_runtime.selected_entry[g_runtime.current_page] >= page->entry_count) {
        g_runtime.selected_entry[g_runtime.current_page] = page->entry_count - 1;
    }
    frontier_clamp_selection(g_runtime.current_page);
    frontier_session_save();
    frontier_mark_ui_dirty();
}

static DWORD WINAPI FrontierBootstrapThread(LPVOID unused)
{
    int waited_ms = 0;
    (void)unused;

    while (waited_ms <= g_runtime.config.bootstrap_timeout_ms) {
        if (FrontierScriptHook_Bind()) {
            FrontierScriptHook_RegisterScript(g_runtime.module, FrontierScriptMain);
            FrontierScriptHook_RegisterKeyboard(FrontierKeyboardMessage);
            InterlockedExchange(&g_runtime.script_hook_registered, 1);
            InterlockedExchange(&g_runtime.bootstrap_finished, 1);
            frontier_log("ScriptHook binding successful after %d ms.", waited_ms);
            return 0;
        }

        Sleep((DWORD)g_runtime.config.bootstrap_poll_ms);
        waited_ms += g_runtime.config.bootstrap_poll_ms;
    }

    frontier_log("ScriptHook binding timed out after %d ms.", waited_ms);
    InterlockedExchange(&g_runtime.bootstrap_finished, 1);
    return 0;
}

static void frontier_handle_initial_keydown(DWORD key)
{
    if (g_runtime.workbench_visible) {
        if ((int)key == g_runtime.config.close_key) {
            frontier_close_workbench_overlay(1);
            return;
        }

        if ((int)key == g_runtime.config.previous_page_key) {
            frontier_change_workbench_tab(-1);
            return;
        }

        if ((int)key == g_runtime.config.next_page_key) {
            frontier_change_workbench_tab(1);
            return;
        }

        if ((int)key == g_runtime.config.previous_entry_key) {
            frontier_change_workbench_entry(-1);
            return;
        }

        if ((int)key == g_runtime.config.next_entry_key) {
            frontier_change_workbench_entry(1);
            return;
        }

        if ((int)key == g_runtime.config.confirm_key) {
            frontier_confirm_workbench_action();
            return;
        }

        return;
    }

    if ((int)key == g_runtime.config.spawn_debug_key) {
        frontier_try_spawn_debug_path();
        return;
    }

    if ((int)key == g_runtime.config.confirm_key) {
        if (!g_runtime.full_visible &&
            !g_runtime.compact_visible &&
            frontier_inside_workbench_zone()) {
            frontier_open_workbench_overlay();
            return;
        }
    }

    if ((int)key == g_runtime.config.compact_toggle_key) {
        if (!g_runtime.full_visible && !g_runtime.workbench_visible) {
            g_runtime.compact_visible = !g_runtime.compact_visible;
            frontier_refresh_context_state(1);
            frontier_refresh_live_profile(1);
            frontier_mark_ui_dirty();
            frontier_log("Compact ledger %s.", g_runtime.compact_visible ? "opened" : "closed");
        } else {
            frontier_set_status(frontier_ui_text("Close the full ledger before toggling compact mode.", "FL_UI_STATUS_CLOSE_FULL_FIRST"));
            frontier_log("Compact toggle blocked because full ledger is open.");
        }
        return;
    }

    if ((int)key == g_runtime.config.full_toggle_key) {
        if (g_runtime.full_visible) {
            g_runtime.full_visible = 0;
            frontier_set_status(frontier_ui_text("Frontier Ledger closed.", "FL_UI_STATUS_LEDGER_CLOSED"));
            frontier_session_save();
            frontier_live_profile_save(1);
            frontier_mark_ui_dirty();
            frontier_log("Full ledger closed by keypress.");
        } else if (frontier_inside_beechers_hope_zone()) {
            int default_page = frontier_live_default_page_index();
            g_runtime.full_visible = 1;
            g_runtime.compact_visible = 0;
            frontier_refresh_context_state(1);
            frontier_refresh_live_profile(1);
            if (default_page >= 0) {
                g_runtime.current_page = default_page;
            }
            frontier_apply_context_focus_for_page(g_runtime.current_page);
            frontier_session_save();
            frontier_mark_ui_dirty();
            frontier_set_status(frontier_ui_text("Frontier Ledger opened.", "FL_UI_STATUS_LEDGER_OPENED"));
            frontier_log("Full ledger opened by keypress.");
        } else {
            frontier_refresh_zone_state(1);
            frontier_set_status(frontier_ui_text("Full ledger is only available at Beecher's Hope.", "FL_UI_STATUS_ZONE_BLOCK"));
            frontier_log(
                "Full ledger blocked. Pos=(%.2f, %.2f, %.2f) DistanceSq=%.2f HeightDelta=%.2f InRadius=%d Interior=%d RequireInterior=%d Radius=%.2f",
                g_runtime.zone_state.position.x,
                g_runtime.zone_state.position.y,
                g_runtime.zone_state.position.z,
                g_runtime.zone_state.distance_sq,
                g_runtime.zone_state.height_delta,
                g_runtime.zone_state.in_radius,
                g_runtime.zone_state.interior_id,
                g_runtime.config.require_interior,
                g_runtime.config.beechers_hope_radius
            );
        }
        return;
    }

    if (!g_runtime.full_visible) {
        return;
    }

    if ((int)key == g_runtime.config.close_key) {
        g_runtime.full_visible = 0;
        frontier_session_save();
        frontier_live_profile_save(1);
        frontier_mark_ui_dirty();
        frontier_set_status(frontier_ui_text("Frontier Ledger closed.", "FL_UI_STATUS_LEDGER_CLOSED"));
        frontier_log("Full ledger closed by close key.");
        return;
    }

    if ((int)key == g_runtime.config.previous_page_key) {
        frontier_change_page(-1);
        frontier_log("Moved to previous page index %d.", g_runtime.current_page);
        return;
    }

    if ((int)key == g_runtime.config.next_page_key) {
        frontier_change_page(1);
        frontier_log("Moved to next page index %d.", g_runtime.current_page);
        return;
    }

    if ((int)key == g_runtime.config.previous_entry_key) {
        frontier_change_entry(-1);
        frontier_log("Moved to previous entry index %d on page %d.", g_runtime.selected_entry[g_runtime.current_page], g_runtime.current_page);
        return;
    }

    if ((int)key == g_runtime.config.next_entry_key) {
        frontier_change_entry(1);
        frontier_log("Moved to next entry index %d on page %d.", g_runtime.selected_entry[g_runtime.current_page], g_runtime.current_page);
        return;
    }

    if ((int)key == g_runtime.config.confirm_key) {
        frontier_confirm_full_ledger_action();
        return;
    }
}

static void frontier_tick(void)
{
    int corpse_prompt_active = 0;
    int prompt_active = 0;
    int workbench_prompt_active = 0;

    frontier_log_label_probe_once();
    frontier_refresh_context_state(0);
    frontier_refresh_live_profile(0);
    frontier_apply_live_gameplay_effects();
    if (g_runtime.pending_spawn_request != FRONTIER_SPAWN_REQUEST_NONE) {
        int spawn_request = g_runtime.pending_spawn_request;
        int debug_single_ped = g_runtime.debug_spawn_single_ped;
        g_runtime.pending_spawn_request = FRONTIER_SPAWN_REQUEST_NONE;
        frontier_log("Processing pending spawn request type=%d in main tick. DebugSinglePed=%d", spawn_request, debug_single_ped);
        if (spawn_request == FRONTIER_SPAWN_REQUEST_BOSS) {
            frontier_spawn_selected_boss();
        } else if (spawn_request == FRONTIER_SPAWN_REQUEST_SELECTED) {
            frontier_spawn_selected_entry_encounter();
        } else if (spawn_request == FRONTIER_SPAWN_REQUEST_REGION) {
            frontier_spawn_region_encounter();
        }
        g_runtime.debug_spawn_single_ped = 0;
    }

    if (!g_runtime.announced_ready) {
        if (g_runtime.config.show_ready_toast) {
            frontier_set_status(frontier_ui_text("Frontier Ledger ready.", "FL_UI_STATUS_READY"));
        }
        frontier_log(
            "Runtime initialized. Dataset=%s RequestedDataset=%s Phase=%d Regions=%d HubReturns=%d",
            g_runtime.dataset != NULL ? g_runtime.dataset->dataset_id : "null",
            g_runtime.requested_dataset_id,
            g_runtime.live_profile.phase,
            g_runtime.live_profile.discovered_region_count,
            g_runtime.live_profile.hub_returns
        );
        g_runtime.announced_ready = 1;
    }

    if (frontier_pause_menu_active()) {
        if (frontier_use_desktop_overlay()) {
            frontier_overlay_hide();
        }
        return;
    }

    workbench_prompt_active = !g_runtime.workbench_visible &&
        !g_runtime.full_visible &&
        !g_runtime.compact_visible &&
        frontier_inside_workbench_zone();
    g_runtime.workbench_prompt_visible = workbench_prompt_active;
    corpse_prompt_active = !g_runtime.workbench_visible &&
        !g_runtime.full_visible &&
        !g_runtime.compact_visible &&
        frontier_active_frontier_corpse() != NULL;
    g_runtime.corpse_prompt_visible = corpse_prompt_active;

    if (g_runtime.workbench_visible && !frontier_inside_workbench_zone()) {
        frontier_close_workbench_overlay(1);
        frontier_set_status(frontier_text("Workbench closed after leaving the station.", "작업대 범위를 벗어나 작업대를 닫았습니다."));
        frontier_log("Workbench auto-closed because player left the workbench zone.");
        workbench_prompt_active = 0;
        g_runtime.workbench_prompt_visible = 0;
    }

    if (g_runtime.full_visible && g_runtime.config.auto_close_full_ledger_outside_zone && !frontier_inside_beechers_hope_zone()) {
        g_runtime.full_visible = 0;
        frontier_session_save();
        frontier_live_profile_save(1);
        frontier_mark_ui_dirty();
        frontier_set_status(frontier_ui_text("Full ledger closed after leaving the hub zone.", "FL_UI_STATUS_ZONE_AUTOCLOSE"));
        frontier_log("Full ledger auto-closed because player left the allowed zone.");
    }

    if ((g_runtime.full_visible || g_runtime.workbench_visible) &&
        g_runtime.config.disable_controls_while_open &&
        !frontier_use_desktop_overlay()) {
        frontier_disable_controls();
    }

    if (frontier_use_desktop_overlay()) {
        frontier_overlay_sync();
    } else {
        frontier_overlay_hide();
        if (g_runtime.workbench_visible) {
            frontier_render_workbench_overlay();
        } else if (g_runtime.full_visible) {
            frontier_render_full_overlay();
        } else if (g_runtime.compact_visible) {
            frontier_render_compact_overlay();
        }

        prompt_active = !g_runtime.workbench_visible &&
            !corpse_prompt_active &&
            !workbench_prompt_active &&
            !g_runtime.full_visible &&
            !g_runtime.compact_visible &&
            g_runtime.config.show_zone_prompt &&
            g_runtime.zone_state.inside_zone;

        if (corpse_prompt_active) {
            frontier_render_frontier_corpse_prompt();
        } else if (workbench_prompt_active) {
            frontier_render_workbench_prompt();
        } else if (prompt_active) {
            frontier_render_zone_prompt();
        }

        frontier_render_status();
    }
}

static void FrontierScriptMain(void)
{
    while (1) {
        int prompt_fast_tick;

        frontier_tick();
        prompt_fast_tick = g_runtime.corpse_prompt_visible ||
            g_runtime.workbench_prompt_visible ||
            (g_runtime.config.show_zone_prompt &&
             !g_runtime.workbench_visible &&
             !g_runtime.full_visible &&
             !g_runtime.compact_visible &&
             g_runtime.zone_state.inside_zone);

        if (g_runtime.workbench_visible ||
            g_runtime.full_visible ||
            g_runtime.compact_visible ||
            frontier_status_active() ||
            prompt_fast_tick) {
            FrontierScriptHook_Wait(0);
        } else {
            FrontierScriptHook_Wait(50);
        }
    }
}

static void FrontierKeyboardMessage(
    DWORD key,
    WORD repeats,
    BYTE scan_code,
    BOOL is_extended,
    BOOL with_alt,
    BOOL was_down_before,
    BOOL is_up_now
)
{
    (void)repeats;
    (void)scan_code;
    (void)is_extended;
    (void)with_alt;

    if (is_up_now || was_down_before) {
        return;
    }

    frontier_handle_initial_keydown(key);
}
