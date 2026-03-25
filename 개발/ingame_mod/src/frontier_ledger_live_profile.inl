static int frontier_live_crafted_recipe_count(void);
static int frontier_crafted_workshop_bonus(void);
static int frontier_crafted_armory_bonus(void);
static int frontier_crafted_stamina_bonus_percent(void);
static int frontier_crafted_stamina_drain_bonus_percent(void);
static int frontier_crafted_weapon_bonus_percent(void);
static int frontier_crafted_deadeye_bonus_percent(void);
static int frontier_find_page_index(const char *page_id);
static int frontier_context_entry_for_page(const FrontierLedgerPage *page);
static void frontier_ensure_runtime_ini_path(char *path, size_t path_size, const char *file_name);
static void frontier_touch_runtime_file(const char *path);

#include "frontier_ledger_live_profile_progression.inl"
#include "frontier_ledger_live_profile_strings.inl"
#include "frontier_ledger_live_profile_crafting.inl"
#include "frontier_ledger_live_profile_rows.inl"
#include "frontier_ledger_live_profile_context.inl"
