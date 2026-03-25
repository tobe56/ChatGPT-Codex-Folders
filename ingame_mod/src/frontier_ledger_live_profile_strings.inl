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
