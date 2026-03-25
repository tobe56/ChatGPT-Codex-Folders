# 2026-03-22 Spawn Regression Deep Dive

## What Today Actually Became

The day started as a follow-up pass on authored encounter routing, corpse prompts, workbench parity, and reward pacing.

It ended as a much deeper regression investigation because the live spawn path stopped behaving like the last trustworthy visible builds.

The most important thing learned today is that this was not one blurry bug.
It was a stack of overlapping regressions:

- authored-routing changes
- corpse-prompt additions
- donor-clone experiments
- metaped flag drift
- debug-path complexity

Those all sat on top of a live mod stack where only the real game process could expose the final failure.

## What Was Confirmed From Evidence

### 1. The last clearly visible lineage was older than the new authored passes

By extracting build tags from the archived live ASI backups, the known sequence became much clearer:

- `2026-03-19-spawn-route-pass`
- `2026-03-21-authored-spawn-pass`
- `2026-03-22-authored-sync-pass`
- later 2026-03-22 recovery/stability passes

The user's "it used to visibly work" memory aligns much more closely with the older `2026-03-19-spawn-route-pass` lineage than with the newer authored follow-up passes.

### 2. The failure was not just one visual symptom

Today produced all of these real live symptoms across different passes:

- bodyless metaped-looking enemies
- head-and-hands-only spawns
- invisible-feeling or partial-body enemies
- `F8` crashes in the real game process

That means the right question is not "what UI text looks wrong?"
The right question is "which exact low-level spawn path still survives the real mod stack without corrupting the spawned body?"

### 3. Archived code comparison found real low-level drift

Comparing the current native spawn path against archived `frontier_ledger.c` code from the workspace zips showed real divergence in:

- model selection order
- donor clone usage
- `REQUEST_MODEL` boolean flags
- `CREATE_PED` boolean flags
- extra debug-path branches

This is much stronger evidence than guessing from screenshots alone.

## What Was Changed Today

### Authored-system follow-up work that stays valuable

- selected-entry authored encounter queueing through the normal confirm path
- stronger boss archive entry matching
- wider workbench parity with exported crafting data
- reward-flow cleanup so unique crafts stop inflating the simulation

Those changes are still valuable and should not be thrown away.

### Spawn-regression work that is still in flight

- donor clone path was disabled again after it continued to correlate with broken bodies
- corpse prompt was stripped back out of the active path to reduce one more variable
- `F8` was reduced to a simpler regional debug role with a single-ped cap
- `REQUEST_MODEL` and `CREATE_PED` flags were restored to the archived visible-lineage values
- model selection was moved back toward the archived visible-lineage order

## Why The Problem Is Still Not Declared Fixed

Because only the real game process tells the truth here.

Workspace validation is green:

- build passes
- tests pass
- smoke passes
- preflight passes

But the user still saw bodyless live spawns after multiple workspace-clean builds.

That means tomorrow should stay disciplined:

1. treat the live game process as the final judge
2. use the archived visible lineage as the baseline
3. change one low-level spawn variable at a time
4. stop the moment the live body renders correctly and the crash disappears

## Tomorrow Only Needs Two Tasks

1. restore a truly visible and crash-free live `F8` spawn path in the real game
2. once that path is stable again, finish the blocked live confirmations for selected-entry summon and boss-archive summon
