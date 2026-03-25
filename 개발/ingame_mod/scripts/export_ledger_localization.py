from __future__ import annotations

import json
import re

EXACT_TRANSLATIONS = {
    "Overview": "개요",
    "Stats": "스탯",
    "Skills": "생활 기술",
    "Perks": "특성",
    "Crafting": "제작",
    "Upgrades": "강화",
    "Encyclopedia": "백과사전",
    "Frontier Status": "프런티어 현황",
    "Quick Intel": "빠른 정보",
    "Ready Now": "지금 가능",
    "Baseline Frontier State": "기본 프런티어 상태",
    "Simulated Frontier Run": "시뮬레이션 진행 상태",
    "Project status, world pressure, and active trails.": "프로젝트 현황, 월드 압박도, 현재 추적 목표.",
    "Core combat and survival attributes for John Marston.": "존 마스턴의 전투 및 생존 핵심 능력치.",
    "Lifestyle progression that feeds the frontier economy.": "프런티어 경제를 받쳐 주는 생활 기술 성장.",
    "Unlocked, ready, and locked perk lines.": "해금, 준비, 잠금 상태의 특성 라인.",
    "Crafting tables, costs, and immediate unlock paths.": "제작 목록, 비용, 즉시 해금 경로.",
    "Workbench, homestead, weapon, and relic upgrade tracks.": "작업대, 거점, 무기, 유물 강화 경로.",
    "Bosses, regions, world tiers, and frontier lore entries.": "보스, 지역, 월드 티어, 프런티어 설정 모음.",
    "Project: John Marston: Ashes of the Frontier": "프로젝트: John Marston: Ashes of the Frontier",
    "Current hub anchor: Beecher's Hope": "현재 허브: Beecher's Hope",
    "F10 opens the Compact Frontier Ledger anywhere in the field.": "F10으로 필드 어디서든 간이 레저를 엽니다.",
    "Beecher's Hope opens the full management ledger.": "Beecher's Hope에서 전체 관리 레저를 엽니다.",
    "Active Trails": "진행 중인 추적선",
    "No entries available.": "표시할 항목이 없습니다.",
    "Trace the blood-marked ledger from Beecher's Hope.": "Beecher's Hope에서 피 묻은 장부의 흔적을 추적한다.",
    "Secure materials for the first workbench expansion.": "첫 작업대 확장에 필요한 자재를 확보한다.",
    "Build enough power to confront Silas Redd.": "Silas Redd와 맞설 힘을 갖춘다.",
    "Beecher's Hope Raid": "Beecher's Hope 습격",
    "Blackwater Broker": "Blackwater 브로커",
    "Silas Redd Hunt": "Silas Redd 추적",
    "Relic War": "유물 전쟁",
    "Surveyor's Reckoning": "Surveyor의 결산",
    "Strength": "근력",
    "Grit": "근성",
    "Deadeye": "데드아이",
    "Survival": "생존",
    "Cunning": "기민함",
    "Hunting": "사냥",
    "Gunsmithing": "총포 개조",
    "Field Medicine": "현장 응급치료",
    "Trapping": "덫 사냥",
    "Salvaging": "회수",
    "Trade": "거래",
    "Horse Handling": "말 조련",
    "Homestead": "거점 운영",
    "Gunslinger": "건슬링어",
    "Frontiersman": "개척자",
    "Survivor": "생존자",
    "Outlaw": "무법자",
    "Relic Hunter": "유물 사냥꾼",
    "Hub Upgrade": "거점 강화",
    "Weapon Mod": "무기 개조",
    "Named Weapon": "고유 무기",
    "Relic Upgrade": "유물 강화",
    "Accessory": "장신구",
    "Project": "프로젝트",
    "Boss": "보스",
    "Family": "가족",
    "Region": "지역",
    "World Tier": "월드 티어",
    "World Route": "월드 루트",
    "Enemy Trait": "적 특성",
    "Enemy Archetype": "적 유형",
    "Story Chapter": "스토리 장",
    "Current": "현재",
    "Locked": "잠김",
    "Cleared": "완료",
    "Completed": "완료",
    "Planned": "예정",
    "Known": "확인됨",
    "Active": "활성",
    "Late Game": "후반",
    "Open": "개방",
    "Veteran": "베테랑",
    "Elite": "정예",
    "Enforcer": "집행자",
    "Boss Support": "보스 지원",
    "Mythic Elite": "신화 정예",
    "Frontier Stirring": "Frontier Stirring",
    "Open Wounds": "Open Wounds",
    "After The Ledger": "After The Ledger",
    "A low-drama supply loop that still pushes the same pickup and storage flow without requiring a major boss fight.": "큰 보스전 없이도 같은 획득-보관 흐름을 검증하게 해 주는 저강도 보급 루프입니다.",
    "A Blackwater-side coach seizure loop that should turn a short fight into stored broker cargo and cleaner contract stock.": "짧은 전투를 저장 가능한 브로커 화물과 더 깔끔한 계약 재고로 바꾸는 블랙워터 마차 압수 루프입니다.",
    "A wrecked convoy loop that should feed scrap, fittings, picked cargo, and homebound storage.": "스크랩, 부품, 회수 화물, 거점 보관으로 이어지는 파손 호송대 루프입니다.",
    "A salvage return loop closer to home that still feeds the same pickup and storage chain through Beecher's Hope support stock.": "비처스 호프 지원 재고를 통해 같은 획득-보관 체인을 유지하는 근거리 회수 루프입니다.",
    "A hunting camp that converts the tracker fight into hides, herbs, picked trophies, and stashable field stock.": "추적조 전투를 가죽, 약초, 전리품, 보관 가능한 야전 재고로 바꾸는 사냥 캠프 루프입니다.",
    "An ore-and-camp route that should make resource collection feel like planned logistics rather than passive numbers.": "자원 수집이 수동적인 숫자가 아니라 계획된 물류처럼 느껴지게 만드는 광석/캠프 루프입니다.",
    "A lower-drama hunting route that still ends in tangible stored materials and archive-worthy trophy pieces.": "저강도 사냥 루트지만 실제 보관 재료와 기록할 만한 전리품으로 마감되는 루프입니다.",
    "An enforcer-backed cache meant to prove that rough combat routes can still become structured material income.": "거친 전투 루트도 구조화된 재료 수입으로 이어질 수 있음을 보여 주는 집행조 수호 캐시입니다.",
    "A harsher New Austin route where a short violent hit should still end in structured stored cargo instead of only loose scrap.": "짧고 거친 습격도 흩어진 스크랩이 아니라 구조화된 저장 화물로 마무리되어야 하는 뉴오스틴 루트입니다.",
    "A relic-side pickup loop that should feed myth materials, picked notes, and Jack-facing archive stock.": "신화 재료, 회수 문서, 잭의 기록 보관 재고를 함께 공급하는 유물 회수 루프입니다.",
    "A Lemoyne-side smuggler shack route that ties pressure combat to stored illicit stock and archive notes.": "압박 전투를 저장 가능한 밀수 물자와 기록 메모로 연결하는 르모인 밀수 오두막 루트입니다.",
    "A late ridge-cache route that should convert hard travel and pressure into meaningful stored late-game materials.": "고된 이동과 압박을 의미 있는 후반 저장 재료로 바꾸는 후반 능선 캐시 루트입니다.",
}

