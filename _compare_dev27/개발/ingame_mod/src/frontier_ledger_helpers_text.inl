static const FrontierGeneratedTextEntry *frontier_find_generated_text(const char *label)
{
    int low = 0;
    int high = kFrontierGeneratedTextCount - 1;

    if (label == NULL || label[0] == '\0') {
        return NULL;
    }

    while (low <= high) {
        int middle = low + ((high - low) / 2);
        int compare = strcmp(label, kFrontierGeneratedTexts[middle].label);
        if (compare == 0) {
            return &kFrontierGeneratedTexts[middle];
        }
        if (compare < 0) {
            high = middle - 1;
        } else {
            low = middle + 1;
        }
    }

    return NULL;
}

static const char *frontier_lookup_generated_text(const char *label)
{
    const FrontierGeneratedTextEntry *entry = frontier_find_generated_text(label);
    return entry != NULL ? entry->value : NULL;
}

static const FrontierBitmapGlyph *frontier_find_bitmap_glyph(uint32_t codepoint)
{
    int low = 0;
    int high = kFrontierBitmapGlyphCount - 1;

    while (low <= high) {
        int middle = low + ((high - low) / 2);
        if (kFrontierBitmapGlyphs[middle].codepoint == codepoint) {
            return &kFrontierBitmapGlyphs[middle];
        }
        if (codepoint < kFrontierBitmapGlyphs[middle].codepoint) {
            high = middle - 1;
        } else {
            low = middle + 1;
        }
    }

    return NULL;
}

static uint32_t frontier_utf8_next_codepoint(const char **cursor)
{
    const unsigned char *bytes;
    uint32_t codepoint;

    if (cursor == NULL || *cursor == NULL || **cursor == '\0') {
        return 0U;
    }

    bytes = (const unsigned char *)*cursor;
    if (bytes[0] < 0x80U) {
        *cursor += 1;
        return bytes[0];
    }
    if ((bytes[0] & 0xE0U) == 0xC0U && bytes[1] != 0U) {
        codepoint = ((uint32_t)(bytes[0] & 0x1FU) << 6) | (uint32_t)(bytes[1] & 0x3FU);
        *cursor += 2;
        return codepoint;
    }
    if ((bytes[0] & 0xF0U) == 0xE0U && bytes[1] != 0U && bytes[2] != 0U) {
        codepoint = ((uint32_t)(bytes[0] & 0x0FU) << 12) |
                    ((uint32_t)(bytes[1] & 0x3FU) << 6) |
                    (uint32_t)(bytes[2] & 0x3FU);
        *cursor += 3;
        return codepoint;
    }
    if ((bytes[0] & 0xF8U) == 0xF0U && bytes[1] != 0U && bytes[2] != 0U && bytes[3] != 0U) {
        codepoint = ((uint32_t)(bytes[0] & 0x07U) << 18) |
                    ((uint32_t)(bytes[1] & 0x3FU) << 12) |
                    ((uint32_t)(bytes[2] & 0x3FU) << 6) |
                    (uint32_t)(bytes[3] & 0x3FU);
        *cursor += 4;
        return codepoint;
    }

    *cursor += 1;
    return (uint32_t)'?';
}

static const char *frontier_make_literal_string(const char *text)
{
    uint64_t *result = frontier_invoke(
        HASH_VAR_STRING,
        3,
        FrontierPackInt(10),
        FrontierPackPointer("LITERAL_STRING"),
        FrontierPackPointer(frontier_prepare_display_text(text))
    );
    if (result == NULL || *result == 0ULL) {
        return text;
    }
    return (const char *)(uintptr_t)(*result);
}

static int frontier_has_text_label(const char *text)
{
    if (!frontier_dataset_is_korean() || text == NULL || text[0] == '\0') {
        return 0;
    }
    return frontier_native_bool(HASH_DOES_TEXT_LABEL_EXIST, 1, FrontierPackPointer(text));
}

static const char *frontier_resolve_text(const char *text)
{
    const char *resolved;

    resolved = frontier_lookup_generated_text(text);
    if (resolved != NULL && resolved[0] != '\0') {
        return resolved;
    }

    if (!frontier_has_text_label(text)) {
        return text;
    }

    resolved = frontier_native_string(HASH_GET_LABEL_TEXT_2, 1, FrontierPackPointer(text));
    if (resolved == NULL || resolved[0] == '\0') {
        return text;
    }
    return resolved;
}

static const FrontierLedgerPage *frontier_find_dataset_page(const char *page_id)
{
    int page_index;

    if (g_runtime.dataset == NULL || page_id == NULL || page_id[0] == '\0') {
        return NULL;
    }

    page_index = frontier_find_page_index(page_id);
    if (page_index < 0 || page_index >= g_runtime.dataset->page_count) {
        return NULL;
    }

    return &g_runtime.dataset->pages[page_index];
}

static const FrontierLedgerEntry *frontier_find_dataset_entry(const char *page_id, int entry_index)
{
    const FrontierLedgerPage *page = frontier_find_dataset_page(page_id);
    if (page == NULL || entry_index < 0 || entry_index >= page->entry_count) {
        return NULL;
    }
    return &page->entries[entry_index];
}

