static void frontier_live_profile_refresh_progression_layers(void)
{
    int field_actions;
    int activity_points;
    int previous_blackwater;
    int previous_silas;
    int previous_relic;
    int previous_abigail;
    int previous_jack;
    int previous_uncle;
    int previous_ranch_threat;
    int previous_ranch_defense;
    int previous_surveyor_clues;
    int previous_sky_devil;

    previous_blackwater = g_runtime.live_profile.boss_blackwater_ready;
    previous_silas = g_runtime.live_profile.boss_silas_ready;
    previous_relic = g_runtime.live_profile.boss_relic_ready;
    previous_abigail = g_runtime.live_profile.abigail_support_rank;
    previous_jack = g_runtime.live_profile.jack_support_rank;
    previous_uncle = g_runtime.live_profile.uncle_support_rank;
    previous_ranch_threat = g_runtime.live_profile.ranch_threat_level;
    previous_ranch_defense = g_runtime.live_profile.ranch_defense_level;
    previous_surveyor_clues = g_runtime.live_profile.surveyor_clue_count;
    previous_sky_devil = g_runtime.live_profile.sky_devil_stage;

    field_actions =
        g_runtime.live_profile.enemy_kills +
        g_runtime.live_profile.loot_count +
        g_runtime.live_profile.animal_kills;
    activity_points = frontier_live_activity_points();

    g_runtime.live_profile.stat_strength = frontier_clamp_int(
        2 + ((g_runtime.live_profile.enemy_kills + (g_runtime.live_profile.hub_returns * 2)) / 4) +
            g_runtime.live_profile.stat_bonus_strength,
        2,
        8
    );
    g_runtime.live_profile.stat_grit = frontier_clamp_int(
        2 + ((g_runtime.live_profile.trail_stage + g_runtime.live_profile.hub_returns + (g_runtime.live_profile.reward_earned / 25)) / 3) +
            g_runtime.live_profile.stat_bonus_grit,
        2,
        8
    );
    g_runtime.live_profile.stat_deadeye = frontier_clamp_int(
        2 + ((g_runtime.live_profile.enemy_kills + g_runtime.live_profile.discovered_region_count + (g_runtime.live_profile.reward_earned / 20)) / 4) +
            g_runtime.live_profile.stat_bonus_deadeye,
        2,
        8
    );
    g_runtime.live_profile.stat_survival = frontier_clamp_int(
        2 + ((g_runtime.live_profile.animal_kills + g_runtime.live_profile.discovered_region_count + ((int)(g_runtime.live_profile.field_distance_total / 900.0f))) / 3) +
            g_runtime.live_profile.stat_bonus_survival,
        2,
        8
    );
    g_runtime.live_profile.stat_cunning = frontier_clamp_int(
        2 + (((g_runtime.live_profile.loot_count * 2) + g_runtime.live_profile.trail_stage + (g_runtime.live_profile.reward_earned / 30)) / 4) +
            g_runtime.live_profile.stat_bonus_cunning,
        2,
        8
    );

    g_runtime.live_profile.salvage_stock = frontier_clamp_int(
        (g_runtime.live_profile.loot_count * 3) + g_runtime.live_profile.enemy_kills + (g_runtime.live_profile.reward_earned / 25),
        0,
        99
    );
    g_runtime.live_profile.hide_stock = frontier_clamp_int(
        (g_runtime.live_profile.animal_kills * 2) + g_runtime.live_profile.discovered_region_count,
        0,
        99
    );
    g_runtime.live_profile.tonic_stock = frontier_clamp_int(
        g_runtime.live_profile.hub_returns + g_runtime.live_profile.stat_grit + (g_runtime.live_profile.reward_earned / 40),
        0,
        99
    );
    g_runtime.live_profile.relic_stock = frontier_clamp_int(
        g_runtime.live_profile.discovered_region_count + g_runtime.live_profile.trail_stage + (g_runtime.live_profile.reward_earned / 60),
        0,
        99
    );
    g_runtime.live_profile.contract_count = frontier_clamp_int(
        (g_runtime.live_profile.reward_earned / 20) + (g_runtime.live_profile.hub_returns / 2) + ((g_runtime.live_profile.trail_stage >= 4) ? 1 : 0),
        0,
        24
    );
    g_runtime.live_profile.weapon_familiarity_rank = frontier_clamp_int(
        2 + ((g_runtime.live_profile.enemy_kills / 2) + (g_runtime.live_profile.animal_kills / 3) + g_runtime.live_profile.trail_stage),
        2,
        10
    );
    g_runtime.live_profile.weapon_maintenance_tier = frontier_clamp_int(
        1 + ((g_runtime.live_profile.salvage_stock + (g_runtime.live_profile.hub_returns * 2)) / 10),
        1,
        3
    );
    g_runtime.live_profile.workshop_tier = frontier_clamp_int(
        1 + ((g_runtime.live_profile.salvage_stock + g_runtime.live_profile.hide_stock + (g_runtime.live_profile.hub_returns * 2)) / 12),
        1,
        4
    );
    g_runtime.live_profile.armory_tier = frontier_clamp_int(
        1 + ((g_runtime.live_profile.weapon_familiarity_rank + g_runtime.live_profile.salvage_stock + g_runtime.live_profile.enemy_kills) / 12),
        1,
        4
    );
    g_runtime.live_profile.medicine_tier = frontier_clamp_int(
        1 + ((g_runtime.live_profile.tonic_stock + g_runtime.live_profile.stat_survival + g_runtime.live_profile.hub_returns) / 10),
        1,
        4
    );

    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_SAINT_MEDAL) {
        g_runtime.live_profile.stat_grit = frontier_clamp_int(g_runtime.live_profile.stat_grit + 1, 2, 8);
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_BLACKWATER_PRECISION_LOADS) {
        g_runtime.live_profile.weapon_maintenance_tier = frontier_clamp_int(g_runtime.live_profile.weapon_maintenance_tier + 1, 1, 3);
        g_runtime.live_profile.contract_count = frontier_clamp_int(g_runtime.live_profile.contract_count + 1, 0, 99);
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_TRAPPER_SIGIL) {
        g_runtime.live_profile.stat_survival = frontier_clamp_int(g_runtime.live_profile.stat_survival + 1, 2, 8);
        g_runtime.live_profile.hide_stock = frontier_clamp_int(g_runtime.live_profile.hide_stock + 2, 0, 99);
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_FIELD_SUPPORT_CABINET) {
        g_runtime.live_profile.tonic_stock = frontier_clamp_int(g_runtime.live_profile.tonic_stock + 3, 0, 99);
        g_runtime.live_profile.medicine_tier = frontier_clamp_int(g_runtime.live_profile.medicine_tier + 1, 1, 4);
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_RELIC_BINDING_KIT) {
        g_runtime.live_profile.stat_deadeye = frontier_clamp_int(g_runtime.live_profile.stat_deadeye + 1, 2, 8);
        g_runtime.live_profile.relic_stock = frontier_clamp_int(g_runtime.live_profile.relic_stock + 2, 0, 99);
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_RELIC_MARKED_ROUNDS) {
        g_runtime.live_profile.stat_deadeye = frontier_clamp_int(g_runtime.live_profile.stat_deadeye + 1, 2, 8);
        g_runtime.live_profile.relic_stock = frontier_clamp_int(g_runtime.live_profile.relic_stock + 1, 0, 99);
    }
    g_runtime.live_profile.weapon_maintenance_tier = frontier_clamp_int(
        g_runtime.live_profile.weapon_maintenance_tier +
            ((g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_RELIABLE_GRIP_WRAP) ? 1 : 0),
        1,
        3
    );
    g_runtime.live_profile.workshop_tier = frontier_clamp_int(
        g_runtime.live_profile.workshop_tier + frontier_crafted_workshop_bonus(),
        1,
        4
    );
    g_runtime.live_profile.armory_tier = frontier_clamp_int(
        g_runtime.live_profile.armory_tier + frontier_crafted_armory_bonus(),
        1,
        4
    );

    g_runtime.live_profile.skill_hunting = frontier_clamp_int(
        1 + ((g_runtime.live_profile.animal_kills + g_runtime.live_profile.discovered_region_count) / 3) +
            g_runtime.live_profile.skill_bonus_hunting,
        1,
        7
    );
    g_runtime.live_profile.skill_gunsmithing = frontier_clamp_int(
        1 + ((g_runtime.live_profile.weapon_familiarity_rank + g_runtime.live_profile.armory_tier + (g_runtime.live_profile.salvage_stock / 6)) / 3) +
            g_runtime.live_profile.skill_bonus_gunsmithing,
        1,
        7
    );
    g_runtime.live_profile.skill_field_medicine = frontier_clamp_int(
        1 + ((g_runtime.live_profile.medicine_tier + g_runtime.live_profile.tonic_stock + g_runtime.live_profile.hub_returns) / 5) +
            g_runtime.live_profile.skill_bonus_field_medicine,
        1,
        7
    );
    g_runtime.live_profile.skill_trapping = frontier_clamp_int(
        1 + ((g_runtime.live_profile.hide_stock + g_runtime.live_profile.animal_kills) / 5) +
            g_runtime.live_profile.skill_bonus_trapping,
        1,
        7
    );
    g_runtime.live_profile.skill_salvaging = frontier_clamp_int(
        1 + ((g_runtime.live_profile.salvage_stock + g_runtime.live_profile.loot_count) / 6) +
            g_runtime.live_profile.skill_bonus_salvaging,
        1,
        7
    );
    g_runtime.live_profile.skill_trade = frontier_clamp_int(
        1 + ((g_runtime.live_profile.contract_count + (g_runtime.live_profile.reward_earned / 30)) / 2) +
            g_runtime.live_profile.skill_bonus_trade,
        1,
        7
    );
    g_runtime.live_profile.skill_horse_handling = frontier_clamp_int(
        1 + (((int)(g_runtime.live_profile.field_distance_total / 1400.0f)) + g_runtime.live_profile.hub_returns) / 2 +
            g_runtime.live_profile.skill_bonus_horse_handling,
        1,
        7
    );
    g_runtime.live_profile.skill_homestead = frontier_clamp_int(
        1 + (g_runtime.live_profile.hub_returns + g_runtime.live_profile.workshop_tier - 1) / 2 +
            g_runtime.live_profile.skill_bonus_homestead,
        1,
        7
    );

    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_TRAPPER_SIGIL) {
        g_runtime.live_profile.skill_hunting = frontier_clamp_int(g_runtime.live_profile.skill_hunting + 1, 1, 7);
        g_runtime.live_profile.skill_trapping = frontier_clamp_int(g_runtime.live_profile.skill_trapping + 1, 1, 7);
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_TROPHY_BOARD) {
        g_runtime.live_profile.skill_homestead = frontier_clamp_int(g_runtime.live_profile.skill_homestead + 1, 1, 7);
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_FIELD_SUPPORT_CABINET) {
        g_runtime.live_profile.skill_field_medicine = frontier_clamp_int(g_runtime.live_profile.skill_field_medicine + 1, 1, 7);
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_BLACKWATER_PRECISION_LOADS) {
        g_runtime.live_profile.skill_gunsmithing = frontier_clamp_int(g_runtime.live_profile.skill_gunsmithing + 1, 1, 7);
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_LONG_BARREL_WALKER_KIT) {
        g_runtime.live_profile.skill_gunsmithing = frontier_clamp_int(g_runtime.live_profile.skill_gunsmithing + 1, 1, 7);
    }

    g_runtime.live_profile.abigail_support_rank = frontier_clamp_int(
        (g_runtime.live_profile.hub_returns + g_runtime.live_profile.medicine_tier + (g_runtime.live_profile.tonic_stock / 3) +
         (g_runtime.live_profile.phase >= FRONTIER_PHASE_ESCALATION ? 1 : 0)) / 3,
        0,
        3
    );
    g_runtime.live_profile.jack_support_rank = frontier_clamp_int(
        (g_runtime.live_profile.discovered_region_count + (g_runtime.live_profile.relic_stock / 2) + g_runtime.live_profile.trail_stage +
         (g_runtime.live_profile.boss_relic_ready ? 2 : 0)) / 4,
        0,
        3
    );
    g_runtime.live_profile.uncle_support_rank = frontier_clamp_int(
        ((g_runtime.live_profile.reward_earned / 20) + g_runtime.live_profile.contract_count + (g_runtime.live_profile.loot_count / 2) +
         (g_runtime.live_profile.boss_blackwater_ready ? 1 : 0)) / 3,
        0,
        3
    );
    g_runtime.live_profile.ranch_threat_level = frontier_clamp_int(
        (g_runtime.live_profile.phase * 2) +
            ((activity_points + (g_runtime.live_profile.discovered_region_count * 2) + (g_runtime.live_profile.trail_stage * 2)) / 12) +
            (g_runtime.live_profile.boss_blackwater_ready ? 1 : 0) +
            (g_runtime.live_profile.boss_silas_ready ? 1 : 0),
        0,
        5
    );
    g_runtime.live_profile.ranch_defense_level = frontier_clamp_int(
        (g_runtime.live_profile.hub_returns + g_runtime.live_profile.workshop_tier + g_runtime.live_profile.armory_tier +
         g_runtime.live_profile.medicine_tier + g_runtime.live_profile.abigail_support_rank +
         g_runtime.live_profile.jack_support_rank + g_runtime.live_profile.uncle_support_rank) / 4,
        0,
        5
    );
    g_runtime.live_profile.surveyor_clue_count = frontier_clamp_int(
        g_runtime.live_profile.discovered_region_count +
            g_runtime.live_profile.contract_count +
            (g_runtime.live_profile.relic_stock / 2) +
            (g_runtime.live_profile.boss_blackwater_ready ? 1 : 0) +
            (g_runtime.live_profile.boss_silas_ready ? 1 : 0),
        0,
        12
    );
    g_runtime.live_profile.sky_devil_stage = frontier_clamp_int(
        (g_runtime.live_profile.phase >= FRONTIER_PHASE_ESCALATION ? 1 : 0) +
            (g_runtime.live_profile.relic_stock >= 6 ? 1 : 0) +
            (g_runtime.live_profile.boss_relic_ready ? 1 : 0),
        0,
        3
    );
    if (g_runtime.live_profile.ranch_defense_level > 0) {
        g_runtime.live_profile.medicine_tier = frontier_clamp_int(
            g_runtime.live_profile.medicine_tier + frontier_min_int(1, g_runtime.live_profile.abigail_support_rank),
            1,
            4
        );
        g_runtime.live_profile.armory_tier = frontier_clamp_int(
            g_runtime.live_profile.armory_tier + frontier_min_int(1, g_runtime.live_profile.uncle_support_rank / 2),
            1,
            4
        );
        g_runtime.live_profile.stat_deadeye = frontier_clamp_int(
            g_runtime.live_profile.stat_deadeye + frontier_min_int(1, g_runtime.live_profile.jack_support_rank / 2),
            2,
            8
        );
    }
    if (g_runtime.live_profile.boss_blackwater_ready || g_runtime.live_profile.boss_silas_ready || g_runtime.live_profile.boss_relic_ready) {
        g_runtime.live_profile.farming_lane = 5;
    } else if (g_runtime.live_profile.hub_returns < 2 || g_runtime.live_profile.abigail_support_rank == 0) {
        g_runtime.live_profile.farming_lane = 4;
    } else if (g_runtime.live_profile.contract_count < 2 || g_runtime.live_profile.uncle_support_rank > g_runtime.live_profile.abigail_support_rank) {
        g_runtime.live_profile.farming_lane = 3;
    } else if (g_runtime.live_profile.enemy_kills > g_runtime.live_profile.animal_kills) {
        g_runtime.live_profile.farming_lane = 2;
    } else {
        g_runtime.live_profile.farming_lane = 1;
    }

    g_runtime.live_profile.elite_pressure = frontier_clamp_int(
        activity_points + (g_runtime.live_profile.discovered_region_count * 2) + (g_runtime.live_profile.trail_stage * 2),
        0,
        99
    );
    g_runtime.live_profile.perk_flags = 0U;
    if (g_runtime.live_profile.field_distance_total >= 500.0f || g_runtime.live_profile.skill_horse_handling >= 2) {
        g_runtime.live_profile.perk_flags |= FRONTIER_PERK_LONG_TRAIL;
    }
    if (g_runtime.live_profile.skill_hunting >= 2 || g_runtime.live_profile.stat_survival >= 4) {
        g_runtime.live_profile.perk_flags |= FRONTIER_PERK_SHARP_TRACKER;
    }
    if (g_runtime.live_profile.stat_deadeye >= 3 && g_runtime.live_profile.enemy_kills >= 3) {
        g_runtime.live_profile.perk_flags |= FRONTIER_PERK_FRONT_SIGHT_FAITH;
    }
    if (g_runtime.live_profile.stat_deadeye >= 4 || g_runtime.live_profile.weapon_familiarity_rank >= 5) {
        g_runtime.live_profile.perk_flags |= FRONTIER_PERK_QUICK_DRAW;
    }
    if (g_runtime.live_profile.loot_count >= 2 || g_runtime.live_profile.skill_salvaging >= 3) {
        g_runtime.live_profile.perk_flags |= FRONTIER_PERK_FAST_POCKETS;
    }
    if (g_runtime.live_profile.skill_homestead >= 2 && g_runtime.live_profile.hub_returns >= 1) {
        g_runtime.live_profile.perk_flags |= FRONTIER_PERK_RANCH_RESOLVE;
    }
    if (g_runtime.live_profile.trail_stage >= 4 &&
        g_runtime.live_profile.discovered_region_count >= 3 &&
        g_runtime.live_profile.relic_stock >= 6) {
        g_runtime.live_profile.perk_flags |= FRONTIER_PERK_ASH_MARK;
    }

    g_runtime.live_profile.boss_blackwater_ready =
        g_runtime.live_profile.trail_stage >= 4 &&
        g_runtime.live_profile.reward_earned >= 20 &&
        g_runtime.live_profile.enemy_kills >= 3;
    g_runtime.live_profile.boss_silas_ready =
        g_runtime.live_profile.trail_stage >= 5 &&
        g_runtime.live_profile.elite_pressure >= 16 &&
        g_runtime.live_profile.weapon_familiarity_rank >= 6;
    g_runtime.live_profile.boss_relic_ready =
        g_runtime.live_profile.phase >= FRONTIER_PHASE_ENDGAME ||
        (g_runtime.live_profile.discovered_region_count >= 5 &&
         g_runtime.live_profile.reward_earned >= 70 &&
         frontier_live_perk_unlocked(FRONTIER_PERK_ASH_MARK));

    if (!previous_blackwater && g_runtime.live_profile.boss_blackwater_ready) {
        frontier_set_status(frontier_text("Blackwater contract lane is now ready.", "블랙워터 계약 라인이 이제 준비되었습니다."));
        frontier_log("Boss readiness unlocked: Blackwater contract lane ready.");
    } else if (!previous_silas && g_runtime.live_profile.boss_silas_ready) {
        frontier_set_status(frontier_text("Silas Redd prep is now live.", "사일러스 레드 준비 단계가 활성화되었습니다."));
        frontier_log("Boss readiness unlocked: Silas Redd prep ready.");
    } else if (!previous_relic && g_runtime.live_profile.boss_relic_ready) {
        frontier_set_status(frontier_text("Relic war route is now open.", "유물 전쟁 루트가 개방되었습니다."));
        frontier_log("Boss readiness unlocked: Relic war route ready.");
    }

    if (g_runtime.live_profile.abigail_support_rank > previous_abigail) {
        frontier_set_status(frontier_text("Abigail support advanced.", "아비게일 지원 단계가 상승했습니다."));
        frontier_log("Family support advanced: Abigail -> %d", g_runtime.live_profile.abigail_support_rank);
    }
    if (g_runtime.live_profile.jack_support_rank > previous_jack) {
        frontier_set_status(frontier_text("Jack archive support advanced.", "잭의 기록 지원 단계가 상승했습니다."));
        frontier_log("Family support advanced: Jack -> %d", g_runtime.live_profile.jack_support_rank);
    }
    if (g_runtime.live_profile.uncle_support_rank > previous_uncle) {
        frontier_set_status(frontier_text("Uncle rumor support advanced.", "엉클의 소문 지원 단계가 상승했습니다."));
        frontier_log("Family support advanced: Uncle -> %d", g_runtime.live_profile.uncle_support_rank);
    }
    if (g_runtime.live_profile.ranch_threat_level >= 3 && previous_ranch_threat < 3) {
        frontier_set_status(frontier_text("Beecher's Hope is now under direct pressure.", "비처스 호프가 직접 압박을 받기 시작했습니다."));
        frontier_log("Ranch threat crossed pressure threshold: %d", g_runtime.live_profile.ranch_threat_level);
    }
    if (g_runtime.live_profile.ranch_defense_level >= 3 && previous_ranch_defense < 3) {
        frontier_set_status(frontier_text("Ranch defenses have stabilized.", "목장 방어 준비가 안정권에 들어왔습니다."));
        frontier_log("Ranch defense crossed stability threshold: %d", g_runtime.live_profile.ranch_defense_level);
    }
    if (g_runtime.live_profile.surveyor_clue_count >= 3 && previous_surveyor_clues < 3) {
        frontier_set_status(frontier_text("Surveyor clue chain has surfaced.", "서베이어 단서선이 드러나기 시작했습니다."));
        frontier_log("Story clue chain advanced: Surveyor clues=%d", g_runtime.live_profile.surveyor_clue_count);
    }
    if (g_runtime.live_profile.sky_devil_stage > previous_sky_devil) {
        frontier_set_status(frontier_text("The sky-devil thread has intensified.", "하늘 악마 괴담 단계가 상승했습니다."));
        frontier_log("Story myth thread advanced: sky_devil_stage=%d", g_runtime.live_profile.sky_devil_stage);
    }

    (void)field_actions;
}

