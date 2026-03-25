#include "frontier_ledger.h"
#include "ledger_config.h"
#include "script_hook_bindings.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "frontier_ledger_generated.h"
#include "frontier_bitmap_font_generated.h"

#define FRONTIER_MAX_PAGES 24
#define HASH_PLAYER_ID 0x217E9DC48139933DULL
#define HASH_PLAYER_PED_ID 0x096275889B8E0EE0ULL
#define HASH_GET_ENTITY_COORDS 0xA86D5F069399F44DULL
#define HASH_GET_INTERIOR_FROM_ENTITY 0xB417689857646F61ULL
#define HASH_IS_PAUSE_MENU_ACTIVE 0x535384D6067BA42EULL
#define HASH_DISABLE_ALL_CONTROL_ACTIONS 0x5F4B6931816E599BULL
#define HASH_GET_GAME_TIMER 0x4F67E8ECA7D3F667ULL
#define HASH_GET_PED_NEARBY_PEDS 0x23F8F5FC7E8C4A6BULL
#define HASH_IS_PED_DEAD_OR_DYING 0x3317DEDB88C95038ULL
#define HASH_IS_PED_A_PLAYER 0x12534C348C6CB68BULL
#define HASH_IS_PED_HUMAN 0xB980061DA992779DULL
#define HASH_GET_PED_SOURCE_OF_DEATH 0x93C8B64DEB84728CULL
#define HASH_HAS_ENTITY_BEEN_DAMAGED_BY_ENTITY 0x7B6E7BEC1143AC86ULL
#define HASH_GET_ENTITY_ATTACHED_TO 0x56D713888A566481ULL
#define HASH_IS_ENTITY_FULLY_LOOTED 0x8DE41E9902E85756ULL
#define HASH_FIND_ENTITY_LOOTING_PED 0xEF2D9ED7CE684F08ULL
#define HASH_MONEY_GET_CASH_BALANCE 0x0C02DABFA3B98176ULL
#define HASH_GET_PED_MONEY 0x3F69145BBA87BAE7ULL
#define HASH_SET_PLAYER_STAMINA_RECHARGE_MULTIPLIER 0xFECA17CF3343694BULL
#define HASH_SET_PLAYER_STAMINA_SPRINT_DEPLETION_MULTIPLIER 0xBBADFB5E5E5766FBULL
#define HASH_SET_WEAPON_DEGRADATION_MODIFIER 0x11A7FF918EF6BC66ULL
#define HASH_SET_DELAY_DEADEYE_DEPLETION 0x870634493CB4372CULL
#define HASH_SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER 0x8899C244EBCF70DEULL
#define HASH_SET_PLAYER_HEALTH_RECHARGE_TIME_MODIFIER 0x535ED4605F89AB6EULL
#define HASH_SET_PLAYER_WEAPON_DAMAGE_MODIFIER 0x94D529F7B73D7A85ULL
#define HASH_SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER 0xE4CB5A3F18170381ULL
#define HASH_SET_RECEIVED_DAMAGE_TAKEN_ON_HORSEBACK_MODIFIER 0xB427911EA6DFFEF3ULL
#define HASH_SET_PED_MOVE_RATE_OVERRIDE 0x085BF80FA50A39D1ULL
#define HASH_GET_ENTITY_HEADING 0xC230DD956E2F5507ULL
#define HASH_GET_ENTITY_MODEL 0xDA76A9F39210D365ULL
#define HASH_DOES_ENTITY_EXIST 0xD42BD6EB2E0F1677ULL
#define HASH_SET_ENTITY_MAX_HEALTH 0x166E7CF68597D8B5ULL
#define HASH_SET_ENTITY_HEALTH 0xAC2767ED8BDFAB15ULL
#define HASH_GET_GROUND_Z_FOR_3D_COORD 0x24FA4267BB8D2431ULL
#define HASH_GET_HASH_KEY 0xFD340785ADF8CFB7ULL
#define HASH_IS_MODEL_VALID 0x392C8D8E07B70EFCULL
#define HASH_REQUEST_MODEL 0xFA28FE3A6246FC30ULL
#define HASH_HAS_MODEL_LOADED 0x1283B8B89DD5D1B6ULL
#define HASH_SET_MODEL_AS_NO_LONGER_NEEDED 0x4AD96EF928BD4F9AULL
#define HASH_CREATE_PED 0xD49F9B0955C367DEULL
#define HASH_CLONE_PED 0xEF29A16337FACADBULL
#define HASH_VAR_STRING 0xFA925AC00EB830B9ULL
#define HASH_DOES_TEXT_LABEL_EXIST 0x73C258C68D6F55B6ULL
#define HASH_GET_LABEL_TEXT_2 0x3429670F9B9EF2D3ULL
#define HASH_DRAW_RECT 0x405224591DF02025ULL
#define HASH_BG_SET_TEXT_SCALE 0xA1253A3C870B6843ULL
#define HASH_BG_SET_TEXT_COLOR 0x16FA5CE47F184F1EULL
#define HASH_BG_DISPLAY_TEXT 0x16794E044C9EFB58ULL
#define HASH_SET_PED_MAX_HEALTH 0xF5F6378C4F3419D3ULL
#define HASH_SET_PED_ACCURACY 0x7AEFB85C1D49DEB6ULL
#define HASH_SET_PED_COMBAT_ABILITY 0xC7622C0D36B2FDA8ULL
#define HASH_SET_PED_COMBAT_RANGE 0x3C606747B23E497BULL
#define HASH_SET_PED_COMBAT_MOVEMENT 0x4D9CA1009AFBD057ULL
#define HASH_SET_PED_COMBAT_ATTRIBUTES 0x9F7794730795E019ULL
#define HASH_SET_PED_CAN_BE_TARGETTED 0x63F58F7C80513AADULL
#define HASH_SET_PED_CAN_RAGDOLL 0xB128377056A54E2AULL
#define HASH_SET_PED_KEEP_TASK 0x971D38760FBC02EFULL
#define HASH_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS 0x9F8AA94D6D97DBF4ULL
#define HASH_TASK_COMBAT_PED 0xF166E48407BAC484ULL
#define HASH_TASK_COMBAT_HATED_TARGETS_AROUND_PED 0x7BF835BB9E2698C8ULL
#define HASH_GIVE_WEAPON_TO_PED 0x5E3BDDBCB83F3D84ULL
#define HASH_SET_PED_RELATIONSHIP_GROUP_HASH 0xC80A74AC829DDD92ULL
#define HASH_GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS 0x1899F328B0E12848ULL
#define HASH_SET_ENTITY_COORDS_NO_OFFSET 0x239A3351AC1DA385ULL
#define HASH_SET_ENTITY_HEADING 0xCF2B9C0645C4651BULL
#define HASH_SET_ENTITY_VISIBLE 0x1794B4FCC84D812FULL
#define HASH_SET_ENTITY_ALPHA 0x0DF7692B1D9E7BA7ULL
#define HASH_FREEZE_ENTITY_POSITION 0x7D9EFB7AD6B19754ULL
#define HASH_SET_ENTITY_INVINCIBLE 0xA5C38736C426FCB8ULL
#define HASH_RESURRECT_PED 0x71BC8E838B9C6035ULL
#define HASH_SET_RANDOM_OUTFIT_VARIATION 0x283978A15512B2FEULL
#define HASH_UPDATE_PED_VARIATION 0xCC8CA3E88256E58FULL
#define FRONTIER_BUILD_TAG "2026-03-22-player-model-restore-pass"
#define FRONTIER_LIVE_TRACKED_PED_SLOTS 64
#define FRONTIER_LIVE_NEARBY_PED_SCAN_MAX 16
#define FRONTIER_LIVE_RECENT_EVENT_SLOTS 96
#define FRONTIER_WORKBENCH_CRAFT_COUNT 13
#define FRONTIER_WORKBENCH_ARCHIVE_COUNT 8
#define FRONTIER_WORKBENCH_RESULT_COUNT 4

typedef struct FrontierColor {
    int red;
    int green;
    int blue;
    int alpha;
} FrontierColor;

typedef struct FrontierZoneState {
    int valid;
    int inside_zone;
    int in_radius;
    int interior_id;
    float distance_sq;
    float height_delta;
    FrontierVector3 position;
    DWORD refreshed_at;
    DWORD last_inside_at;
} FrontierZoneState;

