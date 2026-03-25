# Frontier Ledger Domain Knowledge

## Live Runtime Facts

- `F10` is the compact ledger.
- `F11` is the full ledger.
- `E` is the confirm key for workbench and live ledger interactions.
- Full ledger is normally gated to Beecher's Hope unless config overrides it.

## Live Dataset Rules

- `live_en` and `live_ko` are aliases.
- They resolve to `baseline_*`, `simulated_*`, or `endgame_*` depending on persisted live state.
- A log line showing `RequestedDataset=live_en` and `Dataset=simulated_en` is expected when the live profile has advanced.

## Persistence Files

- `FrontierLedger.session.ini`
  - UI page, selected rows, workbench tab state
- `FrontierLedger.profile.ini`
  - live run progression and now the `Growth` section for persisted stat/skill interaction state
- `FrontierLedger.log`
  - fastest truth source for whether a live interaction actually fired

## Growth Interaction State

As of `2026-03-16`:

- Full ledger `Stats` can persist stat investment bonuses.
- Full ledger `Skills` can persist skill investment bonuses.
- Full ledger `Crafting` can trigger the existing craft flow directly.
- Full ledger `Armory` can route to ready weapon-focused craft upgrades.

Still pending:

- explicit perk spending from the full ledger
- deeper item spend/gain flow
- real enemy and boss activation beyond tracking and summaries

## Family And Affinity Direction

- Main priority NPCs: Abigail, Jack, Uncle
- Affinity is intended to be an independent feature module
- Base stages: `1` to `3`
- Late stages: `4` and `5`, gated by world tier
- Raise/lower sources:
  - gifts
  - dialogue choices
  - stat / skill compatibility
  - dedicated quest completion
- Repeatable quest NPCs should use affinity stage to scale payout

## Safe Verification Order

1. workspace tests
2. workspace UI check
3. native build
4. workspace smoke test
5. preflight
6. live install
7. live log + profile/session verification
