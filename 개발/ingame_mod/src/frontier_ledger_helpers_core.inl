static uint32_t frontier_model_hash_override_for_name(const char *name)
{
    if (name == NULL) {
        return 0U;
    }
    if (_stricmp(name, "a_m_m_bivroughtravellers_01") == 0) return 0x4ADABFBAU;
    if (_stricmp(name, "a_m_m_blwforeman_01") == 0) return 0x5B91D27EU;
    if (_stricmp(name, "a_m_m_blwobesemen_01") == 0) return 0x41907533U;
    if (_stricmp(name, "g_m_m_unicriminals_01") == 0) return 0x3F094007U;
    if (_stricmp(name, "g_m_m_unimountainmen_01") == 0) return 0xAB3EBD92U;
    if (_stricmp(name, "g_m_m_uniranchers_01") == 0) return 0xBF03A565U;
    if (_stricmp(name, "g_m_m_unibanditos_01") == 0) return 0x19486791U;
    if (_stricmp(name, "g_m_o_uniexconfeds_01") == 0) return 0xEAD13D7CU;
    if (_stricmp(name, "cs_johnmarston") == 0) return 0x05B6D06DU;
    return 0U;
}

typedef struct FrontierSpawnProfile {
    const char *label;
    const char *label_ko;
    const char *const *model_candidates;
    int model_candidate_count;
    const char *weapon_name;
    int health;
    int accuracy;
    int combat_ability;
    int combat_range;
    int combat_movement;
    int count;
    float first_offset_x;
    float first_offset_y;
    float spacing_x;
    float spacing_y;
} FrontierSpawnProfile;

static const char *const kSpawnModels_Blackwater[] = {
    "G_M_M_UniCriminals_01",
    "G_M_M_UniRanchers_01",
};
static const char *const kSpawnModels_Trackers[] = {
    "G_M_M_UniMountainMen_01",
    "G_M_M_UniRanchers_01",
    "G_M_M_UniCriminals_01",
};
static const char *const kSpawnModels_Enforcers[] = {
    "G_M_M_UniBanditos_01",
    "G_M_M_UniCriminals_01",
    "G_M_M_UniRanchers_01",
};
static const char *const kSpawnModels_Relic[] = {
    "G_M_O_UniExConfeds_01",
    "G_M_M_UniCriminals_01",
    "G_M_M_UniMountainMen_01",
};
static const char *const kSpawnModels_Surveyor[] = {
    "G_M_M_UniInbred_01",
    "G_M_O_UniExConfeds_01",
    "G_M_M_UniMountainMen_01",
};

static const FrontierSpawnProfile kFrontierRegionSpawnProfiles[] = {
    {
        "Blackwater Mercenary Patrol",
        "블랙워터 용병 순찰대",
        kSpawnModels_Blackwater,
        (int)(sizeof(kSpawnModels_Blackwater) / sizeof(kSpawnModels_Blackwater[0])),
        "WEAPON_REPEATER_CARBINE",
        110,
        55,
        1,
        1,
        2,
        2,
        -2.0f,
        9.0f,
        2.6f,
        -1.4f,
    },
    {
        "Tall Trees Tracker Pack",
        "톨트리스 추적조",
        kSpawnModels_Trackers,
        (int)(sizeof(kSpawnModels_Trackers) / sizeof(kSpawnModels_Trackers[0])),
        "WEAPON_BOW",
        125,
        48,
        1,
        0,
        2,
        3,
        -3.5f,
        10.0f,
        3.0f,
        -1.0f,
    },
    {
        "New Austin Enforcer Sweep",
        "뉴오스틴 집행조",
        kSpawnModels_Enforcers,
        (int)(sizeof(kSpawnModels_Enforcers) / sizeof(kSpawnModels_Enforcers[0])),
        "WEAPON_SHOTGUN_DOUBLEBARREL",
        150,
        44,
        1,
        0,
        2,
        2,
        -2.0f,
        8.0f,
        3.0f,
        -1.0f,
    },
    {
        "New Hanover Veteran Lane",
        "뉴하노버 베테랑 라인",
        kSpawnModels_Blackwater,
        (int)(sizeof(kSpawnModels_Blackwater) / sizeof(kSpawnModels_Blackwater[0])),
        "WEAPON_RIFLE_BOLTACTION",
        118,
        62,
        2,
        2,
        1,
        2,
        -2.5f,
        10.0f,
        3.2f,
        -1.2f,
    },
    {
        "Lemoyne Relic Pressure",
        "르모인 유물 압박조",
        kSpawnModels_Relic,
        (int)(sizeof(kSpawnModels_Relic) / sizeof(kSpawnModels_Relic[0])),
        "WEAPON_REPEATER_WINCHESTER",
        135,
        58,
        2,
        1,
        2,
        2,
        -2.0f,
        9.5f,
        2.8f,
        -1.0f,
    },
    {
        "Surveyor Acolyte Sweep",
        "측량자 추종자 조",
        kSpawnModels_Surveyor,
        (int)(sizeof(kSpawnModels_Surveyor) / sizeof(kSpawnModels_Surveyor[0])),
        "WEAPON_RIFLE_SPRINGFIELD",
        165,
        68,
        2,
        2,
        2,
        2,
        -2.0f,
        11.0f,
        3.4f,
        -1.2f,
    },
};