static void frontier_copy_label_segment(const char *label, char *destination, size_t destination_size, int segment_count)
{
    const char *cursor = label;
    int segments_left = segment_count;

    if (destination == NULL || destination_size == 0U) {
        return;
    }

    destination[0] = '\0';
    if (label == NULL || label[0] == '\0') {
        return;
    }

    while (*cursor == ' ') {
        ++cursor;
    }

    while (*cursor != '\0' && segments_left > 0) {
        const char *separator = strstr(cursor, " | ");
        size_t chunk_length = separator != NULL ? (size_t)(separator - cursor) : strlen(cursor);
        size_t current_length = strlen(destination);

        while (chunk_length > 0U && cursor[chunk_length - 1] == ' ') {
            --chunk_length;
        }

        if (current_length > 0U && current_length + 3U < destination_size) {
            frontier_copy_string(destination + current_length, destination_size - current_length, " | ");
            current_length = strlen(destination);
        }

        if (current_length < destination_size - 1U) {
            size_t copy_length = chunk_length;
            if (copy_length > destination_size - current_length - 1U) {
                copy_length = destination_size - current_length - 1U;
            }
            memcpy(destination + current_length, cursor, copy_length);
            destination[current_length + copy_length] = '\0';
        }

        if (separator == NULL) {
            break;
        }

        cursor = separator + 3;
        while (*cursor == ' ') {
            ++cursor;
        }
        --segments_left;
    }
}

static void frontier_copy_label_without_last_segment(const char *label, char *destination, size_t destination_size)
{
    const char *last_separator;
    size_t copy_length;

    if (destination == NULL || destination_size == 0U) {
        return;
    }

    destination[0] = '\0';
    if (label == NULL || label[0] == '\0') {
        return;
    }

    last_separator = NULL;
    {
        const char *scan = label;
        while ((scan = strstr(scan, " | ")) != NULL) {
            last_separator = scan;
            scan += 3;
        }
    }

    copy_length = last_separator != NULL ? (size_t)(last_separator - label) : strlen(label);
    while (copy_length > 0U && label[copy_length - 1] == ' ') {
        --copy_length;
    }
    if (copy_length >= destination_size) {
        copy_length = destination_size - 1U;
    }
    memcpy(destination, label, copy_length);
    destination[copy_length] = '\0';
}

static void frontier_copy_label_last_segment(const char *label, char *destination, size_t destination_size)
{
    const char *last_separator = label;

    if (destination == NULL || destination_size == 0U) {
        return;
    }

    destination[0] = '\0';
    if (label == NULL || label[0] == '\0') {
        return;
    }

    while (last_separator != NULL) {
        const char *next = strstr(last_separator, " | ");
        if (next == NULL) {
            break;
        }
        last_separator = next + 3;
    }

    while (*last_separator == ' ') {
        ++last_separator;
    }
    frontier_copy_string(destination, destination_size, last_separator);
}

static const char *frontier_workbench_recipe_export_label(int entry_index)
{
    const FrontierLedgerEntry *entry = frontier_find_dataset_entry("crafting", entry_index);
    return entry != NULL ? frontier_resolve_text(entry->label) : "";
}

static const char *frontier_workbench_recipe_export_detail(int entry_index)
{
    const FrontierLedgerEntry *entry = frontier_find_dataset_entry("crafting", entry_index);
    return entry != NULL ? frontier_resolve_text(entry->detail) : "";
}

static int frontier_text_is_ascii(const char *text)
{
    const char *resolved = frontier_resolve_text(text);

    if (resolved == NULL) {
        return 1;
    }

    while (*resolved != '\0') {
        if (((unsigned char)*resolved) >= 128U) {
            return 0;
        }
        ++resolved;
    }
    return 1;
}

static void frontier_log_label_probe_once(void)
{
    const char *label_a = "FL_UI_TITLE";
    const char *label_b = "FL_SIMULATED_KO_DISPLAY";
    const char *resolved_a;
    const char *resolved_b;
    int exists_a;
    int exists_b;
    uint32_t hash_a;
    uint32_t hash_b;

    if (g_runtime.label_probe_logged || !frontier_dataset_is_korean()) {
        return;
    }

    exists_a = frontier_native_bool(HASH_DOES_TEXT_LABEL_EXIST, 1, FrontierPackPointer(label_a));
    exists_b = frontier_native_bool(HASH_DOES_TEXT_LABEL_EXIST, 1, FrontierPackPointer(label_b));
    hash_a = frontier_get_hash_key(label_a);
    hash_b = frontier_get_hash_key(label_b);
    resolved_a = frontier_lookup_generated_text(label_a);
    resolved_b = frontier_lookup_generated_text(label_b);
    frontier_log(
        "Label probe: %s exists=%d hash=0x%08X generated=%s | %s exists=%d hash=0x%08X generated=%s",
        label_a,
        exists_a,
        (unsigned int)hash_a,
        (resolved_a != NULL && resolved_a[0] != '\0') ? resolved_a : "(null)",
        label_b,
        exists_b,
        (unsigned int)hash_b,
        (resolved_b != NULL && resolved_b[0] != '\0') ? resolved_b : "(null)"
    );
    g_runtime.label_probe_logged = 1;
}