static int frontier_live_bonus_tier(void)
{
    int activity_points = frontier_live_activity_points();
    int field_actions =
        g_runtime.live_profile.enemy_kills +
        g_runtime.live_profile.loot_count +
        g_runtime.live_profile.animal_kills;

    if (g_runtime.live_profile.trail_stage >= 4 &&
        g_runtime.live_profile.hub_returns >= 2 &&
        g_runtime.live_profile.reward_earned >= 40 &&
        field_actions >= 5 &&
        activity_points >= 10 &&
        g_runtime.live_profile.armory_tier >= 2 &&
        g_runtime.live_profile.skill_homestead >= 2) {
        return 2;
    }

    if (g_runtime.live_profile.trail_stage >= 2 &&
        g_runtime.live_profile.hub_returns >= 1 &&
        field_actions >= 2 &&
        activity_points >= 4 &&
        (g_runtime.live_profile.skill_hunting >= 2 ||
         g_runtime.live_profile.skill_salvaging >= 2 ||
         frontier_live_perk_unlocked(FRONTIER_PERK_LONG_TRAIL))) {
        return 1;
    }

    return 0;
}

static int frontier_live_trail_stage_from_state(void)
{
    int activity_points = frontier_live_activity_points();
    int field_actions =
        g_runtime.live_profile.enemy_kills +
        g_runtime.live_profile.loot_count +
        g_runtime.live_profile.animal_kills;

    if (g_runtime.live_profile.hub_returns >= 2 &&
        g_runtime.live_profile.discovered_region_count >= 3 &&
        g_runtime.live_profile.field_distance_total >= 4000.0f &&
        g_runtime.live_profile.enemy_kills >= 4 &&
        g_runtime.live_profile.loot_count >= 3 &&
        g_runtime.live_profile.reward_earned >= 45 &&
        activity_points >= 14 &&
        g_runtime.live_profile.contract_count >= 2) {
        return 5;
    }

    if (g_runtime.live_profile.hub_returns >= 2 &&
        g_runtime.live_profile.discovered_region_count >= 2 &&
        g_runtime.live_profile.field_distance_total >= 2600.0f &&
        g_runtime.live_profile.loot_count >= 2 &&
        field_actions >= 5 &&
        g_runtime.live_profile.reward_earned >= 20 &&
        activity_points >= 9 &&
        g_runtime.live_profile.workshop_tier >= 2) {
        return 4;
    }

    if (g_runtime.live_profile.discovered_region_count >= 2 &&
        g_runtime.live_profile.field_distance_total >= 1200.0f &&
        field_actions >= 3 &&
        (g_runtime.live_profile.enemy_kills + g_runtime.live_profile.animal_kills) >= 2 &&
        activity_points >= 5 &&
        g_runtime.live_profile.salvage_stock >= 4) {
        return 3;
    }

    if (g_runtime.live_profile.hub_returns >= 1 &&
        (field_actions >= 1 || g_runtime.live_profile.reward_earned >= 8) &&
        (g_runtime.live_profile.hide_stock >= 2 || g_runtime.live_profile.salvage_stock >= 2)) {
        return 2;
    }

    if (g_runtime.live_profile.field_distance_total >= 220.0f ||
        (g_runtime.live_profile.field_distance_total >= 120.0f && field_actions >= 1)) {
        return 1;
    }

    return 0;
}

