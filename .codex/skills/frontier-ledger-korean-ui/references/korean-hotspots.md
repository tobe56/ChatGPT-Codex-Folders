# Korean Hotspots

## Primary Files

- `ingame_mod\scripts\export_ledger_data.py`
- `ingame_mod\runtime_bitmap_strings.txt`
- `ingame_mod\src\frontier_ledger.c`
- `ingame_mod\FrontierLedger.ini`

## Useful Search Patterns

- `EXACT_TRANSLATIONS`
- `FRAGMENT_REPLACEMENTS`
- `STATIC_KOREAN_UI_LABELS`
- `build_gxt_entries`
- `frontier_dataset_is_korean`
- `frontier_draw_bitmap_text_internal`
- `frontier_render_workbench_overlay`
- `frontier_confirm_workbench_action`

## Known 2026-03-15 Risk Areas

- Duplicate workbench helper paths in `frontier_ledger.c`
- Mixed Korean/ASCII helper output in the workbench body
- Bitmap coverage gaps for strings that only appear at runtime
- Late edits that fix one Korean panel but leave another render path stale

## Verification Notes

- `.\build.ps1` regenerates both `generated\strings.gxt2` and `generated\frontier_bitmap_font_generated.h`.
- `workspace_smoke_test.py` proves the ASI loads and emits session/profile files, but it does not prove final readability.
- If a string is still wrong after rebuild, check whether it comes from generated dataset text, runtime-only text, or direct native UI text selection.