FRAGMENT_REPLACEMENTS = [
    ("- Trace the blood-marked ledger from Beecher's Hope.", "- Beecher's Hope에서 피 묻은 장부의 흔적을 추적한다."),
    ("- Secure materials for the first workbench expansion.", "- 첫 작업대 확장에 필요한 자재를 확보한다."),
    ("- Build enough power to confront Silas Redd.", "- Silas Redd와 맞설 힘을 갖춘다."),
    ("Project: ", "프로젝트: "),
    ("Current hub anchor: ", "현재 허브: "),
    ("Strength", "근력"),
    ("Grit", "근성"),
    ("Deadeye", "데드아이"),
    ("Survival", "생존"),
    ("Cunning", "기민함"),
    ("Hunting", "사냥"),
    ("Gunsmithing", "총포 개조"),
    ("Field Medicine", "현장 응급치료"),
    ("Trapping", "덫 사냥"),
    ("Salvaging", "회수"),
    ("Trade", "거래"),
    ("Horse Handling", "말 조련"),
    ("Homestead", "거점 운영"),
    ("Frontiersman", "개척자"),
    ("Gunslinger", "건슬링어"),
    ("Outlaw", "무법자"),
    ("Relic Hunter", "유물 사냥꾼"),
    ("Survivor", "생존자"),
    ("Hub Upgrade", "거점 강화"),
    ("Weapon Mod", "무기 개조"),
    ("Named Weapon", "고유 무기"),
    ("Relic Upgrade", "유물 강화"),
    ("Accessory", "장신구"),
    ("Project", "프로젝트"),
    ("Boss Support", "보스 지원"),
    ("Enemy Archetype", "적 유형"),
    ("Enemy Trait", "적 특성"),
    ("Story Chapter", "스토리 장"),
    ("Family", "가족"),
    ("Region", "지역"),
    ("Boss", "보스"),
    ("World Tier", "월드 티어"),
    ("World Route", "월드 루트"),
    ("Veteran", "베테랑"),
    ("Elite", "정예"),
    ("Enforcer", "집행자"),
    ("Mythic Elite", "신화 정예"),
    ("Current", "현재"),
    ("Planned", "예정"),
    ("Known", "확인됨"),
    ("Active", "활성"),
    ("Late Game", "후반"),
    ("Open", "개방"),
    ("Level ", "레벨 "),
    ("World Tier ", "월드 티어 "),
    ("Workbench ", "작업대 "),
    ("Home Upgrades ", "거점 강화 "),
    ("Relics ", "유물 "),
    ("Boss Kills ", "보스 처치 "),
    ("Next World Tier ready: ", "다음 월드 티어 준비 완료: "),
    ("Next World Tier blocked by: ", "다음 월드 티어 조건 부족: "),
    ("Requirement: ", "요구 조건: "),
    ("Spend 1 perk point", "특성 포인트 1 소모"),
    ("Already active", "이미 활성화됨"),
    ("Cost: ", "비용: "),
    ("Storage: ", "보관: "),
    ("Result: ", "결과: "),
    ("Missing: ", "부족: "),
    ("Stash ", "보관 "),
    ("Field ", "필드 "),
    ("Combat Role: ", "전투 역할: "),
    ("Combat: HP ", "전투: HP "),
    ("Projection: ", "예상 결과: "),
    ("Rounds: ", "라운드: "),
    ("Support: ", "지원: "),
    ("Support / phases: ", "지원 / 페이즈: "),
    ("Phases: ", "페이즈: "),
    ("Test Spawn: ", "테스트 소환: "),
    ("Spawn Note: ", "소환 메모: "),
    ("World Node: ", "월드 노드: "),
    ("Encounter won -> ", "전투 해결 -> "),
    ("Drop table -> materials ", "드랍 테이블 -> 재료 "),
    ("Picked and routed toward ", "획득 후 다음 보관소로 이동: "),
    ("First clear package -> ", "첫 클리어 패키지 -> "),
    ("Encounter: ", "전투: "),
    ("Drops: ", "드랍: "),
    ("First Clear: ", "첫 클리어: "),
    ("Stash Route: ", "보관 경로: "),
    ("Route Note: ", "루트 메모: "),
    ("Actual: ", "실효: "),
    ("field mats ", "필드 재료 "),
    ("stash mats ", "보관 재료 "),
    ("stash items ", "보관 아이템 "),
    ("routes ", "루트 "),
    ("Perks: ", "특성: "),
    ("Crafting: ", "제작: "),
    ("completed", "완료"),
    ("ready", "준비됨"),
    ("locked", "잠김"),
    ("Unlocked", "해금됨"),
    ("Ready", "준비"),
    ("Locked", "잠김"),
    ("Rank ", "랭크 "),
    ("Tier ", "등급 "),
    ("Melee power", "근접 전투"),
    ("Health", "체력"),
    ("Accuracy", "명중"),
    ("Tracking", "추적"),
    ("Trade leverage", "거래 감각"),
    ("Improves ", "향상: "),
    ("Lowers tuning cost and unlocks deeper weapon work.", "튜닝 비용을 줄이고 무기 개조 폭을 넓힙니다."),
    ("Improves recovery tools and cuts downtime.", "회복 도구 효율을 높이고 공백 시간을 줄입니다."),
    ("Boosts bait efficiency and wilderness recipes.", "미끼 효율과 야생 제작을 강화합니다."),
    ("Finds more parts in camps, wagons, and enemy gear.", "캠프, 마차, 적 장비에서 더 많은 부품을 찾습니다."),
    ("Improves pricing and black-market outcomes.", "거래가와 암시장 결과를 개선합니다."),
    ("Stabilizes mounts under pressure and supports mounted builds.", "압박 상황에서 말을 안정시키고 기승 빌드를 보조합니다."),
    ("Accelerates Beecher's Hope growth and support systems.", "Beecher's Hope 성장과 지원 체계를 가속합니다."),
    ("Melee power, forced entry, carry and handling.", "근접 전투, 강행 돌입, 운반과 취급 능력."),
    ("Health, resistance, recovery, and pressure survival.", "체력, 저항, 회복, 압박 생존력."),
    ("Accuracy, crits, reload flow, and weak-point play.", "명중, 치명타, 재장전 흐름, 약점 공략."),
    ("Tracking, hunting yield, environment reading, and field sense.", "추적, 사냥 수확, 환경 파악, 현장 감각."),
    ("Trade leverage, clue reading, stash detection, and ambush warning.", "거래 감각, 단서 해석, 은닉처 탐지, 매복 경고."),
    ("Improves pelt yield and rare animal drops.", "모피 수익과 희귀 동물 드랍을 향상합니다."),
    ("pelt yield and rare animal drops", "모피 수익과 희귀 동물 드랍"),
    ("Lowers tuning cost and unlocks deeper weapon work", "튜닝 비용을 줄이고 무기 개조 폭을 넓힘"),
    ("recovery tools and cuts downtime", "회복 도구 효율과 공백 시간 감소"),
    ("Boosts bait efficiency and wilderness recipes", "미끼 효율과 야생 제작 강화"),
    ("Finds more parts in camps, wagons, and enemy gear", "캠프, 마차, 적 장비에서 더 많은 부품 발견"),
    ("Stabilizes mounts under pressure and supports mounted builds", "압박 상황에서 말을 안정시키고 기승 빌드 보조"),
    ("Accelerates Beecher's Hope growth and support systems", "Beecher's Hope 성장과 지원 체계 가속"),
    ("pricing and black-market outcomes", "거래가와 암시장 결과"),
    ("yields for hunting and wilderness loops", "사냥과 야생 루프 수익"),
    ("control for sidearms in early progression", "초반 보조무기 제어력"),
    ("Progression upgrade", "성장 업그레이드"),
    ("Home upgrades +1", "거점 강화 +1"),
    ("Clean or better", "Clean 이상"),
    ("battle-ready", "전투 준비 완료"),
    (" short", " 부족"),
    ("A grounded charm that boosts grit-focused builds.", "근성 중심 빌드를 밀어주는 현실적인 부적입니다."),
    ("Unlocks a stronger Beecher's Hope workshop tier.", "Beecher's Hope 작업대 상위 단계를 해금합니다."),
    ("Boosts durability and stopping power for key sidearms.", "핵심 보조무기의 내구도와 저지력을 높입니다."),
    ("A precision sidearm blueprint for mid-game escalation.", "중반 확장을 위한 정밀 보조무기 설계도입니다."),
    ("Expands home progression and tracks named kills.", "거점 성장 단계를 넓히고 네임드 처치를 기록합니다."),
    ("Late-game kit for relic-bound weapon conversions.", "후반 유물 결속 무기 전환용 키트입니다."),
    ("Epilogue-only RPG overhaul structured around the Frontier Ledger, progression, and Beecher's Hope.", "에필로그 전용 RPG 개조안으로, Frontier Ledger와 성장 구조, Beecher's Hope를 중심으로 설계되었습니다."),
    ("First named boss chain and the first major pressure point for the frontier war.", "첫 네임드 보스 연계이자 프런티어 전쟁의 첫 대형 압박 지점입니다."),
    ("Late-game mastermind concept for the mythic arc payoff.", "신화 축 후반 보상을 책임지는 흑막 콘셉트입니다."),
    ("Home stability, medicine, and the emotional cost of escalation.", "집의 안정, 치료, 그리고 확전이 가져오는 감정적 대가를 담당합니다."),
    ("Archive keeper for clues, boss notes, and the Frontier Ledger.", "단서, 보스 기록, Frontier Ledger를 보관하는 기록 담당입니다."),
    ("Rumor broker for dirty jobs, shortcuts, and black-market leads.", "더러운 일, 지름길, 암시장 정보를 흘리는 소문 중개인입니다."),
    ("Opening jobs, first elite bounties, first resource loop.", "초반 의뢰, 첫 정예 현상금, 첫 자원 루프가 열리는 지역입니다."),
    ("Ambushes, relic convoy routes, mid-tier mixed danger.", "매복, 유물 수송로, 중티어 혼합 위험이 겹치는 지역입니다."),
    ("High-danger outlaw frontier and endgame salvage lane.", "고위험 무법 지대이자 엔드게임 회수 루트입니다."),
    ("Ore, hunting, and veteran marksman pressure zone.", "광물, 사냥, 베테랑 사수 압박이 모이는 지역입니다."),
    ("Dense weirdness, cult traces, swamps, and smuggling.", "기이함, 컬트 흔적, 늪지, 밀수가 뒤엉킨 지역입니다."),
    ("Late endurance, hidden caches, and mythic hunt chains.", "후반 지구력 시험, 숨겨진 보관함, 신화 사냥 연계가 있는 지역입니다."),
    ("Opening state with grounded threats and beginner elites.", "현실적인 위협과 초급 정예가 섞인 시작 월드 상태입니다."),
    ("Organized enemies, stronger routes, and harder contracts.", "조직화된 적, 강화된 이동로, 더 까다로운 계약이 등장합니다."),
    ("Named elites widen, rare drops matter, and boss chains deepen.", "네임드 정예가 늘어나고 희귀 드랍 가치와 보스 연계가 깊어집니다."),
    ("Late-game traits, heavy pressure, and mythic escalation.", "후반 특성, 높은 압박, 신화급 확장이 본격화됩니다."),
    ("Post-ending sandbox for endless legendary hunts.", "엔딩 이후 끝없는 전설 사냥을 위한 샌드박스 상태입니다."),
    ("Reduces revolver damage and suppresses stagger from weaker hits.", "리볼버 피해를 줄이고 약한 타격의 경직을 눌러버립니다."),
    ("Partially shrugs off shotgun stagger at close range.", "근거리 산탄총 경직을 일부 무시합니다."),
    ("Ignores fear and weak stagger effects.", "공포와 약한 경직 효과를 무시합니다."),
    ("Flushes the player from cover and follows trails aggressively.", "플레이어를 엄폐에서 끌어내고 흔적을 집요하게 추적합니다."),
    ("Can deflect frontal shots until weak states are exposed.", "약점 상태가 드러나기 전까지 정면 사격을 튕겨낼 수 있습니다."),
    ("Becomes faster and more brutal at low health.", "체력이 낮아질수록 더 빠르고 난폭해집니다."),
    ("Late-game unnatural survivability and retaliatory pressure.", "후반 비정상적인 생존력과 반격 압박을 지닙니다."),
    ("First city-node pressure enemy with disciplined firearms.", "도시 노드 압박을 담당하는 첫 규율형 총기 적입니다."),
    ("Hunts the player out of cover and punishes slow repositioning.", "플레이어를 엄폐 밖으로 몰아내고 느린 재배치를 응징합니다."),
    ("Close-range bruiser that makes New Austin feel dangerous.", "New Austin을 위협적으로 만드는 근접형 돌격 적입니다."),
    ("Boss adds built to lock down frontal pushes.", "정면 돌파를 묶어두기 위한 보스 지원 적입니다."),
    ("Late-game cult-linked elite for world tier 4 and beyond.", "월드 티어 4 이후를 담당하는 후반 컬트 연계 정예입니다."),
    ("John survives the opening raid and begins tracking the frontier war.", "존은 초반 습격에서 살아남고 프런티어 전쟁의 흔적을 추적하기 시작합니다."),
    ("Blackwater becomes the first real city node and contract gateway.", "Blackwater가 첫 본격 도시 노드이자 계약 관문이 됩니다."),
    ("The first named boss chain goes live once John is battle-ready.", "존이 실전에 대비되면 첫 네임드 보스 연계가 시작됩니다."),
    ("Named elites spread and relic economies start to dominate the map.", "네임드 정예가 퍼지고 유물 경제가 맵 전역을 지배하기 시작합니다."),
    ("The late-game myth thread resolves without breaking canon.", "후반 신화 줄기는 원작 정사를 깨지 않는 선에서 마무리됩니다."),
    ("Lower Beecher's Hope tuning cost and stronger salvage flow.", "Beecher's Hope 튜닝 비용을 낮추고 회수 효율을 높입니다."),
    ("Reduced stamina drain while traveling.", "이동 중 스태미나 소모를 줄입니다."),
    ("Better clue visibility and longer enemy trail persistence.", "단서 가시성을 높이고 적 흔적 지속시간을 늘립니다."),
    ("Bonus accuracy while all equipped weapons are Clean or better.", "장착 무기가 모두 Clean 이상일 때 명중 보너스를 얻습니다."),
    ("Reduced recoil on revolvers and pistols.", "리볼버와 권총 반동을 줄입니다."),
    ("Faster ready time after a holster draw.", "권총집에서 꺼낸 뒤 전투 준비 시간이 빨라집니다."),
    ("Chance to scare weaker enemies during an opening burst.", "초반 난사 중 약한 적을 겁먹게 만들 확률이 생깁니다."),
    ("Quicker looting under pressure.", "압박 상황에서도 더 빠르게 루팅합니다."),
    ("Extra reward roll on cargo and convoy targets.", "화물과 호송 목표에서 추가 보상 판정을 얻습니다."),
    ("Bonus damage against named enemies and boss adds.", "네임드 적과 보스 지원 적에게 추가 피해를 줍니다."),
    ("Better boss material yields and higher accessory floors.", "보스 재료 수급량과 장신구 최소 등급을 끌어올립니다."),
    ("Higher chance to detect hidden relic caches.", "숨겨진 유물 보관함을 찾을 확률이 높아집니다."),
    ("Field repairs restore more weapon condition.", "현장 수리 시 무기 상태를 더 많이 회복합니다."),
    ("Combat bonuses while defending allied property.", "아군 거점을 방어할 때 전투 보너스를 얻습니다."),
    ("Flat damage reduction against common enemies.", "일반 적 상대로 고정 피해 감소를 얻습니다."),
]