static const FrontierSpawnProfile kFrontierCourierSpawnProfile = {
    "Blackwater Courier Reclaimers",
    "Blackwater Courier Reclaimers",
    kSpawnModels_Blackwater,
    (int)(sizeof(kSpawnModels_Blackwater) / sizeof(kSpawnModels_Blackwater[0])),
    "WEAPON_REVOLVER_CATTLEMAN",
    120,
    60,
    1,
    1,
    2,
    2,
    -2.0f,
    8.5f,
    2.8f,
    -1.2f,
};

static const FrontierSpawnProfile kFrontierRuskSpawnProfile = {
    "Rusk, The Collector's Dog",
    "Rusk, The Collector's Dog",
    kSpawnModels_Blackwater,
    (int)(sizeof(kSpawnModels_Blackwater) / sizeof(kSpawnModels_Blackwater[0])),
    "WEAPON_REPEATER_CARBINE",
    190,
    70,
    2,
    1,
    2,
    1,
    -2.0f,
    9.5f,
    0.0f,
    0.0f,
};

static const FrontierSpawnProfile kFrontierSilasPosseSpawnProfile = {
    "Silas Redd Posse Sweep",
    "Silas Redd Posse Sweep",
    kSpawnModels_Blackwater,
    (int)(sizeof(kSpawnModels_Blackwater) / sizeof(kSpawnModels_Blackwater[0])),
    "WEAPON_REPEATER_CARBINE",
    165,
    68,
    2,
    2,
    2,
    3,
    -3.0f,
    10.0f,
    3.0f,
    -1.0f,
};

static const FrontierSpawnProfile kFrontierBossSpawnProfiles[] = {
    {
        "Silas Redd Showdown",
        "사일러스 레드 대결",
        kSpawnModels_Blackwater,
        (int)(sizeof(kSpawnModels_Blackwater) / sizeof(kSpawnModels_Blackwater[0])),
        "WEAPON_REPEATER_CARBINE",
        260,
        72,
        2,
        2,
        2,
        3,
        -3.0f,
        12.0f,
        3.0f,
        -1.0f,
    },
    {
        "Blackwater Broker Cell",
        "블랙워터 브로커 셀",
        kSpawnModels_Blackwater,
        (int)(sizeof(kSpawnModels_Blackwater) / sizeof(kSpawnModels_Blackwater[0])),
        "WEAPON_PISTOL_MAUSER",
        220,
        76,
        2,
        2,
        1,
        3,
        -3.0f,
        11.0f,
        2.8f,
        -1.0f,
    },
    {
        "Tall Trees Tracker King",
        "톨트리스 추적왕",
        kSpawnModels_Trackers,
        (int)(sizeof(kSpawnModels_Trackers) / sizeof(kSpawnModels_Trackers[0])),
        "WEAPON_BOW",
        240,
        58,
        2,
        1,
        2,
        3,
        -3.0f,
        11.5f,
        3.1f,
        -1.0f,
    },
    {
        "Lemoyne Relic Keeper",
        "르모인 유물 수호자",
        kSpawnModels_Relic,
        (int)(sizeof(kSpawnModels_Relic) / sizeof(kSpawnModels_Relic[0])),
        "WEAPON_SHOTGUN_SEMIAUTO",
        250,
        64,
        2,
        1,
        2,
        3,
        -3.0f,
        11.0f,
        3.0f,
        -1.0f,
    },
    {
        "The Surveyor",
        "측량자",
        kSpawnModels_Surveyor,
        (int)(sizeof(kSpawnModels_Surveyor) / sizeof(kSpawnModels_Surveyor[0])),
        "WEAPON_RIFLE_BOLTACTION",
        320,
        80,
        2,
        2,
        2,
        4,
        -4.0f,
        13.0f,
        3.2f,
        -1.0f,
    },
};

