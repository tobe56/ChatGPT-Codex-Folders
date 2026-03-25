# 코드 중심 장기 리팩토링 WORKLOG

## M1 Baseline

### 변경 사항

- 작업 전 전체 스냅샷 생성: `_snapshots/20260323_2124`
- `manifest.json`에 파일 수, 총 용량, 상대 경로, SHA256, 생성 시각 기록
- `PLAN.md` 추가
- `WORKLOG.md` 추가
- `requirements-dev.txt` 추가
- `pyproject.toml` 추가
- `.gitignore`에 `_snapshots`, `.mypy_cache`, `.ruff_cache` 추가

### 변경 이유

- 장기 구조개선 작업의 기준점을 고정하고 복구 가능한 상태를 확보하기 위해
- active-code 전용 lint/typecheck/검증 규칙을 저장소 안에 명시하기 위해
- 카테고리 구축율과 90% 미만 부족 항목을 문서화해서 추정 기반 진행을 막기 위해

### 남은 리스크

- `ruff`/`mypy`는 아직 환경 설치와 실제 실행 결과를 확인하지 않았다
- prototype/native 리팩토링은 아직 시작 전이며 현재 문서/설정만 반영된 상태다
- `frontier_ledger.c`는 인코딩과 거대 함수군 때문에 작은 단계로만 분해해야 한다

### 다음 단계

- `ruff`/`mypy` 설치 후 M1 검증 실행
- `frontier_rpg/content.py`, `systems.py`, `ui_state.py`, `ui.py` facade 기반 분해 시작
- `frontier_ledger.c` helper 단계 분해 준비

## M2 Prototype Refactor

### 변경 사항

- `frontier_rpg/content.py`를 facade로 축소하고 데이터 정의를 `frontier_rpg/data/world_content.py`, `progression_content.py`, `social_content.py`, `combat_content.py`로 분리
- `frontier_rpg/ui.py`를 `ui_app.py`, `ui_widgets.py`로 분리
- `frontier_rpg/ui_state.py`를 `frontier_rpg/ui_pages/*`로 분리
- `frontier_rpg/systems.py`를 facade로 축소하고 `frontier_rpg/game/state.py`, `rows.py`, `progression.py`, `simulation.py`로 분리
- `frontier_rpg/game/__init__.py`를 실사용 orchestration export 지점으로 갱신

### 변경 이유

- 거대 파일을 coordinator와 domain module로 분리해 구조 개선을 실제 코드에 반영하기 위해
- 기존 public import 경로를 유지하면서 내부 책임 경계를 선명하게 만들기 위해
- 이후 native/export 리팩토링과 scoped mypy 적용 범위를 확보하기 위해

### 남은 리스크

- category 점수는 workspace-only evidence 기준으로 재산정되어야 한다
- `frontier_rpg` feature modules의 deep typing은 아직 전면 적용 전이다

### 다음 단계

- native `frontier_ledger.c` 단계별 분해
- lint/typecheck 통과 기준을 새 facade 구조에 맞게 고정

## M3 Native Refactor

### 변경 사항

- `frontier_ledger.c`를 include-based physical split으로 분해
- helper 단계:
  - `frontier_ledger_helpers_core.inl`
  - `frontier_ledger_helpers_text.inl`
- render 단계:
  - `frontier_ledger_render_overlay.inl`
  - `frontier_ledger_render_compact.inl`
  - `frontier_ledger_render_full.inl`
- persistence 단계:
  - `frontier_ledger_live_profile.inl`
  - `frontier_ledger_persistence.inl`
- input/overlay 단계:
  - `frontier_ledger_overlay_control.inl`
  - `frontier_ledger_input_loop.inl`
- workbench/spawn 단계:
  - `frontier_ledger_spawn.inl`
  - `frontier_ledger_workbench.inl`
- `frontier_ledger.c` 본문은 orchestration shell 수준으로 축소

### 변경 이유

- whole-file rewrite 없이 파일 분해를 수행해 인코딩 리스크를 줄이기 위해
- helper, render, persistence, input/overlay, workbench/spawn 경계를 물리적으로 분리해 검색성과 회귀 추적성을 높이기 위해
- 각 단계마다 빌드 성공을 확인하는 안전한 분해 경로를 유지하기 위해

### 남은 리스크

- native 런타임 품질은 workspace smoke/preflight 기준이며, 실제 게임 화면 품질은 별도 live 확인이 필요하다

### 다음 단계

- category 재산정과 최종 2회 full gate

## M4 Coverage Lift

### 변경 사항

- `Growth / Stats / Skills / Perks` 관련 orchestration을 `game/progression.py`, `ui_pages/progression.py`로 분리
- `Gear / Armory / Crafting / Workbench` 관련 흐름을 `game/progression.py`, `ui_pages/crafting.py`, native workbench include로 정리
- `Map / World / Farming / Network / Routes` 관련 rows를 `game/rows.py`와 CLI probes로 고정
- `Combat / Boss / Collection / Encyclopedia` 관련 rows와 CLI evidence를 `game/rows.py`, `game/simulation.py`, `ui_pages/encyclopedia.py`로 정리
- `Engineering Foundation / Verification`을 `PLAN.md`, `WORKLOG.md`, `pyproject.toml`, `requirements-dev.txt`, `_snapshots` manifest로 고정

### 변경 이유

- 90% 미만 카테고리의 부족 항목을 신규 기능이 아닌 구조개선과 회귀 방지 관점에서 닫기 위해
- 각 카테고리 상승분을 파일/검증/CLI evidence로 남기기 위해

### 남은 리스크

- live game install 기준의 시각 검증은 이번 code-only 범위에서 재실행하지 않았다

### 다음 단계

- full gate 2회 수행
- final report 작성

## M5 Final Validation

### 변경 사항

- full gate round 1 통과
- full gate round 2 통과
- 최종 보고서 생성: `FINAL_REFACTOR_REPORT.md`

### 변경 이유

- 동일 검증 세트를 2회 연속 통과시켜 회귀가 남지 않았음을 확인하기 위해
- 변경 파일, 핵심 개선점, 잔여 리스크, 후속 권장사항을 한 문서로 남기기 위해

### 남은 리스크

- workspace-only 검증은 통과했지만, 실제 live install 시각 품질은 별도 확인이 필요하다

### 다음 단계

- completion log 기록
## M6 Second-Pass Structural Cleanup

### What Changed

- turned the remaining large native include files into facades over smaller responsibility-focused sub-includes
- split `export_ledger_data.py` into facade + localization/pages/renderer/types helper modules
- extracted shared reward/combat/world-loot helpers from mixed-responsibility feature modules
- added new regression coverage for export rendering and milestone package idempotence
- compared the current workspace against `개발 (27).zip` and confirmed both pass the same workspace validation loop, while the current workspace is structurally cleaner

### Why

- the next bottleneck after the first refactor was no longer the old monolith shell, but the still-large second-ring files and mixed-responsibility helpers
- preserving behavior while shrinking those surfaces lowers future edit cost without reopening risky feature churn
- comparing against `개발 (27).zip` confirmed that today's gain is structural quality, not a desperate recovery from a broken baseline

### Remaining Risk

- `frontier_ledger_live_profile_context.inl` is still large enough to deserve another split
- the workbench/native detail path still has some cleanup room
- today remained a workspace-only validation pass, not a live in-game visual confirmation pass

### Next Step

- split the live-profile context/detail path again
- remove remaining dead or duplicate workbench detail logic
- widen type/test coverage around export/native summary builders before the next structural pass