SORTED_FRAGMENT_REPLACEMENTS = sorted(FRAGMENT_REPLACEMENTS, key=lambda item: len(item[0]), reverse=True)

STATIC_KOREAN_UI_LABELS = {
    "FL_UI_TITLE": "프런티어 레저",
    "FL_UI_ZONE_PROMPT": "Beecher's Hope [F11] 전체 레저",
    "FL_UI_EMPTY": "표시할 항목이 없습니다.",
    "FL_UI_MAIN_TITLE": "존 마스턴: Ashes of the Frontier",
    "FL_UI_FOOTER": "Left/Right: 페이지   Up/Down: 항목   Esc: 닫기",
    "FL_UI_STATUS_READY": "프런티어 레저 준비 완료.",
    "FL_UI_STATUS_CLOSE_FULL_FIRST": "전체 레저를 닫은 뒤 간이 레저를 토글하세요.",
    "FL_UI_STATUS_LEDGER_CLOSED": "프런티어 레저를 닫았습니다.",
    "FL_UI_STATUS_LEDGER_OPENED": "프런티어 레저를 열었습니다.",
    "FL_UI_STATUS_ZONE_BLOCK": "Beecher's Hope 범위 안에서만 전체 레저를 열 수 있습니다.",
    "FL_UI_STATUS_ZONE_AUTOCLOSE": "허브 범위를 벗어나 전체 레저를 닫았습니다.",
    "FL_WB_TITLE": "작업대",
    "FL_WB_TAB_CRAFT": "제작",
    "FL_WB_TAB_ARCHIVE": "백과",
    "FL_WB_TAB_RESULT": "결과",
    "FL_WB_LIST": "목록",
    "FL_WB_DETAIL": "세부",
    "FL_WB_RUN": "현재 런",
    "FL_WB_CRAFTED": "제작 완료",
    "FL_WB_CONTROLS": "좌우 탭 / 상하 선택 / E 확인",
    "FL_WB_STATE_DONE": "완료",
    "FL_WB_STATE_READY": "준비",
    "FL_WB_STATE_LOCKED": "잠김",
    "FL_WB_CRAFT_01": "01 건오일",
    "FL_WB_CRAFT_02": "02 메달",
    "FL_WB_CRAFT_03": "03 작업대",
    "FL_WB_CRAFT_04": "04 프레임",
    "FL_WB_CRAFT_05": "05 인장",
    "FL_WB_CRAFT_06": "06 블랙워터",
    "FL_WB_CRAFT_07": "07 트로피",
    "FL_WB_CRAFT_08": "08 유물키트",
    "FL_WB_CRAFT_09": "09 강장팩",
    "FL_WB_CRAFT_10": "10 공구롤",
    "FL_WB_CRAFT_11": "11 데드아이",
    "FL_WB_ARCHIVE_01": "01 장부",
    "FL_WB_ARCHIVE_02": "02 폐철",
    "FL_WB_ARCHIVE_03": "03 가죽",
    "FL_WB_ARCHIVE_04": "04 강장",
    "FL_WB_ARCHIVE_05": "05 유물",
    "FL_WB_ARCHIVE_06": "06 동선",
    "FL_WB_ARCHIVE_07": "07 위협",
    "FL_WB_ARCHIVE_08": "08 거점",
    "FL_WB_RESULT_01": "01 최근 결과",
    "FL_WB_RESULT_02": "02 보너스",
    "FL_WB_RESULT_03": "03 작업대 상태",
    "FL_WB_RESULT_04": "04 다음 추천",
}


