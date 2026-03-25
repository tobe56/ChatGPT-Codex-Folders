---
name: frontier-ledger-completion-log
description: Append a structured completion record for each meaningful Frontier Ledger task. Use at the end of implementation, refactor, debugging, review, packaging, or investigation work to record what was built, which skills and validations were used, key files touched, and remaining risks in `개발/TASK_COMPLETION_LOG.md`.
---

# Frontier Ledger Completion Log

Read `references/log-format.md` before appending an entry.

## Target

- Log file: `E:\RDR2 RPG MODS\개발\TASK_COMPLETION_LOG.md`
- Append one entry after every meaningful project task.

## Use The Script

- Use `scripts/append_completion_log.py` instead of hand-formatting when possible.
- Include the task title, request summary, result, skills used, key files, validation, and residual risk.

## Logging Rules

- Log real work: implementation, refactor, debugging, review, packaging, or substantial investigation.
- Skip casual chat or purely conversational turns.
- If no files changed, say `analysis only` in the result or files section.
- Mention the log path in the final response so the user knows the record was written.
