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