static void FrontierScriptMain(void);
static void FrontierKeyboardMessage(
    DWORD key,
    WORD repeats,
    BYTE scan_code,
    BOOL is_extended,
    BOOL with_alt,
    BOOL was_down_before,
    BOOL is_up_now
);
static LRESULT CALLBACK FrontierOverlayWindowProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param);
static uint32_t frontier_get_hash_key(const char *text);
static const FrontierGeneratedTextEntry *frontier_find_generated_text(const char *label);
static const char *frontier_lookup_generated_text(const char *label);
static const char *frontier_resolve_text(const char *text);
static const FrontierBitmapGlyph *frontier_find_bitmap_glyph(uint32_t codepoint);
static uint32_t frontier_utf8_next_codepoint(const char **cursor);
static void frontier_refresh_zone_state(int force_refresh);
static void frontier_clamp_selection(int page_index);
static const FrontierLedgerPage *frontier_current_page(void);
static void frontier_select_dataset(const char *dataset_id);
static int frontier_find_page_index(const char *page_id);
static void frontier_refresh_live_profile(int force_refresh);
static void frontier_live_profile_save(int force_now);
static void frontier_session_save(void);
static void frontier_live_profile_refresh_activity(int force_refresh);
static void frontier_apply_live_gameplay_effects(void);
static void frontier_set_status(const char *message);
static int frontier_max_int(int a, int b);
static int frontier_clamp_int(int value, int min_value, int max_value);
static int frontier_inside_workbench_zone(void);
static void frontier_render_workbench_prompt(void);
static void frontier_render_workbench_overlay(void);
static void frontier_open_workbench_overlay(void);
static void frontier_close_workbench_overlay(int save_profile);
static void frontier_change_workbench_tab(int delta);
static void frontier_change_workbench_entry(int delta);
static void frontier_confirm_workbench_action(void);
static void frontier_confirm_full_ledger_action(void);
static const char *frontier_workbench_recipe_effect_compact(int entry_index);
static const char *frontier_workbench_recipe_effect_ascii(int entry_index);
static int frontier_live_recipe_ready(int entry_index);
static int frontier_live_crafted_recipe_count(void);
static int frontier_try_craft_recipe_entry(int entry_index, int update_workbench_result, const char *source_text);
static FrontierTrackedPed *frontier_live_find_tracked_ped(int entity, int create_slot, DWORD now);
static int frontier_live_stat_points_total(void);
static int frontier_live_stat_points_spent(void);
static int frontier_live_stat_points_available(void);
static int frontier_live_skill_points_total(void);
static int frontier_live_skill_points_spent(void);
static int frontier_live_skill_points_available(void);
static int frontier_crafted_workshop_bonus(void);
static int frontier_crafted_armory_bonus(void);
static int frontier_crafted_stamina_bonus_percent(void);
static int frontier_crafted_stamina_drain_bonus_percent(void);
static int frontier_crafted_weapon_bonus_percent(void);
static int frontier_crafted_deadeye_bonus_percent(void);
static const char *frontier_runtime_context_label(void);
static void frontier_try_spawn_debug_path(void);
static int frontier_spawn_region_encounter(void);
static int frontier_spawn_selected_boss(void);
static int frontier_spawn_selected_entry_encounter(void);

static void frontier_copy_string(char *destination, size_t destination_size, const char *source)
{
    if (destination == NULL || destination_size == 0U) {
        return;
    }
    destination[0] = '\0';
    if (source == NULL) {
        return;
    }
#if defined(_MSC_VER)
    strncpy_s(destination, destination_size, source, _TRUNCATE);
#else
    strncpy(destination, source, destination_size - 1U);
    destination[destination_size - 1U] = '\0';
#endif
}

static void frontier_build_neighbor_path(const char *module_path, const char *file_name, char *destination, size_t destination_size)
{
    char buffer[MAX_PATH];
    char *separator;
    size_t length;

    frontier_copy_string(buffer, sizeof(buffer), module_path);
    separator = strrchr(buffer, '\\');
    if (separator == NULL) {
        separator = strrchr(buffer, '/');
    }
    if (separator != NULL) {
        *(separator + 1) = '\0';
    } else {
        frontier_copy_string(buffer, sizeof(buffer), ".\\");
    }

    length = strlen(buffer);
    if (destination == NULL || destination_size == 0U) {
        return;
    }
    frontier_copy_string(destination, destination_size, buffer);
    if (strlen(destination) + strlen(file_name) + 1U < destination_size) {
        frontier_copy_string(destination + length, destination_size - length, file_name);
    }
}

static void frontier_log(const char *format, ...)
{
    FILE *handle;
    va_list args;
    char message[512];

    if (g_runtime.log_path[0] == '\0') {
        return;
    }

    handle = fopen(g_runtime.log_path, "a");
    if (handle == NULL) {
        return;
    }

    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    fprintf(handle, "%s\n", message);
    fclose(handle);
}

