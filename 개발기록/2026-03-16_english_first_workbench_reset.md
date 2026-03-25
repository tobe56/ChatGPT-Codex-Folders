# 2026-03-16 English-First Workbench Reset

## Intent

Reset the active Frontier Ledger UI baseline to English-first before attempting another Korean readability pass.

## Technical Moves

- changed `FrontierLedger.ini` to `DefaultDataset=live_en`
- added `PreferredLocale=en`
- normalized requested dataset selection through the preferred locale seam
- updated the workbench helper/detail/result strings to shorter ASCII-safe English
- removed the no-longer-used special helper render functions from the active workbench path
- parked Korean re-entry guidance under `ingame_mod\locales\ko`

## Expected Benefit

- fewer renderer-path variables while debugging
- shorter active workbench strings
- easier line-wrap/clipping diagnosis
- explicit English/Korean switch point instead of hidden session drift

## Not Claimed Yet

- no final live visual signoff
- no claim that Korean is restored cleanly
- no claim that every page has final copy polish

This is a baseline stabilization pass, not the localization-finish pass.