typedef struct FrontierRegionAnchor {
    const char *label;
    const char *label_ko;
    float x;
    float y;
    const char *route_hint;
    const char *route_hint_ko;
} FrontierRegionAnchor;

typedef struct FrontierContextState {
    int valid;
    int region_index;
    float distance_sq;
    DWORD refreshed_at;
    char region_label[64];
    char route_hint[128];
} FrontierContextState;

typedef struct FrontierLiveProfile {
    int phase;
    int trail_stage;
    int discovered_region_mask;
    int discovered_region_count;
    int hub_returns;
    int enemy_kills;
    int loot_count;
    int animal_kills;
    int reward_earned;
    uint32_t crafted_mask;
    int stat_bonus_strength;
    int stat_bonus_grit;
    int stat_bonus_deadeye;
    int stat_bonus_survival;
    int stat_bonus_cunning;
    int stat_strength;
    int stat_grit;
    int stat_deadeye;
    int stat_survival;
    int stat_cunning;
    int skill_bonus_hunting;
    int skill_bonus_gunsmithing;
    int skill_bonus_field_medicine;
    int skill_bonus_trapping;
    int skill_bonus_salvaging;
    int skill_bonus_trade;
    int skill_bonus_horse_handling;
    int skill_bonus_homestead;
    int skill_hunting;
    int skill_gunsmithing;
    int skill_field_medicine;
    int skill_trapping;
    int skill_salvaging;
    int skill_trade;
    int skill_horse_handling;
    int skill_homestead;
    int workshop_tier;
    int armory_tier;
    int medicine_tier;
    int contract_count;
    int salvage_stock;
    int hide_stock;
    int tonic_stock;
    int relic_stock;
    int weapon_familiarity_rank;
    int weapon_maintenance_tier;
    int elite_pressure;
    int boss_blackwater_ready;
    int boss_silas_ready;
    int boss_relic_ready;
    int abigail_support_rank;
    int jack_support_rank;
    int uncle_support_rank;
    int ranch_threat_level;
    int ranch_defense_level;
    int surveyor_clue_count;
    int sky_devil_stage;
    int farming_lane;
    uint32_t perk_flags;
    float field_distance_total;
    float field_distance_since_hub;
    int has_last_position;
    int was_inside_zone;
    int dirty;
    DWORD last_saved_at;
    FrontierVector3 last_position;
    char phase_label[64];
    char progress_summary[224];
    char activity_summary[160];
    char homefront_summary[160];
    char farming_summary[160];
    char network_summary[160];
    char world_pressure_summary[160];
    char ending_summary[160];
    char bonus_summary[160];
    char active_objective[192];
} FrontierLiveProfile;

typedef struct FrontierTrackedPed {
    int entity;
    int death_resolved;
    int loot_resolved;
    int spawned_by_frontier;
    const char *spawn_label;
    const char *spawn_label_ko;
    DWORD loot_candidate_at;
    DWORD last_seen_at;
} FrontierTrackedPed;

typedef struct FrontierRecentEvent {
    int kind;
    int entity;
    int value;
    DWORD recorded_at;
} FrontierRecentEvent;

typedef enum FrontierOverlayMode {
    FRONTIER_OVERLAY_MODE_HIDDEN = 0,
    FRONTIER_OVERLAY_MODE_COMPACT = 1,
    FRONTIER_OVERLAY_MODE_FULL = 2,
    FRONTIER_OVERLAY_MODE_HUD = 3
} FrontierOverlayMode;

typedef struct FrontierOverlayState {
    HWND window;
    HWND game_window;
    RECT bounds;
    int visible;
    int mode;
    int prompt_visible;
    int status_visible;
    DWORD layout_generation;
} FrontierOverlayState;

typedef struct FrontierLedgerRuntime {
    HMODULE module;
    char module_path[MAX_PATH];
    char ini_path[MAX_PATH];
    char log_path[MAX_PATH];
    char session_path[MAX_PATH];
    char profile_path[MAX_PATH];
    char requested_dataset_id[32];
    FrontierLedgerConfig config;
    const FrontierLedgerDataset *dataset;
    HANDLE bootstrap_thread;
    int compact_visible;
    int full_visible;
    int current_page;
    int selected_entry[FRONTIER_MAX_PAGES];
    int top_entry[FRONTIER_MAX_PAGES];
    DWORD status_expires_at;
    char status_message[192];
    int announced_ready;
    int label_probe_logged;
    LONG script_hook_registered;
    LONG bootstrap_finished;
    DWORD ui_generation;
    FrontierZoneState zone_state;
    FrontierContextState context;
    FrontierLiveProfile live_profile;
    char runtime_page_summary[512];
    char runtime_summary_buffer[256];
    char runtime_entry_label[256];
    char runtime_entry_detail[2048];
    FrontierOverlayState overlay;
    int workbench_visible;
    int workbench_tab;
    int workbench_selected_entry[3];
    int workbench_prompt_visible;
    int corpse_prompt_visible;
    char workbench_result_title[96];
    char workbench_result_body[512];
    FrontierTrackedPed tracked_peds[FRONTIER_LIVE_TRACKED_PED_SLOTS];
    FrontierRecentEvent recent_events[FRONTIER_LIVE_RECENT_EVENT_SLOTS];
    DWORD last_activity_sample_at;
    int has_last_cash_balance;
    int last_cash_balance;
    int applied_bonus_tier;
    float applied_stamina_recharge_multiplier;
    float applied_stamina_depletion_multiplier;
    float applied_weapon_degradation_modifier;
    float applied_deadeye_delay;
    float applied_health_recharge_multiplier;
    float applied_health_recharge_time_modifier;
    float applied_weapon_damage_modifier;
    float applied_melee_damage_modifier;
    float applied_horseback_damage_reduction;
    float applied_move_rate_override;
    DWORD last_reward_guard_log_at;
    DWORD last_profile_write_failure_at;
    DWORD last_session_write_failure_at;
    DWORD last_profile_save_log_at;
    DWORD last_session_save_log_at;
    DWORD last_profile_recovery_log_at;
    int last_reward_delta;
    DWORD last_reward_counted_at;
    DWORD last_spawn_triggered_at;
    int pending_spawn_request;
    int debug_spawn_single_ped;
} FrontierLedgerRuntime;

static FrontierLedgerRuntime g_runtime;

static const FrontierColor kColorBackground = {16, 14, 11, 96};
static const FrontierColor kColorPanel = {28, 24, 18, 118};
static const FrontierColor kColorPanelLight = {54, 44, 32, 92};
static const FrontierColor kColorAccent = {191, 162, 109, 255};
static const FrontierColor kColorAccentSoft = {140, 117, 83, 150};
static const FrontierColor kColorText = {236, 223, 202, 255};
static const FrontierColor kColorTextDim = {176, 163, 140, 255};
static const FrontierColor kColorSelection = {96, 69, 36, 150};
static const FrontierColor kColorToast = {94, 56, 25, 145};
static const DWORD kFrontierZoneRefreshMs = 250;
static const DWORD kFrontierZoneGraceMs = 1200;
static const DWORD kFrontierContextRefreshMs = 750;
static const DWORD kFrontierLiveSaveIntervalMs = 5000;
static const DWORD kFrontierLiveActivityRefreshMs = 500;
static const DWORD kFrontierTrackedPedTtlMs = 45000;
static const DWORD kFrontierWriteFailureLogCooldownMs = 5000;
static const DWORD kFrontierSaveLogCooldownMs = 15000;
static const float kFrontierPhaseOneDistance = 1600.0f;
static const float kFrontierPhaseTwoDistance = 7200.0f;

enum {
    FRONTIER_PHASE_SETUP = 0,
    FRONTIER_PHASE_ESCALATION = 1,
    FRONTIER_PHASE_ENDGAME = 2,
};

enum {
    FRONTIER_EVENT_KIND_ENEMY_KILL = 1,
    FRONTIER_EVENT_KIND_ANIMAL_KILL = 2,
    FRONTIER_EVENT_KIND_LOOT = 3,
    FRONTIER_EVENT_KIND_REWARD = 4,
};

enum {
    FRONTIER_SPAWN_REQUEST_NONE = 0,
    FRONTIER_SPAWN_REQUEST_REGION = 1,
    FRONTIER_SPAWN_REQUEST_BOSS = 2,
    FRONTIER_SPAWN_REQUEST_SELECTED = 3,
};

