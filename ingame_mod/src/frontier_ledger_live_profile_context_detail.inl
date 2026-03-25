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

