static const char *frontier_live_stat_action_label(int entry_index)
{
    int rank = 0;
    int bonus = 0;
    const char *name = "Cunning";

    switch (entry_index) {
    case 0:
        rank = g_runtime.live_profile.stat_strength;
        bonus = g_runtime.live_profile.stat_bonus_strength;
        name = "Strength";
        break;
    case 1:
        rank = g_runtime.live_profile.stat_grit;
        bonus = g_runtime.live_profile.stat_bonus_grit;
        name = "Grit";
        break;
    case 2:
        rank = g_runtime.live_profile.stat_deadeye;
        bonus = g_runtime.live_profile.stat_bonus_deadeye;
        name = "Deadeye";
        break;
    case 3:
        rank = g_runtime.live_profile.stat_survival;
        bonus = g_runtime.live_profile.stat_bonus_survival;
        name = "Survival";
        break;
    default:
        rank = g_runtime.live_profile.stat_cunning;
        bonus = g_runtime.live_profile.stat_bonus_cunning;
        name = "Cunning";
        break;
    }

    snprintf(
        g_runtime.runtime_entry_label,
        sizeof(g_runtime.runtime_entry_label),
        "%s | Rank %d | +%d invested | %d stat pts left",
        name,
        rank,
        bonus,
        frontier_live_stat_points_available()
    );
    return g_runtime.runtime_entry_label;
}

static const char *frontier_live_skill_action_label(int entry_index)
{
    int rank = 1;
    int bonus = 0;
    const char *name = "Homestead";

    switch (entry_index) {
    case 0:
        rank = g_runtime.live_profile.skill_hunting;
        bonus = g_runtime.live_profile.skill_bonus_hunting;
        name = "Hunting";
        break;
    case 1:
        rank = g_runtime.live_profile.skill_gunsmithing;
        bonus = g_runtime.live_profile.skill_bonus_gunsmithing;
        name = "Gunsmithing";
        break;
    case 2:
        rank = g_runtime.live_profile.skill_field_medicine;
        bonus = g_runtime.live_profile.skill_bonus_field_medicine;
        name = "Field Medicine";
        break;
    case 3:
        rank = g_runtime.live_profile.skill_trapping;
        bonus = g_runtime.live_profile.skill_bonus_trapping;
        name = "Trapping";
        break;
    case 4:
        rank = g_runtime.live_profile.skill_salvaging;
        bonus = g_runtime.live_profile.skill_bonus_salvaging;
        name = "Salvaging";
        break;
    case 5:
        rank = g_runtime.live_profile.skill_trade;
        bonus = g_runtime.live_profile.skill_bonus_trade;
        name = "Trade";
        break;
    case 6:
        rank = g_runtime.live_profile.skill_horse_handling;
        bonus = g_runtime.live_profile.skill_bonus_horse_handling;
        name = "Horse Handling";
        break;
    default:
        rank = g_runtime.live_profile.skill_homestead;
        bonus = g_runtime.live_profile.skill_bonus_homestead;
        name = "Homestead";
        break;
    }

    snprintf(
        g_runtime.runtime_entry_label,
        sizeof(g_runtime.runtime_entry_label),
        "%s | Rank %d | +%d trained | %d skill pts left",
        name,
        rank,
        bonus,
        frontier_live_skill_points_available()
    );
    return g_runtime.runtime_entry_label;
}

