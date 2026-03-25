# Quality Bar

## Goal

Make the first serious patch close enough to final quality that validation usually settles in one or two rounds.

## Before Editing

- Read the target file.
- Read at least one adjacent dependency or caller.
- Read the most relevant test file.
- Read the most relevant worklog or design note when the area is unstable.
- Name the invariants that must survive the change.

## Invariants To Check In This Project

- `frontier_rpg` content, systems, UI rows, and CLI outputs stay aligned.
- `ingame_mod/scripts/export_ledger_data.py` remains the source of generated native data.
- `ingame_mod/generated/*` stays generated-only.
- Page ids, dataset ids, and packaged filenames stay stable unless the task explicitly changes them.
- Korean UI changes do not leave duplicate native render paths active.

## Validation Ladder

### Prototype/data changes

- `python -m unittest discover -s tests`
- `python -m frontier_rpg ui --check`
- One narrow CLI command that shows the changed surface

### Export/native bridge changes

- `python -m unittest discover -s tests`
- `.\build.ps1`
- `python .\scripts\workspace_smoke_test.py`
- `.\preflight.ps1`

### Korean/workbench/native renderer changes

- `.\build.ps1`
- `python -m unittest discover -s tests`
- `python -m frontier_rpg ui --check`
- `python .\scripts\workspace_smoke_test.py`
- `.\preflight.ps1`

## Red Flags

- Adding a second helper path instead of deleting the stale one
- Editing generated headers directly
- Fixing a label in one layer while another layer still emits stale text
- Skipping tests because the bug "looks visual"
- Running the whole loop repeatedly without first improving the patch

## Definition Of Done

- The edited surface has one clear active code path.
- The most relevant tests pass.
- The broader workspace verification passes when needed.
- Remaining risk, if any, is stated explicitly and is about live visual confirmation rather than obvious code uncertainty.