static uint32_t frontier_get_hash_key(const char *text)
{
    uint64_t *result;

    if (text == NULL || text[0] == '\0') {
        return 0U;
    }

    result = frontier_invoke(HASH_GET_HASH_KEY, 1, FrontierPackPointer(text));
    if (result == NULL) {
        return 0U;
    }

    return (uint32_t)(*result & 0xFFFFFFFFULL);
}

static const char *frontier_make_display_string(const char *text)
{
    const char *resolved = frontier_resolve_text(text);
    uint64_t *result;
    uint32_t label_hash;

    if (frontier_has_text_label(text)) {
        label_hash = frontier_get_hash_key(text);
        if (label_hash != 0U) {
            result = frontier_invoke(HASH_VAR_STRING, 2, FrontierPackInt(0), FrontierPackUInt64((uint64_t)label_hash));
            if (result != NULL && *result != 0ULL) {
                return (const char *)(uintptr_t)(*result);
            }
        }
    }

    if (resolved != NULL && resolved[0] != '\0' && resolved != text) {
        return frontier_make_literal_string(resolved);
    }

    return frontier_make_literal_string(text);
}

static void frontier_draw_native_text_short(const char *text, float x, float y, float scale, FrontierColor color)
{
    const char *literal;

    if (text == NULL || text[0] == '\0') {
        return;
    }

    literal = frontier_make_display_string(text);
    frontier_invoke(HASH_BG_SET_TEXT_SCALE, 2, FrontierPackFloat(scale), FrontierPackFloat(scale));
    frontier_invoke(
        HASH_BG_SET_TEXT_COLOR,
        4,
        FrontierPackInt(color.red),
        FrontierPackInt(color.green),
        FrontierPackInt(color.blue),
        FrontierPackInt(color.alpha)
    );
    frontier_invoke(HASH_BG_DISPLAY_TEXT, 3, FrontierPackPointer(literal), FrontierPackFloat(x), FrontierPackFloat(y));
}

static void frontier_draw_rect(float center_x, float center_y, float width, float height, FrontierColor color)
{
    frontier_invoke(
        HASH_DRAW_RECT,
        10,
        FrontierPackFloat(center_x),
        FrontierPackFloat(center_y),
        FrontierPackFloat(width),
        FrontierPackFloat(height),
        FrontierPackInt(color.red),
        FrontierPackInt(color.green),
        FrontierPackInt(color.blue),
        FrontierPackInt(color.alpha),
        FrontierPackBool(0),
        FrontierPackBool(0)
    );
}

static void frontier_draw_rect_tl(float left, float top, float width, float height, FrontierColor color)
{
    frontier_draw_rect(left + (width * 0.5f), top + (height * 0.5f), width, height, color);
}

static float frontier_snap_horizontal(float value)
{
    int pixel = (int)(value * 1920.0f + 0.5f);
    return (float)pixel / 1920.0f;
}

static float frontier_snap_vertical(float value)
{
    int pixel = (int)(value * 1080.0f + 0.5f);
    return (float)pixel / 1080.0f;
}

static float frontier_snap_horizontal_span(float value)
{
    int pixel = (int)(value * 1920.0f + 0.5f);
    if (pixel < 1) {
        pixel = 1;
    }
    return (float)pixel / 1920.0f;
}

static float frontier_snap_vertical_span(float value)
{
    int pixel = (int)(value * 1080.0f + 0.5f);
    if (pixel < 1) {
        pixel = 1;
    }
    return (float)pixel / 1080.0f;
}

static float frontier_bitmap_pixel_height_for_scale(float scale)
{
    float desired_line_height = scale * 0.058f;
    float pixel_height;

    if (frontier_dataset_is_korean()) {
        desired_line_height *= 1.16f;
    }
    pixel_height = desired_line_height / (float)FRONTIER_BITMAP_FONT_LINE_ADVANCE;
    if (frontier_dataset_is_korean()) {
        if (pixel_height < (1.20f / 1080.0f)) {
            pixel_height = 1.20f / 1080.0f;
        } else if (pixel_height > (6.0f / 1080.0f)) {
            pixel_height = 6.0f / 1080.0f;
        }
    }
    return pixel_height;
}

static float frontier_bitmap_pixel_width_for_height(float pixel_height)
{
    if (frontier_dataset_is_korean()) {
        return pixel_height * 1.00f;
    }
    return pixel_height * (1080.0f / 1920.0f);
}

static int frontier_bitmap_char_budget(int max_chars)
{
    if (max_chars <= 0 || !frontier_dataset_is_korean()) {
        return max_chars;
    }
    return frontier_max_int(8, (max_chars * 3) / 5);
}

static int frontier_bitmap_use_soft_fringe(float pixel_height)
{
    if (!frontier_dataset_is_korean()) {
        return 1;
    }
    return (pixel_height * 1080.0f) >= 1.65f;
}