static const char *frontier_live_stat_label(int entry_index)
{
    int rank = 0;
    const char *name = "";
    const char *focus = "";

    switch (entry_index) {
    case 0:
        rank = g_runtime.live_profile.stat_strength;
        name = frontier_text("Strength", "근력");
        focus = frontier_text("Melee power", "근접 전투");
        break;
    case 1:
        rank = g_runtime.live_profile.stat_grit;
        name = frontier_text("Grit", "근성");
        focus = frontier_text("Health", "체력");
        break;
    case 2:
        rank = g_runtime.live_profile.stat_deadeye;
        name = frontier_text("Deadeye", "데드아이");
        focus = frontier_text("Accuracy", "명중");
        break;
    case 3:
        rank = g_runtime.live_profile.stat_survival;
        name = frontier_text("Survival", "생존");
        focus = frontier_text("Tracking", "추적");
        break;
    default:
        rank = g_runtime.live_profile.stat_cunning;
        name = frontier_text("Cunning", "기민함");
        focus = frontier_text("Trade leverage", "거래 감각");
        break;
    }

    snprintf(
        g_runtime.runtime_entry_label,
        sizeof(g_runtime.runtime_entry_label),
        "%s | %s %d | %s",
        name,
        frontier_text("Rank", "랭크"),
        rank,
        focus
    );
    return g_runtime.runtime_entry_label;
}

static const char *frontier_live_skill_label(int entry_index)
{
    int rank = 1;
    const char *name = "";

    switch (entry_index) {
    case 0:
        rank = g_runtime.live_profile.skill_hunting;
        name = frontier_text("Hunting", "사냥");
        break;
    case 1:
        rank = g_runtime.live_profile.skill_gunsmithing;
        name = frontier_text("Gunsmithing", "총포 개조");
        break;
    case 2:
        rank = g_runtime.live_profile.skill_field_medicine;
        name = frontier_text("Field Medicine", "현장 치료");
        break;
    case 3:
        rank = g_runtime.live_profile.skill_trapping;
        name = frontier_text("Trapping", "덫 사냥");
        break;
    case 4:
        rank = g_runtime.live_profile.skill_salvaging;
        name = frontier_text("Salvaging", "회수");
        break;
    case 5:
        rank = g_runtime.live_profile.skill_trade;
        name = frontier_text("Trade", "거래");
        break;
    case 6:
        rank = g_runtime.live_profile.skill_horse_handling;
        name = frontier_text("Horse Handling", "말 조련");
        break;
    default:
        rank = g_runtime.live_profile.skill_homestead;
        name = frontier_text("Homestead", "거점 운영");
        break;
    }

    snprintf(
        g_runtime.runtime_entry_label,
        sizeof(g_runtime.runtime_entry_label),
        "%s | %s %d | %s",
        name,
        frontier_text("Rank", "랭크"),
        rank,
        frontier_text("Live", "실시간")
    );
    return g_runtime.runtime_entry_label;
}

static const char *frontier_live_perk_label(int entry_index)
{
    uint32_t flag = 0U;
    const char *name = "";
    const char *line = "";

    switch (entry_index) {
    case 1:
        flag = FRONTIER_PERK_LONG_TRAIL;
        name = frontier_text("Long Trail", "롱 트레일");
        line = frontier_text("Frontiersman", "프론티어맨");
        break;
    case 2:
        flag = FRONTIER_PERK_SHARP_TRACKER;
        name = frontier_text("Sharp Tracker", "샤프 트래커");
        line = frontier_text("Frontiersman", "프론티어맨");
        break;
    case 3:
        flag = FRONTIER_PERK_FRONT_SIGHT_FAITH;
        name = frontier_text("Front Sight Faith", "프런트 사이트 페이스");
        line = frontier_text("Gunslinger", "건슬링어");
        break;
    case 5:
        flag = FRONTIER_PERK_QUICK_DRAW;
        name = frontier_text("Quick Draw", "퀵 드로우");
        line = frontier_text("Gunslinger", "건슬링어");
        break;
    case 7:
        flag = FRONTIER_PERK_FAST_POCKETS;
        name = frontier_text("Fast Pockets", "패스트 포켓");
        line = frontier_text("Outlaw", "무법자");
        break;
    default:
        flag = FRONTIER_PERK_ASH_MARK;
        name = frontier_text("Ash Mark", "애시 마크");
        line = frontier_text("Relic Hunter", "유물 사냥꾼");
        break;
    }

    snprintf(
        g_runtime.runtime_entry_label,
        sizeof(g_runtime.runtime_entry_label),
        "%s | %s | %s",
        name,
        line,
        frontier_live_perk_state_text(flag)
    );
    return g_runtime.runtime_entry_label;
}

