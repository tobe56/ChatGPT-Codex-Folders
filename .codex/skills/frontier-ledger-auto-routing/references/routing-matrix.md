# Routing Matrix

## Baseline

- Tiny note or doc wording only: no special stack, or only the narrowest relevant skill.
- Non-trivial code change: `quality-pass` + `build-loop`

## Add `doc-sync` When

- The request starts from `기획문서`
- Content tables or progression logic are changing
- Page ids, page order, or export-facing data are changing
- Prototype and native views must stay aligned

## Add `korean-ui` When

- Korean text is broken, mixed, clipped, or inconsistent
- Bitmap glyph coverage is involved
- Workbench labels, helpers, or tabs are involved
- `frontier_ledger.c` Korean/native rendering paths are touched
- `live_ko` behavior or generated Korean strings are involved

## Always Close With

- `completion-log` for meaningful project work

## Practical Rule

If unsure between speed and confidence, choose confidence.