static void frontier_live_profile_refresh_strings(void)
{
    const char *region_label = frontier_runtime_context_label();
    const char *phase_label = frontier_text("Live Phase: Homestead Setup", "실시간 단계: 거점 구축");
    const char *objective = frontier_text(
        "Leave Beecher's Hope, open the first field sweep, and start the blood-marked ledger chain.",
        "비처스 호프를 떠나 첫 현장 수색을 열고 피 묻은 장부 체인을 시작하십시오."
    );
    const char *trail_stage_label = frontier_live_trail_stage_label(g_runtime.live_profile.trail_stage);
    int bonus_tier = frontier_live_bonus_tier();
    int stamina_bonus_percent = 0;
    int stamina_depletion_percent = 0;
    int weapon_bonus_percent = 0;
    int deadeye_bonus_percent = 0;
    int health_bonus_percent = 0;
    int weapon_damage_percent = 0;
    int melee_damage_percent = 0;
    int move_rate_percent = 0;

    g_runtime.live_profile.active_objective[0] = '\0';

    if (g_runtime.live_profile.phase >= FRONTIER_PHASE_ENDGAME) {
        phase_label = frontier_text("Live Phase: Endgame Circuit", "실시간 단계: 엔드게임 회로");
        if (g_runtime.zone_state.inside_zone) {
            objective = frontier_text(
                "Recenter at Beecher's Hope, review the ledger, and prepare the last late-frontier deployment.",
                "비처스 호프에서 장부를 다시 정리하고 마지막 프론티어 투입을 준비하십시오."
            );
        } else {
            objective = frontier_text(
                "Maintain pressure in the late frontier, close the remaining dead-ledger routes, and keep the ranch supplied.",
                "후반 프론티어 압박을 유지하고 남은 죽은 장부 루트를 닫으면서 목장 보급을 유지하십시오."
            );
        }
    } else if (g_runtime.live_profile.phase >= FRONTIER_PHASE_ESCALATION) {
        phase_label = frontier_text("Live Phase: Frontier Escalation", "실시간 단계: 프론티어 격화");
        if (g_runtime.zone_state.inside_zone) {
            objective = frontier_text(
                "Use the ranch hub to convert fresh route intel into contracts, armory work, and boss preparation.",
                "목장 허브에서 새 루트 정보를 계약, 병기 손질, 보스 준비로 전환하십시오."
            );
        } else {
            objective = frontier_text(
                "Push deeper into the active frontier lane, expand the contract circuit, and carry the next report back alive.",
                "활성 프론티어 라인 안쪽으로 더 밀고 들어가 계약 회로를 넓힌 뒤 다음 보고를 들고 돌아오십시오."
            );
        }
    }

    switch (g_runtime.live_profile.trail_stage) {
    case 0:
        objective = frontier_text(
            "Leave Beecher's Hope and start the first field sweep for the blood-marked ledger.",
            "비처스 호프를 떠나 피 묻은 장부를 위한 첫 현장 수색을 시작하십시오."
        );
        break;

    case 1:
        if (g_runtime.zone_state.inside_zone) {
            objective = frontier_text(
                "Reach the ledger table and convert the first field sweep into a real ranch report.",
                "장부 테이블로 돌아와 첫 현장 수색을 실제 목장 보고로 전환하십시오."
            );
        } else {
            objective = frontier_text(
                "Keep moving through the opening frontier lane until the first report is worth bringing home.",
                "첫 보고를 들고 돌아올 가치가 생길 때까지 초반 프론티어 라인을 계속 밀어붙이십시오."
            );
        }
        break;

    case 2:
        objective = frontier_text(
            "Push out of West Elizabeth and log the second pressure lane toward Great Plains / Tall Trees.",
            "웨스트 엘리자베스 바깥으로 나가 그레이트플레인/톨트리스 방향의 두 번째 압박 라인을 기록하십시오."
        );
        break;

    case 3:
        if (region_label != NULL && region_label[0] != '\0') {
            snprintf(
                g_runtime.live_profile.active_objective,
                sizeof(g_runtime.live_profile.active_objective),
                "%s",
                frontier_dataset_is_korean()
                    ? "그레이트플레인 압박을 현재 지역까지 이어가고 검증된 두 번째 보고를 들고 돌아오십시오."
                    : "Carry Great Plains pressure through the current region and return with a second verified frontier report."
            );
        } else {
            objective = frontier_text(
                "Carry Great Plains pressure back to the ranch and keep the second frontier report clean.",
                "그레이트플레인 압박을 목장까지 끌고 돌아와 두 번째 프론티어 보고를 정리하십시오."
            );
        }
        break;

    case 4:
        objective = frontier_text(
            "Lock the Blackwater contract lane by returning the field circuit to Beecher's Hope one more time.",
            "현장 회로를 한 번 더 비처스 호프로 되돌려 블랙워터 계약 라인을 고정하십시오."
        );
        break;

    case 5:
        objective = frontier_text(
            "Maintain Tall Trees pressure and build enough force to turn the live run into Silas Redd preparation.",
            "톨트리스 압박을 유지하고 실시간 런을 사일러스 레드 준비 단계로 밀어 올리십시오."
        );
        break;

    default:
        break;
    }

    frontier_copy_string(g_runtime.live_profile.phase_label, sizeof(g_runtime.live_profile.phase_label), phase_label);
    if (g_runtime.live_profile.active_objective[0] == '\0') {
        frontier_copy_string(g_runtime.live_profile.active_objective, sizeof(g_runtime.live_profile.active_objective), objective);
    }

    if (frontier_dataset_is_korean()) {
        snprintf(
            g_runtime.live_profile.activity_summary,
            sizeof(g_runtime.live_profile.activity_summary),
            "적 %d | 루팅 %d | 사냥 %d | 보상 $%d",
            g_runtime.live_profile.enemy_kills,
            g_runtime.live_profile.loot_count,
            g_runtime.live_profile.animal_kills,
            g_runtime.live_profile.reward_earned
        );
        snprintf(
            g_runtime.live_profile.farming_summary,
            sizeof(g_runtime.live_profile.farming_summary),
            "사냥 %d | 루팅 %d | 수색 %.0fm | 가죽 %d | 약품 %d",
            g_runtime.live_profile.animal_kills,
            g_runtime.live_profile.loot_count,
            g_runtime.live_profile.field_distance_since_hub,
            g_runtime.live_profile.hide_stock,
            g_runtime.live_profile.tonic_stock
        );
        snprintf(
            g_runtime.live_profile.network_summary,
            sizeof(g_runtime.live_profile.network_summary),
            "보상 $%d | 복귀 %d | 계약 %d | 공방 T%d",
            g_runtime.live_profile.reward_earned,
            g_runtime.live_profile.hub_returns,
            g_runtime.live_profile.contract_count,
            g_runtime.live_profile.workshop_tier
        );
        snprintf(
            g_runtime.live_profile.world_pressure_summary,
            sizeof(g_runtime.live_profile.world_pressure_summary),
            "압력 %d | 지역 %d/6 | 엘리트 %d | 적 %d | 사냥 %d",
            frontier_live_activity_points(),
            g_runtime.live_profile.discovered_region_count,
            g_runtime.live_profile.elite_pressure,
            g_runtime.live_profile.enemy_kills,
            g_runtime.live_profile.animal_kills
        );
    } else {
        snprintf(
            g_runtime.live_profile.activity_summary,
            sizeof(g_runtime.live_profile.activity_summary),
            "Enemy Kills %d | Loot %d | Hunts %d | Rewards $%d | Crafted %d",
            g_runtime.live_profile.enemy_kills,
            g_runtime.live_profile.loot_count,
            g_runtime.live_profile.animal_kills,
            g_runtime.live_profile.reward_earned,
            frontier_live_crafted_recipe_count()
        );
        snprintf(
            g_runtime.live_profile.farming_summary,
            sizeof(g_runtime.live_profile.farming_summary),
            "Hunts %d | Loots %d | Field Sweep %.0fm | Hides %d | Tonics %d",
            g_runtime.live_profile.animal_kills,
            g_runtime.live_profile.loot_count,
            g_runtime.live_profile.field_distance_since_hub,
            g_runtime.live_profile.hide_stock,
            g_runtime.live_profile.tonic_stock
        );
        snprintf(
            g_runtime.live_profile.network_summary,
            sizeof(g_runtime.live_profile.network_summary),
            "Reward Intake $%d | Hub Returns %d | Contracts %d | Workshop T%d | Crafted %d",
            g_runtime.live_profile.reward_earned,
            g_runtime.live_profile.hub_returns,
            g_runtime.live_profile.contract_count,
            g_runtime.live_profile.workshop_tier,
            frontier_live_crafted_recipe_count()
        );
        snprintf(
            g_runtime.live_profile.world_pressure_summary,
            sizeof(g_runtime.live_profile.world_pressure_summary),
            "Pressure %d | Regions %d/6 | Elite %d | Enemy %d | Hunts %d | Loot %d",
            frontier_live_activity_points(),
            g_runtime.live_profile.discovered_region_count,
            g_runtime.live_profile.elite_pressure,
            g_runtime.live_profile.enemy_kills,
            g_runtime.live_profile.animal_kills,
            g_runtime.live_profile.loot_count
        );
    }
    if (frontier_dataset_is_korean() &&
        (bonus_tier > 0 || deadeye_bonus_percent > 0 || health_bonus_percent > 0 || weapon_damage_percent > 0 || melee_damage_percent > 0 || move_rate_percent > 0)) {
        snprintf(
            g_runtime.live_profile.bonus_summary,
            sizeof(g_runtime.live_profile.bonus_summary),
            "T%d | STA +%d/-%d | WEAR -%d | DE +%d | HP +%d | DMG +%d | MEL +%d | MOVE +%d",
            bonus_tier,
            stamina_bonus_percent,
            stamina_depletion_percent,
            weapon_bonus_percent,
            deadeye_bonus_percent,
            health_bonus_percent,
            weapon_damage_percent,
            melee_damage_percent,
            move_rate_percent
        );
    }

    if (frontier_dataset_is_korean()) {
        snprintf(
            g_runtime.live_profile.homefront_summary,
            sizeof(g_runtime.live_profile.homefront_summary),
            "아비%d 잭%d 엉클%d | 위협%d / 방어%d",
            g_runtime.live_profile.abigail_support_rank,
            g_runtime.live_profile.jack_support_rank,
            g_runtime.live_profile.uncle_support_rank,
            g_runtime.live_profile.ranch_threat_level,
            g_runtime.live_profile.ranch_defense_level
        );
        snprintf(
            g_runtime.live_profile.farming_summary,
            sizeof(g_runtime.live_profile.farming_summary),
            "%s | 사냥 %d | 루팅 %d | 가죽 %d",
            frontier_farming_lane_label(g_runtime.live_profile.farming_lane),
            g_runtime.live_profile.animal_kills,
            g_runtime.live_profile.loot_count,
            g_runtime.live_profile.hide_stock
        );
        snprintf(
            g_runtime.live_profile.network_summary,
            sizeof(g_runtime.live_profile.network_summary),
            "보상 $%d | 복귀 %d | 계약 %d | 엉클 %s",
            g_runtime.live_profile.reward_earned,
            g_runtime.live_profile.hub_returns,
            g_runtime.live_profile.contract_count,
            frontier_family_rank_label(g_runtime.live_profile.uncle_support_rank)
        );
        snprintf(
            g_runtime.live_profile.world_pressure_summary,
            sizeof(g_runtime.live_profile.world_pressure_summary),
            "압력 %d | 단서%d | 하늘%d | 적%d | 사냥%d",
            frontier_live_activity_points(),
            g_runtime.live_profile.surveyor_clue_count,
            g_runtime.live_profile.sky_devil_stage,
            g_runtime.live_profile.enemy_kills,
            g_runtime.live_profile.animal_kills
        );
        snprintf(
            g_runtime.live_profile.ending_summary,
            sizeof(g_runtime.live_profile.ending_summary),
            "%s | %s | 위협%d / 방어%d",
            frontier_story_clue_label(g_runtime.live_profile.surveyor_clue_count),
            frontier_sky_devil_label(g_runtime.live_profile.sky_devil_stage),
            g_runtime.live_profile.ranch_threat_level,
            g_runtime.live_profile.ranch_defense_level
        );
    } else {
        snprintf(
            g_runtime.live_profile.homefront_summary,
            sizeof(g_runtime.live_profile.homefront_summary),
            "Abigail %s | Jack %s | Uncle %s | Threat %d / Defense %d",
            frontier_family_rank_label(g_runtime.live_profile.abigail_support_rank),
            frontier_family_rank_label(g_runtime.live_profile.jack_support_rank),
            frontier_family_rank_label(g_runtime.live_profile.uncle_support_rank),
            g_runtime.live_profile.ranch_threat_level,
            g_runtime.live_profile.ranch_defense_level
        );
        snprintf(
            g_runtime.live_profile.ending_summary,
            sizeof(g_runtime.live_profile.ending_summary),
            "%s | %s | Threat %d / Defense %d",
            frontier_story_clue_label(g_runtime.live_profile.surveyor_clue_count),
            frontier_sky_devil_label(g_runtime.live_profile.sky_devil_stage),
            g_runtime.live_profile.ranch_threat_level,
            g_runtime.live_profile.ranch_defense_level
        );
    }

    stamina_bonus_percent = g_runtime.config.enable_live_stamina_bonus
        ? frontier_clamp_int(
            (int)((g_runtime.config.stamina_recharge_bonus_per_tier * 100.0f * (float)bonus_tier) +
                  (float)((g_runtime.live_profile.stat_grit - 2) * 2) +
                  (float)(g_runtime.live_profile.abigail_support_rank * 2) +
                  (float)frontier_max_int(0, g_runtime.live_profile.ranch_defense_level - g_runtime.live_profile.ranch_threat_level) +
                  (float)frontier_crafted_stamina_bonus_percent()),
            0,
            50
        )
        : 0;
    stamina_depletion_percent = g_runtime.config.enable_live_stamina_bonus
        ? frontier_clamp_int(
            (int)((g_runtime.config.stamina_sprint_depletion_bonus_per_tier * 100.0f * (float)bonus_tier) +
                  (float)(frontier_live_perk_unlocked(FRONTIER_PERK_RANCH_RESOLVE) ? 3 : 0) +
                  (float)(g_runtime.live_profile.abigail_support_rank) +
                  (float)frontier_crafted_stamina_drain_bonus_percent()),
            0,
            40
        )
        : 0;
    weapon_bonus_percent = g_runtime.config.enable_live_weapon_wear_bonus
        ? frontier_clamp_int(
            (int)((g_runtime.config.weapon_degradation_bonus_per_tier * 100.0f * (float)bonus_tier) +
                  (float)((g_runtime.live_profile.armory_tier - 1) * 3) +
                  (float)(g_runtime.live_profile.uncle_support_rank * 2) +
                  (float)frontier_crafted_weapon_bonus_percent()),
            0,
            40
        )
        : 0;
    deadeye_bonus_percent = g_runtime.config.enable_live_deadeye_bonus
        ? frontier_clamp_int(
            (int)((g_runtime.config.deadeye_delay_bonus_per_tier * 100.0f * (float)bonus_tier) +
                  (float)((g_runtime.live_profile.stat_deadeye - 2) * 4) +
                  (float)(g_runtime.live_profile.jack_support_rank * 2) +
                  (float)(g_runtime.live_profile.sky_devil_stage * 2) +
                  (float)(g_runtime.live_profile.surveyor_clue_count / 3) +
                  (float)frontier_crafted_deadeye_bonus_percent()),
            0,
            50
        )
        : 0;
    health_bonus_percent = frontier_clamp_int(
        ((g_runtime.live_profile.stat_grit - 2) * 4) +
            (g_runtime.live_profile.medicine_tier * 6) +
            (g_runtime.live_profile.abigail_support_rank * 6),
        0,
        55
    );
    weapon_damage_percent = frontier_clamp_int(
        ((g_runtime.live_profile.stat_deadeye - 2) * 4) +
            ((g_runtime.live_profile.skill_gunsmithing - 1) * 4) +
            (frontier_live_perk_unlocked(FRONTIER_PERK_QUICK_DRAW) ? 4 : 0) +
            (frontier_live_perk_unlocked(FRONTIER_PERK_ASH_MARK) ? 6 : 0),
        0,
        45
    );
    melee_damage_percent = frontier_clamp_int(
        ((g_runtime.live_profile.stat_strength - 2) * 5) +
            (g_runtime.live_profile.ranch_defense_level * 3) +
            (frontier_live_perk_unlocked(FRONTIER_PERK_RANCH_RESOLVE) ? 5 : 0),
        0,
        45
    );
    move_rate_percent = frontier_clamp_int(
        ((g_runtime.live_profile.skill_horse_handling - 1) * 4) +
            ((g_runtime.live_profile.stat_survival - 2) * 3) +
            (frontier_live_perk_unlocked(FRONTIER_PERK_LONG_TRAIL) ? 5 : 0),
        0,
        18
    );

    if (bonus_tier > 0 || deadeye_bonus_percent > 0) {
        if (frontier_dataset_is_korean()) {
            snprintf(
                g_runtime.live_profile.bonus_summary,
                sizeof(g_runtime.live_profile.bonus_summary),
                "런 T%d | 스태 +%d/-%d | 열화 -%d | 데드아이 +%d",
                bonus_tier,
                stamina_bonus_percent,
                stamina_depletion_percent,
                weapon_bonus_percent,
                deadeye_bonus_percent
            );
        } else {
            snprintf(
                g_runtime.live_profile.bonus_summary,
                sizeof(g_runtime.live_profile.bonus_summary),
                "Run Bonus T%d | Stamina +%d%% / -%d%% | Wear -%d%% | Deadeye +%d%% | Health +%d%% | Damage +%d%% | Melee +%d%% | Move +%d%%",
                bonus_tier,
                stamina_bonus_percent,
                stamina_depletion_percent,
                weapon_bonus_percent,
                deadeye_bonus_percent,
                health_bonus_percent,
                weapon_damage_percent,
                melee_damage_percent,
                move_rate_percent
            );
        }
    } else {
        frontier_copy_string(
            g_runtime.live_profile.bonus_summary,
            sizeof(g_runtime.live_profile.bonus_summary),
            frontier_text("Run Bonus T0 | No live gameplay bonus active yet.", "런 T0 | 보너스 없음")
        );
    }

    if (frontier_dataset_is_korean()) {
        snprintf(
            g_runtime.live_profile.progress_summary,
            sizeof(g_runtime.live_profile.progress_summary),
            "%s | 지%d/6 | 허브 %d | %.1fkm | 적 %d | 사냥 %d | 루팅 %d | $%d",
            frontier_live_trail_stage_compact_label(g_runtime.live_profile.trail_stage),
            g_runtime.live_profile.discovered_region_count,
            g_runtime.live_profile.hub_returns,
            g_runtime.live_profile.field_distance_total / 1000.0f,
            g_runtime.live_profile.enemy_kills,
            g_runtime.live_profile.animal_kills,
            g_runtime.live_profile.loot_count,
            g_runtime.live_profile.reward_earned
        );
    } else {
        snprintf(
            g_runtime.live_profile.progress_summary,
            sizeof(g_runtime.live_profile.progress_summary),
            "%s | Regions %d/6 | Hub Returns %d | Field Distance %.1fk | Enemy %d | Loot %d | Hunts %d | Reward $%d",
            trail_stage_label,
            g_runtime.live_profile.discovered_region_count,
            g_runtime.live_profile.hub_returns,
            g_runtime.live_profile.field_distance_total / 1000.0f,
            g_runtime.live_profile.enemy_kills,
            g_runtime.live_profile.loot_count,
            g_runtime.live_profile.animal_kills,
            g_runtime.live_profile.reward_earned
        );
    }
}

