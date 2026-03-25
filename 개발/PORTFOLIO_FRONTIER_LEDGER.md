# Frontier Ledger Portfolio Record

## Project Summary

- Project name: `Frontier Ledger`
- Target game: `Red Dead Redemption 2`
- Goal: build an epilogue-only RPG progression layer for John Marston with an in-game ledger UI, progression systems, crafting, perks, and hub-based management.
- Workspace root: `E:\RDR2 RPG MODS\개발`
- Native mod root: `E:\RDR2 RPG MODS\개발\ingame_mod`

## What I Built

### 1. System Prototype

- Built a Python gameplay prototype that converts design documents into executable systems.
- Implemented level and XP progression, stat growth, perk unlock rules, crafting tables, workbench upgrades, world tiers, story gates, Active Trails, Frontier Map intel, Beecher's Hope hub progression, grounded gear slots, armory state, boss archive, collection tracking, family support state, farming lanes, subsystem integration tracks, world escalation tables, and ending/post-ending route state.
- Used the prototype as the source of truth for the native in-game UI dataset.

Key files:

- `E:\RDR2 RPG MODS\개발\frontier_rpg\systems.py`
- `E:\RDR2 RPG MODS\개발\frontier_rpg\content.py`
- `E:\RDR2 RPG MODS\개발\frontier_rpg\ui_state.py`

### 2. Native In-Game Mod Foundation

- Built a native ASI mod for ScriptHookRDR2.
- Added `F10` compact ledger overlay.
- Added `F11` full ledger overlay gated by Beecher's Hope zone rules.
- Added page navigation, entry selection, overlay safety rules, auto-close behavior, and runtime logging.
- Expanded the native ledger to consume a much larger document-led page set instead of only the original first-pass overview and progression pages.
- Reworked the native full-ledger tabs into a sliding window with current/total page indicators so the ASI can carry a real multi-page RPG ledger structure.

Key files:

- `E:\RDR2 RPG MODS\개발\ingame_mod\src\frontier_ledger.c`
- `E:\RDR2 RPG MODS\개발\ingame_mod\src\ledger_config.c`
- `E:\RDR2 RPG MODS\개발\ingame_mod\src\script_hook_bindings.c`

### 3. Data Export Pipeline

- Built a generator that exports gameplay data into static C tables consumed by the ASI.
- Added bilingual baked datasets for renderer testing:
  - `baseline_en`
  - `simulated_en`
  - `endgame_en`
  - `baseline_ko`
  - `simulated_ko`
  - `endgame_ko`
- Expanded the export so the native build now carries `Active Trails`, `Frontier Map`, `Homefront`, `Gear`, `Armory`, `Ledger of the Dead`, and `Collection` directly from the Python prototype.

Key file:

- `E:\RDR2 RPG MODS\개발\ingame_mod\scripts\export_ledger_data.py`

## Technical Stack

- Python
- C11
- PowerShell
- Zig C compiler
- ScriptHookRDR2
- Windows logging and filesystem automation

## Engineering Problems Solved

### Save Failure Root Cause Isolation

- Symptom: RDR2 save failed despite enough disk space.
- Root cause: Windows Defender Controlled Folder Access blocked `RDR2.exe` from writing to `%USERPROFILE%\Documents\Rockstar Games`.
- Result: confirmed the issue was OS security policy, not the mod itself.

### F11 Full Ledger Not Opening

- Symptom: `F11` did not open the full ledger even inside Beecher's Hope.
- Root cause: entity position vector was being unpacked incorrectly from native return data, causing invalid zone distance checks.
- Fix: changed vector extraction to read float values from native return slots explicitly.
- Result: `FrontierLedger.log` now shows valid coordinates and `Full ledger opened by keypress.`

### UI Text Overlap

- Symptom: compact overlay lines overlapped in the `Quick Intel` and `Ready Now` sections.
- Root cause: Y offset advanced by a fixed amount even when lines wrapped.
- Fix: counted wrapped lines and advanced layout based on actual rendered height.

### Save-Time Hitching / Menu Hitching

