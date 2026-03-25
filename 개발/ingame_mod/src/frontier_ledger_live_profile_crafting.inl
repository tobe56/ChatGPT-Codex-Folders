static uint32_t frontier_craft_flag_for_entry(int entry_index)
{
    switch (entry_index) {
    case 0: return FRONTIER_CRAFT_RELIABLE_GRIP_WRAP;
    case 1: return FRONTIER_CRAFT_SAINT_MEDAL;
    case 2: return FRONTIER_CRAFT_BLACKWATER_PRECISION_LOADS;
    case 3: return FRONTIER_CRAFT_EXPANDED_WORKSHOP;
    case 4: return FRONTIER_CRAFT_LONG_BARREL_WALKER_KIT;
    case 5: return FRONTIER_CRAFT_REINFORCED_FRAME;
    case 6: return FRONTIER_CRAFT_TRAPPER_SIGIL;
    case 7: return FRONTIER_CRAFT_ASH_MARKED_REPEATER;
    case 8: return FRONTIER_CRAFT_BLACKWATER_PEACEBREAKER;
    case 9: return FRONTIER_CRAFT_FIELD_SUPPORT_CABINET;
    case 10: return FRONTIER_CRAFT_TROPHY_BOARD;
    case 11: return FRONTIER_CRAFT_RELIC_BINDING_KIT;
    default: return FRONTIER_CRAFT_RELIC_MARKED_ROUNDS;
    }
}

static int frontier_live_recipe_crafted(int entry_index)
{
    return (g_runtime.live_profile.crafted_mask & frontier_craft_flag_for_entry(entry_index)) != 0U;
}

static int frontier_live_ready_recipe_count(void);

static int frontier_live_crafted_recipe_count(void)
{
    int count = 0;
    int index;
    for (index = 0; index < FRONTIER_WORKBENCH_CRAFT_COUNT; ++index) {
        count += frontier_live_recipe_crafted(index);
    }
    return count;
}

static const char *frontier_workbench_recipe_name(int entry_index)
{
    switch (entry_index) {
    case 0: return frontier_text("Gun Oil Service Kit", "건오일 정비 세트");
    case 1: return frontier_text("Saint Medal", "세인트 메달");
    case 2: return frontier_text("Expanded Workshop", "확장 작업대");
    case 3: return frontier_text("Reinforced Frame", "보강 프레임");
    case 4: return frontier_text("Trapper Sigil", "트래퍼 인장");
    case 5: return frontier_text("Blackwater Peacebreaker", "블랙워터 피스브레이커");
    case 6: return frontier_text("Trophy Board", "트로피 보드");
    case 7: return frontier_text("Relic Binding Kit", "유물 결속 키트");
    case 8: return frontier_text("Frontier Tonic Pack", "프론티어 강장팩");
    case 9: return frontier_text("Gunsmith Tool Roll", "총포상 공구 롤");
    default: return frontier_text("Deadeye Charm", "데드아이 참");
    }
}

static const char *frontier_workbench_recipe_primary_name(int entry_index)
{
    frontier_copy_label_segment(frontier_workbench_recipe_export_label(entry_index), g_runtime.runtime_entry_label, sizeof(g_runtime.runtime_entry_label), 1);
    return g_runtime.runtime_entry_label;
}

static const char *frontier_workbench_recipe_display_label(int entry_index)
{
    frontier_copy_label_segment(frontier_workbench_recipe_export_label(entry_index), g_runtime.runtime_entry_label, sizeof(g_runtime.runtime_entry_label), 3);
    return g_runtime.runtime_entry_label;
}

