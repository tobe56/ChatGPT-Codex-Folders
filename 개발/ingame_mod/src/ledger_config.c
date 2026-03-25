#include "ledger_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

static void frontier_copy_string(char *destination, size_t destination_size, const char *value)
{
    if (destination == NULL || destination_size == 0U) {
        return;
    }
    destination[0] = '\0';
    if (value == NULL) {
        return;
    }
#if defined(_MSC_VER)
    strncpy_s(destination, destination_size, value, _TRUNCATE);
#else
    strncpy(destination, value, destination_size - 1U);
    destination[destination_size - 1U] = '\0';
#endif
}

static void frontier_build_ini_path(const char *module_path, char *ini_path, size_t ini_path_size)
{
    char buffer[MAX_PATH];
    char *last_separator;
    size_t index = 0U;

    frontier_copy_string(buffer, sizeof(buffer), module_path);
    last_separator = strrchr(buffer, '\\');
    if (last_separator == NULL) {
        last_separator = strrchr(buffer, '/');
    }
    if (last_separator != NULL) {
        *(last_separator + 1) = '\0';
    } else {
        buffer[0] = '.';
        buffer[1] = '\\';
        buffer[2] = '\0';
    }

    if (ini_path != NULL && ini_path_size > 0U) {
        for (index = 0U; index + 1U < ini_path_size && buffer[index] != '\0'; ++index) {
            ini_path[index] = buffer[index];
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = 'F';
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = 'r';
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = 'o';
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = 'n';
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = 't';
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = 'i';
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = 'e';
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = 'r';
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = 'L';
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = 'e';
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = 'd';
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = 'g';
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = 'e';
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = 'r';
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = '.';
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = 'i';
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = 'n';
        }
        if (index + 1U < ini_path_size) {
            ini_path[index++] = 'i';
        }
        ini_path[index] = '\0';
    }
}

static float frontier_get_profile_float(const char *section, const char *key, float default_value, const char *ini_path)
{
    char buffer[64];
    DWORD chars_read;

    chars_read = GetPrivateProfileStringA(section, key, "", buffer, (DWORD)sizeof(buffer), ini_path);
    if (chars_read == 0U) {
        return default_value;
    }
    return (float)atof(buffer);
}

void FrontierLedgerConfig_InitDefaults(FrontierLedgerConfig *config)
{
    if (config == NULL) {
        return;
    }

    config->compact_toggle_key = 121;
    config->full_toggle_key = 122;
    config->confirm_key = 69;
    config->previous_page_key = 37;
    config->next_page_key = 39;
    config->previous_entry_key = 38;
    config->next_entry_key = 40;
    config->close_key = 27;
    config->spawn_debug_key = 119;
    config->allow_full_ledger_anywhere = 1;
    config->require_interior = 0;
    config->disable_controls_while_open = 1;
    config->show_zone_prompt = 0;
    config->auto_close_full_ledger_outside_zone = 1;
    config->show_ready_toast = 0;
    config->bootstrap_poll_ms = 250;
    config->bootstrap_timeout_ms = 30000;
    config->status_duration_ms = 4000;
    config->use_desktop_overlay = 1;
    config->live_tracking_poll_ms = 700;
    config->tracked_ped_ttl_ms = 30000;
    config->reward_delta_clamp = 320;
    config->enable_live_stamina_bonus = 1;
    config->enable_live_weapon_wear_bonus = 1;
    config->enable_live_deadeye_bonus = 1;
    config->beechers_hope_x = -1650.0f;
    config->beechers_hope_y = -1367.0f;
    config->beechers_hope_z = 84.0f;
    config->beechers_hope_radius = 55.0f;
    config->workbench_x = -1650.0f;
    config->workbench_y = -1367.0f;
    config->workbench_z = 84.0f;
    config->workbench_radius = 10.0f;
    config->stamina_recharge_bonus_per_tier = 0.035f;
    config->stamina_sprint_depletion_bonus_per_tier = 0.025f;
    config->weapon_degradation_bonus_per_tier = 0.045f;
    config->deadeye_delay_bonus_per_tier = 0.12f;
    frontier_copy_string(config->default_dataset, sizeof(config->default_dataset), "live_en");
    frontier_copy_string(config->preferred_locale, sizeof(config->preferred_locale), "en");
    frontier_copy_string(config->overlay_font_name, sizeof(config->overlay_font_name), "Malgun Gothic");
}

