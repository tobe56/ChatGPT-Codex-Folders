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

