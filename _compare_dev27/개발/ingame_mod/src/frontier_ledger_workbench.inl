static void frontier_render_zone_prompt(void)
{
    if (!g_runtime.config.show_zone_prompt) {
        return;
    }
    frontier_draw_rect_tl(0.33f, 0.835f, 0.31f, 0.058f, kColorBackground);
    frontier_draw_rect_tl(0.33f, 0.835f, 0.31f, 0.008f, kColorAccentSoft);
    frontier_draw_text_short(
        frontier_text("Beecher's Hope [F11] full ledger", "거점 [F11] 레저"),
        0.350f,
        0.852f,
        frontier_dataset_is_korean() ? 0.40f : 0.34f,
        kColorText
    );
}

static const FrontierTrackedPed *frontier_active_frontier_corpse(void)
{
    return NULL;
    int player_ped = frontier_player_ped();
    FrontierVector3 player_position;
    const FrontierTrackedPed *best = NULL;
    float best_distance_sq = 64.0f;
    int index;

    if (player_ped == 0) {
        return NULL;
    }

    player_position = frontier_get_entity_coords(player_ped);
    for (index = 0; index < FRONTIER_LIVE_TRACKED_PED_SLOTS; ++index) {
        const FrontierTrackedPed *tracked = &g_runtime.tracked_peds[index];
        FrontierVector3 corpse_position;
        float dx;
        float dy;
        float dz;
        float distance_sq;

        if (!tracked->spawned_by_frontier || tracked->loot_resolved || tracked->entity == 0) {
            continue;
        }
        if (!frontier_does_entity_exist(tracked->entity) || !frontier_is_ped_dead_or_dying(tracked->entity)) {
            continue;
        }
        if (frontier_is_entity_fully_looted(tracked->entity)) {
            continue;
        }

        corpse_position = frontier_get_entity_coords(tracked->entity);
        dx = corpse_position.x - player_position.x;
        dy = corpse_position.y - player_position.y;
        dz = frontier_abs_float(corpse_position.z - player_position.z);
        distance_sq = (dx * dx) + (dy * dy);
        if (dz > 3.0f || distance_sq > best_distance_sq) {
            continue;
        }

        best_distance_sq = distance_sq;
        best = tracked;
    }

    return best;
}

static void frontier_render_frontier_corpse_prompt(void)
{
    const FrontierTrackedPed *tracked = frontier_active_frontier_corpse();
    const char *label;

    if (tracked == NULL) {
        return;
    }

    label = frontier_dataset_is_korean()
        ? (tracked->spawn_label_ko != NULL && tracked->spawn_label_ko[0] != '\0' ? tracked->spawn_label_ko : tracked->spawn_label)
        : tracked->spawn_label;
    if (label == NULL || label[0] == '\0') {
        label = "Authored frontier target";
    }

    frontier_draw_rect_tl(0.29f, 0.830f, 0.42f, 0.065f, kColorBackground);
    frontier_draw_rect_tl(0.29f, 0.830f, 0.42f, 0.009f, kColorAccentSoft);
    snprintf(
        g_runtime.runtime_entry_label,
        sizeof(g_runtime.runtime_entry_label),
        frontier_dataset_is_korean() ? "Authored corpse | %s" : "Authored corpse | %s",
        label
    );
    frontier_draw_text_short(
        g_runtime.runtime_entry_label,
        0.312f,
        0.852f,
        frontier_dataset_is_korean() ? 0.30f : 0.28f,
        kColorText
    );
}

static int frontier_inside_workbench_zone(void)
{
    float dx;
    float dy;
    float radius_sq;
    float dz;

    frontier_refresh_zone_state(0);
    if (!g_runtime.zone_state.valid) {
        return 0;
    }

    dx = g_runtime.zone_state.position.x - g_runtime.config.workbench_x;
    dy = g_runtime.zone_state.position.y - g_runtime.config.workbench_y;
    dz = frontier_abs_float(g_runtime.zone_state.position.z - g_runtime.config.workbench_z);
    radius_sq = g_runtime.config.workbench_radius * g_runtime.config.workbench_radius;

    return g_runtime.zone_state.inside_zone &&
        ((dx * dx) + (dy * dy)) <= radius_sq &&
        dz <= 6.0f;
}

