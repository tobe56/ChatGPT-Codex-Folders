---
name: frontier-ledger-quality-pass
description: Deep quality-first implementation and stabilization workflow for Frontier Ledger. Use when the user asks for quality over speed, a slower more deliberate pass, fewer test cycles, stronger confidence, durable refactors, risky polish work, or a fix that should ideally pass within one or two validation rounds across `frontier_rpg`, `tests`, or `ingame_mod`.
---

# Frontier Ledger Quality Pass

Read `references/quality-bar.md` before starting edits on a risky task.

## Use The Quality-First Workflow

- If `$frontier-ledger-phased-delivery` is active, keep this workflow inside its chosen order: full six-step path for complex work, fast path for tiny mechanical edits.

1. Expand context before editing.
2. Reduce the change surface before patching.
3. Land the full fix in one coherent pass.
4. Validate in one targeted run, then one broader run.

The goal is not "more testing forever." The goal is to front-load thinking so the first real patch is closer to final quality.

## Expand Context Before Editing

- Read the touched file, nearby helpers, and the tests that cover the same surface.
- Read the caller/callee path when the bug is inside bridge code, export logic, or the monolithic native renderer.
- Read the latest worklog if the task is in an unstable area such as the Korean workbench flow.
- Identify invariants before editing: page ids, dataset names, generated-file boundaries, renderer assumptions, persistence side effects, and packaging expectations.

## Reduce The Change Surface

- Prefer one clean path over another compatibility branch.
- Collapse duplicates when they are the root cause instead of patching each duplicate separately.
- Update tests, export bridges, and support files in the same pass if the behavior depends on them.
- Avoid "half refactors" that leave old helpers active beside new helpers.

## Land The Full Fix In One Pass

- Implement the root-cause fix and the obvious adjacent cleanup together.
- Add or update regression coverage before the first big validation run when the bug is testable.
- Keep generated outputs out of manual edits; change the source script or source data instead.
- If the request is polish-heavy, still anchor it in a concrete quality bar such as readability, single active render path, stable Korean strings, or page consistency.

## Validate In One Or Two Rounds

- First round: run the narrowest checks that directly prove the edited surface.
- Fix any failure by addressing the whole class of issue, not just the first failing line.
- Second round: run the broader workspace loop needed for confidence.
- If a third round seems likely, pause and reassess whether the patch is still symptom-driven.

## Pair This Skill With The Right Companion

- Let `$frontier-ledger-phased-delivery` decide whether the task needs the full six-step path or fast path; use this skill to raise the quality bar inside that route.
- Use `$frontier-ledger-doc-sync` when the risky change starts from planning docs or prototype data.
- Use `$frontier-ledger-korean-ui` when the risky change is visual, Korean-specific, or inside the workbench/native renderer.
- Use `$frontier-ledger-build-loop` for the actual verification ladder.