static const char *frontier_live_crafting_label(int entry_index)
{
    const char *name = "";
    const char *state = frontier_text("Locked", "잠금");

    switch (entry_index) {
    case 0:
        name = frontier_text("Reliable Grip Wrap", "리라이어블 그립 랩");
        state = g_runtime.live_profile.workshop_tier >= 1 ? frontier_text("Ready", "준비") : state;
        break;
    case 1:
        name = frontier_text("Saint Medal", "세인트 메달");
        state = g_runtime.live_profile.hide_stock >= 2 ? frontier_text("Ready", "준비") : state;
        break;
    case 2:
        name = frontier_text("Expanded Workshop", "확장 작업대");
        state = g_runtime.live_profile.workshop_tier >= 2 ? frontier_text("Ready", "준비") : state;
        break;
    case 3:
        name = frontier_text("Reinforced Frame", "보강 프레임");
        state = g_runtime.live_profile.armory_tier >= 2 ? frontier_text("Ready", "준비") : state;
        break;
    case 4:
        name = frontier_text("Trapper Sigil", "트래퍼 시길");
        state = g_runtime.live_profile.skill_trapping >= 2 ? frontier_text("Ready", "준비") : state;
        break;
    case 5:
        name = frontier_text("Blackwater Peacebreaker", "블랙워터 피스브레이커");
        state = g_runtime.live_profile.boss_blackwater_ready ? frontier_text("Ready", "준비") : state;
        break;
    case 6:
        name = frontier_text("Trophy Board", "트로피 보드");
        state = g_runtime.live_profile.hub_returns >= 2 ? frontier_text("Ready", "준비") : state;
        break;
    default:
        name = frontier_text("Relic Binding Kit", "유물 결속 키트");
        state = g_runtime.live_profile.boss_relic_ready ? frontier_text("Ready", "준비") : state;
        break;
    }

    snprintf(
        g_runtime.runtime_entry_label,
        sizeof(g_runtime.runtime_entry_label),
        "%s | %s | %s",
        name,
        frontier_text("Live Craft", "실시간 제작"),
        state
    );
    return g_runtime.runtime_entry_label;
}