void FrontierLedgerConfig_Load(const char *module_path, FrontierLedgerConfig *config, char *ini_path, size_t ini_path_size)
{
    char dataset_buffer[32];
    char locale_buffer[8];
    char overlay_font_buffer[64];
    char local_ini_path[MAX_PATH];
    DWORD chars_read;

    if (config == NULL) {
        return;
    }

    FrontierLedgerConfig_InitDefaults(config);
    frontier_build_ini_path(module_path, local_ini_path, sizeof(local_ini_path));
    if (ini_path != NULL && ini_path_size > 0U) {
        frontier_copy_string(ini_path, ini_path_size, local_ini_path);
    }

    config->compact_toggle_key = (int)GetPrivateProfileIntA("Input", "CompactToggleKey", config->compact_toggle_key, local_ini_path);
    config->full_toggle_key = (int)GetPrivateProfileIntA("Input", "FullToggleKey", config->full_toggle_key, local_ini_path);
    config->confirm_key = (int)GetPrivateProfileIntA("Input", "ConfirmKey", config->confirm_key, local_ini_path);
    config->previous_page_key = (int)GetPrivateProfileIntA("Input", "PreviousPageKey", config->previous_page_key, local_ini_path);
    config->next_page_key = (int)GetPrivateProfileIntA("Input", "NextPageKey", config->next_page_key, local_ini_path);
    config->previous_entry_key = (int)GetPrivateProfileIntA("Input", "PreviousEntryKey", config->previous_entry_key, local_ini_path);
    config->next_entry_key = (int)GetPrivateProfileIntA("Input", "NextEntryKey", config->next_entry_key, local_ini_path);
    config->close_key = (int)GetPrivateProfileIntA("Input", "CloseKey", config->close_key, local_ini_path);
    config->spawn_debug_key = (int)GetPrivateProfileIntA("Input", "SpawnDebugKey", config->spawn_debug_key, local_ini_path);

    config->allow_full_ledger_anywhere = (int)GetPrivateProfileIntA("Zone", "AllowFullLedgerAnywhere", config->allow_full_ledger_anywhere, local_ini_path);
    config->require_interior = (int)GetPrivateProfileIntA("Zone", "RequireInterior", config->require_interior, local_ini_path);
    config->disable_controls_while_open = (int)GetPrivateProfileIntA("Safety", "DisableControlsWhileOpen", config->disable_controls_while_open, local_ini_path);
    config->show_zone_prompt = (int)GetPrivateProfileIntA("Safety", "ShowZonePrompt", config->show_zone_prompt, local_ini_path);
    config->auto_close_full_ledger_outside_zone = (int)GetPrivateProfileIntA("Safety", "AutoCloseFullLedgerOutsideZone", config->auto_close_full_ledger_outside_zone, local_ini_path);
    config->show_ready_toast = (int)GetPrivateProfileIntA("Safety", "ShowReadyToast", config->show_ready_toast, local_ini_path);
    config->bootstrap_poll_ms = (int)GetPrivateProfileIntA("Safety", "BootstrapPollMs", config->bootstrap_poll_ms, local_ini_path);
    config->bootstrap_timeout_ms = (int)GetPrivateProfileIntA("Safety", "BootstrapTimeoutMs", config->bootstrap_timeout_ms, local_ini_path);
    config->status_duration_ms = (int)GetPrivateProfileIntA("Safety", "StatusDurationMs", config->status_duration_ms, local_ini_path);
    config->use_desktop_overlay = (int)GetPrivateProfileIntA("Renderer", "UseDesktopOverlay", config->use_desktop_overlay, local_ini_path);
    config->live_tracking_poll_ms = (int)GetPrivateProfileIntA("LiveTracking", "PollMs", config->live_tracking_poll_ms, local_ini_path);
    config->tracked_ped_ttl_ms = (int)GetPrivateProfileIntA("LiveTracking", "TrackedPedTtlMs", config->tracked_ped_ttl_ms, local_ini_path);
    config->reward_delta_clamp = (int)GetPrivateProfileIntA("LiveTracking", "RewardDeltaClamp", config->reward_delta_clamp, local_ini_path);
    config->enable_live_stamina_bonus = (int)GetPrivateProfileIntA("LiveBonuses", "EnableStaminaBonus", config->enable_live_stamina_bonus, local_ini_path);
    config->enable_live_weapon_wear_bonus = (int)GetPrivateProfileIntA("LiveBonuses", "EnableWeaponWearBonus", config->enable_live_weapon_wear_bonus, local_ini_path);
    config->enable_live_deadeye_bonus = (int)GetPrivateProfileIntA("LiveBonuses", "EnableDeadeyeBonus", config->enable_live_deadeye_bonus, local_ini_path);

    config->beechers_hope_x = frontier_get_profile_float("Zone", "BeechersHopeX", config->beechers_hope_x, local_ini_path);
    config->beechers_hope_y = frontier_get_profile_float("Zone", "BeechersHopeY", config->beechers_hope_y, local_ini_path);
    config->beechers_hope_z = frontier_get_profile_float("Zone", "BeechersHopeZ", config->beechers_hope_z, local_ini_path);
    config->beechers_hope_radius = frontier_get_profile_float("Zone", "BeechersHopeRadius", config->beechers_hope_radius, local_ini_path);
    config->workbench_x = frontier_get_profile_float("Zone", "WorkbenchX", config->workbench_x, local_ini_path);
    config->workbench_y = frontier_get_profile_float("Zone", "WorkbenchY", config->workbench_y, local_ini_path);
    config->workbench_z = frontier_get_profile_float("Zone", "WorkbenchZ", config->workbench_z, local_ini_path);
    config->workbench_radius = frontier_get_profile_float("Zone", "WorkbenchRadius", config->workbench_radius, local_ini_path);
    config->stamina_recharge_bonus_per_tier = frontier_get_profile_float("LiveBonuses", "StaminaRechargeBonusPerTier", config->stamina_recharge_bonus_per_tier, local_ini_path);
    config->stamina_sprint_depletion_bonus_per_tier = frontier_get_profile_float("LiveBonuses", "StaminaSprintDepletionBonusPerTier", config->stamina_sprint_depletion_bonus_per_tier, local_ini_path);
    config->weapon_degradation_bonus_per_tier = frontier_get_profile_float("LiveBonuses", "WeaponDegradationBonusPerTier", config->weapon_degradation_bonus_per_tier, local_ini_path);
    config->deadeye_delay_bonus_per_tier = frontier_get_profile_float("LiveBonuses", "DeadeyeDelayBonusPerTier", config->deadeye_delay_bonus_per_tier, local_ini_path);

    frontier_copy_string(dataset_buffer, sizeof(dataset_buffer), config->default_dataset);
    chars_read = GetPrivateProfileStringA(
        "Data",
        "DefaultDataset",
        config->default_dataset,
        dataset_buffer,
        (DWORD)sizeof(dataset_buffer),
        local_ini_path
    );
    if (chars_read > 0U) {
        frontier_copy_string(config->default_dataset, sizeof(config->default_dataset), dataset_buffer);
    }

    frontier_copy_string(locale_buffer, sizeof(locale_buffer), config->preferred_locale);
    chars_read = GetPrivateProfileStringA(
        "Data",
        "PreferredLocale",
        config->preferred_locale,
        locale_buffer,
        (DWORD)sizeof(locale_buffer),
        local_ini_path
    );
    if (chars_read > 0U) {
        frontier_copy_string(config->preferred_locale, sizeof(config->preferred_locale), locale_buffer);
    }

    frontier_copy_string(overlay_font_buffer, sizeof(overlay_font_buffer), config->overlay_font_name);
    chars_read = GetPrivateProfileStringA(
        "Renderer",
        "OverlayFontName",
        config->overlay_font_name,
        overlay_font_buffer,
        (DWORD)sizeof(overlay_font_buffer),
        local_ini_path
    );
    if (chars_read > 0U) {
        frontier_copy_string(config->overlay_font_name, sizeof(config->overlay_font_name), overlay_font_buffer);
    }
}