def c_string(value: str) -> str:
    return json.dumps(value, ensure_ascii=False)


def sanitize_text(value: str) -> str:
    return " ".join(part.strip() for part in value.replace("\r", "").split("\n") if part.strip())


def make_label_id(*parts: str) -> str:
    joined = "_".join(str(part) for part in parts if str(part))
    normalized = re.sub(r"[^A-Za-z0-9]+", "_", joined).upper().strip("_")
    normalized = re.sub(r"_+", "_", normalized)
    if not normalized or normalized[0].isdigit():
        normalized = f"FL_{normalized}"
    return normalized


def wrap_for_gxt(value: str, max_chars: int, max_lines: int) -> str:
    text = sanitize_text(value)
    if not text:
        return ""
    if max_chars <= 0:
        return text

    words = [word for word in text.split(" ") if word]
    lines: list[str] = []
    current = ""

    for word in words:
        candidate = word if not current else f"{current} {word}"
        if not current or len(candidate) <= max_chars:
            current = candidate
            continue

        lines.append(current)
        current = word
        if len(lines) >= max_lines:
            break

    if current and len(lines) < max_lines:
        lines.append(current)

    if not lines:
        lines = [text]

    return "~n~".join(lines[:max_lines])


def translate_text(value: str) -> str:
    translated = EXACT_TRANSLATIONS.get(value, value)
    for source, target in SORTED_FRAGMENT_REPLACEMENTS:
        translated = translated.replace(source, target)
    return translated


def translate_compact_line(value: str) -> str:
    translated = translate_text(value)
    if translated.startswith("특성: "):
        items = [item.strip() for item in translated.removeprefix("특성: ").split(",") if item.strip() and item.strip() != "None"]
        if len(items) > 2:
            return f"특성: {items[0]}, {items[1]} 외 {len(items) - 2}개"
    if translated.startswith("제작: "):
        items = [item.strip() for item in translated.removeprefix("제작: ").split(",") if item.strip() and item.strip() != "None"]
        if not items:
            return "제작: 없음"
        if len(items) > 2:
            return f"제작: {items[0]}, {items[1]} 외 {len(items) - 2}개"
    return translated


def localize_text(value: str, language: str) -> str:
    if language == "ko":
        return translate_text(value)
    return value


def localize_compact_line(value: str, language: str) -> str:
    text = str(value)
    if language == "ko":
        return translate_compact_line(text)
    return text