static int frontier_dataset_is_korean(void)
{
    const char *dataset_id;
    size_t length;

    if (g_runtime.dataset == NULL || g_runtime.dataset->dataset_id == NULL) {
        return 0;
    }

    dataset_id = g_runtime.dataset->dataset_id;
    length = strlen(dataset_id);
    return length >= 3U && strcmp(dataset_id + length - 3U, "_ko") == 0;
}

static int frontier_preferred_locale_is_korean(void)
{
    return _stricmp(g_runtime.config.preferred_locale, "ko") == 0 ||
        _stricmp(g_runtime.config.preferred_locale, "kr") == 0;
}

static const char *frontier_dataset_id_for_preferred_locale(const char *dataset_id)
{
    if (dataset_id == NULL || dataset_id[0] == '\0') {
        return dataset_id;
    }

    if (strcmp(dataset_id, "live_en") == 0 || strcmp(dataset_id, "live_ko") == 0) {
        return frontier_preferred_locale_is_korean() ? "live_ko" : "live_en";
    }
    if (strcmp(dataset_id, "baseline_en") == 0 || strcmp(dataset_id, "baseline_ko") == 0) {
        return frontier_preferred_locale_is_korean() ? "baseline_ko" : "baseline_en";
    }
    if (strcmp(dataset_id, "simulated_en") == 0 || strcmp(dataset_id, "simulated_ko") == 0) {
        return frontier_preferred_locale_is_korean() ? "simulated_ko" : "simulated_en";
    }
    if (strcmp(dataset_id, "endgame_en") == 0 || strcmp(dataset_id, "endgame_ko") == 0) {
        return frontier_preferred_locale_is_korean() ? "endgame_ko" : "endgame_en";
    }

    return dataset_id;
}

static int frontier_use_desktop_overlay(void)
{
    return g_runtime.config.use_desktop_overlay;
}

static const char *frontier_text(const char *english, const char *korean)
{
    return frontier_dataset_is_korean() ? korean : english;
}

static const char *frontier_ui_text(const char *english, const char *korean_label)
{
    return frontier_dataset_is_korean() ? korean_label : english;
}

static void frontier_mark_ui_dirty(void)
{
    ++g_runtime.ui_generation;
}

static float frontier_unpack_float_slot(uint64_t raw)
{
    float value = 0.0f;
    uint32_t bits = (uint32_t)(raw & 0xFFFFFFFFULL);
    memcpy(&value, &bits, sizeof(value));
    return value;
}

static const char *frontier_prepare_display_text(const char *text)
{
    return text;
}

static uint64_t *frontier_invoke(uint64_t hash, int arg_count, ...)
{
    int index;
    uint64_t args[16];
    va_list values;

    if (arg_count < 0 || arg_count > (int)(sizeof(args) / sizeof(args[0]))) {
        return NULL;
    }

    va_start(values, arg_count);
    for (index = 0; index < arg_count; ++index) {
        args[index] = va_arg(values, uint64_t);
    }
    va_end(values);

    return FrontierNative_InvokeRaw(hash, args, arg_count);
}

static int frontier_native_int(uint64_t hash, int arg_count, ...)
{
    int index;
    uint64_t args[16];
    uint64_t *result;
    va_list values;

    if (arg_count < 0 || arg_count > (int)(sizeof(args) / sizeof(args[0]))) {
        return 0;
    }

    va_start(values, arg_count);
    for (index = 0; index < arg_count; ++index) {
        args[index] = va_arg(values, uint64_t);
    }
    va_end(values);

    result = FrontierNative_InvokeRaw(hash, args, arg_count);
    if (result == NULL) {
        return 0;
    }
    return (int)(int32_t)(*result);
}

static int frontier_native_bool(uint64_t hash, int arg_count, ...)
{
    int index;
    uint64_t args[16];
    uint64_t *result;
    va_list values;

    if (arg_count < 0 || arg_count > (int)(sizeof(args) / sizeof(args[0]))) {
        return 0;
    }

    va_start(values, arg_count);
    for (index = 0; index < arg_count; ++index) {
        args[index] = va_arg(values, uint64_t);
    }
    va_end(values);

    result = FrontierNative_InvokeRaw(hash, args, arg_count);
    if (result == NULL) {
        return 0;
    }
    return (*result) != 0ULL;
}

static float frontier_native_float(uint64_t hash, int arg_count, ...)
{
    int index;
    uint64_t args[16];
    uint64_t *result;
    va_list values;

    if (arg_count < 0 || arg_count > (int)(sizeof(args) / sizeof(args[0]))) {
        return 0.0f;
    }

    va_start(values, arg_count);
    for (index = 0; index < arg_count; ++index) {
        args[index] = va_arg(values, uint64_t);
    }
    va_end(values);

    result = FrontierNative_InvokeRaw(hash, args, arg_count);
    if (result == NULL) {
        return 0.0f;
    }
    return frontier_unpack_float_slot(*result);
}

