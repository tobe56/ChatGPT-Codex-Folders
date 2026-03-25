## Skills

### Available skills

- frontier-ledger-auto-routing: Automatically choose the right project skill stack and bias toward high-quality delivery. Use for most Frontier Ledger implementation, fix, refactor, polish, and review requests when the user does not explicitly name skills and Codex should decide the best combination. (file: E:/RDR2 RPG MODS/.codex/skills/frontier-ledger-auto-routing/SKILL.md)
- frontier-ledger-skill-banner: Force the reply to start with the exact list of skills actually used for the task. Use when every user-facing reply should show all active skills on the first visible line. (file: E:/RDR2 RPG MODS/.codex/skills/frontier-ledger-skill-banner/SKILL.md)
- frontier-ledger-phased-delivery: Enforce a complexity-aware staged workflow for Frontier Ledger tasks. Use when work should follow a fixed order, with the full 6-step path for complex problems and a fast path for tiny mechanical edits. (file: E:/RDR2 RPG MODS/.codex/skills/frontier-ledger-phased-delivery/SKILL.md)
- frontier-ledger-build-loop: Safe workspace build, smoke, preflight, and packaging verification for the Frontier Ledger prototype and native mod. Use when touching `개발/frontier_rpg`, `개발/tests`, `개발/ingame_mod`, generated exports, bitmap font assets, or release/install scripts. (file: E:/RDR2 RPG MODS/.codex/skills/frontier-ledger-build-loop/SKILL.md)
- frontier-ledger-completion-log: Append a structured completion record after meaningful project tasks. Use to log what was built, which skills were used, which files changed, and how the result was validated in `개발/TASK_COMPLETION_LOG.md`. (file: E:/RDR2 RPG MODS/.codex/skills/frontier-ledger-completion-log/SKILL.md)
- frontier-ledger-doc-sync: Align design docs, Python prototype content, native export pages, and regression tests. Use when changing progression, trails, workbench recipes, family systems, world tiers, page order, or anything driven by `기획문서/*.md`. (file: E:/RDR2 RPG MODS/.codex/skills/frontier-ledger-doc-sync/SKILL.md)
- frontier-ledger-korean-ui: Maintain Korean ledger and workbench presentation, including GXT text generation, bitmap font coverage, and native renderer cleanup. Use when changing Korean strings, mixed Korean/ASCII helpers, workbench UI text/layout, or `live_ko` behavior. (file: E:/RDR2 RPG MODS/.codex/skills/frontier-ledger-korean-ui/SKILL.md)
- frontier-ledger-quality-pass: Deep quality-first implementation workflow for risky or polish-heavy changes. Use when the user asks for quality over speed, a more thorough pass, fewer test cycles, stronger confidence, durable refactors, or a fix that should ideally settle in one or two validation rounds. (file: E:/RDR2 RPG MODS/.codex/skills/frontier-ledger-quality-pass/SKILL.md)
- frontier-ledger-current-request-focus: Keep work and answers anchored to the user's current request. Use when follow-up turns risk getting derailed by stale plans, unrelated prior tasks, or overuse of historical context, and Codex should answer the current ask first while using older context only when it directly matters. (file: E:/RDR2 RPG MODS/.codex/skills/frontier-ledger-current-request-focus/SKILL.md)

### How to use skills

- The user does not need to remember skill names. For meaningful project work, start by applying `frontier-ledger-auto-routing` and let it choose the rest.
- `frontier-ledger-auto-routing` should normally start with `frontier-ledger-skill-banner`, then `frontier-ledger-phased-delivery`, then add companion skills based on risk and scope.
- Use the minimal set that matches the task.
- Common pairings: `frontier-ledger-doc-sync` -> `frontier-ledger-build-loop`, `frontier-ledger-korean-ui` -> `frontier-ledger-build-loop`.
- Quality-first pairing: `frontier-ledger-quality-pass` -> one of `frontier-ledger-doc-sync` or `frontier-ledger-korean-ui` -> `frontier-ledger-build-loop`.
- Default quality bias: for non-trivial code changes, prefer `frontier-ledger-quality-pass` unless the task is a tiny doc-only or clearly mechanical edit.
- Finish meaningful project tasks by using `frontier-ledger-completion-log` and mention the log path in the final response.
- Keep all verification inside the workspace unless the user explicitly asks for a live game install.
- When a task matches prior failure patterns or large-file navigation needs, read the relevant `Memory/*.md` file first instead of rediscovering the same context.
