static void frontier_overlay_paint_compact(HDC device_context, const RECT *client_rect, const FrontierOverlayFonts *fonts)
{
    const FrontierLedgerSection *section;
    RECT block_rect;
    int margin_x;
    int width;
    int y;
    int section_index;
    int line_index;
    int line_height;

    if (g_runtime.dataset == NULL || client_rect == NULL || fonts == NULL) {
        return;
    }

    width = client_rect->right - client_rect->left;
    margin_x = frontier_max_int(16, width / 20);
    block_rect = *client_rect;
    frontier_overlay_fill_rect(device_context, &block_rect, kColorBackground);

    block_rect.left = 0;
    block_rect.top = 0;
    block_rect.right = width;
    block_rect.bottom = frontier_max_int(40, (client_rect->bottom - client_rect->top) / 11);
    frontier_overlay_fill_rect(device_context, &block_rect, kColorAccentSoft);

    block_rect.left = margin_x;
    block_rect.top = 10;
    block_rect.right = width - margin_x;
    block_rect.bottom = frontier_max_int(44, (client_rect->bottom - client_rect->top) / 11);
    frontier_overlay_draw_text(
        device_context,
        fonts->heading,
        frontier_ui_text("Frontier Ledger", "FL_UI_TITLE"),
        &block_rect,
        kColorText,
        DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX
    );

    y = block_rect.bottom + 12;
    for (section_index = 0; section_index < g_runtime.dataset->compact_section_count; ++section_index) {
        section = &g_runtime.dataset->compact_sections[section_index];
        block_rect.left = margin_x;
        block_rect.top = y;
        block_rect.right = width - margin_x;
        block_rect.bottom = y + 28;
        frontier_overlay_draw_text(
            device_context,
            fonts->heading,
            section->title,
            &block_rect,
            kColorAccent,
            DT_LEFT | DT_SINGLELINE | DT_NOPREFIX
        );
        y += 28;

        for (line_index = 0; line_index < section->line_count; ++line_index) {
            line_height = frontier_overlay_measure_text(
                device_context,
                fonts->body,
                section->lines[line_index],
                width - (margin_x * 2),
                DT_LEFT | DT_WORDBREAK | DT_NOPREFIX
            );
            block_rect.left = margin_x;
            block_rect.top = y;
            block_rect.right = width - margin_x;
            block_rect.bottom = y + line_height;
            frontier_overlay_draw_text(
                device_context,
                fonts->body,
                section->lines[line_index],
                &block_rect,
                kColorText,
                DT_LEFT | DT_WORDBREAK | DT_NOPREFIX
            );
            y += line_height + 8;
        }
        y += 8;
    }

    block_rect.left = margin_x;
    block_rect.top = client_rect->bottom - frontier_max_int(30, (client_rect->bottom - client_rect->top) / 12);
    block_rect.right = width - margin_x;
    block_rect.bottom = client_rect->bottom - 8;
    frontier_overlay_draw_text(
        device_context,
        fonts->small,
        frontier_runtime_dataset_label(),
        &block_rect,
        kColorTextDim,
        DT_LEFT | DT_SINGLELINE | DT_NOPREFIX
    );
}

static void frontier_overlay_paint_tabs(
    HDC device_context,
    const FrontierOverlayFonts *fonts,
    int left,
    int top,
    int width,
    int height
)
{
    int page_count;
    int visible_tabs;
    int start_index;
    int end_index;
    int page_index;
    int draw_index;
    int tab_area_width;
    int indicator_width;
    int tab_width;
    char indicator_text[32];

    page_count = frontier_page_count();
    if (page_count <= 0) {
        return;
    }

    visible_tabs = frontier_overlay_visible_tab_count(width, page_count);
    start_index = frontier_tab_window_start(page_count, g_runtime.current_page, visible_tabs);
    end_index = frontier_min_int(page_count, start_index + visible_tabs);
    indicator_width = (page_count > visible_tabs) ? 96 : 0;
    tab_area_width = width - indicator_width;
    tab_width = tab_area_width / frontier_max_int(1, visible_tabs);

    for (draw_index = 0; draw_index < (end_index - start_index); ++draw_index) {
        RECT tab_rect;
        const char *title;
        page_index = start_index + draw_index;
        FrontierColor tab_color = (page_index == g_runtime.current_page) ? kColorSelection : kColorPanelLight;

        tab_rect.left = left + (draw_index * tab_width);
        tab_rect.top = top;
        tab_rect.right = tab_rect.left + frontier_max_int(32, tab_width - 4);
        tab_rect.bottom = top + height;
        frontier_overlay_fill_rect(device_context, &tab_rect, tab_color);

        tab_rect.left += 10;
        tab_rect.right -= 10;
        title = frontier_page_title_display(&g_runtime.dataset->pages[page_index]);
        frontier_overlay_draw_text(
            device_context,
            fonts->small,
            title,
            &tab_rect,
            (page_index == g_runtime.current_page) ? kColorText : kColorTextDim,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX
        );
    }

    if (page_count > visible_tabs) {
        RECT indicator_rect;

        snprintf(indicator_text, sizeof(indicator_text), "%d/%d", g_runtime.current_page + 1, page_count);
        indicator_rect.left = left + tab_area_width + 8;
        indicator_rect.top = top;
        indicator_rect.right = left + width;
        indicator_rect.bottom = top + height;
        frontier_overlay_draw_text(
            device_context,
            fonts->small,
            indicator_text,
            &indicator_rect,
            kColorTextDim,
            DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX
        );
    }
}

