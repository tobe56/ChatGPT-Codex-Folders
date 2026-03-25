from __future__ import annotations

from typing import TypedDict


class ExportEntry(TypedDict):
    label: str
    detail: str


class ExportPage(TypedDict):
    page_id: str
    title: str
    summary: str
    entries: list[ExportEntry]


class CompactSection(TypedDict):
    title: str
    lines: list[str]


class ExportDataset(TypedDict):
    dataset_id: str
    display_name: str
    compact_sections: list[CompactSection]
    pages: list[ExportPage]
