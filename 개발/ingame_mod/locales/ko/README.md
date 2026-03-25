## Korean Locale Parking Area

This folder is the parked Korean-side seam for the English-first rebuild.

Current policy:

- active development baseline stays English
- Korean stays available as a config-driven option
- Korean renderer/layout tuning should not block English stabilization

Use `FrontierLedger.ko.example.ini` as the starting point when the Korean pass resumes.

Korean text sources still exist in:

- `scripts/export_ledger_data.py`
- `runtime_bitmap_strings.txt`
- `src/frontier_ledger.c`

Those sources are intentionally left intact for now, but the active runtime default is no longer Korean-first.