static void frontier_draw_bitmap_mask_runs(
    uint32_t mask,
    int width,
    float x,
    float y,
    float pixel_width,
    float pixel_height,
    FrontierColor color
)
{
    int column = 0;
    float snapped_y = frontier_snap_vertical(y);
    float snapped_height = frontier_snap_vertical_span(pixel_height);

    while (column < width) {
        int run_start;
        int run_length = 0;
        float snapped_x;
        float snapped_width;
        while (column < width && ((mask >> column) & 1U) == 0U) {
            ++column;
        }
        if (column >= width) {
            break;
        }
        run_start = column;
        while (column < width && ((mask >> column) & 1U) != 0U) {
            ++column;
            ++run_length;
        }
        snapped_x = frontier_snap_horizontal(x + ((float)run_start * pixel_width));
        snapped_width = frontier_snap_horizontal_span((float)run_length * pixel_width);
        frontier_draw_rect_tl(
            snapped_x,
            snapped_y,
            snapped_width,
            snapped_height,
            color
        );
    }
}

static void frontier_draw_bitmap_glyph(
    const FrontierBitmapGlyph *glyph,
    float x,
    float y,
    float pixel_width,
    float pixel_height,
    FrontierColor color
)
{
    int row_index;
    int use_soft_fringe;

    if (glyph == NULL || glyph->width == 0 || glyph->height == 0) {
        return;
    }

    use_soft_fringe = frontier_bitmap_use_soft_fringe(pixel_height);
    for (row_index = 0; row_index < (int)glyph->height; ++row_index) {
        FrontierColor soft_color = color;
        uint32_t soft_mask = glyph->soft_rows[row_index];
        uint32_t solid_mask = glyph->rows[row_index];
        float row_y = y + ((float)row_index * pixel_height);

        if (use_soft_fringe && soft_mask != 0U) {
            soft_color.alpha = frontier_max_int(18, (color.alpha * 28) / 100);
            frontier_draw_bitmap_mask_runs(soft_mask, (int)glyph->width, x, row_y, pixel_width, pixel_height, soft_color);
        }
        if (solid_mask != 0U) {
            frontier_draw_bitmap_mask_runs(solid_mask, (int)glyph->width, x, row_y, pixel_width, pixel_height, color);
        }
    }
}

static int frontier_is_forced_newline(const char *cursor)
{
    return cursor != NULL && cursor[0] == '~' && cursor[1] == 'n' && cursor[2] == '~';
}

static const char *frontier_skip_spaces(const char *cursor)
{
    while (cursor != NULL && *cursor == ' ') {
        ++cursor;
    }
    return cursor;
}

static void frontier_draw_bitmap_text_span(
    const char *start,
    const char *end,
    float x,
    float y,
    float pixel_width,
    float pixel_height,
    FrontierColor color
)
{
    const char *cursor = start;
    const FrontierBitmapGlyph *glyph;
    float cursor_x = x;

    if (start == NULL || end == NULL || start >= end) {
        return;
    }

    while (cursor < end && *cursor != '\0') {
        uint32_t codepoint;
        float advance_multiplier = 1.0f;
        codepoint = frontier_utf8_next_codepoint(&cursor);
        if (codepoint == 0U) {
            break;
        }

        glyph = frontier_find_bitmap_glyph(codepoint);
        if (glyph == NULL) {
            glyph = frontier_find_bitmap_glyph((uint32_t)'?');
        }
        if (glyph == NULL) {
            continue;
        }

        if (frontier_dataset_is_korean() && codepoint > 0x7FU) {
            advance_multiplier = 1.00f;
        }
        frontier_draw_bitmap_glyph(glyph, cursor_x, y, pixel_width, pixel_height, color);
        cursor_x += ((float)glyph->advance * pixel_width) * advance_multiplier;
    }
}

static int frontier_count_bitmap_lines(const char *text, int max_chars, int max_lines)
{
    const char *resolved = frontier_resolve_text(text);
    const char *cursor;
    int line_count = 0;
    int effective_max_chars = frontier_bitmap_char_budget(max_chars);

    if (resolved == NULL || resolved[0] == '\0') {
        return 0;
    }

    cursor = resolved;
    while (*cursor != '\0' && line_count < max_lines) {
        const char *line_start = frontier_skip_spaces(cursor);
        const char *scan = line_start;
        const char *last_space_start = NULL;
        const char *last_space_end = NULL;
        const char *overflow_start = NULL;
        int glyph_count = 0;

        if (*line_start == '\0') {
            break;
        }

        while (*scan != '\0') {
            const char *char_start = scan;
            uint32_t codepoint;

            if (frontier_is_forced_newline(scan) || *scan == '\n') {
                break;
            }

            codepoint = frontier_utf8_next_codepoint(&scan);
            if (codepoint == 0U) {
                break;
            }

            if (codepoint == (uint32_t)' ') {
                last_space_start = char_start;
                last_space_end = scan;
            }

            ++glyph_count;
            if (effective_max_chars > 0 && glyph_count > effective_max_chars) {
                overflow_start = char_start;
                break;
            }
        }

        ++line_count;
        if (frontier_is_forced_newline(scan)) {
            cursor = scan + 3;
            continue;
        }
        if (*scan == '\n') {
            cursor = scan + 1;
            continue;
        }
        if (overflow_start != NULL) {
            cursor = (last_space_end != NULL && last_space_start != NULL && last_space_start > line_start)
                ? last_space_end
                : overflow_start;
            continue;
        }
        cursor = scan;
    }

    return line_count > 0 ? line_count : 1;
}