enum {
    FRONTIER_PERK_LONG_TRAIL = 1 << 0,
    FRONTIER_PERK_SHARP_TRACKER = 1 << 1,
    FRONTIER_PERK_FRONT_SIGHT_FAITH = 1 << 2,
    FRONTIER_PERK_QUICK_DRAW = 1 << 3,
    FRONTIER_PERK_FAST_POCKETS = 1 << 4,
    FRONTIER_PERK_RANCH_RESOLVE = 1 << 5,
    FRONTIER_PERK_ASH_MARK = 1 << 6,
};

enum {
    FRONTIER_WORKBENCH_TAB_CRAFT = 0,
    FRONTIER_WORKBENCH_TAB_ARCHIVE = 1,
    FRONTIER_WORKBENCH_TAB_RESULT = 2,
};

enum {
    FRONTIER_CRAFT_RELIABLE_GRIP_WRAP = 1 << 0,
    FRONTIER_CRAFT_SAINT_MEDAL = 1 << 1,
    FRONTIER_CRAFT_BLACKWATER_PRECISION_LOADS = 1 << 2,
    FRONTIER_CRAFT_EXPANDED_WORKSHOP = 1 << 3,
    FRONTIER_CRAFT_LONG_BARREL_WALKER_KIT = 1 << 4,
    FRONTIER_CRAFT_REINFORCED_FRAME = 1 << 5,
    FRONTIER_CRAFT_TRAPPER_SIGIL = 1 << 6,
    FRONTIER_CRAFT_ASH_MARKED_REPEATER = 1 << 7,
    FRONTIER_CRAFT_BLACKWATER_PEACEBREAKER = 1 << 8,
    FRONTIER_CRAFT_FIELD_SUPPORT_CABINET = 1 << 9,
    FRONTIER_CRAFT_TROPHY_BOARD = 1 << 10,
    FRONTIER_CRAFT_RELIC_BINDING_KIT = 1 << 11,
    FRONTIER_CRAFT_RELIC_MARKED_ROUNDS = 1 << 12,
};

static const FrontierRegionAnchor kFrontierRegionAnchors[] = {
    {"West Elizabeth", "웨스트 엘리자베스", -1650.0f, -1367.0f, "Beecher's Hope supply lines and contract pressure remain the cleanest frontier anchor.", "비처스 호프 보급선과 계약 압력이 가장 안정적으로 이어지는 프론티어 거점입니다."},
    {"Great Plains / Tall Trees", "그레이트플레인 / 톨트리스", -1220.0f, -1320.0f, "Convoy salvage, tracker pressure, and Silas-linked routes dominate this lane.", "호송대 회수품, 추적자 압력, 사일러스 연계 루트가 이 구간을 지배합니다."},
    {"New Austin", "뉴 오스틴", -3700.0f, -2200.0f, "Harsh outlaw routes and salvage pressure make this the roughest frontier work loop.", "거친 무법자 루트와 회수 압력 때문에 가장 험한 프론티어 작업 구간입니다."},
    {"New Hanover", "뉴 하노버", 1500.0f, 500.0f, "Cleaner hunting and resource loops make this the safest long-haul growth lane.", "사냥과 자원 순환이 비교적 깨끗해서 장거리 성장 루트로 가장 안전합니다."},
    {"Lemoyne", "르모인", 1900.0f, -1800.0f, "Relic whispers, cult traces, and smuggling pressure push the myth thread here.", "유물의 흔적, 사이비 단서, 밀수 압력이 이 지역의 신화 루트를 밀어 올립니다."},
    {"Ambarino", "암바리노", 600.0f, 1900.0f, "Late endurance routes and Surveyor-linked cache trails turn this into endgame space.", "후반 지구력 루트와 측량사 연계 은닉처가 이 지역을 엔드게임 구간으로 만듭니다."},
};
static const int kFrontierRegionAnchorCount = (int)(sizeof(kFrontierRegionAnchors) / sizeof(kFrontierRegionAnchors[0]));

#include "frontier_ledger_helpers_core.inl"

#include "frontier_ledger_spawn.inl"

#include "frontier_ledger_helpers_text.inl"

#include "frontier_ledger_live_profile.inl"

#include "frontier_ledger_persistence.inl"