static const char *frontier_workbench_tab_name(int tab_index)
{
    switch (tab_index) {
    case FRONTIER_WORKBENCH_TAB_CRAFT:
        return frontier_ui_text("Craft", "FL_WB_TAB_CRAFT");
    case FRONTIER_WORKBENCH_TAB_ARCHIVE:
        return frontier_ui_text("Archive", "FL_WB_TAB_ARCHIVE");
    default:
        return frontier_ui_text("Result", "FL_WB_TAB_RESULT");
    }
}

static int frontier_workbench_entry_count(int tab_index)
{
    switch (tab_index) {
    case FRONTIER_WORKBENCH_TAB_CRAFT:
        return FRONTIER_WORKBENCH_CRAFT_COUNT;
    case FRONTIER_WORKBENCH_TAB_ARCHIVE:
        return FRONTIER_WORKBENCH_ARCHIVE_COUNT;
    default:
        return FRONTIER_WORKBENCH_RESULT_COUNT;
    }
}

static int frontier_workbench_clamp_entry_index(int tab_index, int entry_index)
{
    int entry_count = frontier_workbench_entry_count(tab_index);
    if (entry_count <= 0) {
        return 0;
    }
    return frontier_clamp_int(entry_index, 0, entry_count - 1);
}

static int frontier_workbench_next_ready_recipe(void)
{
    int index;
    for (index = 0; index < FRONTIER_WORKBENCH_CRAFT_COUNT; ++index) {
        if (!frontier_live_recipe_crafted(index) && frontier_live_recipe_ready(index)) {
            return index;
        }
    }
    return -1;
}

static const char *frontier_workbench_archive_name(int entry_index)
{
    switch (entry_index) {
    case 0:
        return frontier_text("Workbench Ledger", "작업대 장부");
    case 1:
        return frontier_text("Salvage", "폐철 자재");
    case 2:
        return frontier_text("Hide Bundles", "가죽 꾸러미");
    case 3:
        return frontier_text("Tonic Stock", "강장 보급");
    case 4:
        return frontier_text("Relic Fragments", "유물 조각");
    case 5:
        return frontier_text("Field Routes", "현장 동선");
    case 6:
        return frontier_text("Threat Notes", "위협 기록");
    default:
        return frontier_text("Homestead Log", "거점 기록");
    }
}

