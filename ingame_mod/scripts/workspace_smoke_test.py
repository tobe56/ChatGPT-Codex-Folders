from __future__ import annotations

import argparse
import configparser
import ctypes
import shutil
import sys
import tempfile
import time
from pathlib import Path


def build_temp_runtime(
    project_root: Path,
    seeded_phase: int | None = None,
    seeded_trail_stage: int | None = None,
    profile_overrides: dict[str, str] | None = None,
    dataset_id: str = "live_en",
    preferred_locale: str | None = None,
) -> Path:
    build_asi = project_root / "build" / "FrontierLedger.asi"
    source_ini = project_root / "FrontierLedger.ini"

    if not build_asi.exists():
        raise FileNotFoundError(f"Missing build output: {build_asi}")
    if not source_ini.exists():
        raise FileNotFoundError(f"Missing source ini: {source_ini}")

    runtime_dir = Path(tempfile.mkdtemp(prefix="frontier_smoke_", dir=str(project_root)))
    shutil.copy2(build_asi, runtime_dir / "FrontierLedger.asi")
    shutil.copy2(source_ini, runtime_dir / "FrontierLedger.ini")

    config = configparser.ConfigParser()
    config.read(runtime_dir / "FrontierLedger.ini", encoding="utf-8")
    config.setdefault("Safety", {})
    config.setdefault("Renderer", {})
    config.setdefault("Data", {})
    resolved_locale = preferred_locale or ("ko" if dataset_id.endswith("_ko") else "en")
    config["Safety"]["BootstrapPollMs"] = "100"
    config["Safety"]["BootstrapTimeoutMs"] = "1000"
    config["Safety"]["ShowReadyToast"] = "0"
    config["Renderer"]["UseDesktopOverlay"] = "0"
    config["Data"]["DefaultDataset"] = dataset_id
    config["Data"]["PreferredLocale"] = resolved_locale
    with (runtime_dir / "FrontierLedger.ini").open("w", encoding="utf-8") as handle:
        config.write(handle)

    if seeded_phase is not None:
        profile = configparser.ConfigParser()
        profile_data = {
            "Phase": str(seeded_phase),
            "TrailStage": str(seeded_trail_stage if seeded_trail_stage is not None else 0),
            "DiscoveredRegionMask": "63",
            "HubReturns": "4",
            "EnemyKills": "6",
            "LootCount": "5",
            "AnimalKills": "3",
            "RewardEarned": "140",
            "CraftedMask": "3",
            "FieldDistanceTotal": "9000",
            "FieldDistanceSinceHub": "0",
        }
        if profile_overrides:
            profile_data.update(profile_overrides)
        profile["Run"] = profile_data
        with (runtime_dir / "FrontierLedger.profile.ini").open("w", encoding="utf-8") as handle:
            profile.write(handle)

    return runtime_dir


def run_smoke(runtime_dir: Path, expected_dataset_fragment: str, expected_requested_dataset: str) -> tuple[str, str, str]:
    asi_path = runtime_dir / "FrontierLedger.asi"
    log_path = runtime_dir / "FrontierLedger.log"
    session_path = runtime_dir / "FrontierLedger.session.ini"
    profile_path = runtime_dir / "FrontierLedger.profile.ini"

    dll = ctypes.WinDLL(str(asi_path))
    try:
        time.sleep(1.6)
    finally:
        ctypes.windll.kernel32.FreeLibrary(ctypes.c_void_p(dll._handle))
        del dll
        time.sleep(0.2)

    if not log_path.exists():
        raise RuntimeError("Smoke runtime never produced FrontierLedger.log")
    if not session_path.exists():
        raise RuntimeError("Smoke runtime never produced FrontierLedger.session.ini")
    if not profile_path.exists():
        raise RuntimeError("Smoke runtime never produced FrontierLedger.profile.ini")

    log_text = log_path.read_text(encoding="utf-8", errors="replace")
    session_text = session_path.read_text(encoding="utf-8", errors="replace")
    profile_text = profile_path.read_text(encoding="utf-8", errors="replace")

    required_log_fragments = (
        "Process attach.",
        f"RequestedDataset={expected_requested_dataset}",
        f"Dataset={expected_dataset_fragment}",
        "Spawned bootstrap thread.",
        "ScriptHook binding timed out after",
        "Process detach.",
    )
    for fragment in required_log_fragments:
        if fragment not in log_text:
            raise RuntimeError(f"Smoke log missing fragment: {fragment}")

    if f"dataset={expected_requested_dataset}".lower() not in session_text.lower():
        raise RuntimeError("Session file did not persist the live dataset selection")
    lowered_session = session_text.lower()
    if "tab" not in lowered_session:
        raise RuntimeError("Session file did not persist the workbench tab")
    if "entrycraft" not in lowered_session:
        raise RuntimeError("Session file did not persist the workbench craft selection")
    if "entryarchive" not in lowered_session:
        raise RuntimeError("Session file did not persist the workbench archive selection")
    if "entryresult" not in lowered_session:
        raise RuntimeError("Session file did not persist the workbench result selection")
    lowered_profile = profile_text.lower()
    if "phase" not in lowered_profile:
        raise RuntimeError("Profile file did not persist the live phase")
    if "trailstage" not in lowered_profile:
        raise RuntimeError("Profile file did not persist the trail stage")
    if "enemykills" not in lowered_profile:
        raise RuntimeError("Profile file did not persist enemy kill tracking")
    if "lootcount" not in lowered_profile:
        raise RuntimeError("Profile file did not persist loot tracking")
    if "animalkills" not in lowered_profile:
        raise RuntimeError("Profile file did not persist hunting tracking")
    if "rewardearned" not in lowered_profile:
        raise RuntimeError("Profile file did not persist reward tracking")
    if "craftedmask" not in lowered_profile:
        raise RuntimeError("Profile file did not persist crafted recipe tracking")

    return log_text, session_text, profile_text