static void frontier_draw_bitmap_text_internal(
    const char *text,
    float x,
    float y,
    float pixel_width,
    float pixel_height,
    float line_height,
    int max_chars,
    int max_lines,
    FrontierColor color
)
{
    const char *resolved = frontier_resolve_text(text);
    const char *cursor;
    int line_index = 0;
    int effective_max_chars = frontier_bitmap_char_budget(max_chars);

    if (resolved == NULL || resolved[0] == '\0') {
        return;
    }

    cursor = resolved;
    while (*cursor != '\0' && line_index < max_lines) {
        const char *line_start = frontier_skip_spaces(cursor);
        const char *scan = line_start;
        const char *draw_end = line_start;
        const char *next_cursor = NULL;
        const char *last_space_start = NULL;
        const char *last_space_end = NULL;
        const char *overflow_start = NULL;
        int glyph_count = 0;

        if (*line_start == '\0') {
            break;
        }

        while (*scan != '\0') {
            const char *char_start = scan;
            uint32_t codepoint;

            if (frontier_is_forced_newline(scan) || *scan == '\n') {
                draw_end = scan;
                next_cursor = frontier_is_forced_newline(scan) ? (scan + 3) : (scan + 1);
                break;
            }

            codepoint = frontier_utf8_next_codepoint(&scan);
            if (codepoint == 0U) {
                draw_end = scan;
                next_cursor = scan;
                break;
            }

            if (codepoint == (uint32_t)' ') {
                last_space_start = char_start;
                last_space_end = scan;
            }

            ++glyph_count;
            if (effective_max_chars > 0 && glyph_count > effective_max_chars) {
                overflow_start = char_start;
                break;
            }

            draw_end = scan;
            next_cursor = scan;
        }

        if (overflow_start != NULL) {
            if (last_space_end != NULL && last_space_start != NULL && last_space_start > line_start) {
                draw_end = last_space_start;
                next_cursor = last_space_end;
            } else {
                draw_end = overflow_start;
                next_cursor = overflow_start;
            }
        } else if (next_cursor == NULL) {
            next_cursor = scan;
        }

        frontier_draw_bitmap_text_span(
            line_start,
            draw_end,
            x,
            y + (line_height * (float)line_index),
            pixel_width,
            pixel_height,
            color
        );

        cursor = frontier_skip_spaces(next_cursor);
        ++line_index;
    }
}

static void frontier_draw_text_short(const char *text, float x, float y, float scale, FrontierColor color)
{
    if (text == NULL || text[0] == '\0') {
        return;
    }

    if (frontier_dataset_is_korean() && !frontier_text_is_ascii(text)) {
        float pixel_height = frontier_bitmap_pixel_height_for_scale(scale);
        float pixel_width = frontier_bitmap_pixel_width_for_height(pixel_height);
        float line_height = pixel_height * (float)FRONTIER_BITMAP_FONT_LINE_ADVANCE * 1.10f;
        frontier_draw_bitmap_text_internal(text, x, y, pixel_width, pixel_height, line_height, 36, 1, color);
        return;
    }

    frontier_draw_native_text_short(text, x, y, scale, color);
}

static const char *frontier_next_wrapped_line(const char *text, int max_chars, char *buffer, size_t buffer_size)
{
    const char *start;
    const char *cursor;
    const char *line_end;
    const char *last_space;
    size_t length;

    if (buffer == NULL || buffer_size == 0U) {
        return text;
    }
    buffer[0] = '\0';

    if (text == NULL) {
        return NULL;
    }

    while (*text == ' ') {
        ++text;
    }
    if (*text == '\0') {
        return text;
    }
    if (frontier_is_forced_newline(text)) {
        return text + 3;
    }
    if (*text == '\n') {
        return text + 1;
    }

    start = text;
    cursor = text;
    line_end = text;
    last_space = NULL;

    while (*cursor != '\0' && !frontier_is_forced_newline(cursor) && *cursor != '\n') {
        if ((int)(cursor - start) >= max_chars) {
            break;
        }
        if (*cursor == ' ') {
            last_space = cursor;
        }
        line_end = cursor + 1;
        ++cursor;
    }

    if (*cursor != '\0' && *cursor != '\n' && last_space != NULL && last_space > start) {
        line_end = last_space;
    }

    length = (size_t)(line_end - start);
    while (length > 0U && start[length - 1U] == ' ') {
        --length;
    }

    if (length >= buffer_size) {
        length = buffer_size - 1U;
    }
    if (length > 0U) {
        memcpy(buffer, start, length);
    }
    buffer[length] = '\0';

    cursor = line_end;
    while (*cursor == ' ') {
        ++cursor;
    }
    if (frontier_is_forced_newline(cursor)) {
        cursor += 3;
    }
    if (*cursor == '\n') {
        ++cursor;
    }
    return cursor;
}

