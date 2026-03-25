from __future__ import annotations

from pathlib import Path

from frontier_rpg.models import PlayerProfile
from frontier_rpg.systems import build_endgame_profile, create_baseline_player, simulate_first_ten_hours
from frontier_rpg.ui_state import build_compact_sections
from ingame_mod.scripts.export_ledger_localization import (
    STATIC_KOREAN_UI_LABELS,
    c_string,
    localize_compact_line,
    localize_text,
    make_label_id,
    sanitize_text,
    wrap_for_gxt,
)
from ingame_mod.scripts.export_ledger_pages import build_pages
from ingame_mod.scripts.export_ledger_types import CompactSection, ExportDataset, ExportPage


def build_dataset(dataset_id: str, display_name: str, player: PlayerProfile, language: str) -> ExportDataset:
    compact_sections: list[CompactSection] = []
    for title, lines in build_compact_sections(player).items():
        compact_sections.append(
            {
                "title": localize_text(title, language),
                "lines": [localize_compact_line(str(line), language) for line in lines],
            }
        )
    return {
        "dataset_id": dataset_id,
        "display_name": localize_text(display_name, language),
        "compact_sections": compact_sections,
        "pages": [
            {
                "page_id": page["page_id"],
                "title": localize_text(str(page["title"]), language),
                "summary": localize_text(str(page["summary"]), language),
                "entries": [
                    {
                        "label": localize_text(str(entry["label"]), language),
                        "detail": localize_text(str(entry["detail"]), language),
                    }
                    for entry in page["entries"]
                ],
            }
            for page in build_pages(player)
        ],
    }


def emit_compact_section(dataset_id: str, section_index: int, section: CompactSection, out: list[str]) -> str:
    lines_name = f"kCompact_{dataset_id}_{section_index}_Lines"
    lines = section["lines"]
    out.append(f"static const char *const {lines_name}[] = {{")
    for line_index, line in enumerate(lines):
        if dataset_id.endswith("_ko"):
            label = make_label_id("fl", dataset_id, "compact", section_index, "line", line_index)
            out.append(f"    {c_string(label)},")
        else:
            out.append(f"    {c_string(str(line))},")
    out.append("};")
    return lines_name


def emit_page_entries(dataset_id: str, page: ExportPage, out: list[str]) -> str:
    page_suffix = page["page_id"]
    entries_name = f"kEntries_{dataset_id}_{page_suffix}"
    out.append(f"static const FrontierLedgerEntry {entries_name}[] = {{")
    for entry_index, entry in enumerate(page["entries"]):
        if dataset_id.endswith("_ko"):
            label_ref = c_string(make_label_id("fl", dataset_id, page_suffix, "entry", entry_index, "label"))
            detail_ref = c_string(make_label_id("fl", dataset_id, page_suffix, "entry", entry_index, "detail"))
        else:
            label_ref = c_string(entry["label"])
            detail_ref = c_string(entry["detail"])
        out.append(
            "    { "
            f"{label_ref}, "
            f"{detail_ref} "
            "},"
        )
    out.append("};")
    return entries_name


def build_gxt_entries(datasets: list[ExportDataset]) -> list[tuple[str, str]]:
    entries: list[tuple[str, str]] = []

    for label, text in STATIC_KOREAN_UI_LABELS.items():
        entries.append((label, text))

    for dataset in datasets:
        dataset_id = str(dataset["dataset_id"])
        if not dataset_id.endswith("_ko"):
            continue

        entries.append(
            (
                make_label_id("fl", dataset_id, "display"),
                sanitize_text(str(dataset["display_name"])),
            )
        )

        for section_index, section in enumerate(dataset["compact_sections"]):
            entries.append(
                (
                    make_label_id("fl", dataset_id, "compact", section_index, "title"),
                    sanitize_text(str(section["title"])),
                )
            )
            for line_index, line in enumerate(section["lines"]):
                entries.append(
                    (
                        make_label_id("fl", dataset_id, "compact", section_index, "line", line_index),
                        wrap_for_gxt(str(line), 42, 3),
                    )
                )

        for page in dataset["pages"]:
            page_id = str(page["page_id"])
            entries.append((make_label_id("fl", dataset_id, page_id, "title"), sanitize_text(str(page["title"]))))
            entries.append((make_label_id("fl", dataset_id, page_id, "summary"), wrap_for_gxt(str(page["summary"]), 42, 3)))
            for entry_index, entry in enumerate(page["entries"]):
                entries.append(
                    (
                        make_label_id("fl", dataset_id, page_id, "entry", entry_index, "label"),
                        wrap_for_gxt(str(entry["label"]), 43, 3),
                    )
                )
                entries.append(
                    (
                        make_label_id("fl", dataset_id, page_id, "entry", entry_index, "detail"),
                        wrap_for_gxt(str(entry["detail"]), 44, 16),
                    )
                )

    deduped: dict[str, str] = {}
    for label, text in entries:
        deduped[label] = text
    return sorted(deduped.items())


