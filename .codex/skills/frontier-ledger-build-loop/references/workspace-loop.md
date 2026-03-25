# Workspace Loop

## Roots

- Python root: `E:\RDR2 RPG MODS\개발`
- Native root: `E:\RDR2 RPG MODS\개발\ingame_mod`

## Core Commands

- Python regression: `python -m unittest discover -s tests`
- Prototype UI sanity check: `python -m frontier_rpg ui --check`
- Native build: `.\build.ps1`
- Native smoke test: `python .\scripts\workspace_smoke_test.py`
- Package preflight: `.\preflight.ps1`

## Packaging Commands

- Release candidate package: `.\package_release_candidate.ps1`
- Fake install/rollback cycle: `.\scripts\test_install_cycle.ps1`

## Important Outputs

- Built ASI: `ingame_mod\build\FrontierLedger.asi`
- Packaged ASI: `ingame_mod\dist\FrontierLedger\FrontierLedger.asi`
- Packaged INI: `ingame_mod\dist\FrontierLedger\FrontierLedger.ini`
- Generated native data: `ingame_mod\generated\frontier_ledger_generated.h`
- Generated Korean strings: `ingame_mod\generated\strings.gxt2`
- Generated bitmap font header: `ingame_mod\generated\frontier_bitmap_font_generated.h`

## When To Escalate

- Ask before touching the real game install.
- Mention if verification stops at workspace-only checks, because smoke and preflight do not prove visual in-game quality.