- Symptom: noticeable background hitching during save or menu activity.
- Root cause: the script loop stayed at a tight update cadence even when overlays were idle.
- Fix: added a slower wait interval when no overlay or status toast is active, and early-returned when pause menu was open.
- Result: user confirmed save hitching is now mostly gone.

### Design-Led System Realignment

- Symptom: early implementation work drifted toward easy-to-read runtime values instead of the actual Frontier RPG design priorities.
- Root cause: implementation sequencing optimized for low-risk native plumbing checks instead of the design documents.
- Fix: rebuilt the prototype around doc-led systems first:
  - Active Trails
  - Frontier Map intel
  - Beecher's Hope hub feature progression
  - Homefront family support
  - Gear / Armory
  - Ledger of the Dead
  - Collection tracking
- Result: the development workspace now reflects the actual project identity instead of duplicating base-game HUD data.

### Prototype Completion Hardening

- Added a full design-doc prototype layer for:
  - farming lanes
  - integrated subsystem tracks
  - world-state escalation per tier
  - ending and post-ending route state
- Added an `endgame` projection profile so the project can be validated at baseline, first-10-hours, and post-ending states.
- Expanded regression coverage to verify the new systems together instead of in isolation.

### In-Game Modization Milestone

- Reached the first serious prototype-to-native migration pass in the workspace-only build pipeline.
- Verified that the ASI package can now be regenerated with the larger document-shaped ledger dataset through `ingame_mod\build.ps1`.
- Kept this milestone isolated from the live game folder so native build iteration stays reversible and portfolio-safe.

### In-Game Modization 70% Push

- Migrated `Farming`, `Network`, `World`, and `Ending` from prototype-only validation into the native ASI export path.
- Brought the full native ledger export to an 18-page structure that now covers most of the design-document campaign layers.
- Added export-level regression tests so the expanded native page map cannot silently shrink in later iterations.

### In-Game Modization 90% Push

- Added live region-aware runtime context derived from real player coordinates instead of relying only on baked dataset text.
- Added session persistence so the ASI remembers the active dataset, last page, and per-page selection state across loads.
- Added an isolated DLL smoke-test harness that validates attach, timeout, detach, and session-file output outside the live game folder.

### Live Frontier Run Push

- Added a persistent native `FrontierLedger.profile.ini` layer so the ASI now remembers discovered frontier regions, accumulated field travel, Beecher's Hope return cycles, and a three-phase live frontier state.
- Added `live_en` / `live_ko` dataset aliases that automatically resolve to the baseline, simulated, or endgame exports based on the live run instead of forcing manual dataset swapping.
- Reworked compact/full ledger summaries so the native UI now carries a live phase label, progress summary, and current frontier objective in addition to the baked document pages.
- Added page-level live overrides for the most important native views so `Overview`, `Trails`, `Frontier Map`, `Homefront`, `Farming`, `Network`, `World`, and `Ending` react to the current run state instead of acting like static exports.
- Added a reusable workspace smoke script at `ingame_mod\scripts\workspace_smoke_test.py` to verify attach, timeout, detach, session output, and live profile output from an isolated temp runtime folder.

### Affinity And Category Midgame Push

- Raised affinity into a real midgame-authored loop with activity gains, stage 1~2 quest clears, reward packages, affinity-gated perks, and a native `Affinity` export page.
- Split category-owned logic out of the old prototype monolith and into dedicated feature modules for `Homefront`, `Farming`, `Network`, `World`, and `Ending`.
- Added milestone / package claim loops to `Farming`, `Network`, `World`, and `Ending`, so those categories now grant real materials, items, and collection state during simulation and endgame projection instead of acting like static summaries only.

### Clean Live Install Completion

- Rotated the previously installed `Frontier Ledger` files into a dedicated backup folder under the real game root.
- Reinstalled the current workspace package into the live game folder and hash-verified:
  - `FrontierLedger.asi`
  - `FrontierLedger.ini`
  - `lml/downloader/Frontier Ledger Korean Text/install.xml`
  - `lml/downloader/Frontier Ledger Korean Text/strings.gxt2`