static const char *frontier_workbench_archive_detail(int entry_index)
{
    switch (entry_index) {
    case 0:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ WORK T%d / ARM T%d / MED T%d ~n~ READY %d / DONE %d",
            frontier_text("LIVE BENCH TRACKER", "작업대는 라이브 런과 제작 확정 결과에 따라 성장합니다."),
            g_runtime.live_profile.workshop_tier,
            g_runtime.live_profile.armory_tier,
            g_runtime.live_profile.medicine_tier,
            frontier_live_ready_recipe_count(),
            frontier_live_crafted_recipe_count()
        );
        return g_runtime.runtime_entry_detail;
    case 1:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ SALV %d ~n~ %s",
            frontier_text("LOOTED SCRAP AND WAGON METAL", "폐철은 루팅한 무기, 마차 잔해, 계약 폐재에서 얻습니다."),
            g_runtime.live_profile.salvage_stock,
            frontier_text("USED FOR KITS / FRAMES / TOOLS", "정비 세트, 프레임 작업, 공구 업그레이드에 쓰입니다.")
        );
        return g_runtime.runtime_entry_detail;
    case 2:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ HIDE %d / ANIMAL %d ~n~ %s",
            frontier_text("HUNTING AND TRAP RECOVERY", "가죽 꾸러미는 사냥과 덫 회수에서 쌓입니다."),
            g_runtime.live_profile.hide_stock,
            g_runtime.live_profile.animal_kills,
            frontier_text("USED FOR WRAPS / SIGILS / PACKS", "스태미나 장비, 트래퍼 물품, 현장 포장 제작에 쓰입니다.")
        );
        return g_runtime.runtime_entry_detail;
    case 3:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ TONIC %d / MED T%d ~n~ %s",
            frontier_text("MED STOCK AND FIELD RETURNS", "강장 보급은 거점 의약 준비와 현장 회수 상태를 반영합니다."),
            g_runtime.live_profile.tonic_stock,
            g_runtime.live_profile.medicine_tier,
            frontier_text("USED FOR PACKS / MED BONUS / RECOVERY", "강장팩, 현장 의약 보너스, 후반 회복 제작의 핵심 재료입니다.")
        );
        return g_runtime.runtime_entry_detail;
    case 4:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ RELIC %d ~n~ %s",
            frontier_text("MYTH ROUTE LATE MATERIAL", "유물 조각은 프론티어 유물 루트에서 얻는 후반 재료입니다."),
            g_runtime.live_profile.relic_stock,
            frontier_text("USED FOR RELIC KIT / CHARM / LATE CRAFT", "유물 키트, 데드아이 참, 후반 제작 라인에 사용됩니다.")
        );
        return g_runtime.runtime_entry_detail;
    case 5:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ REG %d/6 / HUB %d / DIST %.1fKM ~n~ %s",
            frontier_text("FIELD ROUTE AND BENCH PRESSURE", "현장 동선은 작업대 추천과 프론티어 압력을 함께 갱신합니다."),
            g_runtime.live_profile.discovered_region_count,
            g_runtime.live_profile.hub_returns,
            g_runtime.live_profile.field_distance_total / 1000.0f,
            frontier_runtime_route_hint()
        );
        return g_runtime.runtime_entry_detail;
    case 6:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "BLACKWATER %s ~n~ SILAS %s ~n~ RELIC %s ~n~ %s",
            frontier_live_boss_state_text(g_runtime.live_profile.boss_blackwater_ready),
            frontier_live_boss_state_text(g_runtime.live_profile.boss_silas_ready),
            frontier_live_boss_state_text(g_runtime.live_profile.boss_relic_ready),
            frontier_text("THREAT FLAGS OPEN LATE CRAFT", "위협 준비도는 상위 병기와 유물 제작 라인을 여는 기준입니다.")
        );
        return g_runtime.runtime_entry_detail;
    default:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ %s ~n~ %s",
            frontier_text("HUB REPORTS AND RETURN LOG", "비처스 호프는 제작, 보고, 복귀의 기준 거점입니다."),
            g_runtime.live_profile.network_summary,
            g_runtime.live_profile.progress_summary
        );
        return g_runtime.runtime_entry_detail;
    }

    switch (entry_index) {
    case 0:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ %s T%d / %s T%d / %s T%d ~n~ %s %d / %s %d",
            frontier_text("Workbench progress is tied to the live run and crafted unlocks.", "작업대 진행은 라이브 런과 제작 해금에 연결됩니다."),
            frontier_text("Workshop", "작업대"),
            g_runtime.live_profile.workshop_tier,
            frontier_text("Armory", "병기"),
            g_runtime.live_profile.armory_tier,
            frontier_text("Medicine", "보급"),
            g_runtime.live_profile.medicine_tier,
            frontier_text("Ready recipes", "준비 레시피"),
            frontier_live_ready_recipe_count(),
            frontier_text("Crafted", "제작 완료"),
            frontier_live_crafted_recipe_count()
        );
        return g_runtime.runtime_entry_detail;
    case 1:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ SALV %d / HIDE %d / TONIC %d / RELIC %d ~n~ ENEMY %d / LOOT %d / ANIMAL %d",
            frontier_text("Material flow is derived from real field actions and hub returns.", "재료 흐름은 실제 필드 활동과 거점 복귀에서 계산됩니다."),
            g_runtime.live_profile.salvage_stock,
            g_runtime.live_profile.hide_stock,
            g_runtime.live_profile.tonic_stock,
            g_runtime.live_profile.relic_stock,
            g_runtime.live_profile.enemy_kills,
            g_runtime.live_profile.loot_count,
            g_runtime.live_profile.animal_kills
        );
        return g_runtime.runtime_entry_detail;
    case 2:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ REG %d/6 / HUB %d / DIST %.1fKM ~n~ %s",
            frontier_text("Field routing updates the workbench recommendations and frontier pressure.", "현장 동선은 작업대 추천과 프론티어 압력을 함께 갱신합니다."),
            g_runtime.live_profile.discovered_region_count,
            g_runtime.live_profile.hub_returns,
            g_runtime.live_profile.field_distance_total / 1000.0f,
            frontier_runtime_route_hint()
        );
        return g_runtime.runtime_entry_detail;
    case 3:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "BLACKWATER %s ~n~ SILAS %s ~n~ RELIC %s ~n~ %s",
            frontier_live_boss_state_text(g_runtime.live_profile.boss_blackwater_ready),
            frontier_live_boss_state_text(g_runtime.live_profile.boss_silas_ready),
            frontier_live_boss_state_text(g_runtime.live_profile.boss_relic_ready),
            frontier_text("Threat readiness controls the higher craft lanes.", "위협 준비도는 상위 제작 라인을 여는 기준입니다.")
        );
        return g_runtime.runtime_entry_detail;
    default:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ %s ~n~ %s",
            frontier_text("Beecher's Hope remains the anchor for reports, crafting, and recovery.", "비처스 호프는 보고, 제작, 복귀의 기준 거점입니다."),
            g_runtime.live_profile.network_summary,
            g_runtime.live_profile.progress_summary
        );
        return g_runtime.runtime_entry_detail;
    }
}

