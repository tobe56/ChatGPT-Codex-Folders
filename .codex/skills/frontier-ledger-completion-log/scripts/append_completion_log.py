from __future__ import annotations

import argparse
from datetime import datetime
from pathlib import Path


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Append a Frontier Ledger task completion log entry.")
    parser.add_argument("--project-root", help="Project root. Defaults to the repository root inferred from the script path.")
    parser.add_argument("--timestamp", help="Timestamp for the entry. Defaults to current local time.")
    parser.add_argument("--title", required=True, help="Short title for the completed task.")
    parser.add_argument("--request", required=True, help="What the user asked for.")
    parser.add_argument("--result", required=True, help="What was delivered.")
    parser.add_argument("--skills", nargs="+", required=True, help="Skills used for the task.")
    parser.add_argument("--files", nargs="*", default=[], help="Key files touched.")
    parser.add_argument("--validation", nargs="*", default=[], help="Validation steps run.")
    parser.add_argument("--risk", default="None noted.", help="Residual risk or follow-up note.")
    return parser


def normalize_root(project_root: str | None) -> Path:
    if project_root:
        return Path(project_root).resolve()
    return Path(__file__).resolve().parents[4]


def ensure_log_header(path: Path) -> None:
    if path.exists():
        return
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("# Task Completion Log\n\n", encoding="utf-8")


def append_date_header_if_needed(path: Path, date_label: str) -> None:
    text = path.read_text(encoding="utf-8")
    heading = f"## {date_label}\n"
    if heading not in text:
        with path.open("a", encoding="utf-8") as handle:
            if not text.endswith("\n\n"):
                handle.write("\n")
            handle.write(f"{heading}\n")


def bulletize(items: list[str], empty_fallback: str) -> str:
    values = [item.strip() for item in items if item.strip()]
    if not values:
        values = [empty_fallback]
    return "\n".join(f"- {item}" for item in values)


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()

    project_root = normalize_root(args.project_root)
    log_path = project_root / "개발" / "TASK_COMPLETION_LOG.md"
    ensure_log_header(log_path)

    timestamp = args.timestamp or datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    date_label = timestamp.split(" ")[0]
    append_date_header_if_needed(log_path, date_label)

    entry_lines = [
        f"### {timestamp} | {args.title}",
        f"- Request: {args.request.strip()}",
        f"- Result: {args.result.strip()}",
        "- Skills used:",
        bulletize(args.skills, "None"),
        "- Files touched:",
        bulletize(args.files, "analysis only"),
        "- Validation:",
        bulletize(args.validation, "not run"),
        f"- Residual risk: {args.risk.strip()}",
        "",
    ]

    with log_path.open("a", encoding="utf-8") as handle:
        handle.write("\n".join(entry_lines))

    print(log_path)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