static const char *frontier_native_string(uint64_t hash, int arg_count, ...)
{
    int index;
    uint64_t args[16];
    uint64_t *result;
    va_list values;

    if (arg_count < 0 || arg_count > (int)(sizeof(args) / sizeof(args[0]))) {
        return NULL;
    }

    va_start(values, arg_count);
    for (index = 0; index < arg_count; ++index) {
        args[index] = va_arg(values, uint64_t);
    }
    va_end(values);

    result = FrontierNative_InvokeRaw(hash, args, arg_count);
    if (result == NULL || *result == 0ULL) {
        return NULL;
    }
    return (const char *)(uintptr_t)(*result);
}

static FrontierVector3 frontier_native_vector3(uint64_t hash, int arg_count, ...)
{
    int index;
    uint64_t args[16];
    uint64_t *result;
    va_list values;
    FrontierVector3 vector;

    vector.x = 0.0f;
    vector.y = 0.0f;
    vector.z = 0.0f;

    if (arg_count < 0 || arg_count > (int)(sizeof(args) / sizeof(args[0]))) {
        return vector;
    }

    va_start(values, arg_count);
    for (index = 0; index < arg_count; ++index) {
        args[index] = va_arg(values, uint64_t);
    }
    va_end(values);

    result = FrontierNative_InvokeRaw(hash, args, arg_count);
    if (result != NULL) {
        vector.x = frontier_unpack_float_slot(result[0]);
        vector.y = frontier_unpack_float_slot(result[1]);
        vector.z = frontier_unpack_float_slot(result[2]);
    }
    return vector;
}

static int frontier_game_timer(void)
{
    return frontier_native_int(HASH_GET_GAME_TIMER, 0);
}

static int frontier_player_id(void)
{
    return frontier_native_int(HASH_PLAYER_ID, 0);
}

static int frontier_player_ped(void)
{
    return frontier_native_int(HASH_PLAYER_PED_ID, 0);
}

static int frontier_get_cash_balance(void)
{
    int cash = frontier_native_int(HASH_MONEY_GET_CASH_BALANCE, 0);
    if (cash <= 0) {
        int ped = frontier_player_ped();
        if (ped != 0) {
            cash = frontier_native_int(HASH_GET_PED_MONEY, 1, FrontierPackInt(ped));
        }
    }
    return cash;
}

static int frontier_get_ped_nearby_peds(int ped, int *out_peds, int capacity)
{
    int raw[(FRONTIER_LIVE_NEARBY_PED_SCAN_MAX * 2) + 2];
    int index;
    int count;

    if (ped == 0 || out_peds == NULL || capacity <= 0) {
        return 0;
    }

    if (capacity > FRONTIER_LIVE_NEARBY_PED_SCAN_MAX) {
        capacity = FRONTIER_LIVE_NEARBY_PED_SCAN_MAX;
    }

    ZeroMemory(raw, sizeof(raw));
    raw[0] = capacity;
    count = frontier_native_int(
        HASH_GET_PED_NEARBY_PEDS,
        4,
        FrontierPackInt(ped),
        FrontierPackPointer(raw),
        FrontierPackInt(-1),
        FrontierPackInt(0)
    );
    if (count < 0) {
        count = 0;
    }
    if (count > capacity) {
        count = capacity;
    }

    for (index = 0; index < count; ++index) {
        out_peds[index] = raw[(index * 2) + 2];
    }
    return count;
}

static int frontier_is_ped_dead_or_dying(int ped)
{
    return ped != 0 && frontier_native_bool(HASH_IS_PED_DEAD_OR_DYING, 2, FrontierPackInt(ped), FrontierPackBool(1));
}

static int frontier_is_ped_human(int ped)
{
    return ped != 0 && frontier_native_bool(HASH_IS_PED_HUMAN, 1, FrontierPackInt(ped));
}

static int frontier_is_ped_a_player(int ped)
{
    return ped != 0 && frontier_native_bool(HASH_IS_PED_A_PLAYER, 1, FrontierPackInt(ped));
}

static int frontier_is_trackable_enemy_ped(int ped)
{
    return ped != 0 && frontier_is_ped_human(ped) && !frontier_is_ped_a_player(ped);
}

static int frontier_is_trackable_animal_ped(int ped)
{
    return ped != 0 && !frontier_is_ped_human(ped) && !frontier_is_ped_a_player(ped);
}

static int frontier_get_ped_source_of_death(int ped)
{
    return ped != 0 ? frontier_native_int(HASH_GET_PED_SOURCE_OF_DEATH, 1, FrontierPackInt(ped)) : 0;
}