def main() -> int:
    parser = argparse.ArgumentParser(description="Run an isolated workspace smoke test for FrontierLedger.asi.")
    parser.add_argument("--keep-runtime", action="store_true", help="Keep the temporary runtime directory after the smoke test.")
    args = parser.parse_args()

    project_root = Path(__file__).resolve().parents[1]
    runtime_specs = (
        ("baseline", None, None, "baseline_en", None, "live_en", "en"),
        ("baseline_ko", None, None, "baseline_ko", None, "live_ko", "ko"),
        ("endgame", 2, 5, "endgame_en", None, "live_en", "en"),
        (
            "corrupt",
            9,
            99,
            "baseline_en",
            {
                "Phase": "-5",
                "TrailStage": "999",
                "DiscoveredRegionMask": "4096",
                "HubReturns": "-99",
                "EnemyKills": "-3",
                "LootCount": "-7",
                "AnimalKills": "-11",
                "RewardEarned": "-44",
                "CraftedMask": "9999",
                "FieldDistanceTotal": "-1000",
                "FieldDistanceSinceHub": "-250",
            },
            "live_en",
            "en",
        ),
    )

    kept_runtime = None
    for name, seeded_phase, seeded_trail_stage, expected_dataset, profile_overrides, dataset_id, preferred_locale in runtime_specs:
        runtime_dir = build_temp_runtime(
            project_root,
            seeded_phase=seeded_phase,
            seeded_trail_stage=seeded_trail_stage,
            profile_overrides=profile_overrides,
            dataset_id=dataset_id,
            preferred_locale=preferred_locale,
        )
        try:
            log_text, session_text, profile_text = run_smoke(runtime_dir, expected_dataset, dataset_id)
            if name == "corrupt":
                lowered = profile_text.lower().replace(" ", "")
                if "phase=0" not in lowered:
                    raise RuntimeError("Corrupt profile was not clamped back to Phase=0")
                if "trailstage=5" not in lowered:
                    raise RuntimeError("Corrupt profile was not clamped back to TrailStage=5")
                if "hubreturns=0" not in lowered:
                    raise RuntimeError("Corrupt profile did not clamp negative hub returns")
                if "craftedmask=8191" not in lowered:
                    raise RuntimeError("Corrupt profile did not clamp CraftedMask into the supported range")
            print(f"WORKSPACE_SMOKE_{name.upper()}_OK")
            print(f"Runtime={runtime_dir}")
            print(f"LogTail={log_text.strip().splitlines()[-1] if log_text.strip() else 'EMPTY'}")
            print(f"SessionDataset={f'dataset={dataset_id}'.lower() in session_text.lower()}")
            print(f"ProfilePhase={'phase' in profile_text.lower()}")
            print(f"TrailStage={'trailstage' in profile_text.lower()}")
            print(f"EnemyKills={'enemykills' in profile_text.lower()}")
            print(f"LootCount={'lootcount' in profile_text.lower()}")
            print(f"AnimalKills={'animalkills' in profile_text.lower()}")
            print(f"RewardEarned={'rewardearned' in profile_text.lower()}")
            print(f"CraftedMask={'craftedmask' in profile_text.lower()}")
            if args.keep_runtime:
                kept_runtime = runtime_dir
            else:
                shutil.rmtree(runtime_dir, ignore_errors=True)
        finally:
            if not args.keep_runtime and runtime_dir.exists():
                shutil.rmtree(runtime_dir, ignore_errors=True)

    if kept_runtime is not None:
        print(f"KeptRuntime={kept_runtime}")
    print("WORKSPACE_SMOKE_OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