static int frontier_count_gxt_lines(const char *text, int max_lines)
{
    int count = 1;
    const char *cursor = text;

    if (text == NULL || text[0] == '\0') {
        return 0;
    }

    while ((cursor = strstr(cursor, "~n~")) != NULL) {
        ++count;
        cursor += 3;
        if (count >= max_lines) {
            return max_lines;
        }
    }

    return count;
}

static void frontier_draw_wrapped_text(
    const char *text,
    float x,
    float y,
    float scale,
    float line_height,
    int max_chars,
    int max_lines,
    FrontierColor color
)
{
    int line_index = 0;
    char line_buffer[128];
    const char *cursor;

    if (text == NULL) {
        return;
    }
    cursor = frontier_resolve_text(text);

    if (frontier_dataset_is_korean() && !frontier_text_is_ascii(text)) {
        float effective_line_height = line_height * 1.10f;
        float pixel_height = effective_line_height / (float)FRONTIER_BITMAP_FONT_LINE_ADVANCE;
        float pixel_width = frontier_bitmap_pixel_width_for_height(pixel_height);
        frontier_draw_bitmap_text_internal(
            text,
            x,
            y,
            pixel_width,
            pixel_height,
            effective_line_height,
            max_chars,
            max_lines,
            color
        );
        return;
    }

    while (cursor != NULL && *cursor != '\0' && line_index < max_lines) {
        cursor = frontier_next_wrapped_line(cursor, max_chars, line_buffer, sizeof(line_buffer));
        if (line_buffer[0] != '\0') {
            frontier_draw_text_short(line_buffer, x, y + (line_height * (float)line_index), scale, color);
            ++line_index;
        } else if (cursor != NULL && *cursor != '\0') {
            ++line_index;
        }
    }
}

static int frontier_min_int(int a, int b)
{
    return (a < b) ? a : b;
}

static int frontier_max_int(int a, int b)
{
    return (a > b) ? a : b;
}

static float frontier_abs_float(float value)
{
    return (value < 0.0f) ? -value : value;
}

static int frontier_clamp_int(int value, int min_value, int max_value)
{
    return frontier_min_int(frontier_max_int(value, min_value), max_value);
}

static float frontier_clamp_float(float value, float min_value, float max_value)
{
    if (value != value) {
        return min_value;
    }
    if (value < min_value) {
        return min_value;
    }
    if (value > max_value) {
        return max_value;
    }
    return value;
}

static float frontier_distance_2d_approx(float dx, float dy)
{
    float ax = frontier_abs_float(dx);
    float ay = frontier_abs_float(dy);
    float major = ax > ay ? ax : ay;
    float minor = ax > ay ? ay : ax;
    return major + (minor * 0.375f);
}

static int frontier_count_region_bits(int mask)
{
    int count = 0;

    while (mask != 0) {
        count += (mask & 1);
        mask >>= 1;
    }

    return count;
}

static int *frontier_live_stat_bonus_slot(int entry_index)
{
    switch (entry_index) {
    case 0: return &g_runtime.live_profile.stat_bonus_strength;
    case 1: return &g_runtime.live_profile.stat_bonus_grit;
    case 2: return &g_runtime.live_profile.stat_bonus_deadeye;
    case 3: return &g_runtime.live_profile.stat_bonus_survival;
    default: return &g_runtime.live_profile.stat_bonus_cunning;
    }
}

static int *frontier_live_skill_bonus_slot(int entry_index)
{
    switch (entry_index) {
    case 0: return &g_runtime.live_profile.skill_bonus_hunting;
    case 1: return &g_runtime.live_profile.skill_bonus_gunsmithing;
    case 2: return &g_runtime.live_profile.skill_bonus_field_medicine;
    case 3: return &g_runtime.live_profile.skill_bonus_trapping;
    case 4: return &g_runtime.live_profile.skill_bonus_salvaging;
    case 5: return &g_runtime.live_profile.skill_bonus_trade;
    case 6: return &g_runtime.live_profile.skill_bonus_horse_handling;
    default: return &g_runtime.live_profile.skill_bonus_homestead;
    }
}

static int frontier_live_stat_points_total(void)
{
    int total =
        (g_runtime.live_profile.enemy_kills +
         g_runtime.live_profile.animal_kills +
         g_runtime.live_profile.loot_count +
         (g_runtime.live_profile.hub_returns * 2) +
         g_runtime.live_profile.discovered_region_count +
         g_runtime.live_profile.trail_stage) / 4;
    return frontier_clamp_int(total, 0, 12);
}

static int frontier_live_stat_points_spent(void)
{
    return
        g_runtime.live_profile.stat_bonus_strength +
        g_runtime.live_profile.stat_bonus_grit +
        g_runtime.live_profile.stat_bonus_deadeye +
        g_runtime.live_profile.stat_bonus_survival +
        g_runtime.live_profile.stat_bonus_cunning;
}

static int frontier_live_stat_points_available(void)
{
    return frontier_max_int(0, frontier_live_stat_points_total() - frontier_live_stat_points_spent());
}