static int frontier_has_entity_been_damaged_by_entity(int entity, int other_entity)
{
    return entity != 0 &&
        other_entity != 0 &&
        frontier_native_bool(
            HASH_HAS_ENTITY_BEEN_DAMAGED_BY_ENTITY,
            4,
            FrontierPackInt(entity),
            FrontierPackInt(other_entity),
            FrontierPackBool(1),
            FrontierPackBool(1)
        );
}

static int frontier_get_entity_attached_to(int entity)
{
    return entity != 0 ? frontier_native_int(HASH_GET_ENTITY_ATTACHED_TO, 1, FrontierPackInt(entity)) : 0;
}

static int frontier_is_entity_fully_looted(int entity)
{
    return entity != 0 && frontier_native_bool(HASH_IS_ENTITY_FULLY_LOOTED, 1, FrontierPackInt(entity));
}

static int frontier_find_entity_looting_ped(int entity)
{
    return entity != 0 ? frontier_native_int(HASH_FIND_ENTITY_LOOTING_PED, 1, FrontierPackInt(entity)) : 0;
}

static void frontier_set_player_stamina_recharge_multiplier(int player_id, float multiplier)
{
    frontier_invoke(
        HASH_SET_PLAYER_STAMINA_RECHARGE_MULTIPLIER,
        2,
        FrontierPackInt(player_id),
        FrontierPackFloat(multiplier)
    );
}

static void frontier_set_player_stamina_depletion_multiplier(int player_id, float multiplier)
{
    frontier_invoke(
        HASH_SET_PLAYER_STAMINA_SPRINT_DEPLETION_MULTIPLIER,
        2,
        FrontierPackInt(player_id),
        FrontierPackFloat(multiplier)
    );
}

static void frontier_set_weapon_degradation_modifier(int player_id, float modifier)
{
    frontier_invoke(
        HASH_SET_WEAPON_DEGRADATION_MODIFIER,
        2,
        FrontierPackInt(player_id),
        FrontierPackFloat(modifier)
    );
}

static void frontier_set_delay_deadeye_depletion(int player_id, float delay)
{
    frontier_invoke(
        HASH_SET_DELAY_DEADEYE_DEPLETION,
        2,
        FrontierPackInt(player_id),
        FrontierPackFloat(delay)
    );
}

static void frontier_set_player_health_recharge_multiplier(int player_id, float multiplier)
{
    frontier_invoke(
        HASH_SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER,
        2,
        FrontierPackInt(player_id),
        FrontierPackFloat(multiplier)
    );
}

static void frontier_set_player_health_recharge_time_modifier(int player_id, float modifier)
{
    frontier_invoke(
        HASH_SET_PLAYER_HEALTH_RECHARGE_TIME_MODIFIER,
        2,
        FrontierPackInt(player_id),
        FrontierPackFloat(modifier)
    );
}

static void frontier_set_player_weapon_damage_modifier(int player_id, float modifier)
{
    frontier_invoke(
        HASH_SET_PLAYER_WEAPON_DAMAGE_MODIFIER,
        2,
        FrontierPackInt(player_id),
        FrontierPackFloat(modifier)
    );
}

static void frontier_set_player_melee_damage_modifier(int player_id, float modifier)
{
    frontier_invoke(
        HASH_SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER,
        2,
        FrontierPackInt(player_id),
        FrontierPackFloat(modifier)
    );
}

static void frontier_set_received_horseback_damage_modifier(int player_id, float modifier)
{
    frontier_invoke(
        HASH_SET_RECEIVED_DAMAGE_TAKEN_ON_HORSEBACK_MODIFIER,
        2,
        FrontierPackInt(player_id),
        FrontierPackFloat(modifier)
    );
}

static void frontier_set_ped_move_rate_override(int ped, float value)
{
    frontier_invoke(HASH_SET_PED_MOVE_RATE_OVERRIDE, 2, FrontierPackInt(ped), FrontierPackFloat(value));
}

static int frontier_clone_ped(int ped)
{
    return frontier_native_int(
        HASH_CLONE_PED,
        4,
        FrontierPackInt(ped),
        FrontierPackBool(0),
        FrontierPackBool(1),
        FrontierPackBool(1)
    );
}

static void frontier_set_entity_coords_no_offset(int entity, float x, float y, float z)
{
    frontier_invoke(
        HASH_SET_ENTITY_COORDS_NO_OFFSET,
        7,
        FrontierPackInt(entity),
        FrontierPackFloat(x),
        FrontierPackFloat(y),
        FrontierPackFloat(z),
        FrontierPackBool(1),
        FrontierPackBool(1),
        FrontierPackBool(1)
    );
}

