# 2026-03-20 Live Counter And Visible Spawn Fix

## End-Of-Day State

Today the project moved out of the "spawn exists but still feels fake" state.
The live build now has a working visible regional spawn path and a live counter path that records actual field results in the saved profile.

## What Was Proved Today

### Spawn

- the regional spawn trigger now works live
- the spawned enemies are now visible enough for the user to confirm them in-game
- the spawn path no longer depends on the earlier transparent/player-lookalike fallback behavior

### Counters

The following counters now reflected in the tested live profile:

- `EnemyKills`
- `AnimalKills`
- `LootCount`
- `RewardEarned`

This is a major shift from the previous state where spawned fights and field kills were not reliably reflected.

### Live Progression

The tested run also showed:

- progression state updates
- support-rank advancement
- trail-stage advancement

which means the live loop is now behaving more like a real gameplay system and less like an isolated debug path.

## What Changed Under The Hood

- kill attribution was widened beyond the earlier strict source-of-death assumption
- tracked peds are now more useful after the immediate nearby scan window
- spawned ped handling is less brittle than the earlier crash-prone / invisible path
- runtime logging is much better for future live debugging

## Why Tomorrow Should Start From Here

Now that visible spawn plus live counters are both working, tomorrow can stop treating the native path like a fragile prototype stub and instead push on the next real finish tasks:

1. live boss-archive summon verification
2. broader world-route placement
3. tighter balance against trusted live counters
4. cleanup of leftover runtime complexity

## Practical Rule

The next pass should use the now-working live loop as the truth source:

- test in game
- read the live logs/profile
- tune from evidence

instead of guessing from prototype output alone