- The user then confirmed the current in-game UI pass looked correct, and the live log showed clean attach plus continued phase / hub / skill persistence.

### Korean Glyph Rendering Failure

- Symptom: English displays cleanly, but Korean characters render as square placeholders.
- Current conclusion: the active `_BG_DISPLAY_TEXT + VAR_STRING(LITERAL_STRING)` path can draw ASCII/English reliably but does not appear to use a Korean-capable glyph path for this overlay.
- Current mitigation: bilingual datasets are now baked into the build so English can be used as a renderer-control test.
- Day 2 implementation: added a second Win32/GDI desktop overlay path with configurable system font selection (`Malgun Gothic` by default) so Hangul rendering can be tested outside the original glyph-limited HUD path.

### English-First Reset

- Reframed the in-game UI baseline around short English/ASCII-safe strings instead of trying to finish Korean presentation first.
- Added an explicit locale seam through `DefaultDataset` + `PreferredLocale`, with the active workspace now defaulting to English and a parked Korean example config under `ingame_mod\locales\ko`.
- Collapsed the active workbench overlay away from mixed special-case renderer calls and back onto one primary draw rule for inline text and one primary draw rule for wrapped/body text.
- Shortened the active workbench helper/detail/result copy so debugging future layout issues is about one renderer path and one ASCII baseline, not multiple mixed localization branches.

## Current Test Status

- `F10` compact overlay: working
- `F11` full ledger: working
- Save hitching: largely resolved
- English UI dataset: ready for renderer A/B test
- Korean UI dataset: now wired to a new desktop overlay renderer for Hangul validation
- Live frontier run profile: working in workspace build with isolated attach/session/profile smoke validation
- Workspace prototype validation:
  - `python -m unittest discover -s tests`: passing
  - `python -m frontier_rpg ui --check`: passing
  - doc-led CLI pages for trails, intel, hub, gear, armory, boss archive, collection, homefront, farming, network, world, ending, and endgame: passing
  - `python ingame_mod\scripts\workspace_smoke_test.py`: passing
- Live game-folder verification:
  - installed `Frontier Ledger` files hash-match the current workspace `dist` outputs
  - current in-game UI verification from the live game folder: positive

## Portfolio Talking Points

- Built a game-specific UI mod pipeline from design docs to live in-game presentation.
- Combined rapid prototyping in Python with native systems work in C.
- Used runtime logging and targeted experiments to isolate engine/UI issues.
- Added a second persistence layer for live in-game run state so the native ASI can progress independently of the prototype bake.
- Converted unclear gameplay bugs into reproducible technical diagnoses:
  - OS-level save blocking
  - incorrect native vector decoding
  - overlay layout bugs
  - renderer glyph limitations

## Deliverables

- Built ASI package:
  - `E:\RDR2 RPG MODS\개발\ingame_mod\dist\FrontierLedger\FrontierLedger.asi`
  - `E:\RDR2 RPG MODS\개발\ingame_mod\dist\FrontierLedger\FrontierLedger.ini`
- Installed test package:
  - `E:\SteamLibrary\steamapps\common\Red Dead Redemption 2\FrontierLedger.asi`
  - `E:\SteamLibrary\steamapps\common\Red Dead Redemption 2\FrontierLedger.ini`
- Runtime log:
  - `E:\SteamLibrary\steamapps\common\Red Dead Redemption 2\FrontierLedger.log`
- Latest clean-install backup root:
  - `E:\SteamLibrary\steamapps\common\Red Dead Redemption 2\codex_backups\FrontierLedger_clean_install_20260317_210545`

## Suggested Portfolio Framing

Use this project as:

- a gameplay systems engineering case study
- a native game modding UI case study
- a debugging and integration case study

Short pitch:

`Built an in-game RPG ledger system for Red Dead Redemption 2 using Python for design-to-data prototyping and C/ScriptHookRDR2 for native in-game UI integration, including progression systems, live overlays, debugging instrumentation, and engine-level issue isolation.`
