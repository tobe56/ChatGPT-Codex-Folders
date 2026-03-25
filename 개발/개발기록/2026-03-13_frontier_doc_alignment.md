# 2026-03-13 Frontier Core Systems Realignment

## 왜 방향을 바꿨는가

이전 구현은 RDR2 네이티브에서 쉽게 읽을 수 있는 값 위주로 진행되면서, 실제 기획문서의 우선순위와 어긋났다.

이번 정리는 다음 문서를 기준으로 다시 맞춘 것이다.

- `01_world_ui_hub_draft.md`: `F10`과 Beecher's Hope 레저는 기본 HUD가 아니라 `Active Trails`, `Frontier Map`, `Workshop`, `Ledger of the Dead` 같은 전용 상태를 보여줘야 한다.
- `03_progression_systems_draft.md`: 핵심은 `Frontier XP`, 스탯/퍽, 재료 지식, 하우스/워크벤치 진척이다.
- `08_large_scale_implementation_blueprint.md`: Beecher's Hope는 `ledger table`, `map board`, `stash`, `trophy board`, `armory`, `support station`을 가진 progression hub여야 한다.

## 이번에 실제로 붙인 것

### 1. Active Trails 상태머신

`frontier_rpg/content.py`에 문서형 트레일 정의를 추가했다.

- Main Story
- Contract
- House Objective
- Boss Hunt
- Side Chain
- Repeatable

`frontier_rpg/systems.py`는 이제 플레이어 상태를 보고:

- 잠금 여부
- 활성 여부
- 완료 여부
- 반복 컨텐츠 여부

를 계산하고, 현재 필드 레저에 보일 활성 트레일을 자동으로 갱신한다.

### 2. Frontier Map 인텔

지역별 인텔 정의를 새로 만들었다.

- 지역 역할
- 월드 티어별 danger 문구
- pressure source
- node type
- faction route
- boss clue

이 정보는 단순 지역 이름이 아니라, `지금 그 지역이 어떤 파밍/위험/보스 라인인지`를 보여주기 위한 기초 데이터다.

### 3. Beecher's Hope 허브 설비 진행도

허브 설비를 별도 정의로 분리했다.

- Frontier Ledger Table
- Basic Workbench
- Contract Wall
- Relic Stash Chest
- Map Intelligence Board
- Armory Rack
- Boss Trophy Board
- Support Station
- War Room State

플레이어의 레벨, 홈 업그레이드, 워크벤치 티어, 월드 티어, 보스 킬 수를 보고 각 설비가 `Installed`인지 `Locked`인지 계산한다.

## 프로토타입에서 바뀐 체감 포인트

- `python -m frontier_rpg trails`
  - 현재 문서 기준 활성 트레일과 잠긴 트레일 상태를 바로 볼 수 있다.
- `python -m frontier_rpg intel`
  - 지역별 위험과 인텔 상태를 볼 수 있다.
- `python -m frontier_rpg hub`
  - Beecher's Hope 허브 설비가 어디까지 열린 상태인지 볼 수 있다.
- `python -m frontier_rpg ui`
  - UI 프로토타입도 `Active Trails`, `Frontier Map`, `Workshop/Beecher's Hope` 축이 반영된 상태로 읽을 수 있다.

## 남은 다음 단계

이번 작업은 `문서 기반 상태`를 만든 단계였고, 이후 같은 날 아래 축까지 추가로 확장했다.

### 추가로 붙인 축

- `Homefront`
  - Abigail / Jack / Uncle의 역할을 개별 상태로 계산한다.
  - Homefront, Archive, Rumor Broker 축이 플레이어 성장에 따라 열린다.
- `Gear`
  - 장식용이 아니라 실제 슬롯 구조를 갖춘 장비 표를 추가했다.
  - Major Accessory, Minor Charm, Holster Mod, Satchel Mod, Coat Lining, Saddle Token이 들어갔다.
- `Armory`
  - 무기별 condition, familiarity, archetype path, mod 상태를 별도 페이지로 분리했다.
- `Ledger of the Dead`
  - 발견한 보스와 미발견 실루엣을 나누고, territory / reward / lore note를 붙였다.
- `Collection`
  - relic set, accessory set, faction token, trophy rack, archive 진행도를 실제 계산한다.

### 100% 프로토타입 마감 단계에서 추가한 축

- `Farming Lanes`
  - Hunter / Raider / Contract / Frontier Work / Boss lane을 독립 진행도로 만들었다.
  - 각 lane이 어디서 무엇을 파밍하고, 무엇을 푸는지 보이게 했다.
- `Existing-Mod Integration Tracks`
  - Bounties, Jobs, Contracts, Stagecoaches, Duels, Companions, Properties, Bank Robberies를 하나의 frontier economy 안으로 묶었다.
  - 더 이상 외부 모드 이름 목록이 아니라, 프로토타입 내부 상태와 연결된 progression track이다.
- `World State Escalation`
  - 각 월드 티어가 적/루팅/보스/레시피/스토리/이동 루트에 어떻게 영향을 주는지 독립 데이터로 만들었다.
- `Ending / Post-Ending`
  - Ranch Under Threat부터 After The Ledger까지 ending route를 단계별 상태로 계산한다.
  - 별도의 `endgame` 프로필로 post-ending sandbox 상태까지 검증 가능하게 했다.

### 70% 목표 기준 현재 의미

이제 프로토타입은 `성장`, `제작`, `월드 티어`, `트레일`, `맵 인텔`, `허브 설비`, `가족 지원`, `장비`, `무기 관리`, `보스 아카이브`, `컬렉션`까지 묶여 있다.

즉 문서의 주요 축 대부분이 코드 상태로 존재하고, 남은 일은 주로:

- 개별 시스템 간 상호작용을 더 깊게 연결하는 것
- 실제 in-game native layer에 다시 옮기는 것
- 최종 UI/렌더링 품질을 다듬는 것

## 남은 다음 단계

이제 남은 핵심은:

- 제작/퍼크/무기 숙련도와 허브 설비 상태를 더 강하게 연결
- `ingame_mod`가 이 상태를 직접 읽어 인게임 UI와 시스템에 연결하도록 확장
- 프로토타입의 `endgame` 상태를 실제 in-game progression chain으로 분해
