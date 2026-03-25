# 코드 중심 장기 리팩토링 실행 계획

## 기준점

- 실행 루트: `E:\RDR2 RPG MODS\개발`
- 작업 전 스냅샷: `_snapshots/20260323_2124`
- 스냅샷 형태: `workspace_snapshot.zip` + `manifest.json`
- 스냅샷 파일 수: `17,044`
- 스냅샷 총 용량: `615,929,674 bytes`
- 스냅샷 제외 규칙: `_snapshots/**`
- 현재 검증 기준선:
  - `python -m unittest discover -s tests`
  - `python -m frontier_rpg ui --check`
  - `python .\scripts\workspace_smoke_test.py`
  - `.\preflight.ps1`

## 현재 폴더/모듈 구조 요약

### 활성 코드 범위

- `frontier_rpg`
  - 현행 프로토타입 진입점과 게임 규칙, UI 상태, 데스크톱 UI 쉘
- `tests`
  - 프로토타입/내보내기 회귀 테스트
- `ingame_mod/src`
  - ScriptHookRDR2 런타임
- `ingame_mod/scripts`
  - native export, smoke, packaging 보조 스크립트

### 비활성 또는 검증 제외 범위

- `ingame_mod/generated`
- `ingame_mod/build`
- `ingame_mod/dist`
- `ingame_mod/release_candidate`
- `ingame_mod/frontier_smoke_*`
- `ingame_mod/frontier_overlay_check_*`
- `ingame_mod/frontier_manual_*`
- `ingame_mod/frontier_noini_*`
- `temp_video_frames*`
- `temp_video_tools`
- `tools`
- `__pycache__`

### 현재 병목 파일

- `frontier_rpg/content.py`: 2176 lines
- `frontier_rpg/systems.py`: 1229 lines
- `frontier_rpg/ui.py`: 645 lines
- `frontier_rpg/ui_state.py`: 574 lines
- `ingame_mod/src/frontier_ledger.c`: 11705 lines
- `ingame_mod/scripts/export_ledger_data.py`: 1038 lines

## 핵심 문제점 목록

1. 데이터 정의, 게임 규칙, UI 상태, native 런타임이 몇 개의 거대 파일에 집중되어 있다.
2. `frontier_rpg/game` 레이어가 사실상 비어 있어 구조 목표와 실제 코드가 어긋나 있다.
3. `ingame_mod/src/frontier_ledger.c`는 helper/render/persistence/input/workbench/spawn이 한 파일에 섞여 있어 회귀 위험이 크다.
4. active code와 generated/build/dist/log/runtime artifacts가 같은 트리 안에 섞여 있다.
5. `ruff`/`mypy` 기준과 active-code 검증 제외 규칙이 문서화되어 있지 않았다.
6. 구축율은 높아졌지만, 90% 미만 카테고리의 부족분이 구조·검증 근거로 닫히지 않았다.

## 카테고리 정의

### 하이브리드 구축율 카테고리

1. `Story / Homefront / Affinity`
2. `Growth / Stats / Skills / Perks`
3. `Gear / Armory / Crafting / Workbench`
4. `Map / World / Farming / Network / Routes`
5. `Combat / Boss / Collection / Encyclopedia`
6. `Native Runtime / Localization / UX`
7. `Engineering Foundation / Refactorability / Verification`

## 각 카테고리 구축율 산정 기준

각 카테고리는 아래 증거를 합산해서 100점 만점으로 산정한다.

- 문서 정합성: 10
- 데이터/모델 구현: 15
- 규칙/상태/데이터흐름: 20
- UI/라우트/화면 진입점: 15
- native/export 반영: 15
- 테스트/스모크/검증: 15
- 예외처리/로그/복구 경로: 10

점수 인정 규칙:

- 추정 불가
- 코드, 문서, 테스트, 로그, CLI, 화면 경로 중 최소 하나의 실제 증거가 있어야 함
- 상승분은 반드시 `변경 파일`, `검증 로그/테스트`, `실제 사용자 경로 또는 화면/CLI 증거` 3가지를 함께 남김

## 현재 구축율 수치와 근거

### Story / Homefront / Affinity: 93%

- 근거:
  - `기획문서/00_*`, `04_*`, `09_*`
  - `frontier_rpg/features/affinity.py`
  - `frontier_rpg/data/affinity_data.py`
  - `tests/test_affinity.py`
  - native `Affinity`/`Homefront` export 및 관련 worklog/checklist
- 부족 항목:
  - 최종 구조 정리와 evidence aggregation이 아직 문서화되지 않음

### Growth / Stats / Skills / Perks: 84%

- 근거:
  - `frontier_rpg/core/progression.py`
  - `frontier_rpg/ui_state.py`의 stats/skills/perks rows
  - `frontier_ledger.c`의 live growth/profile 경로
- 부족 항목:
  - perk spend 경로 정리 부족
  - profile/session mutation 경계 분산
  - 전용 회귀 테스트 보강 필요

### Gear / Armory / Crafting / Workbench: 88%

- 근거:
  - recipe/gear/armory rows
  - native workbench checklist와 smoke 통과
  - export-driven crafting/live mask 동기화
- 부족 항목:
  - export/live parity 정리
  - workbench 관련 코드 경계 분리

### Map / World / Farming / Network / Routes: 86%

- 근거:
  - `features/world_loot.py`, `features/world_state.py`, `features/farming.py`, `features/network.py`
  - `tests/test_world_loot.py`
  - world/network/farming CLI 및 recent worklogs
