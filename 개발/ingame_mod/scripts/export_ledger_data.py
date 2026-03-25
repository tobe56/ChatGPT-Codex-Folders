from __future__ import annotations

import argparse
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from ingame_mod.scripts.export_ledger_pages import build_pages
from ingame_mod.scripts.export_ledger_renderer import build_dataset, render_header


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate Frontier Ledger C data tables.")
    parser.add_argument("--output", required=True, help="Destination header path.")
    parser.add_argument("--strings-output", help="Destination strings.gxt2 path.")
    args = parser.parse_args()

    output_path = Path(args.output).resolve()
    output_path.parent.mkdir(parents=True, exist_ok=True)
    header_text, gxt_text = render_header(output_path)
    output_path.write_text(header_text, encoding="utf-8")
    if args.strings_output:
        strings_output_path = Path(args.strings_output).resolve()
        strings_output_path.parent.mkdir(parents=True, exist_ok=True)
        strings_output_path.write_text(gxt_text, encoding="utf-8")
    print(f"Wrote {output_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