def render_gxt2(datasets: list[ExportDataset]) -> str:
    lines = [f"{label} = {text}" for label, text in build_gxt_entries(datasets)]
    return "\n".join(lines) + "\n"


def _build_default_datasets() -> list[ExportDataset]:
    baseline = create_baseline_player()
    simulated, _ = simulate_first_ten_hours()
    endgame, _ = build_endgame_profile()
    return [
        build_dataset("baseline_en", "Baseline Frontier State", baseline, "en"),
        build_dataset("simulated_en", "Simulated Frontier Run", simulated, "en"),
        build_dataset("endgame_en", "After The Ledger", endgame, "en"),
        build_dataset("baseline_ko", "Baseline Frontier State", baseline, "ko"),
        build_dataset("simulated_ko", "Simulated Frontier Run", simulated, "ko"),
        build_dataset("endgame_ko", "After The Ledger", endgame, "ko"),
    ]


def render_header(output_path: Path) -> tuple[str, str]:
    _ = output_path
    datasets = _build_default_datasets()

    out: list[str] = [
        "/* Auto-generated by scripts/export_ledger_data.py. */",
        "#ifndef FRONTIER_LEDGER_GENERATED_H",
        "#define FRONTIER_LEDGER_GENERATED_H",
        "",
    ]

    for dataset in datasets:
        compact_section_names: list[str] = []
        for index, section in enumerate(dataset["compact_sections"]):
            lines_name = emit_compact_section(dataset["dataset_id"], index, section, out)
            compact_section_names.append(lines_name)

        compact_array_name = f"kCompactSections_{dataset['dataset_id']}"
        out.append(f"static const FrontierLedgerSection {compact_array_name}[] = {{")
        for section_index, (section, lines_name) in enumerate(zip(dataset["compact_sections"], compact_section_names, strict=True)):
            section_title = (
                c_string(make_label_id("fl", dataset["dataset_id"], "compact", section_index, "title"))
                if str(dataset["dataset_id"]).endswith("_ko")
                else c_string(section["title"])
            )
            out.append(
                "    { "
                f"{section_title}, "
                f"(int)(sizeof({lines_name}) / sizeof({lines_name}[0])), "
                f"{lines_name} "
                "},"
            )
        out.append("};")
        out.append("")

        page_entry_names: list[str] = []
        for page in dataset["pages"]:
            page_entry_names.append(emit_page_entries(dataset["dataset_id"], page, out))

        page_array_name = f"kPages_{dataset['dataset_id']}"
        out.append(f"static const FrontierLedgerPage {page_array_name}[] = {{")
        for page, entries_name in zip(dataset["pages"], page_entry_names, strict=True):
            if str(dataset["dataset_id"]).endswith("_ko"):
                page_title = c_string(make_label_id("fl", dataset["dataset_id"], page["page_id"], "title"))
                page_summary = c_string(make_label_id("fl", dataset["dataset_id"], page["page_id"], "summary"))
            else:
                page_title = c_string(page["title"])
                page_summary = c_string(page["summary"])
            out.append(
                "    { "
                f"{c_string(page['page_id'])}, "
                f"{page_title}, "
                f"{page_summary}, "
                f"(int)(sizeof({entries_name}) / sizeof({entries_name}[0])), "
                f"{entries_name} "
                "},"
            )
        out.append("};")
        out.append("")

    out.append("static const FrontierLedgerDataset kFrontierLedgerDatasets[] = {")
    for dataset in datasets:
        compact_array_name = f"kCompactSections_{dataset['dataset_id']}"
        page_array_name = f"kPages_{dataset['dataset_id']}"
        display_name = (
            c_string(make_label_id("fl", dataset["dataset_id"], "display"))
            if str(dataset["dataset_id"]).endswith("_ko")
            else c_string(dataset["display_name"])
        )
        out.append(
            "    { "
            f"{c_string(dataset['dataset_id'])}, "
            f"{display_name}, "
            f"(int)(sizeof({compact_array_name}) / sizeof({compact_array_name}[0])), "
            f"{compact_array_name}, "
            f"(int)(sizeof({page_array_name}) / sizeof({page_array_name}[0])), "
            f"{page_array_name} "
            "},"
        )
    out.append("};")
    out.append(
        "static const int kFrontierLedgerDatasetCount = "
        "(int)(sizeof(kFrontierLedgerDatasets) / sizeof(kFrontierLedgerDatasets[0]));"
    )
    out.append("")
    out.append("#endif")
    out.append("")
    return "\n".join(out), render_gxt2(datasets)
