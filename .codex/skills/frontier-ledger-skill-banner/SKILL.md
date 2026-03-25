---
name: frontier-ledger-skill-banner
description: Force explicit skill disclosure at the top of Frontier Ledger responses. Use when Codex should list every skill actually used for the task on the first visible line of each user-facing reply, including commentary updates and the final answer, so the user can immediately see which skills were active.
---

# Frontier Ledger Skill Banner

Use this skill as a response-format guardrail for meaningful Frontier Ledger work.

## Core Rule

- Put the skill list on the first visible line of every user-facing reply.
- Use the exact pattern `사용 스킬: ` followed by every actually used skill in execution order.
- Include implicitly selected skills and explicitly named skills.
- Do not place greetings, summaries, or progress text above the skill line.

## What To List

- List only skills that are actually active for the current task.
- Keep the list complete when multiple skills are combined.
- If the active stack changes mid-task, update the first-line banner in later replies to match the current stack.
- Do not invent skills or list inactive options.

## Formatting Rules

- Keep the banner to one line when possible.
- Format each skill in backticks.
- Separate multiple skills with commas.
- After the banner, continue with the normal reply content.

## Scope

- Apply this banner to commentary updates during the task.
- Apply this banner to the final answer.
- For tiny project tasks that still use Frontier Ledger skills, keep the banner and keep the rest of the reply concise.
- If no project skill is active, do not fake a banner just to satisfy the format.

## Companion Skills

- Pair with `$frontier-ledger-auto-routing` so the banner is attached automatically.
- Pair with `$frontier-ledger-phased-delivery` so both the top-line banner and the staged workflow stay consistent.
- Finish meaningful project work with `$frontier-ledger-completion-log`.
