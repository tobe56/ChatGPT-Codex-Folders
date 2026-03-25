from __future__ import annotations

import tkinter as tk
from tkinter import scrolledtext, ttk

from .platform.localization import ui_text

BACKGROUND = "#e3d8c4"
PANEL = "#f3ead6"
ACCENT = "#4b2f1b"
ACCENT_LIGHT = "#8a684b"
TEXT = "#241711"
MUTED = "#6a5444"

class LedgerTableFrame(ttk.Frame):
    def __init__(self, parent: tk.Misc, title: str, columns: list[tuple[str, str, int]]) -> None:
        super().__init__(parent, style="Ledger.TFrame")
        self.column_ids = [column_id for column_id, _, _ in columns]
        self._details: dict[str, str] = {}

        self.title_label = ttk.Label(self, text=title, style="LedgerTitle.TLabel")
        self.title_label.pack(anchor="w", padx=16, pady=(12, 8))

        table_wrap = ttk.Frame(self, style="Ledger.TFrame")
        table_wrap.pack(fill="both", expand=True, padx=16, pady=(0, 16))

        self.tree = ttk.Treeview(table_wrap, columns=self.column_ids, show="headings", height=12)
        self.tree.grid(row=0, column=0, sticky="nsew")
        scrollbar = ttk.Scrollbar(table_wrap, orient="vertical", command=self.tree.yview)
        scrollbar.grid(row=0, column=1, sticky="ns")
        self.tree.configure(yscrollcommand=scrollbar.set)
        table_wrap.columnconfigure(0, weight=1)
        table_wrap.rowconfigure(0, weight=3)

        for column_id, heading, width in columns:
            self.tree.heading(column_id, text=heading)
            self.tree.column(column_id, width=width, anchor="w")

        self.detail_label = ttk.Label(table_wrap, text="Details", style="LedgerSection.TLabel")
        self.detail_label.grid(row=1, column=0, sticky="w", pady=(10, 6))
        self.detail = scrolledtext.ScrolledText(
            table_wrap,
            height=8,
            wrap="word",
            bg=PANEL,
            fg=TEXT,
            insertbackground=TEXT,
            relief="flat",
            font=("Georgia", 11),
        )
        self.detail.grid(row=2, column=0, columnspan=2, sticky="nsew")
        table_wrap.rowconfigure(2, weight=2)
        self.detail.configure(state="disabled")
        self.tree.bind("<<TreeviewSelect>>", self._on_select)

    def set_rows(self, rows: list[dict[str, str]]) -> None:
        for item_id in self.tree.get_children():
            self.tree.delete(item_id)
        self._details.clear()
        for row in rows:
            values = [row.get(column_id, "") for column_id in self.column_ids]
            item_id = self.tree.insert("", "end", values=values)
            self._details[item_id] = row.get("detail", "")
        if rows:
            first_item = self.tree.get_children()[0]
            self.tree.selection_set(first_item)
            self.tree.focus(first_item)
            self._set_detail(self._details[first_item])
        else:
            self._set_detail("No entries available.")

    def _on_select(self, _event: tk.Event[tk.Misc]) -> None:
        selection = self.tree.selection()
        if not selection:
            return
        self._set_detail(self._details.get(selection[0], ""))

    def _set_detail(self, text: str) -> None:
        self.detail.configure(state="normal")
        self.detail.delete("1.0", tk.END)
        self.detail.insert("1.0", text)
        self.detail.configure(state="disabled")

    def set_labels(self, title: str, detail_label: str) -> None:
        self.title_label.configure(text=title)
        self.detail_label.configure(text=detail_label)


class CompactLedgerWindow(tk.Toplevel):
    def __init__(self, master: tk.Misc, locale: str = "en") -> None:
        super().__init__(master)
        self.current_locale = locale
        self.title(ui_text(locale, "compact_title"))
        self.geometry("520x480")
        self.configure(bg=BACKGROUND)
        self.resizable(False, False)
        self.attributes("-topmost", True)
        self.protocol("WM_DELETE_WINDOW", self.hide)

        self.header_label = ttk.Label(
            self,
            text=ui_text(locale, "compact_title"),
            style="LedgerTitle.TLabel",
        )
        self.header_label.pack(anchor="w", padx=16, pady=(16, 8))

        self._section_frames: dict[str, ttk.LabelFrame] = {}
        self._section_labels: dict[str, tk.Label] = {}
        for key in ("compact_section_frontier", "compact_section_intel", "compact_section_ready"):
            frame = ttk.LabelFrame(self, text=ui_text(locale, key), style="LedgerCard.TLabelframe")
            frame.pack(fill="x", padx=16, pady=8)
            label = tk.Label(
                frame,
                justify="left",
                anchor="w",
                bg=PANEL,
                fg=TEXT,
                wraplength=460,
                font=("Georgia", 11),
            )
            label.pack(fill="x", padx=12, pady=10)
            self._section_frames[key] = frame
            self._section_labels[key] = label

        self.hint_label = tk.Label(
            self,
            text=ui_text(locale, "compact_hint"),
            bg=BACKGROUND,
            fg=MUTED,
            font=("Georgia", 10, "italic"),
        )
        self.hint_label.pack(anchor="w", padx=16, pady=(8, 0))

    def refresh(self, sections: dict[str, list[str]]) -> None:
        for key, label in self._section_labels.items():
            mapped_key = {
                "compact_section_frontier": "Frontier Status",
                "compact_section_intel": "Quick Intel",
                "compact_section_ready": "Ready Now",
            }[key]
            label.configure(text="\n".join(sections.get(mapped_key, [])))

    def set_locale(self, locale: str) -> None:
        self.current_locale = locale
        self.title(ui_text(locale, "compact_title"))
        self.header_label.configure(text=ui_text(locale, "compact_title"))
        self.hint_label.configure(text=ui_text(locale, "compact_hint"))
        for key, frame in self._section_frames.items():
            frame.configure(text=ui_text(locale, key))

    def hide(self) -> None:
        self.withdraw()


