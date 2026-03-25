---
name: frontier-ledger-phased-delivery
description: Enforce a complexity-aware staged workflow for Frontier Ledger tasks. Use when Codex should keep implementation, fix, refactor, polish, review, or investigation work in a fixed order, split complex problems into an explicit six-step procedure, and still allow a fast path for tiny or clearly mechanical edits without forcing a long 1-6 stage writeup.
---

# Frontier Ledger Phased Delivery

Use this skill as the execution spine for meaningful Frontier Ledger work. Decide first whether the task needs the full six-step path or the fast path, then keep the order intact.

## Complexity Gate

- Classify the task before deep work.
- Use the full six-step path when the task changes behavior, touches multiple files or layers, affects docs plus code sync, needs debugging, carries regression risk, or has non-obvious tradeoffs.
- Use the fast path only when the task is tiny, local, clearly mechanical, and low-risk.
- Required rule: complex problems must follow the full procedure; simple tasks may use the fast path.
- Do not force "long 1-6 stage output" for every request. Keep user-facing updates short unless the task is genuinely complex.

## Full Six-Step Path

### 1. Scope The Request

- Restate the goal, constraints, and affected surfaces.
- Name which companion skills likely apply.
- Surface assumptions only when they matter.

### 2. Expand Context

- Read the target file, adjacent helpers or callers, and the most relevant tests or docs.
- Identify generated-file boundaries, page ids, dataset names, renderer assumptions, persistence effects, and packaging constraints before editing.

### 3. Diagnose And Shrink The Change

- State the likely root cause or required behavior change before patching.
- Prefer the smallest coherent fix that removes stale paths or duplication.
- Decide whether docs, data, tests, exports, or packaging must move in the same pass.

### 4. Commit To The Patch Plan

- Tell the user what you are about to edit before editing files.
- For complex work, give a short numbered plan tied to the change surface.
- Do not start patching while the design is still fuzzy.

### 5. Implement In One Coherent Pass

- Land source changes, dependent updates, and regression coverage together when possible.
- Edit generators or source data instead of generated files.
- Avoid half-refactors and compatibility branches unless they are clearly necessary.

### 6. Validate And Close

- Run the narrowest proof first, then the broader workspace checks needed for confidence.
- If failures appear, fix the class of issue rather than the first symptom.
- Close with a concise summary, validation status, and any remaining live confirmation risk.

## Fast Path

- Use fast path only after explicitly deciding the task is tiny and low-risk.
- Compress steps 1-4 into one or two short updates, then implement and run the lightest relevant verification.
- Still respect the order internally: understand, inspect, patch, verify.
- Escalate from fast path to the full six-step path immediately if scope expands, a second file becomes relevant, or the first fix uncovers ambiguity.

## Communication Rules

- If `$frontier-ledger-skill-banner` is active, keep the skill list as the first visible line of every commentary update and final answer.
- Announce at the start whether you are using the `full six-step path` or `fast path`.
- For complex tasks, surface progress by phase in short updates instead of dumping a long wall of text.
- For simple tasks, skip ceremonial phase-by-phase narration.
- The goal is consistent execution order, not ritualized verbosity.

## Companion Skills

- Pair with `$frontier-ledger-skill-banner` when the user should always see the active skill stack at the top of each reply.
- Pair with `$frontier-ledger-quality-pass` when the task is non-trivial or risky.
- Pair with `$frontier-ledger-doc-sync` when docs, content tables, prototype data, and native export must stay aligned.
- Pair with `$frontier-ledger-korean-ui` for Korean strings, workbench UI, bitmap coverage, or native renderer work.
- Pair with `$frontier-ledger-build-loop` when the task needs the workspace verification ladder.
- Finish meaningful project work with `$frontier-ledger-completion-log`.