static void frontier_apply_live_gameplay_effects(void)
{
    int player_id = frontier_player_id();
    int player_ped = frontier_player_ped();
    int bonus_tier = 0;
    float stamina_recharge = 1.0f;
    float stamina_depletion = 1.0f;
    float weapon_degradation = 1.0f;
    float deadeye_delay = 0.0f;
    float health_recharge = 1.0f;
    float health_recharge_time = 1.0f;
    float weapon_damage = 1.0f;
    float melee_damage = 1.0f;
    float horseback_damage_reduction = 0.0f;
    float move_rate_override = 1.0f;
    float stamina_recharge_bonus = 0.0f;
    float stamina_drain_bonus = 0.0f;
    float weapon_bonus = 0.0f;
    float deadeye_bonus = 0.0f;
    float health_bonus = 0.0f;
    float health_time_bonus = 0.0f;
    float weapon_damage_bonus = 0.0f;
    float melee_damage_bonus = 0.0f;
    float horseback_bonus = 0.0f;
    float move_rate_bonus = 0.0f;

    if (player_id < 0 || player_ped == 0) {
        return;
    }

    if (frontier_runtime_uses_live_dataset()) {
        bonus_tier = frontier_live_bonus_tier();
        stamina_recharge_bonus =
            ((float)frontier_max_int(0, g_runtime.live_profile.stat_grit - 2) * 0.01f) +
            ((float)g_runtime.live_profile.abigail_support_rank * 0.015f) +
            ((float)frontier_max_int(0, g_runtime.live_profile.ranch_defense_level - g_runtime.live_profile.ranch_threat_level) * 0.01f) +
            ((float)frontier_crafted_stamina_bonus_percent() / 100.0f);
        stamina_drain_bonus =
            ((float)frontier_crafted_stamina_drain_bonus_percent() / 100.0f) +
            (frontier_live_perk_unlocked(FRONTIER_PERK_LONG_TRAIL) ? 0.02f : 0.0f) +
            (frontier_live_perk_unlocked(FRONTIER_PERK_RANCH_RESOLVE) ? 0.025f : 0.0f) +
            ((float)g_runtime.live_profile.abigail_support_rank * 0.008f);
        weapon_bonus =
            ((float)frontier_crafted_weapon_bonus_percent() / 100.0f) +
            ((float)frontier_max_int(0, g_runtime.live_profile.armory_tier - 1) * 0.018f) +
            ((float)g_runtime.live_profile.uncle_support_rank * 0.012f) +
            ((float)frontier_max_int(0, g_runtime.live_profile.skill_gunsmithing - 1) * 0.010f);
        deadeye_bonus =
            ((float)frontier_crafted_deadeye_bonus_percent() / 100.0f) +
            ((float)frontier_max_int(0, g_runtime.live_profile.stat_deadeye - 2) * 0.03f) +
            ((float)g_runtime.live_profile.jack_support_rank * 0.02f) +
            ((float)g_runtime.live_profile.surveyor_clue_count * 0.004f) +
            (frontier_live_perk_unlocked(FRONTIER_PERK_ASH_MARK) ? 0.03f : 0.0f);
        health_bonus =
            ((float)frontier_max_int(0, g_runtime.live_profile.stat_grit - 2) * 0.04f) +
            ((float)g_runtime.live_profile.abigail_support_rank * 0.04f) +
            (frontier_live_perk_unlocked(FRONTIER_PERK_RANCH_RESOLVE) ? 0.08f : 0.0f);
        health_time_bonus =
            ((float)g_runtime.live_profile.medicine_tier * 0.08f) +
            ((float)g_runtime.live_profile.abigail_support_rank * 0.06f) +
            (frontier_live_perk_unlocked(FRONTIER_PERK_RANCH_RESOLVE) ? 0.08f : 0.0f);
        weapon_damage_bonus =
            ((float)frontier_max_int(0, g_runtime.live_profile.stat_deadeye - 2) * 0.025f) +
            ((float)frontier_max_int(0, g_runtime.live_profile.skill_gunsmithing - 1) * 0.02f) +
            (frontier_live_perk_unlocked(FRONTIER_PERK_QUICK_DRAW) ? 0.04f : 0.0f) +
            (frontier_live_perk_unlocked(FRONTIER_PERK_ASH_MARK) ? 0.06f : 0.0f);
        melee_damage_bonus =
            ((float)frontier_max_int(0, g_runtime.live_profile.stat_strength - 2) * 0.04f) +
            ((float)frontier_max_int(0, g_runtime.live_profile.ranch_defense_level - 1) * 0.02f) +
            (frontier_live_perk_unlocked(FRONTIER_PERK_RANCH_RESOLVE) ? 0.05f : 0.0f);
        horseback_bonus =
            ((float)frontier_max_int(0, g_runtime.live_profile.skill_horse_handling - 1) * 0.03f) +
            ((float)frontier_max_int(0, g_runtime.live_profile.discovered_region_count - 1) * 0.01f) +
            (frontier_live_perk_unlocked(FRONTIER_PERK_LONG_TRAIL) ? 0.04f : 0.0f);
        move_rate_bonus =
            ((float)frontier_max_int(0, g_runtime.live_profile.skill_horse_handling - 1) * 0.01f) +
            ((float)frontier_max_int(0, g_runtime.live_profile.stat_survival - 2) * 0.008f) +
            (frontier_live_perk_unlocked(FRONTIER_PERK_LONG_TRAIL) ? 0.02f : 0.0f);
        if (g_runtime.config.enable_live_stamina_bonus && bonus_tier > 0) {
            stamina_recharge = frontier_clamp_float(
                1.0f + (g_runtime.config.stamina_recharge_bonus_per_tier * (float)bonus_tier) + stamina_recharge_bonus,
                1.0f,
                1.55f
            );
            stamina_depletion = frontier_clamp_float(
                1.0f - ((g_runtime.config.stamina_sprint_depletion_bonus_per_tier * (float)bonus_tier) + stamina_drain_bonus),
                0.68f,
                1.0f
            );
        } else if (g_runtime.config.enable_live_stamina_bonus && (stamina_recharge_bonus > 0.0f || stamina_drain_bonus > 0.0f)) {
            stamina_recharge = frontier_clamp_float(1.0f + stamina_recharge_bonus, 1.0f, 1.35f);
            stamina_depletion = frontier_clamp_float(1.0f - stamina_drain_bonus, 0.78f, 1.0f);
        }
        if (g_runtime.config.enable_live_weapon_wear_bonus && (bonus_tier > 0 || weapon_bonus > 0.0f)) {
            weapon_degradation = frontier_clamp_float(
                1.0f - ((g_runtime.config.weapon_degradation_bonus_per_tier * (float)bonus_tier) + weapon_bonus),
                0.66f,
                1.0f
            );
        }
        if (g_runtime.config.enable_live_deadeye_bonus &&
            (bonus_tier > 0 || g_runtime.live_profile.stat_deadeye >= 3 || frontier_live_perk_unlocked(FRONTIER_PERK_FRONT_SIGHT_FAITH))) {
            deadeye_delay =
                (g_runtime.config.deadeye_delay_bonus_per_tier * (float)bonus_tier) +
                deadeye_bonus +
                (frontier_live_perk_unlocked(FRONTIER_PERK_FRONT_SIGHT_FAITH) ? 0.04f : 0.0f) +
                (frontier_live_perk_unlocked(FRONTIER_PERK_QUICK_DRAW) ? 0.03f : 0.0f);
            deadeye_delay = frontier_clamp_float(deadeye_delay, 0.0f, 0.62f);
        }
        health_recharge = frontier_clamp_float(1.0f + health_bonus, 1.0f, 1.65f);
        health_recharge_time = frontier_clamp_float(1.0f - health_time_bonus, 0.30f, 1.0f);
        weapon_damage = frontier_clamp_float(1.0f + weapon_damage_bonus, 1.0f, 1.55f);
        melee_damage = frontier_clamp_float(1.0f + melee_damage_bonus, 1.0f, 1.55f);
        horseback_damage_reduction = frontier_clamp_float(horseback_bonus, 0.0f, 0.45f);
        move_rate_override = frontier_clamp_float(1.0f + move_rate_bonus, 1.0f, 1.15f);
    }

    if (frontier_abs_float(g_runtime.applied_stamina_recharge_multiplier - stamina_recharge) > 0.001f) {
        frontier_set_player_stamina_recharge_multiplier(player_id, stamina_recharge);
        g_runtime.applied_stamina_recharge_multiplier = stamina_recharge;
    }

    if (frontier_abs_float(g_runtime.applied_stamina_depletion_multiplier - stamina_depletion) > 0.001f) {
        frontier_set_player_stamina_depletion_multiplier(player_id, stamina_depletion);
        g_runtime.applied_stamina_depletion_multiplier = stamina_depletion;
    }

    if (frontier_abs_float(g_runtime.applied_weapon_degradation_modifier - weapon_degradation) > 0.001f) {
        frontier_set_weapon_degradation_modifier(player_id, weapon_degradation);
        g_runtime.applied_weapon_degradation_modifier = weapon_degradation;
    }

    if (frontier_abs_float(g_runtime.applied_deadeye_delay - deadeye_delay) > 0.001f) {
        frontier_set_delay_deadeye_depletion(player_id, deadeye_delay);
        g_runtime.applied_deadeye_delay = deadeye_delay;
    }

    if (frontier_abs_float(g_runtime.applied_health_recharge_multiplier - health_recharge) > 0.001f) {
        frontier_set_player_health_recharge_multiplier(player_id, health_recharge);
        g_runtime.applied_health_recharge_multiplier = health_recharge;
    }

    if (frontier_abs_float(g_runtime.applied_health_recharge_time_modifier - health_recharge_time) > 0.001f) {
        frontier_set_player_health_recharge_time_modifier(player_id, health_recharge_time);
        g_runtime.applied_health_recharge_time_modifier = health_recharge_time;
    }

    if (frontier_abs_float(g_runtime.applied_weapon_damage_modifier - weapon_damage) > 0.001f) {
        frontier_set_player_weapon_damage_modifier(player_id, weapon_damage);
        g_runtime.applied_weapon_damage_modifier = weapon_damage;
    }

    if (frontier_abs_float(g_runtime.applied_melee_damage_modifier - melee_damage) > 0.001f) {
        frontier_set_player_melee_damage_modifier(player_id, melee_damage);
        g_runtime.applied_melee_damage_modifier = melee_damage;
    }

    if (frontier_abs_float(g_runtime.applied_horseback_damage_reduction - horseback_damage_reduction) > 0.001f) {
        frontier_set_received_horseback_damage_modifier(player_id, horseback_damage_reduction);
        g_runtime.applied_horseback_damage_reduction = horseback_damage_reduction;
    }

    if (frontier_abs_float(g_runtime.applied_move_rate_override - move_rate_override) > 0.001f) {
        frontier_set_ped_move_rate_override(player_ped, move_rate_override);
        g_runtime.applied_move_rate_override = move_rate_override;
    }

    if (bonus_tier != g_runtime.applied_bonus_tier) {
        frontier_log(
            "Live gameplay bonus tier changed from %d to %d. StaminaRecharge=%.2f StaminaDrain=%.2f WeaponWear=%.2f DeadeyeDelay=%.2f HealthRegen=%.2f HealthTime=%.2f WeaponDamage=%.2f Melee=%.2f Horseback=%.2f MoveRate=%.2f",
            g_runtime.applied_bonus_tier,
            bonus_tier,
            stamina_recharge,
            stamina_depletion,
            weapon_degradation,
            deadeye_delay,
            health_recharge,
            health_recharge_time,
            weapon_damage,
            melee_damage,
            horseback_damage_reduction,
            move_rate_override
        );
        g_runtime.applied_bonus_tier = bonus_tier;
        frontier_mark_ui_dirty();
    }
}