static const char *frontier_workbench_result_name(int entry_index)
{
    switch (entry_index) {
    case 0:
        return frontier_text("Latest Craft", "최근 제작");
    case 1:
        return frontier_text("Active Bonus", "활성 보너스");
    case 2:
        return frontier_text("Workbench State", "작업대 상태");
    default:
        return frontier_text("Next Recommendation", "다음 추천");
    }
}

static const char *frontier_workbench_result_detail(int entry_index)
{
    int next_recipe = frontier_workbench_next_ready_recipe();

    switch (entry_index) {
    case 0:
        if (g_runtime.workbench_result_title[0] == '\0') {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s ~n~ %s",
                frontier_text("NO RESULT YET", "이번 세션에서 기록된 작업대 결과가 없습니다."),
                frontier_text("CRAFT A READY ITEM OR PIN A NOTE", "준비된 제작을 확정하거나 백과 항목을 고정하면 이 카드가 채워집니다.")
            );
        } else {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s ~n~ %s",
                g_runtime.workbench_result_title,
                g_runtime.workbench_result_body
            );
        }
        return g_runtime.runtime_entry_detail;
    case 1:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "BONUS T%d ~n~ %s ~n~ %s",
            frontier_live_bonus_tier(),
            g_runtime.live_profile.bonus_summary,
            frontier_text("SHARED WITH FIELD HUD", "제작 해금은 필드 HUD와 같은 라이브 보너스 계층에 합쳐집니다.")
        );
        return g_runtime.runtime_entry_detail;
    case 2:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "WORK T%d / ARM T%d / MED T%d ~n~ CRAFTED %d / READY %d",
            g_runtime.live_profile.workshop_tier,
            g_runtime.live_profile.armory_tier,
            g_runtime.live_profile.medicine_tier,
            frontier_live_crafted_recipe_count(),
            frontier_live_ready_recipe_count()
        );
        return g_runtime.runtime_entry_detail;
    default:
        if (next_recipe >= 0) {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s ~n~ %s ~n~ %s",
                frontier_workbench_recipe_primary_name(next_recipe),
                frontier_workbench_recipe_export_detail(next_recipe),
                frontier_workbench_recipe_gate_display(next_recipe)
            );
        } else {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s ~n~ %s",
                frontier_text("NO EXTRA RECIPE READY", "지금 바로 확정할 수 있는 추가 제작이 없습니다."),
                frontier_text("HUNT LOOT EARN RETURN TO OPEN MORE", "사냥, 루팅, 보상 획득, 거점 복귀를 이어가면 다음 제작 라인이 열립니다.")
            );
        }
        return g_runtime.runtime_entry_detail;
    }
}

static const char *frontier_workbench_current_label(void)
{
    int tab_index = frontier_clamp_int(g_runtime.workbench_tab, 0, 2);
    int entry_index = frontier_workbench_clamp_entry_index(tab_index, g_runtime.workbench_selected_entry[tab_index]);

    if (tab_index == FRONTIER_WORKBENCH_TAB_CRAFT) {
        return frontier_workbench_recipe_primary_name(entry_index);
    }
    if (tab_index == FRONTIER_WORKBENCH_TAB_ARCHIVE) {
        return frontier_workbench_archive_name(entry_index);
    }
    return frontier_workbench_result_name(entry_index);
}

static const char *frontier_workbench_recipe_state_compact(int entry_index)
{
    if (frontier_live_recipe_crafted(entry_index)) {
        return "완료";
    }
    if (frontier_live_recipe_ready(entry_index)) {
        return "준비";
    }
    return "잠김";
}

static const char *frontier_workbench_recipe_state_badge(int entry_index)
{
    if (frontier_live_recipe_crafted(entry_index)) {
        return frontier_ui_text("Done", "FL_WB_STATE_DONE");
    }
    if (frontier_live_recipe_ready(entry_index)) {
        return frontier_ui_text("Ready", "FL_WB_STATE_READY");
    }
    return frontier_ui_text("Locked", "FL_WB_STATE_LOCKED");
}