static const char *frontier_runtime_phase_label(void)
{
    return g_runtime.live_profile.phase_label[0] != '\0' ? g_runtime.live_profile.phase_label : "Live Phase: Homestead Setup";
}

static const char *frontier_runtime_progress_summary(void)
{
    return g_runtime.live_profile.progress_summary[0] != '\0' ? g_runtime.live_profile.progress_summary : "REG 0/6 | HUB 0 | DIST 0.0k";
}

static const char *frontier_runtime_progress_summary_compact(void)
{
    snprintf(
        g_runtime.runtime_summary_buffer,
        sizeof(g_runtime.runtime_summary_buffer),
        frontier_dataset_is_korean()
            ? "단계 %d | 지역 %d/6 | 복귀 %d | %.1fKM"
            : "STAGE %d | REG %d/6 | HUB %d | %.1fKM",
        g_runtime.live_profile.trail_stage,
        g_runtime.live_profile.discovered_region_count,
        g_runtime.live_profile.hub_returns,
        g_runtime.live_profile.field_distance_total / 1000.0f
    );
    return g_runtime.runtime_summary_buffer;
}

static const char *frontier_runtime_objective(void)
{
    return g_runtime.live_profile.active_objective[0] != '\0'
        ? g_runtime.live_profile.active_objective
        : "Scout the next lane. Bring the report home. Hold Beecher's Hope.";
}

static const char *frontier_runtime_objective_compact(void)
{
    if (!frontier_dataset_is_korean()) {
        return frontier_runtime_objective();
    }

    switch (g_runtime.live_profile.trail_stage) {
    case 1:
        return "첫 수색 보고를 채우고 목장으로 되돌리십시오.";
    case 2:
        return "그레이트플레인 라인을 기록하십시오.";
    case 3:
        return "압박을 목장에 가져와 두 번째 보고를 정리하십시오.";
    case 4:
        return "블랙워터 계약 라인을 고정하십시오.";
    case 5:
        return "톨트리스 압박을 유지해 실라스 준비를 올리십시오.";
    default:
        return "비처스 호프를 떠나 첫 수색을 시작하십시오.";
    }
}

static const char *frontier_live_perk_state_text(uint32_t flag)
{
    return frontier_live_perk_unlocked(flag)
        ? frontier_text("Unlocked", "해금")
        : frontier_text("Locked", "잠금");
}

static const char *frontier_live_boss_state_text(int ready)
{
    return ready
        ? frontier_text("Ready", "준비")
        : frontier_text("Tracking", "추적 중");
}

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
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_BLACKWATER_PRECISION_LOADS) {
        bonus += 2;
    }
    if (g_runtime.live_profile.crafted_mask & FRONTIER_CRAFT_ASH_MARKED_REPEATER) {
        bonus += 4;
    }
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
    count += frontier_clamp_int(g_runtime.live_profile.relic_stock / 2, 0, 4) >= 4;
    count += frontier_clamp_int(g_runtime.live_profile.hide_stock / 2, 0, 4) >= 4;
    count += frontier_clamp_int(g_runtime.live_profile.contract_count, 0, 3) >= 3;
    count += frontier_clamp_int(g_runtime.live_profile.hub_returns, 0, 3) >= 3;
    count += frontier_clamp_int(g_runtime.live_profile.discovered_region_count, 0, 4) >= 4;
    return count;
}

static int frontier_live_default_page_index(void)
{
    int page_index = frontier_find_page_index("trails");

    if (!frontier_runtime_uses_live_dataset()) {
        return -1;
    }

    if (g_runtime.live_profile.phase >= FRONTIER_PHASE_ENDGAME) {
        page_index = frontier_find_page_index("ending");
    } else if (g_runtime.live_profile.phase >= FRONTIER_PHASE_ESCALATION) {
        page_index = frontier_find_page_index("frontier_map");
    } else if (g_runtime.zone_state.inside_zone) {
        page_index = frontier_find_page_index("homefront");
    }

    return page_index;
}