static void frontier_live_profile_refresh_activity(int force_refresh)
{
    int player_ped;
    int nearby_peds[FRONTIER_LIVE_NEARBY_PED_SCAN_MAX];
    int nearby_count;
    int index;
    int cash_balance;
    DWORD now;
    int reward_delta_clamp = frontier_max_int(50, g_runtime.config.reward_delta_clamp);
    DWORD poll_ms = (DWORD)(g_runtime.config.live_tracking_poll_ms > 0 ? g_runtime.config.live_tracking_poll_ms : (int)kFrontierLiveActivityRefreshMs);
    poll_ms = (DWORD)frontier_max_int(250, (int)poll_ms);

    if (!force_refresh && g_runtime.last_activity_sample_at != 0) {
        DWORD elapsed = (DWORD)frontier_game_timer() - g_runtime.last_activity_sample_at;
        if (elapsed < poll_ms) {
            return;
        }
    }

    player_ped = frontier_player_ped();
    if (player_ped == 0) {
        return;
    }

    now = (DWORD)frontier_game_timer();
    g_runtime.last_activity_sample_at = now;

    cash_balance = frontier_get_cash_balance();
    if (cash_balance >= 0) {
        if (g_runtime.has_last_cash_balance) {
            int delta = cash_balance - g_runtime.last_cash_balance;
            if (delta > 0 && delta <= reward_delta_clamp) {
                frontier_live_profile_register_reward(delta);
            } else if (delta > reward_delta_clamp &&
                       now - g_runtime.last_reward_guard_log_at >= kFrontierWriteFailureLogCooldownMs) {
                g_runtime.last_reward_guard_log_at = now;
                frontier_log(
                    "Ignored suspicious reward delta. Delta=%d Clamp=%d CurrentCash=%d PreviousCash=%d",
                    delta,
                    reward_delta_clamp,
                    cash_balance,
                    g_runtime.last_cash_balance
                );
            }
        }
        g_runtime.last_cash_balance = cash_balance;
        g_runtime.has_last_cash_balance = 1;
    }

    ZeroMemory(nearby_peds, sizeof(nearby_peds));
    nearby_count = frontier_get_ped_nearby_peds(player_ped, nearby_peds, FRONTIER_LIVE_NEARBY_PED_SCAN_MAX);
    for (index = 0; index < nearby_count; ++index) {
        int ped = nearby_peds[index];
        FrontierTrackedPed *tracked;

        if (ped == 0 || ped == player_ped) {
            continue;
        }

        tracked = frontier_live_find_tracked_ped(ped, 1, now);
        if (tracked == NULL) {
            continue;
        }

        if (!frontier_is_ped_dead_or_dying(ped)) {
            tracked->death_resolved = 0;
            tracked->loot_candidate_at = 0;
        }
    }

    for (index = 0; index < FRONTIER_LIVE_TRACKED_PED_SLOTS; ++index) {
        FrontierTrackedPed *tracked = &g_runtime.tracked_peds[index];
        int ped = tracked->entity;

        if (ped == 0 || ped == player_ped || !frontier_does_entity_exist(ped)) {
            continue;
        }

        if (!frontier_is_ped_dead_or_dying(ped)) {
            continue;
        }

        if (!tracked->death_resolved) {
            if (frontier_live_should_credit_player_kill(ped, player_ped, tracked)) {
                if (frontier_is_trackable_enemy_ped(ped)) {
                    frontier_live_profile_register_enemy_kill(ped);
                } else if (frontier_is_trackable_animal_ped(ped)) {
                    frontier_live_profile_register_animal_kill(ped);
                }
            }
            tracked->death_resolved = 1;
        }

        if (!tracked->loot_resolved) {
            int looting_ped = frontier_find_entity_looting_ped(ped);
            if (looting_ped == player_ped) {
                if (tracked->loot_candidate_at == 0) {
                    tracked->loot_candidate_at = now;
                }
                if (frontier_is_entity_fully_looted(ped) || now - tracked->loot_candidate_at >= 300U) {
                    frontier_live_profile_register_loot(ped);
                    tracked->loot_resolved = 1;
                    tracked->loot_candidate_at = 0;
                }
            } else if (frontier_is_entity_fully_looted(ped)) {
                tracked->loot_resolved = 1;
                tracked->loot_candidate_at = 0;
            } else {
                tracked->loot_candidate_at = 0;
            }
        }
    }
}

static void frontier_live_profile_mark_region_discovered(int region_index)
{
    int region_bit;
    char message[192];

    if (region_index < 0 || region_index >= kFrontierRegionAnchorCount) {
        return;
    }

    region_bit = 1 << region_index;
    if ((g_runtime.live_profile.discovered_region_mask & region_bit) != 0) {
        return;
    }

    g_runtime.live_profile.discovered_region_mask |= region_bit;
    g_runtime.live_profile.discovered_region_count = frontier_count_region_bits(g_runtime.live_profile.discovered_region_mask);
    g_runtime.live_profile.dirty = 1;
    snprintf(message, sizeof(message), "Frontier region logged: %s", kFrontierRegionAnchors[region_index].label);
    frontier_set_status(message);
    frontier_log("Live run discovered region: %s", kFrontierRegionAnchors[region_index].label);
}

static void frontier_live_profile_apply_phase(int previous_phase)
{
    int new_phase = frontier_live_profile_phase_from_state();

    g_runtime.live_profile.phase = new_phase;
    frontier_live_profile_refresh_strings();

    if (new_phase == previous_phase) {
        return;
    }

    g_runtime.live_profile.dirty = 1;
    frontier_log("Live frontier phase advanced from %d to %d.", previous_phase, new_phase);

    if (frontier_runtime_uses_live_dataset()) {
        frontier_resolve_dataset_pointer();
        frontier_clamp_selection(g_runtime.current_page);
        frontier_apply_context_focus_for_page(g_runtime.current_page);
        frontier_session_save();
    }

    frontier_mark_ui_dirty();

    if (new_phase == FRONTIER_PHASE_ENDGAME) {
        frontier_set_status("Live frontier run entered the endgame circuit.");
    } else if (new_phase == FRONTIER_PHASE_ESCALATION) {
        frontier_set_status("Live frontier run escalated into the wider contract war.");
    } else {
        frontier_set_status("Live frontier run reset to the homestead setup phase.");
    }
}

static void frontier_refresh_live_profile(int force_refresh)
{
    FrontierVector3 position;
    float step_distance = 0.0f;
    int previous_phase;
    int previous_trail_stage;
    int previous_hub_returns;
    int previous_region_count;
    int previous_abigail;
    int previous_jack;
    int previous_uncle;
    int previous_ranch_threat;
    int previous_ranch_defense;
    int previous_surveyor_clues;
    int previous_sky_devil;
    float previous_field_distance;

    (void)force_refresh;

    if (!g_runtime.zone_state.valid || !g_runtime.context.valid) {
        frontier_live_profile_refresh_strings();
        return;
    }

    position = g_runtime.zone_state.position;
    previous_phase = g_runtime.live_profile.phase;
    previous_trail_stage = g_runtime.live_profile.trail_stage;
    previous_hub_returns = g_runtime.live_profile.hub_returns;
    previous_region_count = g_runtime.live_profile.discovered_region_count;
    previous_abigail = g_runtime.live_profile.abigail_support_rank;
    previous_jack = g_runtime.live_profile.jack_support_rank;
    previous_uncle = g_runtime.live_profile.uncle_support_rank;
    previous_ranch_threat = g_runtime.live_profile.ranch_threat_level;
    previous_ranch_defense = g_runtime.live_profile.ranch_defense_level;
    previous_surveyor_clues = g_runtime.live_profile.surveyor_clue_count;
    previous_sky_devil = g_runtime.live_profile.sky_devil_stage;
    previous_field_distance = g_runtime.live_profile.field_distance_total;

    frontier_live_profile_mark_region_discovered(g_runtime.context.region_index);

    if (g_runtime.live_profile.has_last_position) {
        step_distance = frontier_distance_2d_approx(
            position.x - g_runtime.live_profile.last_position.x,
            position.y - g_runtime.live_profile.last_position.y
        );
        if (step_distance > 2.0f && step_distance < 800.0f) {
            g_runtime.live_profile.field_distance_total += step_distance;
            if (!g_runtime.zone_state.inside_zone) {
                g_runtime.live_profile.field_distance_since_hub += step_distance;
            }
            g_runtime.live_profile.dirty = 1;
        }
    }

    g_runtime.live_profile.last_position = position;
    g_runtime.live_profile.has_last_position = 1;

    if (g_runtime.zone_state.inside_zone &&
        !g_runtime.live_profile.was_inside_zone &&
        g_runtime.live_profile.field_distance_since_hub >= 220.0f) {
        g_runtime.live_profile.hub_returns += 1;
        g_runtime.live_profile.field_distance_since_hub = 0.0f;
        g_runtime.live_profile.dirty = 1;
        frontier_set_status("Live frontier report returned to Beecher's Hope.");
        frontier_log("Live run hub return counted. Total=%d", g_runtime.live_profile.hub_returns);
    }

    g_runtime.live_profile.was_inside_zone = g_runtime.zone_state.inside_zone;
    frontier_live_profile_refresh_activity(force_refresh);
    frontier_live_profile_refresh_progression_layers();
    g_runtime.live_profile.trail_stage = frontier_live_trail_stage_from_state();
    frontier_live_profile_refresh_progression_layers();
    frontier_live_profile_apply_phase(previous_phase);
    frontier_live_profile_refresh_strings();

    if (g_runtime.live_profile.trail_stage != previous_trail_stage) {
        char message[192];
        g_runtime.live_profile.dirty = 1;
        snprintf(
            message,
            sizeof(message),
            "Frontier chain advanced to %s.",
            frontier_live_trail_stage_label(g_runtime.live_profile.trail_stage)
        );
        frontier_set_status(message);
        frontier_log(
            "Live frontier chain advanced from stage %d to %d.",
            previous_trail_stage,
            g_runtime.live_profile.trail_stage
        );
    }

    if (previous_hub_returns != g_runtime.live_profile.hub_returns ||
        previous_region_count != g_runtime.live_profile.discovered_region_count ||
        previous_phase != g_runtime.live_profile.phase ||
        previous_trail_stage != g_runtime.live_profile.trail_stage ||
        previous_abigail != g_runtime.live_profile.abigail_support_rank ||
        previous_jack != g_runtime.live_profile.jack_support_rank ||
        previous_uncle != g_runtime.live_profile.uncle_support_rank ||
        previous_ranch_threat != g_runtime.live_profile.ranch_threat_level ||
        previous_ranch_defense != g_runtime.live_profile.ranch_defense_level ||
        previous_surveyor_clues != g_runtime.live_profile.surveyor_clue_count ||
        previous_sky_devil != g_runtime.live_profile.sky_devil_stage ||
        previous_field_distance != g_runtime.live_profile.field_distance_total) {
        frontier_mark_ui_dirty();
    }

    frontier_live_profile_save(0);
}