static const char *frontier_workbench_list_label(int tab_index, int entry_index)
{
    switch (tab_index) {
    case FRONTIER_WORKBENCH_TAB_CRAFT:
        frontier_copy_label_without_last_segment(
            frontier_workbench_recipe_export_label(entry_index),
            g_runtime.runtime_entry_label,
            sizeof(g_runtime.runtime_entry_label)
        );
        return g_runtime.runtime_entry_label;
    case FRONTIER_WORKBENCH_TAB_ARCHIVE:
        switch (entry_index) {
        case 0: return frontier_ui_text("01 Ledger", "FL_WB_ARCHIVE_01");
        case 1: return frontier_ui_text("02 Salvage", "FL_WB_ARCHIVE_02");
        case 2: return frontier_ui_text("03 Hides", "FL_WB_ARCHIVE_03");
        case 3: return frontier_ui_text("04 Tonics", "FL_WB_ARCHIVE_04");
        case 4: return frontier_ui_text("05 Relics", "FL_WB_ARCHIVE_05");
        case 5: return frontier_ui_text("06 Routes", "FL_WB_ARCHIVE_06");
        case 6: return frontier_ui_text("07 Threats", "FL_WB_ARCHIVE_07");
        default: return frontier_ui_text("08 Hub", "FL_WB_ARCHIVE_08");
        }
    default:
        switch (entry_index) {
        case 0: return frontier_ui_text("01 Latest", "FL_WB_RESULT_01");
        case 1: return frontier_ui_text("02 Bonus", "FL_WB_RESULT_02");
        case 2: return frontier_ui_text("03 Bench", "FL_WB_RESULT_03");
        default: return frontier_ui_text("04 Next", "FL_WB_RESULT_04");
        }
    }
}

static const char *frontier_workbench_recipe_effect_compact(int entry_index)
{
    switch (entry_index) {
    case 0: return "열화 -4 / 정비 +1";
    case 1: return "근성 +1 / 스태 +5";
    case 2: return "작업대 +1 / 거점";
    case 3: return "병기 +1 / 열화 -6";
    case 4: return "사냥 +1 / 가죽 +2";
    case 5: return "데드 +6 / 병기 +1";
    case 6: return "거점 +1 / 작업대 +1";
    case 7: return "유물 +2 / 데드 +8";
    case 8: return "보급 +1 / 스태 +4";
    case 9: return "총포 +1 / 열화 -5";
    default: return "데드 +6 / 유물 +1";
    }
}

static const char *frontier_workbench_recipe_effect_ascii(int entry_index)
{
    switch (entry_index) {
    case 0: return "WEAR -4 / MAINT +1";
    case 1: return "GRIT +1 / STAM +5";
    case 2: return "WORK +1 / HUB";
    case 3: return "ARM +1 / WEAR -6";
    case 4: return "HUNT +1 / HIDE +2";
    case 5: return "DE +6 / ARM +1";
    case 6: return "HUB +1 / WORK +1";
    case 7: return "RELIC +2 / DE +8";
    case 8: return "MED +1 / STAM +4";
    case 9: return "SMITH +1 / WEAR -5";
    default: return "DE +6 / RELIC +1";
    }
}

static const char *frontier_workbench_archive_detail_korean(int entry_index)
{
    switch (entry_index) {
    case 0:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "작업대 %d / 병기 %d / 보급 %d~n~준비 %d / 완료 %d~n~거점 작업대 진행 현황",
            g_runtime.live_profile.workshop_tier,
            g_runtime.live_profile.armory_tier,
            g_runtime.live_profile.medicine_tier,
            frontier_live_ready_recipe_count(),
            frontier_live_crafted_recipe_count()
        );
        break;
    case 1:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "폐철 %d~n~무기, 마차, 계약 폐재~n~정비/프레임/공구 제작 재료",
            g_runtime.live_profile.salvage_stock
        );
        break;
    case 2:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "가죽 %d / 동물 %d~n~사냥과 덫 회수 자재~n~강장/인장 제작 재료",
            g_runtime.live_profile.hide_stock,
            g_runtime.live_profile.animal_kills
        );
        break;
    case 3:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "강장 %d / 보급 %d~n~거점 보급과 회복 자재~n~강장팩/의약 제작 재료",
            g_runtime.live_profile.tonic_stock,
            g_runtime.live_profile.medicine_tier
        );
        break;
    case 4:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "유물 %d~n~유물 루트 핵심 재료~n~결속 키트/참 제작 재료",
            g_runtime.live_profile.relic_stock
        );
        break;
    case 5:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "지역 %d/6 / 복귀 %d~n~거리 %.1fKM~n~%s",
            g_runtime.live_profile.discovered_region_count,
            g_runtime.live_profile.hub_returns,
            g_runtime.live_profile.field_distance_total / 1000.0f,
            frontier_runtime_route_hint()
        );
        break;
    case 6:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "블랙워터 %s~n~사일러스 %s~n~유물전 %s",
            g_runtime.live_profile.boss_blackwater_ready ? "준비" : "추적",
            g_runtime.live_profile.boss_silas_ready ? "준비" : "추적",
            g_runtime.live_profile.boss_relic_ready ? "준비" : "추적"
        );
        break;
    default:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "보상 %d / 복귀 %d~n~현재 런 %s~n~거점 보고와 복귀 기록",
            g_runtime.live_profile.reward_earned,
            g_runtime.live_profile.hub_returns,
            g_runtime.live_profile.progress_summary
        );
        break;
    }

    return g_runtime.runtime_entry_detail;
}

