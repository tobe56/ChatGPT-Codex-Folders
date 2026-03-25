# 2026-03-21 Authored Spawn Routing And Live Follow-Up

## End-Of-Day State

Today finished in a better place than it started:

- the weaker planning-doc categories were widened through real prototype/export/test work
- the real game folder was refreshed with the newer build
- the user-provided live screenshot/logs made the remaining native blocker much clearer

That blocker was not "the data is missing."
It was "the live runtime is still choosing the old generic regional spawn profiles even though the authored/export data already contains richer encounter slices."

## What Was Confirmed

### The generated content was already there

The generated ledger/export path already contained:

- `Dead Courier Satchel`
- `Rusk, The Collector's Dog`
- blueprint entries
- named-weapon routes
- new world-route entries

So the prototype/export side was ahead of the native live runtime.

### The native runtime was still generic

The live log initially kept showing:

- `Blackwater Mercenary Patrol`
- `Tall Trees Tracker Pack`

That matched the user's complaint that the in-field result still felt generic and that specific authored names like `Rusk` were not really showing up.

### The live patch moved the runtime forward

After the native router patch and reinstall, the newest live log switched to:

- `Build=2026-03-21-authored-spawn-pass`
- `Processing regional spawn request for profile=Rusk, The Collector's Dog`

That is the strongest proof from today that the authored encounter layer is finally reaching the live spawn path.

## What Changed Under The Hood

- the lower-coverage doc-driven categories were expanded in the prototype with more authored early-game structure, blueprint gates, named-weapon/ammo routes, and world-node density
- the native live install was refreshed again with hash-match confirmation
- `frontier_ledger.c` now includes extra authored/contextual spawn profiles instead of relying only on the older region-to-generic-profile table
- the live spawn debug path was widened so a full-ledger selected entry can queue a linked authored encounter, not only the boss-archive path

## Why Tomorrow Should Start From Here

Tomorrow should not restart from "is the data even there?"
That question is answered.

The new starting point is:

1. the data exists
2. the export exists
3. the live router can now hit at least one authored named encounter
4. the remaining gaps are presentation, consistency, and the last live-confirmation seams

## Practical Rule

Tomorrow should treat these as separate tracks instead of one fuzzy problem:

- spawn selection / encounter identity
- corpse/prompt presentation after death
- workbench parity with exported blueprint content
- boss-archive and selected-entry live confirmation

That separation matters because today proved the spawn-routing fix alone does not automatically fix every "it still feels generic" symptom.
