# 2026-03-13 In-Game Modization 90% Push

## Summary

This pass focused on making the native ASI behave more like a real in-game system instead of a static exported ledger viewer.

## Native runtime additions

- live frontier context derived from real player coordinates
- nearest-region detection across the document-led campaign space
- route hints shown inside the compact ledger and full ledger detail flow
- region-aware auto-focus for `Frontier Map`, `Farming`, `Network`, `World`, and `Ending`
- session persistence through `FrontierLedger.session.ini`

## Why this matters

At 70%, the ASI could already render most of the document-shaped ledger, but it still behaved like a baked snapshot browser.

This pass pushed the in-game layer closer to a real mod by adding:

- runtime awareness
- state restoration
- page behavior that changes based on where the player actually is

That is the point where the native build stops feeling like a passive test harness and starts behaving like an actual game-facing UI layer.

## Verification

- `python -m unittest discover -s tests`
- `powershell -File E:\RDR2 RPG MODS\개발\ingame_mod\build.ps1`
- `powershell -File E:\RDR2 RPG MODS\개발\ingame_mod\preflight.ps1 -GameRoot "E:\SteamLibrary\steamapps\common\Red Dead Redemption 2"`
- isolated `LoadLibrary` smoke test of the built ASI in a temporary runtime folder

The smoke test confirmed:

- process attach log
- bootstrap thread creation
- clean ScriptHook timeout outside the game
- clean process detach
- successful session file creation

All of this was done without copying a new build into the live game folder.
