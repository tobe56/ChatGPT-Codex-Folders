---
name: frontier-ledger-build-loop
description: Safe build, test, smoke, packaging, and preflight workflow for the Frontier Ledger Python prototype and ScriptHookRDR2 mod. Use when Codex changes files under `개발/frontier_rpg`, `개발/tests`, or `개발/ingame_mod`, regenerates native exports or Korean bitmap assets, touches packaging scripts, or needs to verify workspace changes before any live game install.
---

# Frontier Ledger Build Loop

Read `references/workspace-loop.md` before running commands.

## Choose The Verification Depth

- Skip command execution when the task only changes planning notes or other non-runtime docs.
- Run the Python loop from `E:\RDR2 RPG MODS\개발` when the change stays inside `frontier_rpg` or `tests`.
- Run the full native loop from `E:\RDR2 RPG MODS\개발\ingame_mod` when the change touches `ingame_mod`, `ingame_mod/scripts/export_ledger_data.py`, `runtime_bitmap_strings.txt`, `FrontierLedger.ini`, packaging scripts, or any Korean UI asset path.

## Run The Python Loop

1. Run `python -m unittest discover -s tests`.
2. Run `python -m frontier_rpg ui --check`.
3. Run a narrow CLI command if the task changes one surface only, such as `overview`, `trails`, `hub`, `farming`, or `ending`.

## Run The Full Native Loop

1. Run `.\build.ps1`.
2. Run `python .\scripts\workspace_smoke_test.py`.
3. Run `.\preflight.ps1`.
4. Run `.\package_release_candidate.ps1` and `.\scripts\test_install_cycle.ps1` only when packaging or install flow changes.

## Guardrails

- Keep work inside the workspace unless the user explicitly asks to copy files into the real game folder.
- Treat `ingame_mod\generated\frontier_ledger_generated.h`, `ingame_mod\generated\strings.gxt2`, and `ingame_mod\generated\frontier_bitmap_font_generated.h` as generated outputs. Regenerate them through `.\build.ps1` instead of editing them manually.
- Report any command you could not run, especially if the Zig toolchain or Python dependencies are missing.
