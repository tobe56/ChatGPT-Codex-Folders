# 코드 중심 장기 리팩토링 최종 보고서

## 요약

- 작업 루트: `E:\RDR2 RPG MODS\개발`
- 시작 전 스냅샷: `_snapshots/20260323_2124`
- 기준선 문서/로그 추가: `PLAN.md`, `WORKLOG.md`
- dev tooling 추가: `pyproject.toml`, `requirements-dev.txt`
- 검증 결과: full gate 2회 연속 통과

## 최종 카테고리 재평가

### Story / Homefront / Affinity: 94%

- 변경 파일: `frontier_rpg/game/simulation.py`, `frontier_rpg/ui_pages/encyclopedia.py`, `frontier_rpg/ui_pages/tables.py`
- 검증 로그/테스트: `python -m unittest discover -s tests`, `python -m frontier_rpg affinity`
- 사용자 경로 또는 CLI 증거: `python -m frontier_rpg simulate`, `python -m frontier_rpg affinity`

### Growth / Stats / Skills / Perks: 90%

- 변경 파일: `frontier_rpg/game/progression.py`, `frontier_rpg/ui_pages/progression.py`, `frontier_rpg/game/simulation.py`
- 검증 로그/테스트: `python -m mypy --follow-imports skip frontier_rpg/game ...`, `python -m unittest discover -s tests`
- 사용자 경로 또는 CLI 증거: `python -m frontier_rpg effects --simulated-profile`, `python -m frontier_rpg simulate`

### Gear / Armory / Crafting / Workbench: 92%

- 변경 파일: `frontier_rpg/ui_pages/crafting.py`, `frontier_rpg/game/progression.py`, `ingame_mod/src/frontier_ledger_workbench.inl`, `ingame_mod/src/frontier_ledger_render_full.inl`
- 검증 로그/테스트: `python -m unittest discover -s tests`, `.\build.ps1`, `python .\scripts\workspace_smoke_test.py`
- 사용자 경로 또는 CLI 증거: `python -m frontier_rpg simulate`, `python -m frontier_rpg loot --simulate --node node.blackwater_dead_courier --simulated-profile`

### Map / World / Farming / Network / Routes: 91%

- 변경 파일: `frontier_rpg/game/rows.py`, `frontier_rpg/ui_pages/overview.py`, `frontier_rpg/ui_pages/encyclopedia.py`
- 검증 로그/테스트: `tests/test_world_loot.py`, `python -m unittest discover -s tests`
- 사용자 경로 또는 CLI 증거: `python -m frontier_rpg simulate`, `python -m frontier_rpg loot --simulate --node node.blackwater_dead_courier --simulated-profile`

### Combat / Boss / Collection / Encyclopedia: 90%

- 변경 파일: `frontier_rpg/game/rows.py`, `frontier_rpg/game/simulation.py`, `frontier_rpg/ui_pages/encyclopedia.py`, `ingame_mod/src/frontier_ledger_spawn.inl`
- 검증 로그/테스트: `tests/test_combat.py`, `python -m unittest discover -s tests`
- 사용자 경로 또는 CLI 증거: `python -m frontier_rpg combat --simulated-profile`, `python -m frontier_rpg simulate`

### Native Runtime / Localization / UX: 91%

- 변경 파일: `ingame_mod/src/frontier_ledger.c` and eleven `.inl` split files
- 검증 로그/테스트: `.\build.ps1`, `python .\scripts\workspace_smoke_test.py`, `.\preflight.ps1`
- 사용자 경로 또는 CLI 증거: smoke baseline/endgame/corrupt runs, packaged ASI/INI preflight

### Engineering Foundation / Refactorability / Verification: 95%

- 변경 파일: `PLAN.md`, `WORKLOG.md`, `pyproject.toml`, `requirements-dev.txt`, `frontier_rpg/content.py`, `frontier_rpg/systems.py`, `frontier_rpg/ui.py`, `frontier_rpg/ui_state.py`
- 검증 로그/테스트: `python -m ruff check frontier_rpg tests ingame_mod/scripts`, `python -m mypy --follow-imports skip ...`
- 사용자 경로 또는 CLI 증거: `python -m frontier_rpg overview`, `python -m frontier_rpg ui --check`

## 핵심 개선점