static const char *frontier_workbench_recipe_effect(int entry_index)
{
    switch (entry_index) {
    case 0:
        return frontier_text("WEAR -4 / MAINT +1", "무기 유지 효율을 올리고 일상 전투의 열화를 줄입니다.");
    case 1:
        return frontier_text("GRIT +1 / STAM +5", "근성과 스태미나 유지력을 소폭 올립니다.");
    case 2:
        return frontier_text("WORK +1 / RUN", "이번 런 전체에 작업대 티어 보너스를 더합니다.");
    case 3:
        return frontier_text("ARM +1 / WEAR -6", "병기 라인을 강화하고 무기 열화를 더 늦춥니다.");
    case 4:
        return frontier_text("HUNT +1 / HIDE +2", "사냥, 가죽 회수, 함정 수급을 강화합니다.");
    case 5:
        return frontier_text("DE +6 / ARM +1", "데드아이 압력과 고유 무기 준비도를 올립니다.");
    case 6:
        return frontier_text("HUB +1 / WORK +1", "기명 처치를 거점 운영 보너스로 전환합니다.");
    case 7:
        return frontier_text("RELIC +2 / DE +8", "유물 루트와 후반 데드아이 지원을 강화합니다.");
    case 8:
        return frontier_text("MED +1 / STAM +4", "비축 강장제를 스태미나와 보급 효율로 전환합니다.");
    case 9:
        return frontier_text("SMITH +1 / WEAR -5", "총포상 숙련, 정비, 조작성 보너스를 올립니다.");
    default:
        return frontier_text("DE +6 / RELIC +1", "유물 연동 부적으로 데드아이 여유를 더합니다.");
    }
}

static const char *frontier_workbench_recipe_materials(int entry_index)
{
    switch (entry_index) {
    case 0:
        return "폐철 3 / 보상 $6";
    case 1:
        return "가죽 2 / 강장 2 / 복귀 1";
    case 2:
        return "작업대 T2 / 폐철 10 / 가죽 4";
    case 3:
        return "작업대 T2 / 폐철 8 / 적 4";
    case 4:
        return "사냥 2 / 동물 3 / 가죽 6";
    case 5:
        return "블랙워터 / 계약 2 / $30";
    case 6:
        return "복귀 2 / 적 4 / 작업대 T2";
    case 7:
        return "유물 준비 / 유물 8 / 단계 1";
    case 8:
        return "보급 T2 / 강장 5 / 복귀 2";
    case 9:
        return "병기 T2 / 폐철 6 / 적 5";
    default:
        return "데드아이 4 / 유물 4 / 보상 $25";
    }
}

static const char *frontier_workbench_recipe_materials_ascii(int entry_index)
{
    switch (entry_index) {
    case 0: return "SALV 3 / $6";
    case 1: return "HIDE 2 / TONIC 2 / HUB 1";
    case 2: return "WORK T2 / SALV 10 / HIDE 4";
    case 3: return "WORK T2 / SALV 8 / ENEMY 4";
    case 4: return "HUNT 2 / ANIMAL 3 / HIDE 6";
    case 5: return "BLACKWATER / CONTRACT 2 / $30";
    case 6: return "HUB 2 / ENEMY 4 / WORK T2";
    case 7: return "RELIC READY / RELIC 8 / PHASE 1";
    case 8: return "MED T2 / TONIC 5 / HUB 2";
    case 9: return "ARM T2 / SALV 6 / ENEMY 5";
    default: return "DE 4 / RELIC 4 / $25";
    }
}

static const char *frontier_workbench_recipe_gate_compact(int entry_index)
{
    if (frontier_live_recipe_crafted(entry_index)) {
        return "이번 런에서 이미 확보했습니다.";
    }
    if (frontier_live_recipe_ready(entry_index)) {
        return "E로 바로 제작할 수 있습니다.";
    }

    switch (entry_index) {
    case 0: return "정비 세트 조건이 아직 부족합니다.";
    case 1: return "메달 제작 조건이 아직 부족합니다.";
    case 2: return "확장 작업대 조건이 아직 부족합니다.";
    case 3: return "프레임 제작 조건이 아직 부족합니다.";
    case 4: return "인장 제작 조건이 아직 부족합니다.";
    case 5: return "블랙워터 제작 조건이 아직 부족합니다.";
    case 6: return "트로피 제작 조건이 아직 부족합니다.";
    case 7: return "유물키트 조건이 아직 부족합니다.";
    case 8: return "강장팩 제작 조건이 아직 부족합니다.";
    case 9: return "공구롤 제작 조건이 아직 부족합니다.";
    default: return "데드아이 제작 조건이 아직 부족합니다.";
    }
}

static const char *frontier_workbench_recipe_gate_ascii(int entry_index)
{
    if (frontier_live_recipe_crafted(entry_index)) {
        return "ALREADY SECURED";
    }
    if (frontier_live_recipe_ready(entry_index)) {
        return "PRESS E TO CRAFT";
    }
    return "MORE FIELD WORK NEEDED";
}

