# Frontier Ledger Status And Update Memo

Date: `2026-03-14`
Scope: `ingame_mod` live-game status memo after the late Korean UI passes

## Current Development Status

The project is no longer stuck at the “UI does not appear” stage.
As of the latest live captures on `2026-03-14`, the current state is:

- `F10` compact HUD appears in-game.
- `F11` full HUD appears in-game.
- Korean text is visible enough to read the HUD structure.
- The remaining work is mostly polish, readability, interaction staging, and mixed-stack stability.

## Honest Current UI Assessment

### F10

What is true now:

- It opens in-game.
- It functions as a compact field dashboard.
- It already exposes core live-run state such as combat, hunting, progress, rewards, and hub return flow.

What is still not final:

- The font and panel sizing can still be pushed one more step for easier readability.
- The current content is still a compact summary, not the full original field payload from the planning documents.
- It is closer to an `80% compact field HUD` than a final polished field HUD.

### F11

What is true now:

- It opens in-game.
- It shows a larger Korean dashboard reliably enough to inspect the run state.
- It is good enough to prove that the native Korean HUD exists and can be used as a development base.

What is still not final:

- It is currently a Korean dashboard fallback, not the final Korean version of the original tab/list/detail browser.
- The underlying stats / skills / perks / crafting / armory / collection / boss tracking structures exist, but Korean presentation still prioritizes stable visibility over dense browsing.

## Implemented Under The UI

These systems are already wired into the native runtime:

- live counters
  - `EnemyKills`
  - `LootCount`
  - `AnimalKills`
  - `RewardEarned`
- trail stage progression
- live phase / region discovery
- session / profile save and restore
- run bonus tiers
- stamina bonus
- weapon wear bonus
- deadeye delay bonus
- runtime-driven stats / skills / perks / crafting / armory / boss / collection state

So the current limitation is not “missing systems.”
The limitation is how much of that system density the Korean UI can present cleanly right now.

## Not Yet Final

The following work is still intentionally deferred or unfinished:

- final Korean font / box-size polish
- full `F10` payload parity with the original field-HUD plan
- final Korean `F11` presentation parity with the original page-browser concept
- direct interaction actions such as confirm / invest / apply / craft execution in the in-game UI
- clean mixed-mod crash isolation in a crowded real-game mod stack

## Mixed-Mod Stability Note

Late on `2026-03-14`, a temporary `FrontierLedger-only` isolation attempt was tried in the live game folder.
That experiment changed the whole live mod stack and produced a separate load-stage crash.
It was rolled back.

This means:

- the live mixed stack is still a real stability variable
- UI polish and crash isolation should not be mixed into the same step
- future crash diagnosis should use narrower and more controlled isolation steps

## Recommended Next Update Order

1. Korean font and panel size polish
2. `F10` compact payload improvement
3. `F11` dashboard readability polish
4. interaction work after the Korean presentation layer is stable
5. separate mixed-mod crash isolation pass

## Bottom Line

It is now fair to say:

- the Korean UI exists
- the core native runtime exists
- the project can move forward to the next phase

But it is also important to say:

- the Korean UI is not yet the final polished version
- the current Korean `F10` / `F11` are a usable live basis, not the final presentation target