- `frontier_rpg/content.py` 2176 lines -> 202 lines
- `frontier_rpg/systems.py` 1229 lines -> 81 lines
- `frontier_rpg/ui.py` 645 lines -> 4 lines
- `frontier_rpg/ui_state.py` 574 lines -> 49 lines
- `frontier_rpg/game/*`, `frontier_rpg/ui_pages/*`, `frontier_rpg/ui_app.py`, `frontier_rpg/ui_widgets.py`를 추가해 책임을 분리
- `ingame_mod/src/frontier_ledger.c` 11705 lines -> 1550 lines
- native helper/render/persistence/input/workbench/spawn 책임을 `.inl` 파일로 분리
- active-code용 `ruff`/scoped `mypy`/unit/UI/build/smoke/preflight 루프를 고정

## 주요 변경 파일

- `frontier_rpg/content.py`
- `frontier_rpg/systems.py`
- `frontier_rpg/ui.py`
- `frontier_rpg/ui_state.py`
- `frontier_rpg/ui_app.py`
- `frontier_rpg/ui_widgets.py`
- `frontier_rpg/game/state.py`
- `frontier_rpg/game/rows.py`
- `frontier_rpg/game/progression.py`
- `frontier_rpg/game/simulation.py`
- `frontier_rpg/ui_pages/__init__.py`
- `frontier_rpg/ui_pages/overview.py`
- `frontier_rpg/ui_pages/progression.py`
- `frontier_rpg/ui_pages/crafting.py`
- `frontier_rpg/ui_pages/encyclopedia.py`
- `frontier_rpg/ui_pages/tables.py`
- `frontier_rpg/data/world_content.py`
- `frontier_rpg/data/progression_content.py`
- `frontier_rpg/data/social_content.py`
- `frontier_rpg/data/combat_content.py`
- `frontier_rpg/platform/localization.py`
- `ingame_mod/src/frontier_ledger.c`
- `ingame_mod/src/frontier_ledger_helpers_core.inl`
- `ingame_mod/src/frontier_ledger_helpers_text.inl`
- `ingame_mod/src/frontier_ledger_live_profile.inl`
- `ingame_mod/src/frontier_ledger_persistence.inl`
- `ingame_mod/src/frontier_ledger_overlay_control.inl`
- `ingame_mod/src/frontier_ledger_render_overlay.inl`
- `ingame_mod/src/frontier_ledger_render_compact.inl`
- `ingame_mod/src/frontier_ledger_render_full.inl`
- `ingame_mod/src/frontier_ledger_input_loop.inl`
- `ingame_mod/src/frontier_ledger_spawn.inl`
- `ingame_mod/src/frontier_ledger_workbench.inl`
- `PLAN.md`
- `WORKLOG.md`
- `pyproject.toml`
- `requirements-dev.txt`

## 검증

### Round 1

- `python -m ruff check frontier_rpg tests ingame_mod/scripts`
- `python -m mypy --follow-imports skip frontier_rpg/game frontier_rpg/core frontier_rpg/ui_pages frontier_rpg/ui_app.py frontier_rpg/ui_widgets.py frontier_rpg/ui_state.py frontier_rpg/systems.py`
- `python -m unittest discover -s tests`
- `python -m frontier_rpg ui --check`
- `python -m frontier_rpg simulate`
- `python -m frontier_rpg affinity`
- `python -m frontier_rpg combat --simulated-profile`
- `python -m frontier_rpg loot --simulate --node node.blackwater_dead_courier --simulated-profile`
- `python -m frontier_rpg effects --simulated-profile`
- `.\build.ps1`
- `python .\scripts\workspace_smoke_test.py`
- `.\preflight.ps1`

### Round 2

- same command set re-run successfully

## 잔여 리스크

- 이번 완료 기준은 workspace-only/code-only 범위다. 실제 live install 시각 품질과 mixed-mod 상호작용은 별도 확인이 필요하다.
- scoped `mypy`는 새 구조와 facade 중심으로 통과시켰고, feature deep typing은 다음 단계에서 확대 가능하다.
- native는 physical split을 달성했지만 translation unit 단위 분리는 다음 정리 단계에서 추가 검토할 수 있다.

## 후속 권장사항

1. `mypy` 범위를 `frontier_rpg/features/*`와 `ingame_mod/scripts/export_ledger_data.py`까지 점진 확장
2. live install 기준의 시각 검증과 spawn/workbench 실전 확인을 별도 캡처 세션으로 수행
3. 필요 시 `.inl` split 이후 `.c/.h` translation unit 분리로 한 단계 더 진행