static int frontier_live_skill_points_total(void)
{
    int total =
        (g_runtime.live_profile.hub_returns +
         g_runtime.live_profile.discovered_region_count +
         g_runtime.live_profile.contract_count +
         g_runtime.live_profile.workshop_tier +
         g_runtime.live_profile.armory_tier +
         g_runtime.live_profile.medicine_tier) / 4;
    return frontier_clamp_int(total, 0, 12);
}

static int frontier_live_skill_points_spent(void)
{
    return
        g_runtime.live_profile.skill_bonus_hunting +
        g_runtime.live_profile.skill_bonus_gunsmithing +
        g_runtime.live_profile.skill_bonus_field_medicine +
        g_runtime.live_profile.skill_bonus_trapping +
        g_runtime.live_profile.skill_bonus_salvaging +
        g_runtime.live_profile.skill_bonus_trade +
        g_runtime.live_profile.skill_bonus_horse_handling +
        g_runtime.live_profile.skill_bonus_homestead;
}

static int frontier_live_skill_points_available(void)
{
    return frontier_max_int(0, frontier_live_skill_points_total() - frontier_live_skill_points_spent());
}

static void frontier_sanitize_growth_allocations(void)
{
    if (frontier_live_stat_points_spent() > frontier_live_stat_points_total()) {
        g_runtime.live_profile.stat_bonus_strength = 0;
        g_runtime.live_profile.stat_bonus_grit = 0;
        g_runtime.live_profile.stat_bonus_deadeye = 0;
        g_runtime.live_profile.stat_bonus_survival = 0;
        g_runtime.live_profile.stat_bonus_cunning = 0;
        g_runtime.live_profile.dirty = 1;
    }

    if (frontier_live_skill_points_spent() > frontier_live_skill_points_total()) {
        g_runtime.live_profile.skill_bonus_hunting = 0;
        g_runtime.live_profile.skill_bonus_gunsmithing = 0;
        g_runtime.live_profile.skill_bonus_field_medicine = 0;
        g_runtime.live_profile.skill_bonus_trapping = 0;
        g_runtime.live_profile.skill_bonus_salvaging = 0;
        g_runtime.live_profile.skill_bonus_trade = 0;
        g_runtime.live_profile.skill_bonus_horse_handling = 0;
        g_runtime.live_profile.skill_bonus_homestead = 0;
        g_runtime.live_profile.dirty = 1;
    }
}

static int frontier_page_count(void)
{
    if (g_runtime.dataset == NULL || g_runtime.dataset->page_count <= 0) {
        return 0;
    }
    return frontier_min_int(g_runtime.dataset->page_count, FRONTIER_MAX_PAGES);
}

static int frontier_page_entry_capacity(void)
{
    return frontier_dataset_is_korean() ? 7 : 10;
}

static const char *frontier_runtime_context_label(void)
{
    return g_runtime.context.valid ? g_runtime.context.region_label : frontier_text("Unknown Frontier", "미확인 프론티어");
}

static const char *frontier_runtime_route_hint(void)
{
    return g_runtime.context.valid ? g_runtime.context.route_hint : frontier_text(
        "The frontier context will update once player coordinates are available.",
        "플레이어 좌표를 읽으면 프론티어 문맥이 갱신됩니다."
    );
}

static int frontier_runtime_uses_live_dataset(void)
{
    return strcmp(g_runtime.requested_dataset_id, "live_en") == 0 || strcmp(g_runtime.requested_dataset_id, "live_ko") == 0;
}

static const char *frontier_live_phase_dataset_id(void)
{
    const char *suffix = frontier_dataset_is_korean() ? "_ko" : "_en";
    int korean_requested = strcmp(g_runtime.requested_dataset_id, "live_ko") == 0;

    if (korean_requested) {
        suffix = "_ko";
    } else if (strcmp(g_runtime.requested_dataset_id, "live_en") == 0) {
        suffix = "_en";
    }

    if (g_runtime.live_profile.phase >= FRONTIER_PHASE_ENDGAME) {
        return (suffix[1] == 'k') ? "endgame_ko" : "endgame_en";
    }
    if (g_runtime.live_profile.phase >= FRONTIER_PHASE_ESCALATION) {
        return (suffix[1] == 'k') ? "simulated_ko" : "simulated_en";
    }
    return (suffix[1] == 'k') ? "baseline_ko" : "baseline_en";
}

static const char *frontier_runtime_dataset_label(void)
{
    if (frontier_runtime_uses_live_dataset()) {
        return frontier_text("Live Run", "실시간 런");
    }
    return (g_runtime.dataset != NULL && g_runtime.dataset->display_name != NULL) ? g_runtime.dataset->display_name : "";
}

static const char *frontier_main_title_display(void)
{
    if (frontier_dataset_is_korean()) {
        return "프런티어 레저";
    }
    return "Frontier Ledger";
}

static const char *frontier_footer_controls_display(void)
{
    return frontier_text(
        "Left/Right: tab   Up/Down: row   Esc: close",
        "좌우: 페이지   상하: 항목   Esc: 닫기"
    );
}