static const char *frontier_workbench_recipe_materials_display(int entry_index)
{
    return frontier_dataset_is_korean()
        ? frontier_workbench_recipe_materials(entry_index)
        : frontier_workbench_recipe_materials_ascii(entry_index);
}

static const char *frontier_workbench_recipe_effect_display(int entry_index)
{
    return frontier_dataset_is_korean()
        ? frontier_workbench_recipe_effect_compact(entry_index)
        : frontier_workbench_recipe_effect_ascii(entry_index);
}

static const char *frontier_workbench_recipe_gate_display(int entry_index)
{
    return frontier_dataset_is_korean()
        ? frontier_workbench_recipe_gate_compact(entry_index)
        : frontier_workbench_recipe_gate_ascii(entry_index);
}

static int frontier_live_recipe_ready(int entry_index)
{
    switch (entry_index) {
    case 0:
        return g_runtime.live_profile.workshop_tier >= 1 &&
            g_runtime.live_profile.hide_stock >= 2 &&
            g_runtime.live_profile.reward_earned >= 6;
    case 1:
        return g_runtime.live_profile.hide_stock >= 2 &&
            g_runtime.live_profile.tonic_stock >= 2 &&
            g_runtime.live_profile.hub_returns >= 1;
    case 2:
        return g_runtime.live_profile.contract_count >= 1 &&
            g_runtime.live_profile.reward_earned >= 20 &&
            g_runtime.live_profile.salvage_stock >= 3;
    case 3:
        return g_runtime.live_profile.workshop_tier >= 2 &&
            g_runtime.live_profile.salvage_stock >= 10 &&
            g_runtime.live_profile.hide_stock >= 4;
    case 4:
        return g_runtime.live_profile.workshop_tier >= 2 &&
            g_runtime.live_profile.discovered_region_count >= 2 &&
            g_runtime.live_profile.animal_kills >= 1 &&
            g_runtime.live_profile.salvage_stock >= 4;
    case 5:
        return g_runtime.live_profile.workshop_tier >= 2 &&
            g_runtime.live_profile.salvage_stock >= 8 &&
            g_runtime.live_profile.enemy_kills >= 4;
    case 6:
        return g_runtime.live_profile.skill_trapping >= 2 &&
            g_runtime.live_profile.animal_kills >= 3 &&
            g_runtime.live_profile.hide_stock >= 6;
    case 7:
        return g_runtime.live_profile.boss_silas_ready &&
            g_runtime.live_profile.workshop_tier >= 2 &&
            g_runtime.live_profile.enemy_kills >= 6;
    case 8:
        return g_runtime.live_profile.boss_blackwater_ready &&
            g_runtime.live_profile.contract_count >= 2 &&
            g_runtime.live_profile.reward_earned >= 30;
    case 9:
        return g_runtime.live_profile.workshop_tier >= 2 &&
            g_runtime.live_profile.hub_returns >= 2 &&
            g_runtime.live_profile.medicine_tier >= 2;
    case 10:
        return g_runtime.live_profile.hub_returns >= 2 &&
            g_runtime.live_profile.enemy_kills >= 4 &&
            g_runtime.live_profile.workshop_tier >= 2;
    case 11:
        return g_runtime.live_profile.boss_relic_ready &&
            g_runtime.live_profile.relic_stock >= 8 &&
            g_runtime.live_profile.phase >= FRONTIER_PHASE_ESCALATION;
    default:
        return g_runtime.live_profile.boss_relic_ready &&
            g_runtime.live_profile.relic_stock >= 6 &&
            g_runtime.live_profile.reward_earned >= 60;
    }
}

static const char *frontier_workbench_recipe_state(int entry_index)
{
    if (frontier_live_recipe_crafted(entry_index)) {
        return frontier_text("Crafted", "제작됨");
    }
    if (frontier_live_recipe_ready(entry_index)) {
        return frontier_text("Ready", "준비");
    }
    return frontier_text("Locked", "잠김");
}