static const char *frontier_workbench_result_detail_korean(int entry_index);

static const char *frontier_workbench_overlay_detail(int tab_index, int entry_index)
{
    if (tab_index == FRONTIER_WORKBENCH_TAB_CRAFT) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s~n~%s~n~%s",
            frontier_workbench_recipe_export_detail(entry_index),
            frontier_workbench_recipe_state(entry_index),
            frontier_workbench_recipe_gate_display(entry_index)
        );
        return g_runtime.runtime_entry_detail;
        if (frontier_dataset_is_korean()) {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s~n~%s~n~상태 %s~n~%s",
                frontier_workbench_recipe_materials(entry_index),
                frontier_workbench_recipe_effect_compact(entry_index),
                frontier_workbench_recipe_state_compact(entry_index),
                frontier_live_recipe_ready(entry_index) ? "E로 제작 확정" : frontier_workbench_recipe_gate_compact(entry_index)
            );
        } else {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s~n~%s~n~STATE %s~n~%s",
                frontier_workbench_recipe_materials_display(entry_index),
                frontier_workbench_recipe_effect_display(entry_index),
                frontier_workbench_recipe_state(entry_index),
                frontier_workbench_recipe_gate_display(entry_index)
            );
        }
        return g_runtime.runtime_entry_detail;
    }

    if (tab_index == FRONTIER_WORKBENCH_TAB_ARCHIVE) {
        return frontier_dataset_is_korean()
            ? frontier_workbench_archive_detail_korean(entry_index)
            : frontier_workbench_archive_detail(entry_index);
    }

    return frontier_dataset_is_korean()
        ? frontier_workbench_result_detail_korean(entry_index)
        : frontier_workbench_result_detail(entry_index);
}

static const char *frontier_workbench_result_detail_korean(int entry_index)
{
    int next_recipe = frontier_workbench_next_ready_recipe();

    switch (entry_index) {
    case 0:
        if (g_runtime.workbench_result_title[0] == '\0') {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "최근 결과 없음~n~준비된 제작을 확정하거나~n~백과 메모를 고정하세요."
            );
        } else {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s~n~%s",
                g_runtime.workbench_result_title,
                g_runtime.workbench_result_body
            );
        }
        break;
    case 1:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "보너스 T%d~n~%s~n~필드 HUD와 같은 보너스 계층",
            frontier_live_bonus_tier(),
            g_runtime.live_profile.bonus_summary
        );
        break;
    case 2:
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "작업대 %d / 병기 %d / 보급 %d~n~완료 %d / 준비 %d",
            g_runtime.live_profile.workshop_tier,
            g_runtime.live_profile.armory_tier,
            g_runtime.live_profile.medicine_tier,
            frontier_live_crafted_recipe_count(),
            frontier_live_ready_recipe_count()
        );
        break;
    default:
        if (next_recipe >= 0) {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s~n~%s~n~%s",
                frontier_workbench_recipe_primary_name(next_recipe),
                frontier_workbench_recipe_export_detail(next_recipe),
                frontier_workbench_recipe_gate_display(next_recipe)
            );
        } else {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "추가 준비 제작 없음~n~사냥/루팅/보상/복귀를~n~더 진행하세요."
            );
        }
        break;
    }

    return g_runtime.runtime_entry_detail;
}

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