static void frontier_refresh_zone_state(int force_refresh)
{
    int ped;
    int interior;
    int game_timer;
    int previous_inside_zone;
    FrontierVector3 position;
    float dx;
    float dy;
    float dz;
    float radius_sq;
    float distance_sq;
    float release_radius_sq;
    float grace_radius_sq;

    game_timer = frontier_game_timer();
    if (!force_refresh && g_runtime.zone_state.valid && (game_timer - (int)g_runtime.zone_state.refreshed_at) < (int)kFrontierZoneRefreshMs) {
        return;
    }

    previous_inside_zone = g_runtime.zone_state.inside_zone;
    g_runtime.zone_state.valid = 1;
    g_runtime.zone_state.refreshed_at = (DWORD)game_timer;
    g_runtime.zone_state.interior_id = 0;
    g_runtime.zone_state.distance_sq = 0.0f;
    g_runtime.zone_state.height_delta = 0.0f;
    g_runtime.zone_state.position.x = 0.0f;
    g_runtime.zone_state.position.y = 0.0f;
    g_runtime.zone_state.position.z = 0.0f;
    g_runtime.zone_state.in_radius = 0;
    g_runtime.zone_state.inside_zone = 0;

    if (g_runtime.config.allow_full_ledger_anywhere) {
        g_runtime.zone_state.in_radius = 1;
        g_runtime.zone_state.inside_zone = 1;
        g_runtime.zone_state.last_inside_at = (DWORD)game_timer;
        return;
    }

    ped = frontier_player_ped();
    if (ped == 0) {
        return;
    }

    position = frontier_get_entity_coords(ped);
    g_runtime.zone_state.position = position;
    dx = position.x - g_runtime.config.beechers_hope_x;
    dy = position.y - g_runtime.config.beechers_hope_y;
    dz = position.z - g_runtime.config.beechers_hope_z;
    radius_sq = g_runtime.config.beechers_hope_radius * g_runtime.config.beechers_hope_radius;
    release_radius_sq = radius_sq * 1.25f;
    grace_radius_sq = radius_sq * 1.56f;
    distance_sq = (dx * dx) + (dy * dy);
    interior = frontier_get_interior_from_entity(ped);
    g_runtime.zone_state.distance_sq = distance_sq;
    g_runtime.zone_state.height_delta = frontier_abs_float(dz);
    g_runtime.zone_state.interior_id = interior;
    g_runtime.zone_state.in_radius = distance_sq <= radius_sq;

    if (g_runtime.zone_state.in_radius) {
        g_runtime.zone_state.inside_zone = 1;
        g_runtime.zone_state.last_inside_at = (DWORD)game_timer;
        return;
    }

    if (g_runtime.zone_state.height_delta <= 18.0f && distance_sq <= (radius_sq * 1.69f)) {
        g_runtime.zone_state.inside_zone = 1;
        g_runtime.zone_state.last_inside_at = (DWORD)game_timer;
        return;
    }

    if (interior != 0 && distance_sq <= (radius_sq * 6.25f)) {
        g_runtime.zone_state.inside_zone = 1;
        g_runtime.zone_state.last_inside_at = (DWORD)game_timer;
        return;
    }

    if (previous_inside_zone &&
        g_runtime.zone_state.last_inside_at != 0 &&
        ((DWORD)game_timer - g_runtime.zone_state.last_inside_at) <= kFrontierZoneGraceMs &&
        g_runtime.zone_state.height_delta <= 24.0f &&
        distance_sq <= grace_radius_sq) {
        g_runtime.zone_state.inside_zone = 1;
        return;
    }

    if (previous_inside_zone &&
        g_runtime.zone_state.height_delta <= 22.0f &&
        distance_sq <= release_radius_sq) {
        g_runtime.zone_state.inside_zone = 1;
    }

    if (previous_inside_zone != g_runtime.zone_state.inside_zone) {
        frontier_log(
            "Zone state changed. Inside=%d InRadius=%d DistanceSq=%.2f HeightDelta=%.2f Interior=%d",
            g_runtime.zone_state.inside_zone,
            g_runtime.zone_state.in_radius,
            g_runtime.zone_state.distance_sq,
            g_runtime.zone_state.height_delta,
            g_runtime.zone_state.interior_id
        );
        frontier_mark_ui_dirty();
    }
}

static int frontier_inside_beechers_hope_zone(void)
{
    frontier_refresh_zone_state(0);
    return g_runtime.zone_state.inside_zone;
}

typedef struct FrontierWindowSearch {
    DWORD process_id;
    HWND window;
} FrontierWindowSearch;

typedef struct FrontierOverlayFonts {
    HFONT title;
    HFONT heading;
    HFONT body;
    HFONT small;
} FrontierOverlayFonts;

static int frontier_utf8_to_wide(const char *text, WCHAR *buffer, int buffer_count)
{
    int converted;
    const char *resolved = frontier_resolve_text(text);

    if (buffer == NULL || buffer_count <= 0) {
        return 0;
    }

    buffer[0] = L'\0';
    if (resolved == NULL || resolved[0] == '\0') {
        return 0;
    }

    converted = MultiByteToWideChar(CP_UTF8, 0, resolved, -1, buffer, buffer_count);
    if (converted > 0) {
        return converted;
    }

    converted = MultiByteToWideChar(CP_ACP, 0, resolved, -1, buffer, buffer_count);
    if (converted > 0) {
        return converted;
    }

    return 0;
}

static COLORREF frontier_color_ref(FrontierColor color)
{
    return RGB(color.red, color.green, color.blue);
}

static HFONT frontier_overlay_create_font(int pixel_height, int weight)
{
    WCHAR font_name[64];

    if (pixel_height <= 0) {
        pixel_height = 18;
    }

    if (!frontier_utf8_to_wide(g_runtime.config.overlay_font_name, font_name, (int)(sizeof(font_name) / sizeof(font_name[0])))) {
        lstrcpyW(font_name, L"Malgun Gothic");
    }

    return CreateFontW(
        -pixel_height,
        0,
        0,
        0,
        weight,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        font_name
    );
}

