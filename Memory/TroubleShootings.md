# Frontier Ledger TroubleShootings

## 1. `frontier_ledger.c` mixed-encoding corruption after whole-file rewrites

### Problem

- Zig build suddenly reports many `missing terminating '"'`, `unexpected character`, or invalid UTF-8 style errors inside `개발/ingame_mod/src/frontier_ledger.c`.
- The failures cluster around Korean string literals that were previously compiling.

### Root Cause

- A raw whole-file rewrite or regex-based full-text replacement changed the byte representation of the existing mixed-encoding string literals.
- `frontier_ledger.c` is not a safe target for broad raw rewrites because it contains many old string blocks that are fragile under encoding changes.

### Fix

- Restore the last known-good source from the newest workspace zip snapshot first.
- Re-apply only small, localized edits after the restore.
- Prefer `apply_patch` on tight ASCII-safe slices instead of full-file raw writes.

### Rule

- Do not use whole-file raw rewrite tactics on `개발/ingame_mod/src/frontier_ledger.c` unless a full encoding-safe migration is the explicit task.

## 2. `live_en` log output looks different from the selected dataset

### Problem

- The user selects `live_en`, but the attach log may show `RequestedDataset=live_en` and `Dataset=simulated_en`.

### Root Cause

- `live_en` and `live_ko` are aliases, not static datasets.
- The runtime resolves those aliases to `baseline_*`, `simulated_*`, or `endgame_*` depending on the persisted live profile phase and travel state.

### Fix

- Read `FrontierLedger.profile.ini` first.
- Check `Phase`, `TrailStage`, and discovered-region progress before treating the dataset line as a mismatch.

### Rule

- Treat `RequestedDataset` and resolved `Dataset` as different fields on purpose.

## 3. Uncertainty about whether the live game install matches the workspace build

### Problem

- A live build may feel different from the workspace build, and it is easy to lose track of which package is actually installed.

### Root Cause

- Repeated live installs create multiple backup chains under the game root.
- Without a preserved snapshot and hash comparison, it is unclear whether the game root diverged from `개발/ingame_mod/dist`.

### Fix

- Compare SHA256 for `FrontierLedger.asi` and `strings.gxt2`.
- Preserve the live snapshot under `개발/ingame_mod/live_reference/2026-03-16_live_verified_current`.

### Rule

- Before debugging "the live one looks different", compare hashes first.