#if 0
static const char *frontier_live_crafting_label_synced(int entry_index)
{
    char base_label[256];
    frontier_copy_label_segment(frontier_workbench_recipe_export_label(entry_index), base_label, sizeof(base_label), 3);
    snprintf(
        g_runtime.runtime_entry_label,
        sizeof(g_runtime.runtime_entry_label),
        "%s | %s",
        base_label,
        frontier_live_recipe_crafted(entry_index)
            ? frontier_text("Crafted", "?쒖옉??)
            : (frontier_live_recipe_ready(entry_index)
                ? frontier_text("Ready", "以鍮?)
                : frontier_text("Locked", "?좉툑"))
    );
    return g_runtime.runtime_entry_label;
}
#endif

static const char *frontier_live_crafting_label_safe(int entry_index)
{
    char base_label[256];
    frontier_copy_label_segment(frontier_workbench_recipe_export_label(entry_index), base_label, sizeof(base_label), 3);
    snprintf(
        g_runtime.runtime_entry_label,
        sizeof(g_runtime.runtime_entry_label),
        "%s | %s",
        base_label,
        frontier_live_recipe_crafted(entry_index)
            ? frontier_text("Crafted", "Crafted")
            : (frontier_live_recipe_ready(entry_index)
                ? frontier_text("Ready", "Ready")
                : frontier_text("Locked", "Locked"))
    );
    return g_runtime.runtime_entry_label;
}

static const char *frontier_live_armory_label(int entry_index)
{
    const char *weapon_name = entry_index == 0
        ? frontier_text("Marston Repeater", "마스턴 리피터")
        : frontier_text("Beecher's Sidearm", "비처스 사이드암");
    snprintf(
        g_runtime.runtime_entry_label,
        sizeof(g_runtime.runtime_entry_label),
        "%s | %s %d | %s T%d",
        weapon_name,
        frontier_text("Familiarity", "숙련"),
        g_runtime.live_profile.weapon_familiarity_rank,
        frontier_text("Maintenance", "손질"),
        g_runtime.live_profile.weapon_maintenance_tier
    );
    return g_runtime.runtime_entry_label;
}

static const char *frontier_live_dead_label(int entry_index)
{
    const char *name = "";
    const char *state = "";
    const char *region = "";

    switch (entry_index) {
    case 0:
        name = frontier_text("Silas Redd", "사일러스 레드");
        state = frontier_live_boss_state_text(g_runtime.live_profile.boss_silas_ready);
        region = frontier_text("Great Plains / Tall Trees", "그레이트플레인 / 톨트리스");
        break;
    case 1:
        name = frontier_text("Blackwater Broker", "블랙워터 브로커");
        state = frontier_live_boss_state_text(g_runtime.live_profile.boss_blackwater_ready);
        region = frontier_text("West Elizabeth", "웨스트 엘리자베스");
        break;
    default:
        name = frontier_text("Relic War Route", "유물 전쟁 루트");
        state = frontier_live_boss_state_text(g_runtime.live_profile.boss_relic_ready);
        region = frontier_text("Lemoyne / Ambarino", "르모인 / 암바리노");
        break;
    }

    snprintf(
        g_runtime.runtime_entry_label,
        sizeof(g_runtime.runtime_entry_label),
        "%s | %s | %s",
        name,
        state,
        region
    );
    return g_runtime.runtime_entry_label;
}

static int frontier_live_boss_entry_ready(int entry_index)
{
    switch (entry_index) {
    case 0:
        return g_runtime.live_profile.boss_silas_ready;
    case 1:
        return g_runtime.live_profile.boss_blackwater_ready;
    case 2:
        return g_runtime.live_profile.boss_silas_ready || g_runtime.live_profile.trail_stage >= 4;
    case 3:
        return g_runtime.live_profile.boss_relic_ready;
    default:
        return g_runtime.live_profile.phase >= FRONTIER_PHASE_ENDGAME || g_runtime.live_profile.trail_stage >= 5;
    }
}

static const char *frontier_live_dead_label_synced(int entry_index)
{
    char boss_name[160];
    char region_name[160];
    const char *export_label = frontier_resolve_text(frontier_find_dataset_entry("dead", entry_index) != NULL
        ? frontier_find_dataset_entry("dead", entry_index)->label
        : "");

    frontier_copy_label_segment(export_label, boss_name, sizeof(boss_name), 1);
    frontier_copy_label_last_segment(export_label, region_name, sizeof(region_name));
    snprintf(
        g_runtime.runtime_entry_label,
        sizeof(g_runtime.runtime_entry_label),
        "%s | %s | %s",
        boss_name[0] != '\0' ? boss_name : "Boss Entry",
        frontier_live_boss_state_text(frontier_live_boss_entry_ready(entry_index)),
        region_name[0] != '\0' ? region_name : frontier_runtime_context_label()
    );
    return g_runtime.runtime_entry_label;
}

static const char *frontier_live_collection_label(int entry_index)
{
    int progress = 0;
    int total = 4;
    const char *name = "";

    switch (entry_index) {
    case 0:
        name = frontier_text("Relic Fragment Ledger", "유물 파편 장부");
        progress = frontier_clamp_int(g_runtime.live_profile.relic_stock / 2, 0, 4);
        total = 4;
        break;
    case 1:
        name = frontier_text("Frontier Charms", "프론티어 참");
        progress = frontier_clamp_int(g_runtime.live_profile.hide_stock / 2, 0, 4);
        total = 4;
        break;
    case 2:
        name = frontier_text("Broker Marks", "브로커 표식");
        progress = frontier_clamp_int(g_runtime.live_profile.contract_count, 0, 3);
        total = 3;
        break;
    case 3:
        name = frontier_text("House Trophies", "집 트로피");
        progress = frontier_clamp_int(g_runtime.live_profile.hub_returns, 0, 3);
        total = 3;
        break;
    default:
        name = frontier_text("Frontier Records", "프론티어 기록");
        progress = frontier_clamp_int(g_runtime.live_profile.discovered_region_count, 0, 4);
        total = 4;
        break;
    }

    snprintf(
        g_runtime.runtime_entry_label,
        sizeof(g_runtime.runtime_entry_label),
        "%s | %d/%d | %s",
        name,
        progress,
        total,
        progress >= total ? frontier_text("Completed", "완료") : frontier_text("In Progress", "진행 중")
    );
    return g_runtime.runtime_entry_label;
}

static const char *frontier_live_gear_label(int entry_index)
{
    const char *label = "";

    switch (entry_index) {
    case 0:
        label = frontier_text("Major Accessory 1 | Ranch Band | Equipped", "주 장신구 1 | 랜치 밴드 | 장착");
        break;
    case 1:
        label = (g_runtime.live_profile.workshop_tier >= 2)
            ? frontier_text("Major Accessory 2 | Saint Medal | Equipped", "주 장신구 2 | 세인트 메달 | 장착")
            : frontier_text("Major Accessory 2 | Empty Slot | Open", "주 장신구 2 | 빈 슬롯 | 개방");
        break;
    case 5:
        label = (g_runtime.live_profile.skill_salvaging >= 2)
            ? frontier_text("Satchel Mod | Field Satchel Tools+ | Equipped", "가방 모드 | 현장 도구+ | 장착")
            : frontier_text("Satchel Mod | Field Satchel Tools | Equipped", "가방 모드 | 현장 도구 | 장착");
        break;
    default:
        label = frontier_text("Support Slot | Live Progress Locked", "보조 슬롯 | 실시간 진행 잠금");
        break;
    }

    frontier_copy_string(g_runtime.runtime_entry_label, sizeof(g_runtime.runtime_entry_label), label);
    return g_runtime.runtime_entry_label;
}

static int frontier_live_unlocked_perk_count(void)
{
    int count = 0;
    count += frontier_live_perk_unlocked(FRONTIER_PERK_LONG_TRAIL);
    count += frontier_live_perk_unlocked(FRONTIER_PERK_SHARP_TRACKER);
    count += frontier_live_perk_unlocked(FRONTIER_PERK_FRONT_SIGHT_FAITH);
    count += frontier_live_perk_unlocked(FRONTIER_PERK_QUICK_DRAW);
    count += frontier_live_perk_unlocked(FRONTIER_PERK_FAST_POCKETS);
    count += frontier_live_perk_unlocked(FRONTIER_PERK_RANCH_RESOLVE);
    count += frontier_live_perk_unlocked(FRONTIER_PERK_ASH_MARK);
    return count;
}

static int frontier_live_ready_recipe_count(void)
{
    int count = 0;
    int index;

    for (index = 0; index < FRONTIER_WORKBENCH_CRAFT_COUNT; ++index) {
        if (frontier_live_recipe_ready(index) && !frontier_live_recipe_crafted(index)) {
            ++count;
        }
    }
    return count;
}

static int frontier_live_ready_boss_count(void)
{
    return
        g_runtime.live_profile.boss_blackwater_ready +
        g_runtime.live_profile.boss_silas_ready +
        g_runtime.live_profile.boss_relic_ready;
}

static int frontier_live_completed_collection_count(void)
{
    int count = 0;

    if (g_runtime.live_profile.relic_stock >= 8) {
        ++count;
    }
    if (g_runtime.live_profile.hide_stock >= 8) {
        ++count;
    }
    if (g_runtime.live_profile.contract_count >= 3) {
        ++count;
    }
    if (g_runtime.live_profile.hub_returns >= 3) {
        ++count;
    }
    if (g_runtime.live_profile.discovered_region_count >= 4) {
        ++count;
    }

    return count;
}