static void frontier_set_entity_heading(int entity, float heading)
{
    frontier_invoke(HASH_SET_ENTITY_HEADING, 2, FrontierPackInt(entity), FrontierPackFloat(heading));
}

static void frontier_set_entity_visible(int entity, int toggle)
{
    frontier_invoke(HASH_SET_ENTITY_VISIBLE, 2, FrontierPackInt(entity), FrontierPackBool(toggle));
}

static void frontier_set_entity_alpha(int entity, int alpha)
{
    frontier_invoke(HASH_SET_ENTITY_ALPHA, 3, FrontierPackInt(entity), FrontierPackInt(alpha), FrontierPackBool(1));
}

static void frontier_freeze_entity_position(int entity, int toggle)
{
    frontier_invoke(HASH_FREEZE_ENTITY_POSITION, 2, FrontierPackInt(entity), FrontierPackBool(toggle));
}

static void frontier_set_entity_invincible(int entity, int toggle)
{
    frontier_invoke(HASH_SET_ENTITY_INVINCIBLE, 2, FrontierPackInt(entity), FrontierPackBool(toggle));
}

static void frontier_resurrect_ped(int ped)
{
    frontier_invoke(HASH_RESURRECT_PED, 1, FrontierPackInt(ped));
}

static void frontier_set_random_outfit_variation(int ped)
{
    frontier_invoke(HASH_SET_RANDOM_OUTFIT_VARIATION, 2, FrontierPackInt(ped), FrontierPackBool(1));
}

static void frontier_update_ped_variation(int ped)
{
    frontier_invoke(
        HASH_UPDATE_PED_VARIATION,
        6,
        FrontierPackInt(ped),
        FrontierPackBool(0),
        FrontierPackBool(1),
        FrontierPackBool(1),
        FrontierPackBool(1),
        FrontierPackBool(0)
    );
}

static FrontierVector3 frontier_get_entity_coords(int entity)
{
    return frontier_native_vector3(
        HASH_GET_ENTITY_COORDS,
        3,
        FrontierPackInt(entity),
        FrontierPackBool(1),
        FrontierPackBool(1)
    );
}

static FrontierVector3 frontier_get_offset_from_entity_coords(int entity, float offset_x, float offset_y, float offset_z)
{
    return frontier_native_vector3(
        HASH_GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS,
        4,
        FrontierPackInt(entity),
        FrontierPackFloat(offset_x),
        FrontierPackFloat(offset_y),
        FrontierPackFloat(offset_z)
    );
}

static float frontier_get_entity_heading(int entity)
{
    return frontier_native_float(HASH_GET_ENTITY_HEADING, 1, FrontierPackInt(entity));
}

static uint32_t frontier_get_entity_model(int entity)
{
    return (uint32_t)frontier_native_int(HASH_GET_ENTITY_MODEL, 1, FrontierPackInt(entity));
}

static int frontier_does_entity_exist(int entity)
{
    return entity != 0 && frontier_native_bool(HASH_DOES_ENTITY_EXIST, 1, FrontierPackInt(entity));
}

static int frontier_is_model_valid(uint32_t model_hash)
{
    return model_hash != 0U && frontier_native_bool(HASH_IS_MODEL_VALID, 1, FrontierPackUInt64(model_hash));
}

static void frontier_request_model(uint32_t model_hash)
{
    frontier_invoke(HASH_REQUEST_MODEL, 2, FrontierPackUInt64(model_hash), FrontierPackBool(0));
}

static int frontier_has_model_loaded(uint32_t model_hash)
{
    return model_hash != 0U && frontier_native_bool(HASH_HAS_MODEL_LOADED, 1, FrontierPackUInt64(model_hash));
}

static void frontier_set_model_as_no_longer_needed(uint32_t model_hash)
{
    if (model_hash != 0U) {
        frontier_invoke(HASH_SET_MODEL_AS_NO_LONGER_NEEDED, 1, FrontierPackUInt64(model_hash));
    }
}

static int frontier_create_ped(uint32_t model_hash, float x, float y, float z, float heading)
{
    return frontier_native_int(
        HASH_CREATE_PED,
        9,
        FrontierPackUInt64(model_hash),
        FrontierPackFloat(x),
        FrontierPackFloat(y),
        FrontierPackFloat(z),
        FrontierPackFloat(heading),
        FrontierPackBool(0),
        FrontierPackBool(1),
        FrontierPackBool(1),
        FrontierPackBool(0)
    );
}

static void frontier_set_entity_max_health(int entity, int value)
{
    frontier_invoke(HASH_SET_ENTITY_MAX_HEALTH, 2, FrontierPackInt(entity), FrontierPackInt(value));
}

