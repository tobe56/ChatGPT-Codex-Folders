# RDR2 RPG MODS Workspace

This repository is the full working workspace for the Frontier Ledger / RDR2 RPG mod project.

It includes:

- the active development project under `개발/`
- backup archives under `개발 백업/`
- comparison snapshots such as `_compare_dev27/`
- design and memory documents
- media, quarantine, and helper folders used during development

## Important Clone Notes

This repository uses Git LFS for large assets such as `.zip`, `.mp4`, and `.exe`.

Clone with:

```bash
git clone --recurse-submodules https://github.com/tobe56/ChatGPT-Codex-Folders.git
cd ChatGPT-Codex-Folders
git lfs pull
```

If you already cloned without submodules or LFS:

```bash
git submodule update --init --recursive
git lfs pull
```

## Main Working Folder

The primary development entry point is:

- `개발/`

Inside that folder:

- `frontier_rpg/` contains the Python prototype and data-driven gameplay validation
- `ingame_mod/` contains the native ScriptHookRDR2 mod project
- `tests/` contains regression tests
- `TASK_COMPLETION_LOG.md` records completed work

## Workspace Layout

- `개발/`: current source of truth for the active project
- `개발 백업/`: archived ZIP backups of development milestones
- `_compare_dev27/`: comparison workspace used for side-by-side inspection
- `기획문서/`: design and planning documents
- `Memory/`: reusable project memory and troubleshooting notes
- `이미지 및 동영상/`: captured reference videos and images
- `game_folder_quarantine/`, `게임폴더_격리/`: isolated game-folder related materials
- `작업보조_외부보관/`: helper assets and temporary external working data

## Development Notes

- Large binary assets are intentionally preserved here because this repository is also being used as a workspace backup.
- `개발/tools/ScriptHookRDR2DotNet-V2` and `_compare_dev27/개발/tools/ScriptHookRDR2DotNet-V2` are tracked as submodules.
- The root repository is broader than the gameplay codebase; day-to-day coding should normally start in `개발/`.

## Quick Start

For the active project overview, see:

- `개발/README.md`

