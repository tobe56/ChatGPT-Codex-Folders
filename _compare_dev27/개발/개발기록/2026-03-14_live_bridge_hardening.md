# 2026-03-14 Live Bridge Hardening

- `ingame_mod/src/frontier_ledger.c` now hardens the live gameplay bridge with safer profile/session writes, clamped profile restore, conservative reward spike guards, and ped-handle dedupe resets for kill/loot tracking.
- The native live run now applies two real gameplay effects from the tracked run state: a conservative stamina bonus and a weapon degradation reduction, both scaled by live bonus tier instead of static text-only presentation.
- Existing `Overview`, `Farming`, `Network`, and `World` pages were strengthened to surface current run activity and active bonuses without creating new pages or expanding the prototype layer.
- Validation stayed workspace-only: build, unit tests, UI check, smoke test, corrupt-profile smoke recovery, and packaged ASI generation all completed without touching the live game folder.
- A second polish pass reduced release risk further by adding a short looting confirmation window, throttled save/load logging, forced live summary refreshes on ledger open/close, and safer default INI values for live polling, reward clamp, and bonus strength.