static void frontier_overlay_init_fonts(const RECT *client_rect, FrontierOverlayFonts *fonts)
{
    int height;

    if (fonts == NULL || client_rect == NULL) {
        return;
    }

    ZeroMemory(fonts, sizeof(*fonts));
    height = client_rect->bottom - client_rect->top;
    fonts->title = frontier_overlay_create_font(frontier_max_int(20, height / 18), FW_BOLD);
    fonts->heading = frontier_overlay_create_font(frontier_max_int(17, height / 28), FW_BOLD);
    fonts->body = frontier_overlay_create_font(frontier_max_int(15, height / 34), FW_NORMAL);
    fonts->small = frontier_overlay_create_font(frontier_max_int(13, height / 40), FW_NORMAL);
}

static void frontier_overlay_destroy_fonts(FrontierOverlayFonts *fonts)
{
    if (fonts == NULL) {
        return;
    }

    if (fonts->title != NULL) {
        DeleteObject(fonts->title);
    }
    if (fonts->heading != NULL) {
        DeleteObject(fonts->heading);
    }
    if (fonts->body != NULL) {
        DeleteObject(fonts->body);
    }
    if (fonts->small != NULL) {
        DeleteObject(fonts->small);
    }
    ZeroMemory(fonts, sizeof(*fonts));
}

static void frontier_overlay_fill_rect(HDC device_context, const RECT *rect, FrontierColor color)
{
    HBRUSH brush;

    if (device_context == NULL || rect == NULL) {
        return;
    }

    brush = CreateSolidBrush(frontier_color_ref(color));
    if (brush == NULL) {
        return;
    }

    FillRect(device_context, rect, brush);
    DeleteObject(brush);
}

static int frontier_overlay_measure_text(HDC device_context, HFONT font, const char *text, int width, UINT format)
{
    RECT rect;
    WCHAR wide_buffer[4096];
    HGDIOBJ old_font;
    int height;

    if (device_context == NULL || font == NULL || text == NULL || text[0] == '\0' || width <= 0) {
        return 0;
    }

    if (!frontier_utf8_to_wide(text, wide_buffer, (int)(sizeof(wide_buffer) / sizeof(wide_buffer[0])))) {
        return 0;
    }

    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = 0;
    old_font = SelectObject(device_context, font);
    DrawTextW(device_context, wide_buffer, -1, &rect, format | DT_CALCRECT);
    SelectObject(device_context, old_font);

    height = rect.bottom - rect.top;
    if (height <= 0) {
        height = 1;
    }
    return height;
}

static void frontier_overlay_draw_text(
    HDC device_context,
    HFONT font,
    const char *text,
    const RECT *rect,
    FrontierColor color,
    UINT format
)
{
    WCHAR wide_buffer[4096];
    HGDIOBJ old_font;
    RECT draw_rect;

    if (device_context == NULL || font == NULL || rect == NULL || text == NULL || text[0] == '\0') {
        return;
    }

    if (!frontier_utf8_to_wide(text, wide_buffer, (int)(sizeof(wide_buffer) / sizeof(wide_buffer[0])))) {
        return;
    }

    draw_rect = *rect;
    SetBkMode(device_context, TRANSPARENT);
    SetTextColor(device_context, frontier_color_ref(color));
    old_font = SelectObject(device_context, font);
    DrawTextW(device_context, wide_buffer, -1, &draw_rect, format);
    SelectObject(device_context, old_font);
}

static BOOL CALLBACK frontier_find_game_window_callback(HWND window, LPARAM parameter)
{
    FrontierWindowSearch *search = (FrontierWindowSearch *)(uintptr_t)parameter;
    DWORD process_id = 0;
    RECT client_rect;

    if (search == NULL) {
        return TRUE;
    }

    GetWindowThreadProcessId(window, &process_id);
    if (process_id != search->process_id || !IsWindowVisible(window) || GetWindow(window, GW_OWNER) != NULL) {
        return TRUE;
    }

    if (!GetClientRect(window, &client_rect)) {
        return TRUE;
    }
    if ((client_rect.right - client_rect.left) < 640 || (client_rect.bottom - client_rect.top) < 480) {
        return TRUE;
    }

    search->window = window;
    return FALSE;
}

static HWND frontier_find_game_window(void)
{
    FrontierWindowSearch search;

    if (IsWindow(g_runtime.overlay.game_window)) {
        return g_runtime.overlay.game_window;
    }

    search.process_id = GetCurrentProcessId();
    search.window = NULL;
    EnumWindows(frontier_find_game_window_callback, (LPARAM)(uintptr_t)&search);
    g_runtime.overlay.game_window = search.window;
    return g_runtime.overlay.game_window;
}

static int frontier_get_game_client_rect(RECT *screen_rect)
{
    HWND game_window;
    RECT client_rect;
    POINT top_left;
    POINT bottom_right;

    if (screen_rect == NULL) {
        return 0;
    }

    game_window = frontier_find_game_window();
    if (!IsWindow(game_window)) {
        g_runtime.overlay.game_window = NULL;
        return 0;
    }

    if (!GetClientRect(game_window, &client_rect)) {
        return 0;
    }

    top_left.x = client_rect.left;
    top_left.y = client_rect.top;
    bottom_right.x = client_rect.right;
    bottom_right.y = client_rect.bottom;
    ClientToScreen(game_window, &top_left);
    ClientToScreen(game_window, &bottom_right);

    screen_rect->left = top_left.x;
    screen_rect->top = top_left.y;
    screen_rect->right = bottom_right.x;
    screen_rect->bottom = bottom_right.y;
    return 1;
}

static int frontier_game_window_is_foreground(void)
{
    HWND game_window = frontier_find_game_window();
    return game_window != NULL && GetForegroundWindow() == game_window;
}

static void frontier_overlay_hide(void)
{
    if (!IsWindow(g_runtime.overlay.window)) {
        return;
    }

    if (g_runtime.overlay.visible) {
        frontier_log("Desktop overlay hidden.");
    }
    ShowWindow(g_runtime.overlay.window, SW_HIDE);
    g_runtime.overlay.visible = 0;
    g_runtime.overlay.mode = FRONTIER_OVERLAY_MODE_HIDDEN;
}

static void frontier_overlay_pump_messages(void)
{
    MSG message;

    while (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
}

static int frontier_overlay_ensure_window(void)
{
    WNDCLASSEXW window_class;
    LONG_PTR ex_style;
    LONG_PTR style;

    if (IsWindow(g_runtime.overlay.window)) {
        return 1;
    }

    ZeroMemory(&window_class, sizeof(window_class));
    window_class.cbSize = sizeof(window_class);
    window_class.hInstance = g_runtime.module;
    window_class.lpfnWndProc = FrontierOverlayWindowProc;
    window_class.lpszClassName = L"FrontierLedgerDesktopOverlay";
    window_class.hCursor = NULL;
    RegisterClassExW(&window_class);

    g_runtime.overlay.window = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
        L"FrontierLedgerDesktopOverlay",
        L"Frontier Ledger Overlay",
        WS_POPUP | WS_DISABLED,
        0,
        0,
        0,
        0,
        NULL,
        NULL,
        g_runtime.module,
        NULL
    );
    if (g_runtime.overlay.window == NULL) {
        frontier_log("Desktop overlay window creation failed. Error=%lu", GetLastError());
        return 0;
    }

    ex_style = GetWindowLongPtrW(g_runtime.overlay.window, GWL_EXSTYLE);
    SetWindowLongPtrW(
        g_runtime.overlay.window,
        GWL_EXSTYLE,
        ex_style | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE
    );
    style = GetWindowLongPtrW(g_runtime.overlay.window, GWL_STYLE);
    SetWindowLongPtrW(g_runtime.overlay.window, GWL_STYLE, style | WS_POPUP | WS_DISABLED);
    EnableWindow(g_runtime.overlay.window, FALSE);
    ShowWindow(g_runtime.overlay.window, SW_HIDE);
    frontier_log("Desktop overlay window created as plain popup overlay.");
    return 1;
}

static FrontierOverlayMode frontier_overlay_mode(int *prompt_visible, int *status_visible)
{
    if (g_runtime.full_visible) {
        if (prompt_visible != NULL) {
            *prompt_visible = 0;
        }
        if (status_visible != NULL) {
            *status_visible = 0;
        }
        return FRONTIER_OVERLAY_MODE_FULL;
    }

    if (g_runtime.compact_visible) {
        if (prompt_visible != NULL) {
            *prompt_visible = 0;
        }
        if (status_visible != NULL) {
            *status_visible = 0;
        }
        return FRONTIER_OVERLAY_MODE_COMPACT;
    }

    if (prompt_visible != NULL) {
        *prompt_visible = 0;
    }
    if (status_visible != NULL) {
        *status_visible = 0;
    }
    return FRONTIER_OVERLAY_MODE_HIDDEN;
}