static const char *frontier_live_entry_label(
    const FrontierLedgerPage *page,
    int entry_index,
    const FrontierLedgerEntry *entry
)
{
    if (!frontier_runtime_uses_live_dataset() || page == NULL || entry == NULL) {
        return entry != NULL ? entry->label : "";
    }

    if (frontier_dataset_is_korean()) {
        if (entry_index == 0 && strcmp(page->page_id, "overview") == 0) {
            snprintf(
                g_runtime.runtime_entry_label,
                sizeof(g_runtime.runtime_entry_label),
                "실시간 런 | %s",
                frontier_live_trail_stage_compact_label(g_runtime.live_profile.trail_stage)
            );
            return g_runtime.runtime_entry_label;
        }

        if (entry_index == 0 && strcmp(page->page_id, "trails") == 0) {
            snprintf(
                g_runtime.runtime_entry_label,
                sizeof(g_runtime.runtime_entry_label),
                "목표 | %s | %s",
                frontier_live_trail_stage_compact_label(g_runtime.live_profile.trail_stage),
                frontier_runtime_context_label()
            );
            return g_runtime.runtime_entry_label;
        }

        if (entry_index == 0 && strcmp(page->page_id, "world") == 0) {
            snprintf(
                g_runtime.runtime_entry_label,
                sizeof(g_runtime.runtime_entry_label),
                "압력 %d | 지역 %d/6",
                frontier_live_activity_points(),
                g_runtime.live_profile.discovered_region_count
            );
            return g_runtime.runtime_entry_label;
        }

        if (entry_index == 0 && strcmp(page->page_id, "farming") == 0) {
            snprintf(
                g_runtime.runtime_entry_label,
                sizeof(g_runtime.runtime_entry_label),
                "사냥 %d | 루팅 %d | %.0fm",
                g_runtime.live_profile.animal_kills,
                g_runtime.live_profile.loot_count,
                g_runtime.live_profile.field_distance_since_hub
            );
            return g_runtime.runtime_entry_label;
        }

        if (entry_index == 0 && strcmp(page->page_id, "network") == 0) {
            snprintf(
                g_runtime.runtime_entry_label,
                sizeof(g_runtime.runtime_entry_label),
                "보상 $%d | 복귀 %d",
                g_runtime.live_profile.reward_earned,
                g_runtime.live_profile.hub_returns
            );
            return g_runtime.runtime_entry_label;
        }

        if (entry_index == 0 && strcmp(page->page_id, "ending") == 0) {
            snprintf(
                g_runtime.runtime_entry_label,
                sizeof(g_runtime.runtime_entry_label),
                "엔딩 | %s",
                frontier_live_trail_stage_compact_label(g_runtime.live_profile.trail_stage)
            );
            return g_runtime.runtime_entry_label;
        }
    }

    if (strcmp(page->page_id, "stats") == 0) {
        return frontier_live_stat_action_label(entry_index);
    }
    if (strcmp(page->page_id, "skills") == 0) {
        return frontier_live_skill_action_label(entry_index);
    }
    if (strcmp(page->page_id, "perks") == 0) {
        return frontier_live_perk_label(entry_index);
    }
    if (strcmp(page->page_id, "crafting") == 0) {
        return frontier_live_crafting_label_safe(entry_index);
    }
    if (strcmp(page->page_id, "armory") == 0) {
        return frontier_live_armory_label(entry_index);
    }
    if (strcmp(page->page_id, "dead") == 0) {
        return frontier_live_dead_label_synced(entry_index);
    }
    if (strcmp(page->page_id, "collection") == 0) {
        return frontier_live_collection_label(entry_index);
    }
    if (strcmp(page->page_id, "gear") == 0) {
        return frontier_live_gear_label(entry_index);
    }

    if (entry_index == 0 && strcmp(page->page_id, "overview") == 0) {
        snprintf(
            g_runtime.runtime_entry_label,
            sizeof(g_runtime.runtime_entry_label),
            "%s | %s",
            frontier_text("Live Frontier Run", "실시간 프론티어 런"),
            frontier_runtime_phase_label()
        );
        return g_runtime.runtime_entry_label;
    }

    if (entry_index == 0 && strcmp(page->page_id, "trails") == 0) {
        snprintf(
            g_runtime.runtime_entry_label,
            sizeof(g_runtime.runtime_entry_label),
            "%s | %s | %s",
            frontier_text("Live Objective", "실시간 목표"),
            frontier_live_trail_stage_label(g_runtime.live_profile.trail_stage),
            frontier_runtime_context_label()
        );
        return g_runtime.runtime_entry_label;
    }

    if (entry_index == 0 && strcmp(page->page_id, "homefront") == 0) {
        snprintf(
            g_runtime.runtime_entry_label,
            sizeof(g_runtime.runtime_entry_label),
            "%s | %s %d / %s %d",
            frontier_text("Homefront", "거점"),
            frontier_text("Threat", "위협"),
            g_runtime.live_profile.ranch_threat_level,
            frontier_text("Defense", "방어"),
            g_runtime.live_profile.ranch_defense_level
        );
        return g_runtime.runtime_entry_label;
    }

    if (entry_index == 0 && strcmp(page->page_id, "world") == 0) {
        snprintf(
            g_runtime.runtime_entry_label,
            sizeof(g_runtime.runtime_entry_label),
            "%s | %s %d | %s %d/6",
            frontier_text("Live World State", "실시간 월드 상태"),
            frontier_text("Pressure", "압력"),
            frontier_live_activity_points(),
            frontier_text("Regions", "지역"),
            g_runtime.live_profile.discovered_region_count
        );
        return g_runtime.runtime_entry_label;
    }

    if (entry_index == 0 && strcmp(page->page_id, "farming") == 0) {
        snprintf(
            g_runtime.runtime_entry_label,
            sizeof(g_runtime.runtime_entry_label),
            "%s | %s | %s %d",
            frontier_text("Live Farming Sweep", "실시간 농장 스윕"),
            frontier_farming_lane_label(g_runtime.live_profile.farming_lane),
            frontier_text("Hunts", "사냥"),
            g_runtime.live_profile.animal_kills
        );
        return g_runtime.runtime_entry_label;
    }

    if (entry_index == 0 && strcmp(page->page_id, "network") == 0) {
        snprintf(
            g_runtime.runtime_entry_label,
            sizeof(g_runtime.runtime_entry_label),
            "%s | %s $%d",
            frontier_text("Live Network Intake", "실시간 네트워크 수입"),
            frontier_text("Reward", "보상"),
            g_runtime.live_profile.reward_earned
        );
        return g_runtime.runtime_entry_label;
    }

    if (entry_index == 0 && strcmp(page->page_id, "ending") == 0) {
        snprintf(
            g_runtime.runtime_entry_label,
            sizeof(g_runtime.runtime_entry_label),
            "%s | %s | %s",
            frontier_text("Live Ending Route", "실시간 엔딩 루트"),
            frontier_story_clue_label(g_runtime.live_profile.surveyor_clue_count),
            frontier_sky_devil_label(g_runtime.live_profile.sky_devil_stage)
        );
        return g_runtime.runtime_entry_label;
    }

    return entry->label;
}