- 부족 항목:
  - authored route coverage gap
  - category-owned regression 추가 필요

### Combat / Boss / Collection / Encyclopedia: 82%

- 근거:
  - `features/combat.py`
  - `tests/test_combat.py`
  - dead/collection/encyclopedia UI + export
- 부족 항목:
  - selected-entry / boss summon 회귀 방지
  - live spawn 안정화

### Native Runtime / Localization / UX: 78%

- 근거:
  - `frontier_ledger.c` live dataset/session/profile/logging
  - `workspace_smoke_test.py`
  - `preflight.ps1`
- 부족 항목:
  - helper/render/persistence/input/workbench/spawn 분해
  - locale/render 경계 정리

### Engineering Foundation / Refactorability / Verification: 65%

- 근거:
  - 거대 파일 편중
  - `frontier_rpg/game` 미사용
  - lint/typecheck 부재
- 부족 항목:
  - active-code 검증 규칙 도입
  - facade 축소 기준 수립
  - mypy 범위 점진 확장

## 90% 미만 카테고리 부족 항목

### Growth / Stats / Skills / Perks

- perk spend/native growth 경계 정리
- profile/session mutation 경로 일원화
- 관련 테스트 추가

### Gear / Armory / Crafting / Workbench

- export-driven label/detail parity 정리
- workbench/live path와 prototype/export 경계 정리

### Map / World / Farming / Network / Routes

- authored route/state 누락 구간 보강
- category-owned 회귀 테스트 추가

### Combat / Boss / Collection / Encyclopedia

- selected-entry/boss summon 회귀 방지
- live path와 data/export 매칭 안정화

### Native Runtime / Localization / UX

- render/input/profile/workbench 경계 분리
- locale path 단일화

### Engineering Foundation / Refactorability / Verification

- `ruff`/`mypy`/tests/build 명령의 active-code 기준 정식화
- lint/typecheck 제외 규칙 고정
- facade 제거 기준 문서화

## 마일스톤별 완료 조건

### M1 Baseline

- 스냅샷 생성 완료
- `PLAN.md`, `WORKLOG.md`, `pyproject.toml`, `requirements-dev.txt` 생성
- active-code lint/typecheck 범위와 제외 규칙 고정

### M2 Prototype Refactor

- `content.py`, `systems.py`, `ui.py`, `ui_state.py`가 coordinator 수준으로 축소
- `frontier_rpg/game`이 실제 orchestration 레이어를 갖춤

### M3 Native Refactor

- `frontier_ledger.c`가 단계별로 분해됨
- helper -> render -> persistence -> input/overlay -> workbench/spawn 순서를 지킴
- 각 단계 뒤 빌드 성공 기록 보존

### M4 Coverage Lift

- 90% 미만 카테고리 없음
- 모든 상승분에 대해 파일/검증/사용자 경로 증거 확보

### M5 Final Validation

- 동일 검증 2회 통과
- 최종 보고서 작성 완료

## 마일스톤별 검증 명령

### 공통 원칙

- active code만 대상으로 실행
- 없는 엔트리포인트는 사용하지 않음
- `generated`, `build`, `dist`, `.venv`, `__pycache__`, runtime/log/artifact 폴더는 제외

### M1 검증

- `python -m ruff check frontier_rpg tests ingame_mod/scripts`
- `python -m mypy frontier_rpg/game frontier_rpg/features frontier_rpg/core frontier_rpg/ui_state.py frontier_rpg/systems.py ingame_mod/scripts/export_ledger_data.py`
- `python -m unittest discover -s tests`
- `python -m frontier_rpg ui --check`

### M2 검증

- `python -m ruff check frontier_rpg tests`
- `python -m mypy frontier_rpg/game frontier_rpg/features frontier_rpg/core frontier_rpg/ui_state.py frontier_rpg/systems.py`
- `python -m unittest discover -s tests`
- `python -m frontier_rpg ui --check`
- `python -m frontier_rpg simulate`
- `python -m frontier_rpg affinity`
- `python -m frontier_rpg effects --simulated-profile`

### M3/M4 검증

- `python -m ruff check frontier_rpg tests ingame_mod/scripts`
- `python -m mypy frontier_rpg/game frontier_rpg/features frontier_rpg/core frontier_rpg/ui_state.py frontier_rpg/systems.py ingame_mod/scripts/export_ledger_data.py`
- `python -m unittest discover -s tests`
- `python -m frontier_rpg ui --check`
- `python -m frontier_rpg combat --simulated-profile`
- `python -m frontier_rpg loot --simulate --node node.blackwater_dead_courier --simulated-profile`
- `python -m frontier_rpg effects --simulated-profile`
- `.\build.ps1`
- `python .\scripts\workspace_smoke_test.py`
- `.\preflight.ps1`

### 최종 2회 검증

- 라운드 1: 위 full gate 실행, 실패 수정, 재실행
- 라운드 2: 동일 full gate 재실행 + 변경 영향 재점검

## 고정 규칙

- 신규 기능 추가 금지
- 구조개선, 경계정리, 누락된 핵심 흐름 보강, 회귀 방지 테스트만 허용
- 외부 심볼명, generated 포맷, exported page id/dataset id/session/profile key 유지
- `frontier_ledger.c`는 whole-file rewrite 금지, 작은 단계 분해만 허용
