## Locale Notes

The active development baseline is now English-first.

Goals:

- keep the default runtime/UI flow in stable English
- leave a clean seam for later Korean reintroduction
- avoid mixing Korean-first renderer/layout tuning into the active baseline

Current switch point:

- `FrontierLedger.ini`
- `[Data] DefaultDataset=live_en`
- `[Data] PreferredLocale=en`

Korean-specific notes and example config live under `locales/ko/`.
