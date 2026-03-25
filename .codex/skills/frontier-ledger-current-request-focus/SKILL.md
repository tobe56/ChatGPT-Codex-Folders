---
name: frontier-ledger-current-request-focus
description: Keep work and answers anchored to the user's current request. Use when follow-up turns risk getting derailed by stale plans, unrelated prior tasks, or overuse of historical context, and Codex should answer the current ask first while using older context only when it directly matters.
---

# Frontier Ledger Current Request Focus

Use this skill when the user wants the agent to stop pulling in unrelated past work and respond directly to the current ask.

## Core Rules

- Start from the current request, not the oldest unresolved backlog item.
- Do not refuse or delay the current request by resurfacing unrelated prior work.
- If past context matters, explain the relevance in one short sentence and continue.
- Use memory docs to accelerate execution, not to redirect the task.
- When the user asks a narrow question, answer it before adding status or history.
- Only bring up older work when:
  - it is a direct blocker
  - it changes the requested action materially
  - the user explicitly asks for historical context

## Response Discipline

- Prefer direct execution over retrospective narration.
- Keep background brief unless the user asks for a deep recap.
- Do not use past plans as a shield against the current request.

## Memory Use

- Read `Memory/API_Index.md` when fast navigation is needed.
- Read `Memory/TroubleShootings.md` when a similar failure pattern appears.
- Read the relevant `Memory/*Domain_Knowledge.md` file only when it helps the current task.
