# 2026-03-19 Live Spawn Rollout And Next Fix

## End-Of-Day State

Today moved the project into the first real live-runtime validation pass for the new spawn and world-behavior work.
This was no longer just a workspace-only development day.

## What Was Finished Today

### Native Spawn Path

- a real native spawn hook path now exists
- regional encounter spawning can be triggered live
- the boss archive also has a summon-side hook path
- the input path was hardened by queuing the spawn request and processing it during the runtime tick instead of doing the work directly inside the keyboard callback

### Route / Storage Expansion

- more world route nodes were authored
- storage destinations were widened so the system no longer reads like one universal stash placeholder
- route/storage text now has better prototype and export exposure

### Live Gameplay Hook Expansion

- the runtime now pushes more than the older stamina / weapon-wear / deadeye layer
- health, weapon damage, melee damage, horseback mitigation, and move-rate support were added to the applied live effect stack

### Live Install

- the latest tested package was pushed into the actual `E:\SteamLibrary\steamapps\common\Red Dead Redemption 2` root
- the live files were checked against the workspace `dist` outputs by hash

## What The Live Test Actually Proved

The good news:

- the latest build is definitely the one loading in the game
- the spawn trigger now works live
- spawned encounter peds are being created successfully
- the project is no longer stuck at the earlier immediate-spawn crash state

The bad news:

- kill / loot / reward counters still did not reflect the user-reported field actions in the saved live profile

So the next fix is no longer:

- "make spawn exist"

It is now:

- "make live action tracking correctly credit the spawned fight and the user's normal field kills"

## Why Tomorrow Should Start There

Without stable live counters:

- combat progression is misleading
- route reward balance is hard to judge
- boss readiness and later progression tuning remain partially blind

So tomorrow's first useful task is to repair live kill / loot accounting before doing wider world polish.

## Tomorrow Plan

1. inspect why live spawned peds and recent field kills are not incrementing `EnemyKills`
2. inspect whether the death source / nearby ped tracking path is too strict for the spawned actors
3. confirm whether animal kills and looting share the same missed-accounting cause
4. rerun a short live test after the fix
5. only after counters work, continue the next spawn/balance/world pass