static const char *frontier_workbench_recipe_gate(int entry_index)
{
    if (frontier_live_recipe_crafted(entry_index)) {
        return frontier_text("ALREADY SECURED", "이번 런에서 이미 확보한 제작입니다.");
    }
    if (frontier_live_recipe_ready(entry_index)) {
        return frontier_text("PRESS E TO CRAFT", "[E]로 이 제작을 확정할 수 있습니다.");
    }

    switch (entry_index) {
    case 0:
        return frontier_text("NEED WORK T1 / SALV 3 / $6", "작업대 T1, 폐철 3, 보상 6달러가 필요합니다.");
    case 1:
        return frontier_text("NEED HIDE 2 / TONIC 2 / HUB 1", "가죽 2, 강장제 2, 거점 복귀 1회가 필요합니다.");
    case 2:
        return frontier_text("NEED WORK T2 / SALV 10 / HIDE 4", "작업대 T2, 폐철 10, 가죽 4가 필요합니다.");
    case 3:
        return frontier_text("NEED WORK T2 / SALV 8 / ENEMY 4", "작업대 T2, 폐철 8, 적 처치 4회가 필요합니다.");
    case 4:
        return frontier_text("NEED HUNT 2 / ANIMAL 3 / HIDE 6", "덫/사냥 2단, 동물 3, 가죽 6이 필요합니다.");
    case 5:
        return frontier_text("NEED BLACKWATER / CONTRACT 2 / $30", "블랙워터 준비, 계약 2건, 보상 30달러가 필요합니다.");
    case 6:
        return frontier_text("NEED HUB 2 / ENEMY 4 / WORK T2", "거점 복귀 2회, 적 처치 4회, 작업대 T2가 필요합니다.");
    case 7:
        return frontier_text("NEED RELIC READY / RELIC 8 / PHASE 1", "유물 루트 준비, 유물 8, 확장 단계가 필요합니다.");
    case 8:
        return frontier_text("NEED MED T2 / TONIC 5 / HUB 2", "보급 T2, 강장제 5, 거점 복귀 2회가 필요합니다.");
    case 9:
        return frontier_text("NEED ARM T2 / SALV 6 / ENEMY 5", "병기 T2, 폐철 6, 적 처치 5회가 필요합니다.");
    default:
        return frontier_text("NEED DE 4 / RELIC 4 / $25", "데드아이 4, 유물 4, 보상 25달러가 필요합니다.");
    }
}

static int frontier_crafted_workshop_bonus(void)
{
    int bonus = 0;
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_EXPANDED_WORKSHOP) {
        bonus += 1;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_FIELD_SUPPORT_CABINET) {
        bonus += 1;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_TROPHY_BOARD) {
        bonus += 1;
    }
    return bonus;
}

static int frontier_crafted_armory_bonus(void)
{
    int bonus = 0;
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_BLACKWATER_PRECISION_LOADS) {
        bonus += 1;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_LONG_BARREL_WALKER_KIT) {
        bonus += 1;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_REINFORCED_FRAME) {
        bonus += 1;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_ASH_MARKED_REPEATER) {
        bonus += 1;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_BLACKWATER_PEACEBREAKER) {
        bonus += 1;
    }
    return bonus;
}

static int frontier_crafted_stamina_bonus_percent(void)
{
    int bonus = 0;
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_SAINT_MEDAL) {
        bonus += 5;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_LONG_BARREL_WALKER_KIT) {
        bonus += 2;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_FIELD_SUPPORT_CABINET) {
        bonus += 4;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_TROPHY_BOARD) {
        bonus += 2;
    }
    return bonus;
}

static int frontier_crafted_stamina_drain_bonus_percent(void)
{
    int bonus = 0;
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_SAINT_MEDAL) {
        bonus += 3;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_LONG_BARREL_WALKER_KIT) {
        bonus += 2;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_FIELD_SUPPORT_CABINET) {
        bonus += 2;
    }
    return bonus;
}

static int frontier_crafted_weapon_bonus_percent(void)
{
    int bonus = 0;
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_RELIABLE_GRIP_WRAP) {
        bonus += 4;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_BLACKWATER_PRECISION_LOADS) {
        bonus += 3;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_LONG_BARREL_WALKER_KIT) {
        bonus += 3;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_REINFORCED_FRAME) {
        bonus += 6;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_ASH_MARKED_REPEATER) {
        bonus += 4;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_BLACKWATER_PEACEBREAKER) {
        bonus += 4;
    }
    return bonus;
}

static int frontier_crafted_deadeye_bonus_percent(void)
{
    int bonus = 0;
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_BLACKWATER_PEACEBREAKER) {
        bonus += 6;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_RELIC_BINDING_KIT) {
        bonus += 8;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_RELIC_MARKED_ROUNDS) {
        bonus += 6;
    }
    return bonus;
}