static const char *frontier_live_page_summary(const FrontierLedgerPage *page)
{
    if (!frontier_runtime_uses_live_dataset() || page == NULL || page->summary == NULL) {
        return page != NULL && page->summary != NULL ? page->summary : "";
    }

    if (strcmp(page->page_id, "homefront") == 0) {
        snprintf(
            g_runtime.runtime_page_summary,
            sizeof(g_runtime.runtime_page_summary),
            "%s ~n~ %s",
            g_runtime.live_profile.homefront_summary,
            frontier_runtime_objective()
        );
        return g_runtime.runtime_page_summary;
    }

    if (strcmp(page->page_id, "farming") == 0) {
        snprintf(
            g_runtime.runtime_page_summary,
            sizeof(g_runtime.runtime_page_summary),
            "%s ~n~ %s",
            g_runtime.live_profile.farming_summary,
            frontier_runtime_objective()
        );
        return g_runtime.runtime_page_summary;
    }

    if (strcmp(page->page_id, "ending") == 0) {
        snprintf(
            g_runtime.runtime_page_summary,
            sizeof(g_runtime.runtime_page_summary),
            "%s ~n~ %s",
            g_runtime.live_profile.ending_summary,
            frontier_runtime_objective()
        );
        return g_runtime.runtime_page_summary;
    }

    if (frontier_dataset_is_korean()) {
        if (strcmp(page->page_id, "overview") == 0) {
            snprintf(
                g_runtime.runtime_page_summary,
                sizeof(g_runtime.runtime_page_summary),
                "%s ~n~ %s",
                g_runtime.live_profile.progress_summary,
                g_runtime.live_profile.bonus_summary
            );
            return g_runtime.runtime_page_summary;
        }

        if (strcmp(page->page_id, "trails") == 0) {
            snprintf(
                g_runtime.runtime_page_summary,
                sizeof(g_runtime.runtime_page_summary),
                "%s ~n~ 앵커 %s | 복귀 %d",
                frontier_live_trail_stage_label(g_runtime.live_profile.trail_stage),
                frontier_runtime_context_label(),
                g_runtime.live_profile.hub_returns
            );
            return g_runtime.runtime_page_summary;
        }

        if (strcmp(page->page_id, "frontier_map") == 0) {
            snprintf(
                g_runtime.runtime_page_summary,
                sizeof(g_runtime.runtime_page_summary),
                "앵커 %s ~n~ 지역 %d/6 | 이동 %.1fkm",
                frontier_runtime_context_label(),
                g_runtime.live_profile.discovered_region_count,
                g_runtime.live_profile.field_distance_total / 1000.0f
            );
            return g_runtime.runtime_page_summary;
        }

        if (strcmp(page->page_id, "homefront") == 0) {
            snprintf(
                g_runtime.runtime_page_summary,
                sizeof(g_runtime.runtime_page_summary),
                "복귀 %d ~n~ 작업 T%d | 병기 T%d | 의약 T%d",
                g_runtime.live_profile.hub_returns,
                g_runtime.live_profile.workshop_tier,
                g_runtime.live_profile.armory_tier,
                g_runtime.live_profile.medicine_tier
            );
            return g_runtime.runtime_page_summary;
        }

        if (strcmp(page->page_id, "stats") == 0) {
            snprintf(
                g_runtime.runtime_page_summary,
                sizeof(g_runtime.runtime_page_summary),
                "근%d 근성%d 데%d ~n~ 생%d 기%d",
                g_runtime.live_profile.stat_strength,
                g_runtime.live_profile.stat_grit,
                g_runtime.live_profile.stat_deadeye,
                g_runtime.live_profile.stat_survival,
                g_runtime.live_profile.stat_cunning
            );
            return g_runtime.runtime_page_summary;
        }

        if (strcmp(page->page_id, "skills") == 0) {
            snprintf(
                g_runtime.runtime_page_summary,
                sizeof(g_runtime.runtime_page_summary),
                "사냥%d 회수%d 거래%d ~n~ 거점%d 총포%d 의약%d",
                g_runtime.live_profile.skill_hunting,
                g_runtime.live_profile.skill_salvaging,
                g_runtime.live_profile.skill_trade,
                g_runtime.live_profile.skill_homestead,
                g_runtime.live_profile.skill_gunsmithing,
                g_runtime.live_profile.skill_field_medicine
            );
            return g_runtime.runtime_page_summary;
        }

        if (strcmp(page->page_id, "perks") == 0) {
            snprintf(
                g_runtime.runtime_page_summary,
                sizeof(g_runtime.runtime_page_summary),
                "퍽 %d/7 ~n~ %s",
                frontier_live_unlocked_perk_count(),
                g_runtime.live_profile.bonus_summary
            );
            return g_runtime.runtime_page_summary;
        }

        if (strcmp(page->page_id, "crafting") == 0) {
            snprintf(
                g_runtime.runtime_page_summary,
                sizeof(g_runtime.runtime_page_summary),
                "제작 %d/8 ~n~ 회수 %d | 가죽 %d | 유물 %d",
                frontier_live_ready_recipe_count(),
                g_runtime.live_profile.salvage_stock,
                g_runtime.live_profile.hide_stock,
                g_runtime.live_profile.relic_stock
            );
            return g_runtime.runtime_page_summary;
        }

        if (strcmp(page->page_id, "armory") == 0) {
            snprintf(
                g_runtime.runtime_page_summary,
                sizeof(g_runtime.runtime_page_summary),
                "병기 T%d | 숙련 %d ~n~ 손질 T%d",
                g_runtime.live_profile.armory_tier,
                g_runtime.live_profile.weapon_familiarity_rank,
                g_runtime.live_profile.weapon_maintenance_tier
            );
            return g_runtime.runtime_page_summary;
        }

        if (strcmp(page->page_id, "dead") == 0) {
            snprintf(
                g_runtime.runtime_page_summary,
                sizeof(g_runtime.runtime_page_summary),
                "보스 %d/3 | 압력 %d ~n~ 계약 %d",
                frontier_live_ready_boss_count(),
                g_runtime.live_profile.elite_pressure,
                g_runtime.live_profile.contract_count
            );
            return g_runtime.runtime_page_summary;
        }

        if (strcmp(page->page_id, "collection") == 0) {
            snprintf(
                g_runtime.runtime_page_summary,
                sizeof(g_runtime.runtime_page_summary),
                "완료 %d/5 ~n~ 표식 %d | 지역 %d",
                frontier_live_completed_collection_count(),
                g_runtime.live_profile.contract_count,
                g_runtime.live_profile.discovered_region_count
            );
            return g_runtime.runtime_page_summary;
        }

        if (strcmp(page->page_id, "gear") == 0) {
            snprintf(
                g_runtime.runtime_page_summary,
                sizeof(g_runtime.runtime_page_summary),
                "작업 T%d | 의약 T%d ~n~ 복귀 %d",
                g_runtime.live_profile.workshop_tier,
                g_runtime.live_profile.medicine_tier,
                g_runtime.live_profile.hub_returns
            );
            return g_runtime.runtime_page_summary;
        }

        if (strcmp(page->page_id, "farming") == 0) {
            snprintf(
                g_runtime.runtime_page_summary,
                sizeof(g_runtime.runtime_page_summary),
                "%s ~n~ 앵커 %s",
                g_runtime.live_profile.farming_summary,
                frontier_runtime_context_label()
            );
            return g_runtime.runtime_page_summary;
        }

        if (strcmp(page->page_id, "network") == 0) {
            snprintf(
                g_runtime.runtime_page_summary,
                sizeof(g_runtime.runtime_page_summary),
                "%s ~n~ %s",
                g_runtime.live_profile.network_summary,
                g_runtime.live_profile.bonus_summary
            );
            return g_runtime.runtime_page_summary;
        }

        if (strcmp(page->page_id, "world") == 0) {
            snprintf(
                g_runtime.runtime_page_summary,
                sizeof(g_runtime.runtime_page_summary),
                "%s ~n~ %s",
                g_runtime.live_profile.world_pressure_summary,
                g_runtime.live_profile.bonus_summary
            );
            return g_runtime.runtime_page_summary;
        }

        if (strcmp(page->page_id, "ending") == 0) {
            snprintf(
                g_runtime.runtime_page_summary,
                sizeof(g_runtime.runtime_page_summary),
                "%s ~n~ 앵커 %s",
                frontier_runtime_phase_label(),
                frontier_runtime_context_label()
            );
            return g_runtime.runtime_page_summary;
        }
    }

    if (strcmp(page->page_id, "overview") == 0) {
        snprintf(
            g_runtime.runtime_page_summary,
            sizeof(g_runtime.runtime_page_summary),
            "%s ~n~ %s ~n~ %s ~n~ %s",
            frontier_runtime_phase_label(),
            frontier_runtime_progress_summary(),
            g_runtime.live_profile.activity_summary,
            g_runtime.live_profile.bonus_summary
        );
        return g_runtime.runtime_page_summary;
    }

    if (strcmp(page->page_id, "trails") == 0) {
        snprintf(
            g_runtime.runtime_page_summary,
            sizeof(g_runtime.runtime_page_summary),
            "%s: %s ~n~ %s",
            frontier_text("Current trail focus", "현재 트레일 초점"),
            frontier_runtime_context_label(),
            frontier_runtime_objective()
        );
        return g_runtime.runtime_page_summary;
    }

    if (strcmp(page->page_id, "frontier_map") == 0) {
        snprintf(
            g_runtime.runtime_page_summary,
            sizeof(g_runtime.runtime_page_summary),
            "%s: %s ~n~ %s %d/6 ~n~ %s",
            frontier_text("Current frontier anchor", "현재 프론티어 앵커"),
            frontier_runtime_context_label(),
            frontier_text("Regions logged", "기록된 지역"),
            g_runtime.live_profile.discovered_region_count,
            frontier_runtime_route_hint()
        );
        return g_runtime.runtime_page_summary;
    }

    if (strcmp(page->page_id, "homefront") == 0) {
        snprintf(
            g_runtime.runtime_page_summary,
            sizeof(g_runtime.runtime_page_summary),
            "%s %d ~n~ %s T%d | %s T%d | %s T%d",
            frontier_text("Hub returns", "허브 복귀"),
            g_runtime.live_profile.hub_returns,
            frontier_text("Workshop", "작업대"),
            g_runtime.live_profile.workshop_tier,
            frontier_text("Armory", "병기대"),
            g_runtime.live_profile.armory_tier,
            frontier_text("Medicine", "의약"),
            g_runtime.live_profile.medicine_tier
        );
        return g_runtime.runtime_page_summary;
    }

    if (strcmp(page->page_id, "stats") == 0) {
        snprintf(
            g_runtime.runtime_page_summary,
            sizeof(g_runtime.runtime_page_summary),
            "%s %d | %s %d | %s %d | %s %d | %s %d",
            frontier_text("Strength", "근력"),
            g_runtime.live_profile.stat_strength,
            frontier_text("Grit", "근성"),
            g_runtime.live_profile.stat_grit,
            frontier_text("Deadeye", "데드아이"),
            g_runtime.live_profile.stat_deadeye,
            frontier_text("Survival", "생존"),
            g_runtime.live_profile.stat_survival,
            frontier_text("Cunning", "기민함"),
            g_runtime.live_profile.stat_cunning
        );
        return g_runtime.runtime_page_summary;
    }

    if (strcmp(page->page_id, "skills") == 0) {
        snprintf(
            g_runtime.runtime_page_summary,
            sizeof(g_runtime.runtime_page_summary),
            "%s %d | %s %d | %s %d | %s %d",
            frontier_text("Hunting", "사냥"),
            g_runtime.live_profile.skill_hunting,
            frontier_text("Salvaging", "회수"),
            g_runtime.live_profile.skill_salvaging,
            frontier_text("Trade", "거래"),
            g_runtime.live_profile.skill_trade,
            frontier_text("Homestead", "거점 운영"),
            g_runtime.live_profile.skill_homestead
        );
        return g_runtime.runtime_page_summary;
    }

    if (strcmp(page->page_id, "perks") == 0) {
        snprintf(
            g_runtime.runtime_page_summary,
            sizeof(g_runtime.runtime_page_summary),
            "%s %d/7 ~n~ %s",
            frontier_text("Unlocked perks", "해금된 퍽"),
            frontier_live_unlocked_perk_count(),
            g_runtime.live_profile.bonus_summary
        );
        return g_runtime.runtime_page_summary;
    }

    if (strcmp(page->page_id, "crafting") == 0) {
        snprintf(
            g_runtime.runtime_page_summary,
            sizeof(g_runtime.runtime_page_summary),
            "%s %d/8 ~n~ %s %d | %s %d | %s %d",
            frontier_text("Ready recipes", "준비된 제작"),
            frontier_live_ready_recipe_count(),
            frontier_text("Salvage", "회수 자원"),
            g_runtime.live_profile.salvage_stock,
            frontier_text("Hides", "가죽"),
            g_runtime.live_profile.hide_stock,
            frontier_text("Relics", "유물"),
            g_runtime.live_profile.relic_stock
        );
        return g_runtime.runtime_page_summary;
    }

    if (strcmp(page->page_id, "armory") == 0) {
        snprintf(
            g_runtime.runtime_page_summary,
            sizeof(g_runtime.runtime_page_summary),
            "%s T%d | %s %d | %s T%d",
            frontier_text("Armory", "병기대"),
            g_runtime.live_profile.armory_tier,
            frontier_text("Familiarity", "숙련"),
            g_runtime.live_profile.weapon_familiarity_rank,
            frontier_text("Maintenance", "손질"),
            g_runtime.live_profile.weapon_maintenance_tier
        );
        return g_runtime.runtime_page_summary;
    }

    if (strcmp(page->page_id, "dead") == 0) {
        snprintf(
            g_runtime.runtime_page_summary,
            sizeof(g_runtime.runtime_page_summary),
            "%s %d/3 | %s %d",
            frontier_text("Ready routes", "준비된 루트"),
            frontier_live_ready_boss_count(),
            frontier_text("Elite pressure", "엘리트 압력"),
            g_runtime.live_profile.elite_pressure
        );
        return g_runtime.runtime_page_summary;
    }

    if (strcmp(page->page_id, "collection") == 0) {
        snprintf(
            g_runtime.runtime_page_summary,
            sizeof(g_runtime.runtime_page_summary),
            "%s %d/5 ~n~ %s %d | %s %d",
            frontier_text("Completed sets", "완료 세트"),
            frontier_live_completed_collection_count(),
            frontier_text("Broker marks", "브로커 표식"),
            g_runtime.live_profile.contract_count,
            frontier_text("Region records", "지역 기록"),
            g_runtime.live_profile.discovered_region_count
        );
        return g_runtime.runtime_page_summary;
    }

    if (strcmp(page->page_id, "gear") == 0) {
        snprintf(
            g_runtime.runtime_page_summary,
            sizeof(g_runtime.runtime_page_summary),
            "%s T%d | %s T%d | %s %d",
            frontier_text("Workshop", "작업대"),
            g_runtime.live_profile.workshop_tier,
            frontier_text("Medicine", "의약"),
            g_runtime.live_profile.medicine_tier,
            frontier_text("Hub returns", "허브 복귀"),
            g_runtime.live_profile.hub_returns
        );
        return g_runtime.runtime_page_summary;
    }

    if (strcmp(page->page_id, "farming") == 0 || strcmp(page->page_id, "network") == 0) {
        snprintf(
            g_runtime.runtime_page_summary,
            sizeof(g_runtime.runtime_page_summary),
            "%s ~n~ %s",
            strcmp(page->page_id, "farming") == 0 ? g_runtime.live_profile.farming_summary : g_runtime.live_profile.network_summary,
            strcmp(page->page_id, "farming") == 0 ? frontier_runtime_objective() : g_runtime.live_profile.bonus_summary
        );
        return g_runtime.runtime_page_summary;
    }

    if (strcmp(page->page_id, "world") == 0 || strcmp(page->page_id, "ending") == 0) {
        snprintf(
            g_runtime.runtime_page_summary,
            sizeof(g_runtime.runtime_page_summary),
            "%s ~n~ Current anchor: %s",
            strcmp(page->page_id, "world") == 0 ? g_runtime.live_profile.world_pressure_summary : frontier_runtime_phase_label(),
            frontier_runtime_context_label()
        );
        return g_runtime.runtime_page_summary;
    }

    return page->summary;
}

