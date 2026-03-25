---
name: frontier-ledger-korean-ui
description: Maintain Korean Frontier Ledger and workbench presentation, including GXT text generation, bitmap font coverage, live_ko dataset behavior, and native renderer cleanup. Use when Codex edits Korean strings, mixed Korean/ASCII helpers, workbench labels, bitmap coverage, or the `frontier_ledger.c` Korean rendering path.
---

# Frontier Ledger Korean UI

Read `references/korean-hotspots.md` before editing text or renderer code.

## Choose The Correct Edit Surface

- Edit `ingame_mod\scripts\export_ledger_data.py` for dataset text, translation tables, generated labels, and Korean page content.
- Edit `ingame_mod\runtime_bitmap_strings.txt` when the build needs extra glyph coverage for runtime-only strings.
- Edit `ingame_mod\src\frontier_ledger.c` for workbench layout, tab rendering, prompt flow, bitmap text drawing, or Korean-specific overlay behavior.
- Check `ingame_mod\FrontierLedger.ini` if the task changes the default dataset or renderer mode.

## Prefer Cleanup Over Another Branch

- Collapse duplicate Korean/workbench helpers instead of adding one more render path.
- Reuse existing generated-label flow before introducing hard-coded Korean strings in native C.
- Keep English and Korean behavior aligned unless the difference is required by rendering constraints.

## Rebuild After Every Korean UI Change

1. Run `.\build.ps1`.
2. Run `python .\scripts\workspace_smoke_test.py`.
3. Run `.\preflight.ps1`.

Use `$frontier-ledger-build-loop` if the change also affects packaging or broader regression coverage.

## Report The Remaining Confidence Gap

- Workspace checks confirm buildability and generated output, not final visual quality inside live gameplay.
- If the user asks for visual confidence, say whether validation stopped at workspace checks or extended to real in-game install/capture review.