static void frontier_overlay_compact_bounds(const RECT *game_rect, RECT *bounds)
{
    int game_width;
    int game_height;
    int panel_height;
    int section_index;
    int line_index;
    int wrapped_lines;

    game_width = game_rect->right - game_rect->left;
    game_height = game_rect->bottom - game_rect->top;
    panel_height = (int)(0.08f * (float)game_height);

    for (section_index = 0; g_runtime.dataset != NULL && section_index < g_runtime.dataset->compact_section_count; ++section_index) {
        panel_height += (int)(0.031f * (float)game_height);
        for (line_index = 0; line_index < g_runtime.dataset->compact_sections[section_index].line_count; ++line_index) {
            wrapped_lines = frontier_count_wrapped_lines(
                g_runtime.dataset->compact_sections[section_index].lines[line_index],
                40,
                3
            );
            panel_height += (int)((0.018f * (float)game_height) * (float)wrapped_lines) + 8;
        }
    }
    panel_height += (int)(0.02f * (float)game_height);

    bounds->left = game_rect->left + (int)(0.68f * (float)game_width);
    bounds->top = game_rect->top + (int)(0.055f * (float)game_height);
    bounds->right = bounds->left + frontier_max_int(420, (int)(0.28f * (float)game_width));
    bounds->bottom = bounds->top + frontier_max_int(260, panel_height);
}

static void frontier_overlay_full_bounds(const RECT *game_rect, RECT *bounds)
{
    int game_width = game_rect->right - game_rect->left;
    int game_height = game_rect->bottom - game_rect->top;

    bounds->left = game_rect->left + (int)(0.06f * (float)game_width);
    bounds->top = game_rect->top + (int)(0.07f * (float)game_height);
    bounds->right = bounds->left + frontier_max_int(1040, (int)(0.88f * (float)game_width));
    bounds->bottom = bounds->top + frontier_max_int(720, (int)(0.84f * (float)game_height));
}

static void frontier_overlay_hud_bounds(const RECT *game_rect, int prompt_visible, int status_visible, RECT *bounds)
{
    int game_width = game_rect->right - game_rect->left;
    int game_height = game_rect->bottom - game_rect->top;
    int width = frontier_max_int(520, (int)(0.38f * (float)game_width));
    int height = 18;

    if (prompt_visible) {
        height += frontier_max_int(56, (int)(0.05f * (float)game_height));
    }
    if (status_visible) {
        height += frontier_max_int(64, (int)(0.06f * (float)game_height));
    }
    if (prompt_visible && status_visible) {
        height += 10;
    }

    bounds->left = game_rect->left + ((game_width - width) / 2);
    bounds->top = game_rect->bottom - height - frontier_max_int(42, (int)(0.05f * (float)game_height));
    bounds->right = bounds->left + width;
    bounds->bottom = bounds->top + height;
}

static void frontier_overlay_compute_bounds(
    FrontierOverlayMode mode,
    int prompt_visible,
    int status_visible,
    const RECT *game_rect,
    RECT *bounds
)
{
    switch (mode) {
    case FRONTIER_OVERLAY_MODE_COMPACT:
        frontier_overlay_compact_bounds(game_rect, bounds);
        break;

    case FRONTIER_OVERLAY_MODE_FULL:
        frontier_overlay_full_bounds(game_rect, bounds);
        break;

    case FRONTIER_OVERLAY_MODE_HUD:
        frontier_overlay_hud_bounds(game_rect, prompt_visible, status_visible, bounds);
        break;

    default:
        bounds->left = 0;
        bounds->top = 0;
        bounds->right = 0;
        bounds->bottom = 0;
        break;
    }
}

#include "frontier_ledger_render_overlay.inl"

#include "frontier_ledger_overlay_control.inl"

#include "frontier_ledger_render_compact.inl"

#include "frontier_ledger_workbench.inl"

#include "frontier_ledger_render_full.inl"

#include "frontier_ledger_input_loop.inl"

BOOL WINAPI DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    (void)reserved;

    switch (reason) {
    case DLL_PROCESS_ATTACH:
        ZeroMemory(&g_runtime, sizeof(g_runtime));
        g_runtime.module = module;
        DisableThreadLibraryCalls(module);
        GetModuleFileNameA(module, g_runtime.module_path, (DWORD)sizeof(g_runtime.module_path));
        frontier_build_neighbor_path(g_runtime.module_path, "FrontierLedger.log", g_runtime.log_path, sizeof(g_runtime.log_path));
        frontier_build_neighbor_path(g_runtime.module_path, "FrontierLedger.session.ini", g_runtime.session_path, sizeof(g_runtime.session_path));
        frontier_build_neighbor_path(g_runtime.module_path, "FrontierLedger.profile.ini", g_runtime.profile_path, sizeof(g_runtime.profile_path));
        FrontierLedgerConfig_Load(g_runtime.module_path, &g_runtime.config, g_runtime.ini_path, sizeof(g_runtime.ini_path));
        frontier_live_profile_load();
        frontier_select_dataset(g_runtime.config.default_dataset);
        frontier_session_load();
        frontier_refresh_context_state(1);
        frontier_live_profile_refresh_strings();
        frontier_log(
            "Process attach. Build=%s Ini=%s Session=%s Profile=%s RequestedDataset=%s Dataset=%s Locale=%s Radius=%.2f RequireInterior=%d ACP=%u DesktopOverlay=%d ConfigDesktopOverlay=%d Font=%s Region=%s Phase=%d",
            FRONTIER_BUILD_TAG,
            g_runtime.ini_path,
            g_runtime.session_path,
            g_runtime.profile_path,
            g_runtime.requested_dataset_id,
            g_runtime.dataset != NULL ? g_runtime.dataset->dataset_id : "null",
            g_runtime.config.preferred_locale,
            g_runtime.config.beechers_hope_radius,
            g_runtime.config.require_interior,
            GetACP(),
            frontier_use_desktop_overlay(),
            g_runtime.config.use_desktop_overlay,
            g_runtime.config.overlay_font_name,
            frontier_runtime_context_label(),
            g_runtime.live_profile.phase
        );
        g_runtime.bootstrap_thread = CreateThread(NULL, 0, FrontierBootstrapThread, NULL, 0, NULL);
        if (g_runtime.bootstrap_thread != NULL) {
            CloseHandle(g_runtime.bootstrap_thread);
            g_runtime.bootstrap_thread = NULL;
            frontier_log("Spawned bootstrap thread.");
        } else {
            frontier_log("Failed to create bootstrap thread.");
        }
        break;

    case DLL_PROCESS_DETACH:
        frontier_session_save();
        frontier_live_profile_save(1);
        frontier_overlay_shutdown();
        if (InterlockedCompareExchange(&g_runtime.script_hook_registered, 0, 0) != 0 && FrontierScriptHook_IsReady()) {
            int player_id = frontier_player_id();
            int player_ped = frontier_player_ped();
            if (player_id >= 0) {
                frontier_set_player_stamina_recharge_multiplier(player_id, 1.0f);
                frontier_set_player_stamina_depletion_multiplier(player_id, 1.0f);
                frontier_set_weapon_degradation_modifier(player_id, 1.0f);
                frontier_set_delay_deadeye_depletion(player_id, 0.0f);
                frontier_set_player_health_recharge_multiplier(player_id, 1.0f);
                frontier_set_player_health_recharge_time_modifier(player_id, 1.0f);
                frontier_set_player_weapon_damage_modifier(player_id, 1.0f);
                frontier_set_player_melee_damage_modifier(player_id, 1.0f);
                frontier_set_received_horseback_damage_modifier(player_id, 0.0f);
            }
            if (player_ped != 0) {
                frontier_set_ped_move_rate_override(player_ped, 1.0f);
            }
        }
        if (InterlockedCompareExchange(&g_runtime.script_hook_registered, 0, 0) != 0 && FrontierScriptHook_IsReady()) {
            FrontierScriptHook_UnregisterKeyboard(FrontierKeyboardMessage);
            FrontierScriptHook_UnregisterModule(module);
        }
        frontier_log("Process detach.");
        break;

    default:
        break;
    }

    return TRUE;
}