static void frontier_overlay_paint_full(HDC device_context, const RECT *client_rect, const FrontierOverlayFonts *fonts)
{
    const FrontierLedgerPage *page;
    const FrontierLedgerEntry *entry;
    RECT block_rect;
    int width;
    int height;
    int margin;
    int header_height;
    int tabs_top;
    int tabs_height;
    int list_top;
    int list_width;
    int detail_left;
    int row_height;
    int draw_index;
    int visible_count;
    int start_index;

    if (client_rect == NULL || fonts == NULL) {
        return;
    }

    page = frontier_current_page();
    width = client_rect->right - client_rect->left;
    height = client_rect->bottom - client_rect->top;
    margin = frontier_max_int(18, width / 54);
    header_height = frontier_max_int(52, height / 16);
    tabs_height = frontier_max_int(42, height / 18);
    tabs_top = header_height + margin;
    list_top = tabs_top + tabs_height + 12;
    list_width = (width - (margin * 3)) / 2;
    detail_left = margin + list_width + margin;
    row_height = frontier_max_int(34, height / 20);

    frontier_overlay_fill_rect(device_context, client_rect, kColorBackground);

    block_rect.left = 0;
    block_rect.top = 0;
    block_rect.right = width;
    block_rect.bottom = header_height;
    frontier_overlay_fill_rect(device_context, &block_rect, kColorAccentSoft);

    block_rect.left = margin;
    block_rect.top = 8;
    block_rect.right = width - margin;
    block_rect.bottom = header_height - 8;
    frontier_overlay_draw_text(
        device_context,
        fonts->title,
        frontier_main_title_display(),
        &block_rect,
        kColorText,
        DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX
    );

    block_rect.left = width - frontier_max_int(260, width / 4);
    block_rect.right = width - margin;
    frontier_overlay_draw_text(
        device_context,
        fonts->small,
        frontier_runtime_dataset_label(),
        &block_rect,
        kColorTextDim,
        DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX
    );

    frontier_overlay_paint_tabs(device_context, fonts, margin, tabs_top, width - (margin * 2), tabs_height);

    block_rect.left = margin;
    block_rect.top = list_top;
    block_rect.right = margin + list_width;
    block_rect.bottom = height - margin - 56;
    frontier_overlay_fill_rect(device_context, &block_rect, kColorPanel);

    block_rect.left = detail_left;
    block_rect.top = list_top;
    block_rect.right = width - margin;
    block_rect.bottom = height - margin - 56;
    frontier_overlay_fill_rect(device_context, &block_rect, kColorPanel);

    if (page == NULL || page->entry_count <= 0) {
        block_rect.left = detail_left + 16;
        block_rect.top = list_top + 18;
        block_rect.right = width - margin - 16;
        block_rect.bottom = list_top + 80;
        frontier_overlay_draw_text(
            device_context,
            fonts->heading,
            frontier_ui_text("No entries available.", "FL_UI_EMPTY"),
            &block_rect,
            kColorText,
            DT_LEFT | DT_WORDBREAK | DT_NOPREFIX
        );
        return;
    }

    start_index = g_runtime.top_entry[g_runtime.current_page];
    visible_count = frontier_min_int(frontier_page_entry_capacity(), page->entry_count - start_index);
    for (draw_index = 0; draw_index < visible_count; ++draw_index) {
        int entry_index = start_index + draw_index;
        const char *label = frontier_live_entry_label(page, entry_index, &page->entries[entry_index]);
        FrontierColor row_color = (entry_index == g_runtime.selected_entry[g_runtime.current_page]) ? kColorSelection : kColorPanelLight;

        block_rect.left = margin + 10;
        block_rect.top = list_top + 12 + (draw_index * row_height);
        block_rect.right = margin + list_width - 10;
        block_rect.bottom = block_rect.top + row_height - 6;
        frontier_overlay_fill_rect(device_context, &block_rect, row_color);

        block_rect.left += 10;
        block_rect.right -= 10;
        frontier_overlay_draw_text(
            device_context,
            fonts->body,
            label,
            &block_rect,
            kColorText,
            DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX
        );
    }

    entry = &page->entries[g_runtime.selected_entry[g_runtime.current_page]];
    block_rect.left = detail_left + 16;
    block_rect.top = list_top + 16;
    block_rect.right = width - margin - 16;
    block_rect.bottom = block_rect.top + 28;
    frontier_overlay_draw_text(
        device_context,
        fonts->heading,
        frontier_page_title_display(page),
        &block_rect,
        kColorAccent,
        DT_LEFT | DT_SINGLELINE | DT_NOPREFIX
    );

    block_rect.left = detail_left + 16;
    block_rect.top += 36;
    block_rect.right = width - margin - 16;
    block_rect.bottom = block_rect.top + frontier_overlay_measure_text(
        device_context,
        fonts->small,
        frontier_live_page_summary(page),
        (width - margin - 16) - (detail_left + 16),
        DT_LEFT | DT_WORDBREAK | DT_NOPREFIX
    );
    frontier_overlay_draw_text(
        device_context,
        fonts->small,
        frontier_live_page_summary(page),
        &block_rect,
        kColorTextDim,
        DT_LEFT | DT_WORDBREAK | DT_NOPREFIX
    );

    block_rect.left = detail_left + 16;
    block_rect.top = block_rect.bottom + 16;
    block_rect.right = width - margin - 16;
    block_rect.bottom = block_rect.top + 30;
    frontier_overlay_draw_text(
        device_context,
        fonts->heading,
        frontier_live_entry_label(page, g_runtime.selected_entry[g_runtime.current_page], entry),
        &block_rect,
        kColorText,
        DT_LEFT | DT_WORDBREAK | DT_NOPREFIX
    );

    block_rect.left = detail_left + 16;
    block_rect.top = block_rect.bottom + 18;
    block_rect.right = width - margin - 16;
    block_rect.bottom = height - margin - 74;
    frontier_overlay_draw_text(
        device_context,
        fonts->body,
        frontier_live_entry_detail(page, g_runtime.selected_entry[g_runtime.current_page], entry),
        &block_rect,
        kColorTextDim,
        DT_LEFT | DT_WORDBREAK | DT_NOPREFIX
    );

    block_rect.left = detail_left + 16;
    block_rect.top = height - margin - 46;
    block_rect.right = width - margin - 16;
    block_rect.bottom = height - margin - 12;
    frontier_overlay_draw_text(
        device_context,
        fonts->small,
        frontier_footer_controls_display(),
        &block_rect,
        kColorTextDim,
        DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX
    );
}

