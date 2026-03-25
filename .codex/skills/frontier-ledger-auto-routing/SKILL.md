---
name: frontier-ledger-auto-routing
description: Automatic skill selection and staged, quality-first orchestration for Frontier Ledger tasks. Use when the user gives a feature, fix, refactor, polish, review, or investigation request in this workspace without naming skills and Codex should choose the right combination of project skills, enforce the right execution order, prefer strong quality by default, and avoid making the user remember skill names.
---

# Frontier Ledger Auto Routing

Read `references/routing-matrix.md` before choosing a stack.

## Default Behavior

- The user does not need to name skills explicitly.
- For meaningful implementation, fix, refactor, polish, or review work, choose the best stack automatically.
- Bias toward higher confidence and fewer re-test cycles, not maximum speed.
- Start from an execution-order skill first so complex work does not skip context, planning, or validation.
- Start from a response-format skill first so the active skill stack is always visible to the user.

## Routing Rules

- Start with `$frontier-ledger-skill-banner` for any meaningful project task so every user-facing reply begins with the active skill list.
- Start with `$frontier-ledger-phased-delivery` for any meaningful project task; let it choose the full six-step path for complex work and the fast path for tiny mechanical edits.
- Start with `$frontier-ledger-quality-pass` for any non-trivial code change unless the task is a tiny doc-only or clearly mechanical edit.
- Add `$frontier-ledger-doc-sync` when the task touches design docs, content tables, progression logic, page order, gameplay systems, or any change that must flow from `기획문서` through `frontier_rpg` and into native export.
- Add `$frontier-ledger-korean-ui` when the task touches Korean strings, bitmap coverage, workbench text/layout, `live_ko`, or native renderer behavior.
- Add `$frontier-ledger-build-loop` for any runtime-impacting change that needs verification.
- Finish meaningful project tasks with `$frontier-ledger-completion-log`.

## Output Expectations

- Put the active skill list on the first visible line when `$frontier-ledger-skill-banner` is active.
- Tell the user which skills were selected and why in one short line at the start or before edits.
- Tell the user whether the task is on the `full six-step path` or `fast path` when `$frontier-ledger-phased-delivery` is active.
- Keep the stack minimal, but do not skip quality or verification skills when the risk is real.
- Do not force a long stage-by-stage writeup for tiny tasks.
- If the task is broad and touches multiple layers, combine the relevant skills rather than forcing one skill to do everything.
