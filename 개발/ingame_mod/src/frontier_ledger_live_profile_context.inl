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

#include "frontier_ledger_live_profile_context_labels.inl"
#include "frontier_ledger_live_profile_context_summary.inl"
#include "frontier_ledger_live_profile_context_detail.inl"
#include "frontier_ledger_live_profile_context_focus.inl"
#include "frontier_ledger_live_profile_runtime_io.inl"