static void frontier_overlay_paint_hud(HDC device_context, const RECT *client_rect, const FrontierOverlayFonts *fonts)
{
    RECT block_rect;
    int width;
    int y = 10;

    if (client_rect == NULL || fonts == NULL) {
        return;
    }

    width = client_rect->right - client_rect->left;
    frontier_overlay_fill_rect(device_context, client_rect, kColorPanel);

    if (g_runtime.overlay.prompt_visible) {
        block_rect.left = 18;
        block_rect.top = y;
        block_rect.right = width - 18;
        block_rect.bottom = y + 34;
        frontier_overlay_draw_text(
            device_context,
            fonts->heading,
            frontier_ui_text(
                "Beecher's Hope [F11] full ledger",
                "FL_UI_ZONE_PROMPT"
            ),
            &block_rect,
            kColorText,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX
        );
        y = block_rect.bottom + 8;
    }

    if (g_runtime.overlay.status_visible) {
        block_rect.left = 18;
        block_rect.top = y;
        block_rect.right = width - 18;
        block_rect.bottom = client_rect->bottom - 10;
        frontier_overlay_draw_text(
            device_context,
            fonts->body,
            g_runtime.status_message,
            &block_rect,
            kColorText,
            DT_CENTER | DT_WORDBREAK | DT_NOPREFIX
        );
    }
}

static void frontier_overlay_paint(HWND window)
{
    PAINTSTRUCT paint;
    HDC device_context;
    RECT client_rect;
    FrontierOverlayFonts fonts;

    device_context = BeginPaint(window, &paint);
    if (device_context == NULL) {
        return;
    }

    GetClientRect(window, &client_rect);
    frontier_overlay_init_fonts(&client_rect, &fonts);

    switch ((FrontierOverlayMode)g_runtime.overlay.mode) {
    case FRONTIER_OVERLAY_MODE_COMPACT:
        frontier_overlay_paint_compact(device_context, &client_rect, &fonts);
        break;

    case FRONTIER_OVERLAY_MODE_FULL:
        frontier_overlay_paint_full(device_context, &client_rect, &fonts);
        break;

    case FRONTIER_OVERLAY_MODE_HUD:
        frontier_overlay_paint_hud(device_context, &client_rect, &fonts);
        break;

    default:
        frontier_overlay_fill_rect(device_context, &client_rect, kColorBackground);
        break;
    }

    frontier_overlay_destroy_fonts(&fonts);
    EndPaint(window, &paint);
}

static LRESULT CALLBACK FrontierOverlayWindowProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
    (void)w_param;
    (void)l_param;

    switch (message) {
    case WM_SETCURSOR:
        SetCursor(NULL);
        return TRUE;

    case WM_MOUSEACTIVATE:
        return MA_NOACTIVATE;

    case WM_NCHITTEST:
        return HTTRANSPARENT;

    case WM_ERASEBKGND:
        return 1;

    case WM_PAINT:
        frontier_overlay_paint(window);
        return 0;

    default:
        return DefWindowProcW(window, message, w_param, l_param);
    }
}