static const char *frontier_live_entry_detail(
    const FrontierLedgerPage *page,
    int entry_index,
    const FrontierLedgerEntry *entry
)
{
    const char *base_detail;

    if (page == NULL || entry == NULL) {
        return "";
    }

    base_detail = entry->detail != NULL ? entry->detail : "";
    if (!frontier_runtime_uses_live_dataset()) {
        return base_detail;
    }

    if (entry_index == 0 && strcmp(page->page_id, "homefront") == 0) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s %s / %s %s / %s %s ~n~ %s %d / %s %d ~n~ %s",
            frontier_text("Abigail", "아비게일"),
            frontier_family_rank_label(g_runtime.live_profile.abigail_support_rank),
            frontier_text("Jack", "잭"),
            frontier_family_rank_label(g_runtime.live_profile.jack_support_rank),
            frontier_text("Uncle", "엉클"),
            frontier_family_rank_label(g_runtime.live_profile.uncle_support_rank),
            frontier_text("Threat", "위협"),
            g_runtime.live_profile.ranch_threat_level,
            frontier_text("Defense", "방어"),
            g_runtime.live_profile.ranch_defense_level,
            frontier_runtime_objective()
        );
        return g_runtime.runtime_entry_detail;
    }

    if (entry_index == 0 && strcmp(page->page_id, "farming") == 0) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ %s %d | %s %d | %s %d ~n~ %s",
            frontier_farming_lane_label(g_runtime.live_profile.farming_lane),
            frontier_text("Hunts", "사냥"),
            g_runtime.live_profile.animal_kills,
            frontier_text("Loot", "루팅"),
            g_runtime.live_profile.loot_count,
            frontier_text("Contracts", "계약"),
            g_runtime.live_profile.contract_count,
            frontier_runtime_objective()
        );
        return g_runtime.runtime_entry_detail;
    }

    if (entry_index == 0 && strcmp(page->page_id, "ending") == 0) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ %s %d / %s %s ~n~ %s",
            g_runtime.live_profile.ending_summary,
            frontier_text("Clues", "단서"),
            g_runtime.live_profile.surveyor_clue_count,
            frontier_text("Sky Devil", "하늘 악마"),
            frontier_sky_devil_label(g_runtime.live_profile.sky_devil_stage),
            frontier_runtime_objective()
        );
        return g_runtime.runtime_entry_detail;
    }

    if (frontier_dataset_is_korean()) {
        if (strcmp(page->page_id, "stats") == 0) {
            switch (entry_index) {
            case 0:
                snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                    "근력 %d | 적 %d | 허브 %d",
                    g_runtime.live_profile.stat_strength,
                    g_runtime.live_profile.enemy_kills,
                    g_runtime.live_profile.hub_returns);
                return g_runtime.runtime_entry_detail;
            case 1:
                snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                    "근성 %d | 단계 %d | 보상 $%d",
                    g_runtime.live_profile.stat_grit,
                    g_runtime.live_profile.trail_stage,
                    g_runtime.live_profile.reward_earned);
                return g_runtime.runtime_entry_detail;
            case 2:
                snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                    "데드아이 %d | 적 %d | 지역 %d/6",
                    g_runtime.live_profile.stat_deadeye,
                    g_runtime.live_profile.enemy_kills,
                    g_runtime.live_profile.discovered_region_count);
                return g_runtime.runtime_entry_detail;
            case 3:
                snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                    "생존 %d | 사냥 %d | 이동 %.1fkm",
                    g_runtime.live_profile.stat_survival,
                    g_runtime.live_profile.animal_kills,
                    g_runtime.live_profile.field_distance_total / 1000.0f);
                return g_runtime.runtime_entry_detail;
            default:
                snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                    "기민함 %d | 루팅 %d | 보상 $%d",
                    g_runtime.live_profile.stat_cunning,
                    g_runtime.live_profile.loot_count,
                    g_runtime.live_profile.reward_earned);
                return g_runtime.runtime_entry_detail;
            }
        }

        if (strcmp(page->page_id, "skills") == 0) {
            switch (entry_index) {
            case 0:
                snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                    "사냥 %d | 동물 %d | 가죽 %d",
                    g_runtime.live_profile.skill_hunting,
                    g_runtime.live_profile.animal_kills,
                    g_runtime.live_profile.hide_stock);
                return g_runtime.runtime_entry_detail;
            case 1:
                snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                    "총포 %d | 병기 T%d | 손질 T%d",
                    g_runtime.live_profile.skill_gunsmithing,
                    g_runtime.live_profile.armory_tier,
                    g_runtime.live_profile.weapon_maintenance_tier);
                return g_runtime.runtime_entry_detail;
            case 2:
                snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                    "의약 %d | 토닉 %d | 의약 T%d",
                    g_runtime.live_profile.skill_field_medicine,
                    g_runtime.live_profile.tonic_stock,
                    g_runtime.live_profile.medicine_tier);
                return g_runtime.runtime_entry_detail;
            case 3:
                snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                    "덫 %d | 가죽 %d | 동물 %d",
                    g_runtime.live_profile.skill_trapping,
                    g_runtime.live_profile.hide_stock,
                    g_runtime.live_profile.animal_kills);
                return g_runtime.runtime_entry_detail;
            case 4:
                snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                    "회수 %d | 자원 %d | 루팅 %d",
                    g_runtime.live_profile.skill_salvaging,
                    g_runtime.live_profile.salvage_stock,
                    g_runtime.live_profile.loot_count);
                return g_runtime.runtime_entry_detail;
            case 5:
                snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                    "거래 %d | 계약 %d | 보상 $%d",
                    g_runtime.live_profile.skill_trade,
                    g_runtime.live_profile.contract_count,
                    g_runtime.live_profile.reward_earned);
                return g_runtime.runtime_entry_detail;
            case 6:
                snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                    "말 조련 %d | 이동 %.1fkm | 복귀 %d",
                    g_runtime.live_profile.skill_horse_handling,
                    g_runtime.live_profile.field_distance_total / 1000.0f,
                    g_runtime.live_profile.hub_returns);
                return g_runtime.runtime_entry_detail;
            default:
                snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                    "거점 %d | 작업 T%d | 복귀 %d",
                    g_runtime.live_profile.skill_homestead,
                    g_runtime.live_profile.workshop_tier,
                    g_runtime.live_profile.hub_returns);
                return g_runtime.runtime_entry_detail;
            }
        }

        if (strcmp(page->page_id, "perks") == 0) {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s ~n~ 퍽 %d/7 | %s",
                base_detail,
                frontier_live_unlocked_perk_count(),
                g_runtime.live_profile.bonus_summary
            );
            return g_runtime.runtime_entry_detail;
        }

        if (strcmp(page->page_id, "crafting") == 0) {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s ~n~ 회수 %d | 가죽 %d | 유물 %d | 작업 T%d",
                base_detail,
                g_runtime.live_profile.salvage_stock,
                g_runtime.live_profile.hide_stock,
                g_runtime.live_profile.relic_stock,
                g_runtime.live_profile.workshop_tier
            );
            return g_runtime.runtime_entry_detail;
        }

        if (strcmp(page->page_id, "armory") == 0) {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s ~n~ 숙련 %d | 손질 T%d | 병기 T%d | 적 %d",
                base_detail,
                g_runtime.live_profile.weapon_familiarity_rank,
                g_runtime.live_profile.weapon_maintenance_tier,
                g_runtime.live_profile.armory_tier,
                g_runtime.live_profile.enemy_kills
            );
            return g_runtime.runtime_entry_detail;
        }

        if (strcmp(page->page_id, "dead") == 0) {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s ~n~ 압력 %d | 계약 %d ~n~ %s",
                base_detail,
                g_runtime.live_profile.elite_pressure,
                g_runtime.live_profile.contract_count,
                g_runtime.live_profile.bonus_summary
            );
            return g_runtime.runtime_entry_detail;
        }

        if (strcmp(page->page_id, "collection") == 0) {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s ~n~ 완료 %d/5 | 복귀 %d | 지역 %d",
                base_detail,
                frontier_live_completed_collection_count(),
                g_runtime.live_profile.hub_returns,
                g_runtime.live_profile.discovered_region_count
            );
            return g_runtime.runtime_entry_detail;
        }

        if (strcmp(page->page_id, "gear") == 0) {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s ~n~ 작업 T%d | 의약 T%d | 회수 %d",
                base_detail,
                g_runtime.live_profile.workshop_tier,
                g_runtime.live_profile.medicine_tier,
                g_runtime.live_profile.salvage_stock
            );
            return g_runtime.runtime_entry_detail;
        }

        if (entry_index == 0 && strcmp(page->page_id, "overview") == 0) {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s ~n~ %s ~n~ 앵커 %s",
                g_runtime.live_profile.progress_summary,
                g_runtime.live_profile.bonus_summary,
                frontier_runtime_context_label()
            );
            return g_runtime.runtime_entry_detail;
        }

        if (entry_index == 0 && strcmp(page->page_id, "trails") == 0) {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s ~n~ 앵커 %s | 복귀 %d ~n~ %s",
                frontier_live_trail_stage_label(g_runtime.live_profile.trail_stage),
                frontier_runtime_context_label(),
                g_runtime.live_profile.hub_returns,
                frontier_runtime_objective_compact()
            );
            return g_runtime.runtime_entry_detail;
        }

        if (entry_index == 0 && strcmp(page->page_id, "frontier_map") == 0) {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "앵커 %s ~n~ 지역 %d/6 | 이동 %.1fkm ~n~ %s",
                frontier_runtime_context_label(),
                g_runtime.live_profile.discovered_region_count,
                g_runtime.live_profile.field_distance_total / 1000.0f,
                frontier_runtime_route_hint()
            );
            return g_runtime.runtime_entry_detail;
        }

        if (entry_index == 0 && strcmp(page->page_id, "homefront") == 0) {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "복귀 %d | 작업 T%d | 병기 T%d ~n~ 의약 T%d | 단계 %s",
                g_runtime.live_profile.hub_returns,
                g_runtime.live_profile.workshop_tier,
                g_runtime.live_profile.armory_tier,
                g_runtime.live_profile.medicine_tier,
                frontier_live_trail_stage_compact_label(g_runtime.live_profile.trail_stage)
            );
            return g_runtime.runtime_entry_detail;
        }

        if (entry_index == 0 && strcmp(page->page_id, "farming") == 0) {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s ~n~ 적 %d | 사냥 %d ~n~ 마지막 복귀 후 %.0fm",
                g_runtime.live_profile.farming_summary,
                g_runtime.live_profile.enemy_kills,
                g_runtime.live_profile.animal_kills,
                g_runtime.live_profile.field_distance_since_hub
            );
            return g_runtime.runtime_entry_detail;
        }

        if (entry_index == 0 && strcmp(page->page_id, "network") == 0) {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s ~n~ 보상 $%d | 복귀 %d | 루팅 %d ~n~ %s",
                g_runtime.live_profile.network_summary,
                g_runtime.live_profile.reward_earned,
                g_runtime.live_profile.hub_returns,
                g_runtime.live_profile.loot_count,
                g_runtime.live_profile.bonus_summary
            );
            return g_runtime.runtime_entry_detail;
        }

        if (entry_index == 0 && strcmp(page->page_id, "world") == 0) {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s ~n~ %s ~n~ 지역 %d | 허브 %d | 보상 $%d",
                frontier_runtime_phase_label(),
                g_runtime.live_profile.world_pressure_summary,
                g_runtime.live_profile.discovered_region_count,
                g_runtime.live_profile.hub_returns,
                g_runtime.live_profile.reward_earned
            );
            return g_runtime.runtime_entry_detail;
        }

        if (entry_index == 0 && strcmp(page->page_id, "ending") == 0) {
            snprintf(
                g_runtime.runtime_entry_detail,
                sizeof(g_runtime.runtime_entry_detail),
                "%s ~n~ 앵커 %s ~n~ %s",
                frontier_runtime_phase_label(),
                frontier_runtime_context_label(),
                frontier_runtime_objective_compact()
            );
            return g_runtime.runtime_entry_detail;
        }
    }

    if (strcmp(page->page_id, "stats") == 0) {
        switch (entry_index) {
        case 0:
            snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                "%s %d. %s %d, %s %d.",
                frontier_text("Strength rank", "근력 랭크"), g_runtime.live_profile.stat_strength,
                frontier_text("Enemy kills", "적 처치"), g_runtime.live_profile.enemy_kills,
                frontier_text("hub returns", "허브 복귀"), g_runtime.live_profile.hub_returns);
            return g_runtime.runtime_entry_detail;
        case 1:
            snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                "%s %d. %s %d, %s $%d.",
                frontier_text("Grit rank", "근성 랭크"), g_runtime.live_profile.stat_grit,
                frontier_text("Trail stage", "트레일 단계"), g_runtime.live_profile.trail_stage,
                frontier_text("reward intake", "보상 수입"), g_runtime.live_profile.reward_earned);
            return g_runtime.runtime_entry_detail;
        case 2:
            snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                "%s %d. %s %d, %s %d/6.",
                frontier_text("Deadeye rank", "데드아이 랭크"), g_runtime.live_profile.stat_deadeye,
                frontier_text("Enemy kills", "적 처치"), g_runtime.live_profile.enemy_kills,
                frontier_text("regions", "지역"), g_runtime.live_profile.discovered_region_count);
            return g_runtime.runtime_entry_detail;
        case 3:
            snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                "%s %d. %s %d, %s %.1f km.",
                frontier_text("Survival rank", "생존 랭크"), g_runtime.live_profile.stat_survival,
                frontier_text("Animal kills", "동물 처치"), g_runtime.live_profile.animal_kills,
                frontier_text("field distance", "필드 이동"), g_runtime.live_profile.field_distance_total / 1000.0f);
            return g_runtime.runtime_entry_detail;
        default:
            snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                "%s %d. %s %d, %s $%d.",
                frontier_text("Cunning rank", "기민함 랭크"), g_runtime.live_profile.stat_cunning,
                frontier_text("Loot count", "루팅 횟수"), g_runtime.live_profile.loot_count,
                frontier_text("reward intake", "보상 수입"), g_runtime.live_profile.reward_earned);
            return g_runtime.runtime_entry_detail;
        }
    }

    if (strcmp(page->page_id, "skills") == 0) {
        switch (entry_index) {
        case 0:
            snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                "%s %d. %s %d, %s %d.",
                frontier_text("Hunting rank", "사냥 랭크"), g_runtime.live_profile.skill_hunting,
                frontier_text("Animal kills", "동물 처치"), g_runtime.live_profile.animal_kills,
                frontier_text("hide stock", "가죽 비축"), g_runtime.live_profile.hide_stock);
            return g_runtime.runtime_entry_detail;
        case 1:
            snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                "%s %d. %s T%d, %s T%d.",
                frontier_text("Gunsmithing rank", "총포 개조 랭크"), g_runtime.live_profile.skill_gunsmithing,
                frontier_text("Armory", "병기대"), g_runtime.live_profile.armory_tier,
                frontier_text("maintenance", "손질"), g_runtime.live_profile.weapon_maintenance_tier);
            return g_runtime.runtime_entry_detail;
        case 2:
            snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                "%s %d. %s %d, %s T%d.",
                frontier_text("Field medicine rank", "현장 치료 랭크"), g_runtime.live_profile.skill_field_medicine,
                frontier_text("Tonic stock", "토닉 비축"), g_runtime.live_profile.tonic_stock,
                frontier_text("medicine tier", "의약 단계"), g_runtime.live_profile.medicine_tier);
            return g_runtime.runtime_entry_detail;
        case 3:
            snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                "%s %d. %s %d, %s %d.",
                frontier_text("Trapping rank", "덫 사냥 랭크"), g_runtime.live_profile.skill_trapping,
                frontier_text("Hide stock", "가죽 비축"), g_runtime.live_profile.hide_stock,
                frontier_text("animal kills", "동물 처치"), g_runtime.live_profile.animal_kills);
            return g_runtime.runtime_entry_detail;
        case 4:
            snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                "%s %d. %s %d, %s %d.",
                frontier_text("Salvaging rank", "회수 랭크"), g_runtime.live_profile.skill_salvaging,
                frontier_text("Salvage stock", "회수 자원"), g_runtime.live_profile.salvage_stock,
                frontier_text("loot count", "루팅 횟수"), g_runtime.live_profile.loot_count);
            return g_runtime.runtime_entry_detail;
        case 5:
            snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                "%s %d. %s %d, %s $%d.",
                frontier_text("Trade rank", "거래 랭크"), g_runtime.live_profile.skill_trade,
                frontier_text("Contracts", "계약"), g_runtime.live_profile.contract_count,
                frontier_text("reward intake", "보상 수입"), g_runtime.live_profile.reward_earned);
            return g_runtime.runtime_entry_detail;
        case 6:
            snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                "%s %d. %s %.1f km, %s %d.",
                frontier_text("Horse handling rank", "말 조련 랭크"), g_runtime.live_profile.skill_horse_handling,
                frontier_text("field distance", "필드 이동"), g_runtime.live_profile.field_distance_total / 1000.0f,
                frontier_text("hub returns", "허브 복귀"), g_runtime.live_profile.hub_returns);
            return g_runtime.runtime_entry_detail;
        default:
            snprintf(g_runtime.runtime_entry_detail, sizeof(g_runtime.runtime_entry_detail),
                "%s %d. %s T%d, %s %d.",
                frontier_text("Homestead rank", "거점 운영 랭크"), g_runtime.live_profile.skill_homestead,
                frontier_text("Workshop", "작업대"), g_runtime.live_profile.workshop_tier,
                frontier_text("hub returns", "허브 복귀"), g_runtime.live_profile.hub_returns);
            return g_runtime.runtime_entry_detail;
        }
    }

    if (strcmp(page->page_id, "perks") == 0) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ %s %d/7 ~n~ %s",
            base_detail,
            frontier_text("Unlocked perks", "해금된 퍽"),
            frontier_live_unlocked_perk_count(),
            g_runtime.live_profile.bonus_summary
        );
        return g_runtime.runtime_entry_detail;
    }

    if (strcmp(page->page_id, "crafting") == 0) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ %s %d | %s %d | %s %d | %s T%d",
            base_detail,
            frontier_text("Salvage", "회수 자원"),
            g_runtime.live_profile.salvage_stock,
            frontier_text("Hides", "가죽"),
            g_runtime.live_profile.hide_stock,
            frontier_text("Relics", "유물"),
            g_runtime.live_profile.relic_stock,
            frontier_text("Workshop", "작업대"),
            g_runtime.live_profile.workshop_tier
        );
        return g_runtime.runtime_entry_detail;
    }

    if (strcmp(page->page_id, "armory") == 0) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ %s %d | %s T%d | %s T%d ~n~ %s %d",
            base_detail,
            frontier_text("Familiarity", "숙련"),
            g_runtime.live_profile.weapon_familiarity_rank,
            frontier_text("Maintenance", "손질"),
            g_runtime.live_profile.weapon_maintenance_tier,
            frontier_text("Armory", "병기대"),
            g_runtime.live_profile.armory_tier,
            frontier_text("Enemy kills", "적 처치"),
            g_runtime.live_profile.enemy_kills
        );
        return g_runtime.runtime_entry_detail;
    }

    if (strcmp(page->page_id, "dead") == 0) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ %s %d | %s %d | %s",
            base_detail,
            frontier_text("Elite pressure", "엘리트 압력"),
            g_runtime.live_profile.elite_pressure,
            frontier_text("Contracts", "계약"),
            g_runtime.live_profile.contract_count,
            g_runtime.live_profile.bonus_summary
        );
        return g_runtime.runtime_entry_detail;
    }

    if (strcmp(page->page_id, "collection") == 0) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ %s %d/5 ~n~ %s %d | %s %d",
            base_detail,
            frontier_text("Completed sets", "완료 세트"),
            frontier_live_completed_collection_count(),
            frontier_text("Hub returns", "허브 복귀"),
            g_runtime.live_profile.hub_returns,
            frontier_text("Region records", "지역 기록"),
            g_runtime.live_profile.discovered_region_count
        );
        return g_runtime.runtime_entry_detail;
    }

    if (strcmp(page->page_id, "gear") == 0) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ %s T%d | %s T%d | %s %d",
            base_detail,
            frontier_text("Workshop", "작업대"),
            g_runtime.live_profile.workshop_tier,
            frontier_text("Medicine", "의약"),
            g_runtime.live_profile.medicine_tier,
            frontier_text("Salvage stock", "회수 자원"),
            g_runtime.live_profile.salvage_stock
        );
        return g_runtime.runtime_entry_detail;
    }

    if (entry_index == 0 && strcmp(page->page_id, "overview") == 0) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ %s ~n~ %s ~n~ %s ~n~ %s ~n~ %s: %s ~n~ %s: %s",
            frontier_text("The native ledger is now following a live frontier run instead of only a baked dataset.", "네이티브 레저는 이제 고정 데이터가 아니라 실제 프론티어 런을 따라갑니다."),
            frontier_runtime_phase_label(),
            frontier_runtime_progress_summary(),
            g_runtime.live_profile.activity_summary,
            g_runtime.live_profile.bonus_summary,
            frontier_text("Current anchor", "현재 앵커"),
            frontier_runtime_context_label(),
            frontier_text("Route hint", "루트 힌트"),
            frontier_runtime_route_hint()
        );
        return g_runtime.runtime_entry_detail;
    }

    if (entry_index == 0 && strcmp(page->page_id, "trails") == 0) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ %s ~n~ %s: %s ~n~ %s ~n~ %s: %d",
            frontier_live_trail_stage_label(g_runtime.live_profile.trail_stage),
            frontier_runtime_objective(),
            frontier_text("Active region", "활성 지역"),
            frontier_runtime_context_label(),
            frontier_runtime_route_hint(),
            frontier_text("Hub returns", "허브 복귀"),
            g_runtime.live_profile.hub_returns
        );
        return g_runtime.runtime_entry_detail;
    }

    if (entry_index == 0 && strcmp(page->page_id, "frontier_map") == 0) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ %s %d/6 ~n~ %s %.1f km ~n~ %s",
            frontier_runtime_context_label(),
            frontier_text("Regions discovered so far:", "현재까지 발견 지역:"),
            g_runtime.live_profile.discovered_region_count,
            frontier_text("Total field travel:", "총 필드 이동:"),
            g_runtime.live_profile.field_distance_total / 1000.0f,
            frontier_runtime_route_hint()
        );
        return g_runtime.runtime_entry_detail;
    }

    if (entry_index == 0 && strcmp(page->page_id, "homefront") == 0) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s %d. %s %s. %s",
            frontier_text("Beecher's Hope has received verified live reports:", "비처스 호프가 받은 검증 보고 수:"),
            g_runtime.live_profile.hub_returns,
            frontier_text("The ranch phase is", "현재 목장 단계는"),
            frontier_runtime_phase_label(),
            frontier_text("The next return should turn the current field run into deeper hub work.", "다음 복귀는 현재 필드 런을 더 깊은 허브 작업으로 전환해야 합니다.")
        );
        return g_runtime.runtime_entry_detail;
    }

    if (entry_index == 0 && strcmp(page->page_id, "farming") == 0) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ %s ~n~ %s %d | %s %d ~n~ %s: %s ~n~ %s %.0f",
            frontier_runtime_objective(),
            g_runtime.live_profile.farming_summary,
            frontier_text("Enemy pressure broken", "정리한 적 압력"),
            g_runtime.live_profile.enemy_kills,
            frontier_text("Animal pressure harvested", "수확한 동물 압력"),
            g_runtime.live_profile.animal_kills,
            frontier_text("Current region anchor", "현재 지역 앵커"),
            frontier_runtime_context_label(),
            frontier_text("Distance since last hub return", "마지막 허브 복귀 이후 이동"),
            g_runtime.live_profile.field_distance_since_hub
        );
        return g_runtime.runtime_entry_detail;
    }

    if (entry_index == 0 && strcmp(page->page_id, "network") == 0) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ %s ~n~ %s $%d / %s %d / %s %d ~n~ %s",
            frontier_text("The live network is now tracking real payout flow.", "실시간 네트워크가 실제 보상 흐름을 추적하고 있습니다."),
            g_runtime.live_profile.network_summary,
            frontier_text("Reward", "보상"),
            g_runtime.live_profile.reward_earned,
            frontier_text("Hub returns", "허브 복귀"),
            g_runtime.live_profile.hub_returns,
            frontier_text("Loots", "루팅"),
            g_runtime.live_profile.loot_count,
            frontier_runtime_route_hint()
        );
        return g_runtime.runtime_entry_detail;
    }

    if (entry_index == 0 && strcmp(page->page_id, "world") == 0) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s: %s ~n~ %s ~n~ %s ~n~ %s %d/6 ~n~ %s %d ~n~ %s %.1f km ~n~ %s $%d",
            frontier_text("Current live phase", "현재 실시간 단계"),
            frontier_runtime_phase_label(),
            g_runtime.live_profile.world_pressure_summary,
            g_runtime.live_profile.bonus_summary,
            frontier_text("Regions discovered", "발견 지역"),
            g_runtime.live_profile.discovered_region_count,
            frontier_text("Hub returns", "허브 복귀"),
            g_runtime.live_profile.hub_returns,
            frontier_text("Field travel", "필드 이동"),
            g_runtime.live_profile.field_distance_total / 1000.0f,
            frontier_text("Rewards banked", "누적 보상"),
            g_runtime.live_profile.reward_earned
        );
        return g_runtime.runtime_entry_detail;
    }

    if (entry_index == 0 && strcmp(page->page_id, "ending") == 0) {
        snprintf(
            g_runtime.runtime_entry_detail,
            sizeof(g_runtime.runtime_entry_detail),
            "%s ~n~ %s ~n~ %s: %s ~n~ %s",
            frontier_text("The ending route is now driven by the live frontier phase.", "엔딩 루트는 이제 실시간 프론티어 단계에 따라 움직입니다."),
            frontier_runtime_phase_label(),
            frontier_text("Active anchor", "활성 앵커"),
            frontier_runtime_context_label(),
            frontier_runtime_objective()
        );
        return g_runtime.runtime_entry_detail;
    }

    snprintf(
        g_runtime.runtime_entry_detail,
        sizeof(g_runtime.runtime_entry_detail),
        "%s ~n~ ~n~ %s: %s ~n~ %s ~n~ %s",
        base_detail,
        frontier_text("Live frontier context", "실시간 프론티어 문맥"),
        frontier_runtime_context_label(),
        frontier_runtime_progress_summary(),
        frontier_runtime_objective()
    );
    return g_runtime.runtime_entry_detail;
}

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

static void frontier_ensure_runtime_ini_path(char *path, size_t path_size, const char *file_name)
{
    if (path == NULL || path_size == 0U || file_name == NULL || file_name[0] == '\0') {
        return;
    }
    if (path[0] == '\0' && g_runtime.module_path[0] != '\0') {
        frontier_build_neighbor_path(g_runtime.module_path, file_name, path, path_size);
    }
}

static void frontier_touch_runtime_file(const char *path)
{
    HANDLE handle;

    if (path == NULL || path[0] == '\0') {
        return;
    }

    handle = CreateFileA(
        path,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (handle != INVALID_HANDLE_VALUE) {
        CloseHandle(handle);
    }
}
