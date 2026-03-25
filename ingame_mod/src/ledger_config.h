#ifndef LEDGER_CONFIG_H
#define LEDGER_CONFIG_H

#include <stddef.h>

typedef struct FrontierLedgerConfig {
    int compact_toggle_key;
    int full_toggle_key;
    int confirm_key;
    int previous_page_key;
    int next_page_key;
    int previous_entry_key;
    int next_entry_key;
    int close_key;
    int spawn_debug_key;
    int allow_full_ledger_anywhere;
    int require_interior;
    int disable_controls_while_open;
    int show_zone_prompt;
    int auto_close_full_ledger_outside_zone;
    int show_ready_toast;
    int bootstrap_poll_ms;
    int bootstrap_timeout_ms;
    int status_duration_ms;
    int use_desktop_overlay;
    int live_tracking_poll_ms;
    int tracked_ped_ttl_ms;
    int reward_delta_clamp;
    int enable_live_stamina_bonus;
    int enable_live_weapon_wear_bonus;
    int enable_live_deadeye_bonus;
    float beechers_hope_x;
    float beechers_hope_y;
    float beechers_hope_z;
    float beechers_hope_radius;
    float workbench_x;
    float workbench_y;
    float workbench_z;
    float workbench_radius;
    float stamina_recharge_bonus_per_tier;
    float stamina_sprint_depletion_bonus_per_tier;
    float weapon_degradation_bonus_per_tier;
    float deadeye_delay_bonus_per_tier;
    char default_dataset[32];
    char preferred_locale[8];
    char overlay_font_name[64];
} FrontierLedgerConfig;

void FrontierLedgerConfig_InitDefaults(FrontierLedgerConfig *config);
void FrontierLedgerConfig_Load(const char *module_path, FrontierLedgerConfig *config, char *ini_path, size_t ini_path_size);

#endif