static const char *frontier_page_title_display(const FrontierLedgerPage *page)
{
    if (page == NULL || page->title == NULL) {
        return "";
    }
    if (!frontier_dataset_is_korean()) {
        return page->title;
    }

    if (strcmp(page->page_id, "overview") == 0) return "개요";
    if (strcmp(page->page_id, "trails") == 0) return "추적";
    if (strcmp(page->page_id, "frontier_map") == 0) return "지도";
    if (strcmp(page->page_id, "homefront") == 0) return "거점";
    if (strcmp(page->page_id, "stats") == 0) return "스탯";
    if (strcmp(page->page_id, "skills") == 0) return "스킬";
    if (strcmp(page->page_id, "perks") == 0) return "퍽";
    if (strcmp(page->page_id, "crafting") == 0) return "제작";
    if (strcmp(page->page_id, "armory") == 0) return "병기";
    if (strcmp(page->page_id, "dead") == 0) return "보스";
    if (strcmp(page->page_id, "collection") == 0) return "수집";
    if (strcmp(page->page_id, "gear") == 0) return "장비";
    if (strcmp(page->page_id, "farming") == 0) return "농장";
    if (strcmp(page->page_id, "network") == 0) return "인맥";
    if (strcmp(page->page_id, "world") == 0) return "월드";
    if (strcmp(page->page_id, "ending") == 0) return "엔딩";
    if (strcmp(page->page_id, "encyclopedia") == 0) return "백과";
    return page->title;
}

static int frontier_korean_compact_chars(int english_chars)
{
    if (!frontier_dataset_is_korean()) {
        return english_chars;
    }
    return frontier_max_int(14, (english_chars * 3) / 5);
}

static const char *frontier_live_trail_stage_compact_label(int stage)
{
    switch (stage) {
    case 1:
        return frontier_text("Field Sweep", "현장 수색");
    case 2:
        return frontier_text("First Report", "첫 보고");
    case 3:
        return frontier_text("Plains Pressure", "평원 압박");
    case 4:
        return frontier_text("Blackwater", "블랙워터");
    case 5:
        return frontier_text("Silas Prep", "실라스 준비");
    default:
        return frontier_text("Briefing", "브리핑");
    }
}

static const char *frontier_live_trail_stage_label(int stage)
{
    switch (stage) {
    case 1:
        return frontier_text("Stage 1: First Field Sweep", "1단계: 첫 현장 수색");
    case 2:
        return frontier_text("Stage 2: First Report Returned", "2단계: 첫 보고 귀환");
    case 3:
        return frontier_text("Stage 3: Great Plains Pressure", "3단계: 그레이트플레인 압박");
    case 4:
        return frontier_text("Stage 4: Blackwater Contract Ready", "4단계: 블랙워터 계약 준비");
    case 5:
        return frontier_text("Stage 5: Silas Redd Prep", "5단계: 사일러스 레드 준비");
    default:
        return frontier_text("Stage 0: Ranch Briefing", "0단계: 목장 브리핑");
    }
}

static const char *frontier_family_rank_label(int rank)
{
    switch (rank) {
    case 1:
        return frontier_text("Active", "활성");
    case 2:
        return frontier_text("Steady", "안정");
    case 3:
        return frontier_text("Anchored", "핵심");
    default:
        return frontier_text("Dormant", "대기");
    }
}

static const char *frontier_farming_lane_label(int lane)
{
    switch (lane) {
    case 1:
        return frontier_text("Hunter Lane", "사냥 루트");
    case 2:
        return frontier_text("Raider Lane", "습격 루트");
    case 3:
        return frontier_text("Contract Lane", "계약 루트");
    case 4:
        return frontier_text("Frontier Work Lane", "거점 작업 루트");
    default:
        return frontier_text("Boss Lane", "보스 루트");
    }
}

static const char *frontier_story_clue_label(int clue_count)
{
    if (clue_count >= 9) {
        return frontier_text("Surveyor Locked", "서베이어 고정");
    }
    if (clue_count >= 6) {
        return frontier_text("Surveyor Tracked", "서베이어 추적");
    }
    if (clue_count >= 3) {
        return frontier_text("Surveyor Rumors", "서베이어 소문");
    }
    return frontier_text("Courier Clues", "전령 단서");
}

static const char *frontier_sky_devil_label(int stage)
{
    switch (stage) {
    case 1:
        return frontier_text("Ridge Rumor", "능선 소문");
    case 2:
        return frontier_text("Sky Devil Sign", "하늘 악마 징후");
    case 3:
        return frontier_text("Myth Break", "괴담 개방");
    default:
        return frontier_text("No Sign", "징후 없음");
    }
}

static int frontier_live_activity_points(void)
{
    int reward_points = g_runtime.live_profile.reward_earned / 15;
    return
        (g_runtime.live_profile.enemy_kills * 2) +
        g_runtime.live_profile.loot_count +
        g_runtime.live_profile.animal_kills +
        reward_points;
}

static int frontier_live_perk_unlocked(uint32_t flag)
{
    return (g_runtime.live_profile.perk_flags & flag) != 0U;
}