static void frontier_set_ped_max_health(int ped, int value)
{
    frontier_invoke(HASH_SET_PED_MAX_HEALTH, 2, FrontierPackInt(ped), FrontierPackInt(value));
}

static void frontier_set_entity_health(int entity, int value, int entity_killed_by)
{
    frontier_invoke(HASH_SET_ENTITY_HEALTH, 3, FrontierPackInt(entity), FrontierPackInt(value), FrontierPackInt(entity_killed_by));
}

static void frontier_set_ped_accuracy(int ped, int accuracy)
{
    frontier_invoke(HASH_SET_PED_ACCURACY, 2, FrontierPackInt(ped), FrontierPackInt(accuracy));
}

static void frontier_set_ped_combat_ability(int ped, int ability)
{
    frontier_invoke(HASH_SET_PED_COMBAT_ABILITY, 2, FrontierPackInt(ped), FrontierPackInt(ability));
}

static void frontier_set_ped_combat_range(int ped, int range)
{
    frontier_invoke(HASH_SET_PED_COMBAT_RANGE, 2, FrontierPackInt(ped), FrontierPackInt(range));
}

static void frontier_set_ped_combat_movement(int ped, int movement)
{
    frontier_invoke(HASH_SET_PED_COMBAT_MOVEMENT, 2, FrontierPackInt(ped), FrontierPackInt(movement));
}

static void frontier_set_ped_combat_attribute(int ped, int attribute_index, int enabled)
{
    frontier_invoke(HASH_SET_PED_COMBAT_ATTRIBUTES, 3, FrontierPackInt(ped), FrontierPackInt(attribute_index), FrontierPackBool(enabled));
}

static void frontier_set_ped_can_be_targetted(int ped, int enabled)
{
    frontier_invoke(HASH_SET_PED_CAN_BE_TARGETTED, 2, FrontierPackInt(ped), FrontierPackBool(enabled));
}

static void frontier_set_ped_can_ragdoll(int ped, int enabled)
{
    frontier_invoke(HASH_SET_PED_CAN_RAGDOLL, 2, FrontierPackInt(ped), FrontierPackBool(enabled));
}

static void frontier_set_ped_keep_task(int ped, int enabled)
{
    frontier_invoke(HASH_SET_PED_KEEP_TASK, 2, FrontierPackInt(ped), FrontierPackBool(enabled));
}

static void frontier_set_blocking_of_non_temporary_events(int ped, int enabled)
{
    frontier_invoke(HASH_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS, 2, FrontierPackInt(ped), FrontierPackBool(enabled));
}

static void frontier_set_ped_relationship_group_hash(int ped, uint32_t relationship_group)
{
    frontier_invoke(HASH_SET_PED_RELATIONSHIP_GROUP_HASH, 2, FrontierPackInt(ped), FrontierPackUInt64(relationship_group));
}

static void frontier_task_combat_ped(int ped, int target_ped)
{
    frontier_invoke(HASH_TASK_COMBAT_PED, 4, FrontierPackInt(ped), FrontierPackInt(target_ped), FrontierPackInt(0), FrontierPackInt(16));
}

static void frontier_task_combat_hated_targets_around_ped(int ped, float radius, int duration_ms)
{
    frontier_invoke(
        HASH_TASK_COMBAT_HATED_TARGETS_AROUND_PED,
        3,
        FrontierPackInt(ped),
        FrontierPackFloat(radius),
        FrontierPackInt(duration_ms)
    );
}

static void frontier_give_weapon_to_ped(int ped, uint32_t weapon_hash, int ammo_count)
{
    frontier_invoke(
        HASH_GIVE_WEAPON_TO_PED,
        13,
        FrontierPackInt(ped),
        FrontierPackUInt64(weapon_hash),
        FrontierPackInt(ammo_count),
        FrontierPackBool(0),
        FrontierPackBool(0),
        FrontierPackInt(0),
        FrontierPackBool(0),
        FrontierPackFloat(0.0f),
        FrontierPackFloat(0.0f),
        FrontierPackUInt64(0U),
        FrontierPackBool(1),
        FrontierPackFloat(0.0f),
        FrontierPackBool(0)
    );
}

static int frontier_get_interior_from_entity(int entity)
{
    return frontier_native_int(HASH_GET_INTERIOR_FROM_ENTITY, 1, FrontierPackInt(entity));
}

static int frontier_pause_menu_active(void)
{
    return frontier_native_bool(HASH_IS_PAUSE_MENU_ACTIVE, 0);
}

static void frontier_disable_controls(void)
{
    frontier_invoke(HASH_DISABLE_ALL_CONTROL_ACTIONS, 1, FrontierPackInt(0));
}
