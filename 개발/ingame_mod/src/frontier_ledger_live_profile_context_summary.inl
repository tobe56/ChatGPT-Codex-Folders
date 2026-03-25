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

