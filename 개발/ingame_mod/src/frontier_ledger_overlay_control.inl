static void frontier_overlay_sync(void)
{
    FrontierOverlayMode mode;
    RECT game_rect;
    RECT bounds;
    int prompt_visible = 0;
    int status_visible = 0;
    int needs_redraw = 0;

    frontier_overlay_pump_messages();

    if (!frontier_get_game_client_rect(&game_rect) || !frontier_game_window_is_foreground()) {
        frontier_overlay_hide();
        return;
    }

    mode = frontier_overlay_mode(&prompt_visible, &status_visible);
    if (mode == FRONTIER_OVERLAY_MODE_HIDDEN) {
        frontier_overlay_hide();
        return;
    }

    if (!frontier_overlay_ensure_window()) {
        return;
    }

    frontier_overlay_compute_bounds(mode, prompt_visible, status_visible, &game_rect, &bounds);
    if (!g_runtime.overlay.visible ||
        g_runtime.overlay.mode != (int)mode ||
        g_runtime.overlay.prompt_visible != prompt_visible ||
        g_runtime.overlay.status_visible != status_visible ||
        memcmp(&g_runtime.overlay.bounds, &bounds, sizeof(bounds)) != 0) {
        SetWindowPos(
            g_runtime.overlay.window,
            HWND_TOPMOST,
            bounds.left,
            bounds.top,
            bounds.right - bounds.left,
            bounds.bottom - bounds.top,
            SWP_NOACTIVATE | SWP_SHOWWINDOW
        );
        ShowWindow(g_runtime.overlay.window, SW_SHOWNOACTIVATE);
        g_runtime.overlay.bounds = bounds;
        g_runtime.overlay.mode = (int)mode;
        g_runtime.overlay.prompt_visible = prompt_visible;
        g_runtime.overlay.status_visible = status_visible;
        g_runtime.overlay.visible = 1;
        frontier_log(
            "Desktop overlay shown. Mode=%d Bounds=(%ld,%ld,%ld,%ld)",
            (int)mode,
            (long)bounds.left,
            (long)bounds.top,
            (long)bounds.right,
            (long)bounds.bottom
        );
        needs_redraw = 1;
    }

    if (g_runtime.overlay.layout_generation != g_runtime.ui_generation) {
        g_runtime.overlay.layout_generation = g_runtime.ui_generation;
        needs_redraw = 1;
    }

    if (needs_redraw) {
        InvalidateRect(g_runtime.overlay.window, NULL, TRUE);
        UpdateWindow(g_runtime.overlay.window);
    }

    frontier_overlay_pump_messages();
}

static void frontier_overlay_shutdown(void)
{
    if (IsWindow(g_runtime.overlay.window)) {
        DestroyWindow(g_runtime.overlay.window);
    }
    g_runtime.overlay.window = NULL;
    g_runtime.overlay.game_window = NULL;
    g_runtime.overlay.visible = 0;
    g_runtime.overlay.mode = FRONTIER_OVERLAY_MODE_HIDDEN;
}
