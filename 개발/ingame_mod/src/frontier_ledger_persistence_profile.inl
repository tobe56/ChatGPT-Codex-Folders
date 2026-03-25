static void frontier_live_profile_save(int force_now)
{
    char buffer[64];
    DWORD now = force_now ? GetTickCount() : (DWORD)frontier_game_timer();
    int write_failed = 0;

    frontier_ensure_runtime_ini_path(g_runtime.profile_path, sizeof(g_runtime.profile_path), "FrontierLedger.profile.ini");
    if (g_runtime.profile_path[0] == '\0') {
        return;
    }

    if (!force_now && !g_runtime.live_profile.dirty) {
        return;
    }

    if (!force_now && now - g_runtime.live_profile.last_saved_at < kFrontierLiveSaveIntervalMs) {
        return;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.phase);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "Phase", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.trail_stage);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "TrailStage", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.discovered_region_mask);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "DiscoveredRegionMask", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.hub_returns);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "HubReturns", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.enemy_kills);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "EnemyKills", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.loot_count);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "LootCount", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.animal_kills);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "AnimalKills", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.reward_earned);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "RewardEarned", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%u", g_runtime.live_profile.crafted_mask);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "CraftedMask", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.stat_bonus_strength);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "StatBonusStrength", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.stat_bonus_grit);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "StatBonusGrit", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.stat_bonus_deadeye);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "StatBonusDeadeye", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.stat_bonus_survival);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "StatBonusSurvival", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.stat_bonus_cunning);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "StatBonusCunning", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.skill_bonus_hunting);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "SkillBonusHunting", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.skill_bonus_gunsmithing);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "SkillBonusGunsmithing", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.skill_bonus_field_medicine);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "SkillBonusFieldMedicine", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.skill_bonus_trapping);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "SkillBonusTrapping", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.skill_bonus_salvaging);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "SkillBonusSalvaging", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.skill_bonus_trade);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "SkillBonusTrade", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.skill_bonus_horse_handling);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "SkillBonusHorseHandling", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }
    snprintf(buffer, sizeof(buffer), "%d", g_runtime.live_profile.skill_bonus_homestead);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Growth", "SkillBonusHomestead", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%.2f", g_runtime.live_profile.field_distance_total);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "FieldDistanceTotal", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    snprintf(buffer, sizeof(buffer), "%.2f", g_runtime.live_profile.field_distance_since_hub);
    if (!frontier_write_ini_string_retry("FrontierLedger.profile.ini", "Run", "FieldDistanceSinceHub", buffer, g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    if (!frontier_flush_ini_retry("FrontierLedger.profile.ini", g_runtime.profile_path, sizeof(g_runtime.profile_path))) {
        write_failed = 1;
    }

    if (write_failed && now - g_runtime.last_profile_write_failure_at >= kFrontierWriteFailureLogCooldownMs) {
        g_runtime.last_profile_write_failure_at = now;
        frontier_log("Profile save encountered a write failure. Path=%s", g_runtime.profile_path);
    } else if (!write_failed &&
               (force_now || now - g_runtime.last_profile_save_log_at >= kFrontierSaveLogCooldownMs)) {
        g_runtime.last_profile_save_log_at = now;
        frontier_log(
            "Profile saved. Phase=%d Stage=%d Enemy=%d Loot=%d Hunt=%d Reward=%d CraftedMask=0x%X StatPts=%d SkillPts=%d",
            g_runtime.live_profile.phase,
            g_runtime.live_profile.trail_stage,
            g_runtime.live_profile.enemy_kills,
            g_runtime.live_profile.loot_count,
            g_runtime.live_profile.animal_kills,
            g_runtime.live_profile.reward_earned,
            g_runtime.live_profile.crafted_mask,
            frontier_live_stat_points_available(),
            frontier_live_skill_points_available()
        );
    }

    g_runtime.live_profile.last_saved_at = now;
    g_runtime.live_profile.dirty = 0;
}

static void frontier_live_profile_load(void)
{
    char buffer[64];
    int max_region_mask = (1 << kFrontierRegionAnchorCount) - 1;
    int raw_phase;
    int raw_stage;
    int raw_mask;
    int raw_hub_returns;
    int raw_enemy_kills;
    int raw_loot_count;
    int raw_animal_kills;
    int raw_reward_earned;
    int raw_crafted_mask;
    int raw_stat_bonus_strength;
    int raw_stat_bonus_grit;
    int raw_stat_bonus_deadeye;
    int raw_stat_bonus_survival;
    int raw_stat_bonus_cunning;
    int raw_skill_bonus_hunting;
    int raw_skill_bonus_gunsmithing;
    int raw_skill_bonus_field_medicine;
    int raw_skill_bonus_trapping;
    int raw_skill_bonus_salvaging;
    int raw_skill_bonus_trade;
    int raw_skill_bonus_horse_handling;
    int raw_skill_bonus_homestead;
    int sanitized = 0;
    float raw_field_distance_total;
    float raw_field_distance_since_hub;

    ZeroMemory(&g_runtime.live_profile, sizeof(g_runtime.live_profile));
    g_runtime.applied_bonus_tier = -1;

    frontier_ensure_runtime_ini_path(g_runtime.profile_path, sizeof(g_runtime.profile_path), "FrontierLedger.profile.ini");
    if (g_runtime.profile_path[0] == '\0') {
        frontier_live_profile_refresh_strings();
        return;
    }

    raw_phase = (int)GetPrivateProfileIntA("Run", "Phase", FRONTIER_PHASE_SETUP, g_runtime.profile_path);
    raw_stage = (int)GetPrivateProfileIntA("Run", "TrailStage", 0, g_runtime.profile_path);
    raw_mask = (int)GetPrivateProfileIntA("Run", "DiscoveredRegionMask", 0, g_runtime.profile_path);
    raw_hub_returns = (int)GetPrivateProfileIntA("Run", "HubReturns", 0, g_runtime.profile_path);
    raw_enemy_kills = (int)GetPrivateProfileIntA("Run", "EnemyKills", 0, g_runtime.profile_path);
    raw_loot_count = (int)GetPrivateProfileIntA("Run", "LootCount", 0, g_runtime.profile_path);
    raw_animal_kills = (int)GetPrivateProfileIntA("Run", "AnimalKills", 0, g_runtime.profile_path);
    raw_reward_earned = (int)GetPrivateProfileIntA("Run", "RewardEarned", 0, g_runtime.profile_path);
    raw_crafted_mask = (int)GetPrivateProfileIntA("Run", "CraftedMask", 0, g_runtime.profile_path);
    raw_stat_bonus_strength = (int)GetPrivateProfileIntA("Growth", "StatBonusStrength", 0, g_runtime.profile_path);
    raw_stat_bonus_grit = (int)GetPrivateProfileIntA("Growth", "StatBonusGrit", 0, g_runtime.profile_path);
    raw_stat_bonus_deadeye = (int)GetPrivateProfileIntA("Growth", "StatBonusDeadeye", 0, g_runtime.profile_path);
    raw_stat_bonus_survival = (int)GetPrivateProfileIntA("Growth", "StatBonusSurvival", 0, g_runtime.profile_path);
    raw_stat_bonus_cunning = (int)GetPrivateProfileIntA("Growth", "StatBonusCunning", 0, g_runtime.profile_path);
    raw_skill_bonus_hunting = (int)GetPrivateProfileIntA("Growth", "SkillBonusHunting", 0, g_runtime.profile_path);
    raw_skill_bonus_gunsmithing = (int)GetPrivateProfileIntA("Growth", "SkillBonusGunsmithing", 0, g_runtime.profile_path);
    raw_skill_bonus_field_medicine = (int)GetPrivateProfileIntA("Growth", "SkillBonusFieldMedicine", 0, g_runtime.profile_path);
    raw_skill_bonus_trapping = (int)GetPrivateProfileIntA("Growth", "SkillBonusTrapping", 0, g_runtime.profile_path);
    raw_skill_bonus_salvaging = (int)GetPrivateProfileIntA("Growth", "SkillBonusSalvaging", 0, g_runtime.profile_path);
    raw_skill_bonus_trade = (int)GetPrivateProfileIntA("Growth", "SkillBonusTrade", 0, g_runtime.profile_path);
    raw_skill_bonus_horse_handling = (int)GetPrivateProfileIntA("Growth", "SkillBonusHorseHandling", 0, g_runtime.profile_path);
    raw_skill_bonus_homestead = (int)GetPrivateProfileIntA("Growth", "SkillBonusHomestead", 0, g_runtime.profile_path);

    g_runtime.live_profile.phase = frontier_clamp_int(
        raw_phase,
        FRONTIER_PHASE_SETUP,
        FRONTIER_PHASE_ENDGAME
    );
    g_runtime.live_profile.trail_stage = frontier_clamp_int(
        raw_stage,
        0,
        5
    );
    g_runtime.live_profile.discovered_region_mask = frontier_clamp_int(
        raw_mask,
        0,
        max_region_mask
    ) & max_region_mask;
    g_runtime.live_profile.hub_returns = frontier_clamp_int(
        raw_hub_returns,
        0,
        99
    );
    g_runtime.live_profile.enemy_kills = frontier_clamp_int(
        raw_enemy_kills,
        0,
        99999
    );
    g_runtime.live_profile.loot_count = frontier_clamp_int(
        raw_loot_count,
        0,
        99999
    );
    g_runtime.live_profile.animal_kills = frontier_clamp_int(
        raw_animal_kills,
        0,
        99999
    );
    g_runtime.live_profile.reward_earned = frontier_clamp_int(
        raw_reward_earned,
        0,
        999999
    );
    g_runtime.live_profile.crafted_mask = (uint32_t)frontier_clamp_int(
        raw_crafted_mask,
        0,
        0x1FFF
    );
    g_runtime.live_profile.stat_bonus_strength = frontier_clamp_int(raw_stat_bonus_strength, 0, 3);
    g_runtime.live_profile.stat_bonus_grit = frontier_clamp_int(raw_stat_bonus_grit, 0, 3);
    g_runtime.live_profile.stat_bonus_deadeye = frontier_clamp_int(raw_stat_bonus_deadeye, 0, 3);
    g_runtime.live_profile.stat_bonus_survival = frontier_clamp_int(raw_stat_bonus_survival, 0, 3);
    g_runtime.live_profile.stat_bonus_cunning = frontier_clamp_int(raw_stat_bonus_cunning, 0, 3);
    g_runtime.live_profile.skill_bonus_hunting = frontier_clamp_int(raw_skill_bonus_hunting, 0, 3);
    g_runtime.live_profile.skill_bonus_gunsmithing = frontier_clamp_int(raw_skill_bonus_gunsmithing, 0, 3);
    g_runtime.live_profile.skill_bonus_field_medicine = frontier_clamp_int(raw_skill_bonus_field_medicine, 0, 3);
    g_runtime.live_profile.skill_bonus_trapping = frontier_clamp_int(raw_skill_bonus_trapping, 0, 3);
    g_runtime.live_profile.skill_bonus_salvaging = frontier_clamp_int(raw_skill_bonus_salvaging, 0, 3);
    g_runtime.live_profile.skill_bonus_trade = frontier_clamp_int(raw_skill_bonus_trade, 0, 3);
    g_runtime.live_profile.skill_bonus_horse_handling = frontier_clamp_int(raw_skill_bonus_horse_handling, 0, 3);
    g_runtime.live_profile.skill_bonus_homestead = frontier_clamp_int(raw_skill_bonus_homestead, 0, 3);

    GetPrivateProfileStringA("Run", "FieldDistanceTotal", "0", buffer, (DWORD)sizeof(buffer), g_runtime.profile_path);
    raw_field_distance_total = (float)atof(buffer);
    g_runtime.live_profile.field_distance_total = frontier_clamp_float(raw_field_distance_total, 0.0f, 250000.0f);

    GetPrivateProfileStringA("Run", "FieldDistanceSinceHub", "0", buffer, (DWORD)sizeof(buffer), g_runtime.profile_path);
    raw_field_distance_since_hub = (float)atof(buffer);
    g_runtime.live_profile.field_distance_since_hub = frontier_clamp_float(raw_field_distance_since_hub, 0.0f, 50000.0f);

    sanitized =
        raw_phase != g_runtime.live_profile.phase ||
        raw_stage != g_runtime.live_profile.trail_stage ||
        raw_mask != g_runtime.live_profile.discovered_region_mask ||
        raw_hub_returns != g_runtime.live_profile.hub_returns ||
        raw_enemy_kills != g_runtime.live_profile.enemy_kills ||
        raw_loot_count != g_runtime.live_profile.loot_count ||
        raw_animal_kills != g_runtime.live_profile.animal_kills ||
        raw_reward_earned != g_runtime.live_profile.reward_earned ||
        raw_crafted_mask != (int)g_runtime.live_profile.crafted_mask ||
        raw_stat_bonus_strength != g_runtime.live_profile.stat_bonus_strength ||
        raw_stat_bonus_grit != g_runtime.live_profile.stat_bonus_grit ||
        raw_stat_bonus_deadeye != g_runtime.live_profile.stat_bonus_deadeye ||
        raw_stat_bonus_survival != g_runtime.live_profile.stat_bonus_survival ||
        raw_stat_bonus_cunning != g_runtime.live_profile.stat_bonus_cunning ||
        raw_skill_bonus_hunting != g_runtime.live_profile.skill_bonus_hunting ||
        raw_skill_bonus_gunsmithing != g_runtime.live_profile.skill_bonus_gunsmithing ||
        raw_skill_bonus_field_medicine != g_runtime.live_profile.skill_bonus_field_medicine ||
        raw_skill_bonus_trapping != g_runtime.live_profile.skill_bonus_trapping ||
        raw_skill_bonus_salvaging != g_runtime.live_profile.skill_bonus_salvaging ||
        raw_skill_bonus_trade != g_runtime.live_profile.skill_bonus_trade ||
        raw_skill_bonus_horse_handling != g_runtime.live_profile.skill_bonus_horse_handling ||
        raw_skill_bonus_homestead != g_runtime.live_profile.skill_bonus_homestead ||
        raw_field_distance_total != g_runtime.live_profile.field_distance_total ||
        raw_field_distance_since_hub != g_runtime.live_profile.field_distance_since_hub;

    if (sanitized) {
        g_runtime.live_profile.dirty = 1;
        if (GetTickCount() - g_runtime.last_profile_recovery_log_at >= kFrontierWriteFailureLogCooldownMs) {
            g_runtime.last_profile_recovery_log_at = GetTickCount();
            frontier_log(
                "Profile recovery applied. Phase=%d Stage=%d Regions=%d Hub=%d Enemy=%d Loot=%d Hunt=%d Reward=%d CraftedMask=0x%X",
                g_runtime.live_profile.phase,
                g_runtime.live_profile.trail_stage,
                g_runtime.live_profile.discovered_region_mask,
                g_runtime.live_profile.hub_returns,
                g_runtime.live_profile.enemy_kills,
                g_runtime.live_profile.loot_count,
                g_runtime.live_profile.animal_kills,
                g_runtime.live_profile.reward_earned,
                g_runtime.live_profile.crafted_mask
            );
        }
    }

    g_runtime.live_profile.discovered_region_count = frontier_count_region_bits(g_runtime.live_profile.discovered_region_mask);
    frontier_live_profile_refresh_progression_layers();
    frontier_sanitize_growth_allocations();
    frontier_live_profile_refresh_progression_layers();
    frontier_live_profile_refresh_strings();
    frontier_log(
        "Profile loaded. Phase=%d Stage=%d Enemy=%d Loot=%d Hunt=%d Reward=%d Regions=%d Hub=%d CraftedMask=0x%X StatPts=%d SkillPts=%d",
        g_runtime.live_profile.phase,
        g_runtime.live_profile.trail_stage,
        g_runtime.live_profile.enemy_kills,
        g_runtime.live_profile.loot_count,
        g_runtime.live_profile.animal_kills,
        g_runtime.live_profile.reward_earned,
        g_runtime.live_profile.discovered_region_count,
        g_runtime.live_profile.hub_returns,
        g_runtime.live_profile.crafted_mask,
        frontier_live_stat_points_available(),
        frontier_live_skill_points_available()
    );
}

