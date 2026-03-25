from __future__ import annotations

import tkinter as tk
from tkinter import scrolledtext, ttk

from .platform.localization import ui_text
from .systems import build_endgame_profile, create_baseline_player, simulate_first_ten_hours
from .ui_state import (
    build_affinity_table_rows,
    build_armory_table_rows,
    build_collection_table_rows,
    build_compact_sections,
    build_crafting_rows,
    build_dead_ledger_rows,
    build_encyclopedia_rows,
    build_ending_table_rows,
    build_farming_table_rows,
    build_field_overview_lines,
    build_frontier_map_table_rows,
    build_gear_table_rows,
    build_header_state,
    build_homefront_rows,
    build_network_rows,
    build_perk_rows,
    build_skill_rows,
    build_stats_rows,
    build_trail_rows,
    build_upgrade_rows,
    build_world_state_table_rows,
)
from .ui_widgets import (
    ACCENT,
    BACKGROUND,
    MUTED,
    PANEL,
    TEXT,
    CompactLedgerWindow,
    LedgerTableFrame,
)


class FrontierLedgerApp:
    def __init__(self, player=None) -> None:
        self.player = player or create_baseline_player()
        self.locale = self.player.preferred_locale
        self.root = tk.Tk()
        self.root.title(ui_text(self.locale, "app_title"))
        self.root.geometry("1440x960")
        self.root.configure(bg=BACKGROUND)
        self.compact_window: CompactLedgerWindow | None = None

        self._configure_styles()
        self._build_layout()
        self.root.bind("<F10>", self._on_f10)
        self.refresh_views()

    def _configure_styles(self) -> None:
        style = ttk.Style(self.root)
        style.theme_use("clam")
        style.configure("Ledger.TFrame", background=BACKGROUND)
        style.configure("LedgerCard.TFrame", background=PANEL)
        style.configure("LedgerTitle.TLabel", background=BACKGROUND, foreground=ACCENT, font=("Georgia", 22, "bold"))
        style.configure("LedgerSub.TLabel", background=BACKGROUND, foreground=MUTED, font=("Georgia", 11))
        style.configure("LedgerSection.TLabel", background=BACKGROUND, foreground=ACCENT, font=("Georgia", 13, "bold"))
        style.configure("LedgerValue.TLabel", background=PANEL, foreground=TEXT, font=("Georgia", 12))
        style.configure("LedgerButton.TButton", font=("Georgia", 11, "bold"))
        style.configure("LedgerNotebook.TNotebook", background=BACKGROUND, tabmargins=[8, 4, 8, 0])
        style.configure("LedgerNotebook.TNotebook.Tab", font=("Georgia", 11, "bold"), padding=[16, 10])
        style.configure("LedgerCard.TLabelframe", background=PANEL, foreground=ACCENT, font=("Georgia", 12, "bold"))
        style.configure("LedgerCard.TLabelframe.Label", background=PANEL, foreground=ACCENT)

    def _build_layout(self) -> None:
        shell = ttk.Frame(self.root, style="Ledger.TFrame")
        shell.pack(fill="both", expand=True, padx=20, pady=20)

        header = ttk.Frame(shell, style="Ledger.TFrame")
        header.pack(fill="x", pady=(0, 14))

        title_wrap = ttk.Frame(header, style="Ledger.TFrame")
        title_wrap.pack(side="left", fill="y")
        self.header_title_label = ttk.Label(title_wrap, text=ui_text(self.locale, "header_title"), style="LedgerTitle.TLabel")
        self.header_title_label.pack(anchor="w")
        self.header_subtitle_label = ttk.Label(
            title_wrap,
            text=ui_text(self.locale, "header_subtitle"),
            style="LedgerSub.TLabel",
        )
        self.header_subtitle_label.pack(anchor="w", pady=(4, 0))

        actions = ttk.Frame(header, style="Ledger.TFrame")
        actions.pack(side="right")
        self.compact_button = ttk.Button(actions, text="", command=self.toggle_compact_ledger, style="LedgerButton.TButton")
        self.compact_button.grid(
            row=0, column=0, padx=6
        )
        self.full_button = ttk.Button(actions, text="", command=self.show_full_ledger, style="LedgerButton.TButton")
        self.full_button.grid(
            row=0, column=1, padx=6
        )
        self.field_button = ttk.Button(actions, text="", command=self.show_field_view, style="LedgerButton.TButton")
        self.field_button.grid(
            row=0, column=2, padx=6
        )
        self.sim_button = ttk.Button(actions, text="", command=self.load_simulated_profile, style="LedgerButton.TButton")
        self.sim_button.grid(
            row=0, column=3, padx=6
        )
        self.endgame_button = ttk.Button(actions, text="", command=self.load_endgame_profile, style="LedgerButton.TButton")
        self.endgame_button.grid(
            row=0, column=4, padx=6
        )
        self.reset_button = ttk.Button(actions, text="", command=self.reset_profile, style="LedgerButton.TButton")
        self.reset_button.grid(
            row=0, column=5, padx=6
        )
        self.locale_en_button = ttk.Button(actions, text="", command=lambda: self.set_locale("en"), style="LedgerButton.TButton")
        self.locale_en_button.grid(row=0, column=6, padx=6)
        self.locale_ko_button = ttk.Button(actions, text="", command=lambda: self.set_locale("ko"), style="LedgerButton.TButton")
        self.locale_ko_button.grid(row=0, column=7, padx=6)

        self.status_cards = ttk.Frame(shell, style="Ledger.TFrame")
        self.status_cards.pack(fill="x", pady=(0, 16))
        self._card_labels: dict[str, ttk.Label] = {}
        for title in ("project", "level", "world_tier", "home"):
            card = ttk.Frame(self.status_cards, style="LedgerCard.TFrame")
            card.pack(side="left", fill="x", expand=True, padx=6)
            ttk.Label(card, text=title.replace("_", " ").title(), style="LedgerSection.TLabel").pack(
                anchor="w", padx=12, pady=(10, 4)
            )
            value = ttk.Label(card, text="", style="LedgerValue.TLabel")
            value.pack(anchor="w", padx=12, pady=(0, 12))
            self._card_labels[title] = value

        self.content_frame = ttk.Frame(shell, style="Ledger.TFrame")
        self.content_frame.pack(fill="both", expand=True)

        self.field_frame = ttk.Frame(self.content_frame, style="Ledger.TFrame")
        self.field_frame.pack(fill="both", expand=True)
        self.field_text = scrolledtext.ScrolledText(
            self.field_frame,
            wrap="word",
            bg=PANEL,
            fg=TEXT,
            insertbackground=TEXT,
            relief="flat",
            font=("Georgia", 12),
        )
        self.field_text.pack(fill="both", expand=True)
        self.field_text.configure(state="disabled")

        self.full_frame = ttk.Frame(self.content_frame, style="Ledger.TFrame")
        self.full_notebook = ttk.Notebook(self.full_frame, style="LedgerNotebook.TNotebook")
        self.full_notebook.pack(fill="both", expand=True)

        self.overview_tab = ttk.Frame(self.full_notebook, style="Ledger.TFrame")
        self.overview_text = scrolledtext.ScrolledText(
            self.overview_tab,
            wrap="word",
            bg=PANEL,
            fg=TEXT,
            insertbackground=TEXT,
            relief="flat",
            font=("Georgia", 12),
        )
        self.overview_text.pack(fill="both", expand=True, padx=16, pady=16)
        self.overview_text.configure(state="disabled")
        self.full_notebook.add(self.overview_tab, text="")

        self.trails_tab = LedgerTableFrame(
            self.full_notebook,
            "Active Trails",
            [("category", "Category", 160), ("trail", "Trail", 260), ("state", "State", 120), ("region", "Region", 260)],
        )
        self.full_notebook.add(self.trails_tab, text="")

        self.map_tab = LedgerTableFrame(
            self.full_notebook,
            "Frontier Map",
            [("region", "Region", 220), ("state", "State", 120), ("danger", "Danger", 320), ("focus", "Focus", 240)],
        )
        self.full_notebook.add(self.map_tab, text="")

        self.homefront_tab = LedgerTableFrame(
            self.full_notebook,
            "Homefront",
            [("member", "Family", 140), ("focus", "Focus", 160), ("request", "Request", 420), ("state", "State", 120)],
        )
        self.full_notebook.add(self.homefront_tab, text="")

        self.affinity_tab = LedgerTableFrame(
            self.full_notebook,
            "Affinity",
            [
                ("npc", "NPC", 150),
                ("stage", "Stage", 120),
                ("score", "Score", 90),
                ("quest_state", "Quest State", 140),
                ("repeatable_bonus", "Repeatable Bonus", 150),
            ],
        )
        self.full_notebook.add(self.affinity_tab, text="")

        self.farming_tab = LedgerTableFrame(
            self.full_notebook,
            "Farming Lanes",
            [
                ("lane", "Lane", 170),
                ("owner", "Owner", 110),
                ("tier", "Tier", 110),
                ("next_reward", "Next Reward", 220),
                ("state", "State", 110),
            ],
        )
        self.full_notebook.add(self.farming_tab, text="")

        self.network_tab = LedgerTableFrame(
            self.full_notebook,
            "Frontier Network",
            [
                ("system", "System", 190),
                ("owner", "Owner", 110),
                ("progress", "Progress", 90),
                ("milestone", "Milestone", 220),
                ("state", "State", 110),
            ],
        )
        self.full_notebook.add(self.network_tab, text="")

        self.world_state_tab = LedgerTableFrame(
            self.full_notebook,
            "World State",
            [
                ("tier", "Tier", 90),
                ("title", "Title", 170),
                ("state", "State", 100),
                ("package_state", "Package", 100),
                ("package", "Unlock Package", 240),
                ("checkpoint", "Checkpoint", 250),
            ],
        )
        self.full_notebook.add(self.world_state_tab, text="")

        self.ending_tab = LedgerTableFrame(
            self.full_notebook,
            "Ending Route",
            [
                ("phase", "Phase", 210),
                ("type", "Type", 110),
                ("reward_state", "Reward", 90),
                ("reward_package", "Reward Package", 230),
                ("state", "State", 90),
            ],
        )
        self.full_notebook.add(self.ending_tab, text="")

        self.stats_tab = LedgerTableFrame(
            self.full_notebook,
            "Stats",
            [("stat", "Stat", 140), ("value", "Value", 100), ("focus", "Gameplay Focus", 420)],
        )
        self.full_notebook.add(self.stats_tab, text="")

        self.gear_tab = LedgerTableFrame(
            self.full_notebook,
            "Gear",
            [("slot", "Slot", 180), ("item", "Item", 240), ("category", "Category", 160), ("state", "State", 120)],
        )
        self.full_notebook.add(self.gear_tab, text="")

        self.armory_tab = LedgerTableFrame(
            self.full_notebook,
            "Armory",
            [("weapon", "Weapon", 240), ("category", "Category", 120), ("condition", "Condition", 140), ("familiarity", "Familiarity", 120)],
        )
        self.full_notebook.add(self.armory_tab, text="")

        self.skills_tab = LedgerTableFrame(
            self.full_notebook,
            "Skills",
            [("skill", "Skill", 180), ("rank", "Rank", 100), ("focus", "Use", 420)],
        )
        self.full_notebook.add(self.skills_tab, text="")

        self.perks_tab = LedgerTableFrame(
            self.full_notebook,
            "Perks",
            [("tree", "Tree", 160), ("perk", "Perk", 220), ("state", "State", 120), ("requirement", "Requirement", 420)],
        )
        self.full_notebook.add(self.perks_tab, text="")

        self.crafting_tab = LedgerTableFrame(
            self.full_notebook,
            "Crafting Table",
            [("recipe", "Recipe", 220), ("station", "Station", 160), ("tier", "Tier", 80), ("status", "Status", 120)],
        )
        self.full_notebook.add(self.crafting_tab, text="")

        self.dead_tab = LedgerTableFrame(
            self.full_notebook,
            "Ledger of the Dead",
            [("boss", "Entry", 260), ("region", "Region", 220), ("rank", "Rank", 140), ("state", "State", 120)],
        )
        self.full_notebook.add(self.dead_tab, text="")

        self.collection_tab = LedgerTableFrame(
            self.full_notebook,
            "Collection",
            [("collection", "Collection", 240), ("category", "Category", 160), ("progress", "Progress", 120), ("state", "State", 120)],
        )
        self.full_notebook.add(self.collection_tab, text="")

        self.upgrades_tab = LedgerTableFrame(
            self.full_notebook,
            "Beecher's Hope",
            [("category", "Category", 180), ("upgrade", "Upgrade", 220), ("effect", "Effect", 360), ("state", "State", 120)],
        )
        self.full_notebook.add(self.upgrades_tab, text="")

        self.encyclopedia_tab = LedgerTableFrame(
            self.full_notebook,
            "Encyclopedia",
            [("category", "Category", 180), ("entry", "Entry", 240), ("status", "Status", 140)],
        )
        self.full_notebook.add(self.encyclopedia_tab, text="")
        self._refresh_locale_text()

    def refresh_views(self) -> None:
        self._refresh_locale_text()
        header = build_header_state(self.player)
        for key, label in self._card_labels.items():
            label.configure(text=header[key])
        self._set_text(self.field_text, "\n".join(build_field_overview_lines(self.player)))
        overview_lines = [
            f"{header['project']}",
            f"Hub: {header['hub']}",
            f"{header['level']} | {header['xp']}",
            f"{header['world_tier']}",
            f"{header['home']}",
            "",
            "This prototype now prioritizes doc-led systems instead of vanilla HUD duplication.",
            "The compact ledger stays field-light while the house ledger tracks real frontier progression state.",
            "",
            "Current implementation focus:",
            "- Active Trails",
            "- Frontier Map",
            "- Homefront and family support",
            "- Affinity stage quests and relationship growth",
            "- World loot routes and stash flow",
            "- Farming lanes and frontier network",
            "- World escalation and ending route",
            "- Gear and Armory",
            "- Ledger of the Dead",
            "- Collection tracking",
            "- Beecher's Hope hub progression",
            "- Perks",
            "- Crafting and workshop routing",
            "- Stats and lifestyle support",
            "- English / Korean UI shell draft",
        ]
        self._set_text(self.overview_text, "\n".join(overview_lines))
        self.trails_tab.set_rows(build_trail_rows(self.player))
        self.map_tab.set_rows(build_frontier_map_table_rows(self.player))
        self.homefront_tab.set_rows(build_homefront_rows(self.player))
        self.affinity_tab.set_rows(build_affinity_table_rows(self.player))
        self.farming_tab.set_rows(build_farming_table_rows(self.player))
        self.network_tab.set_rows(build_network_rows(self.player))
        self.world_state_tab.set_rows(build_world_state_table_rows(self.player))
        self.ending_tab.set_rows(build_ending_table_rows(self.player))
        self.stats_tab.set_rows(build_stats_rows(self.player))
        self.gear_tab.set_rows(build_gear_table_rows(self.player))
        self.armory_tab.set_rows(build_armory_table_rows(self.player))
        self.skills_tab.set_rows(build_skill_rows(self.player))
        self.perks_tab.set_rows(build_perk_rows(self.player))
        self.crafting_tab.set_rows(build_crafting_rows(self.player))
        self.dead_tab.set_rows(build_dead_ledger_rows(self.player))
        self.collection_tab.set_rows(build_collection_table_rows(self.player))
        self.upgrades_tab.set_rows(build_upgrade_rows(self.player))
        self.encyclopedia_tab.set_rows(build_encyclopedia_rows(self.player))
        if self.compact_window is not None:
            self.compact_window.set_locale(self.locale)
            self.compact_window.refresh(build_compact_sections(self.player))

    def show_full_ledger(self) -> None:
        self.field_frame.pack_forget()
        self.full_frame.pack(fill="both", expand=True)

    def show_field_view(self) -> None:
        self.full_frame.pack_forget()
        self.field_frame.pack(fill="both", expand=True)

    def toggle_compact_ledger(self) -> None:
        if self.compact_window is None or not self.compact_window.winfo_exists():
            self.compact_window = CompactLedgerWindow(self.root, locale=self.locale)
            self.compact_window.refresh(build_compact_sections(self.player))
            return
        if self.compact_window.state() == "withdrawn":
            self.compact_window.deiconify()
            self.compact_window.set_locale(self.locale)
            self.compact_window.refresh(build_compact_sections(self.player))
        else:
            self.compact_window.withdraw()

    def load_simulated_profile(self) -> None:
        self.player = simulate_first_ten_hours()[0]
        self.player.preferred_locale = self.locale
        self.refresh_views()
        self.show_full_ledger()

    def load_endgame_profile(self) -> None:
        self.player = build_endgame_profile()[0]
        self.player.preferred_locale = self.locale
        self.refresh_views()
        self.show_full_ledger()

    def reset_profile(self) -> None:
        self.player = create_baseline_player()
        self.player.preferred_locale = self.locale
        self.refresh_views()
        self.show_field_view()

    def _on_f10(self, _event: tk.Event[tk.Misc]) -> None:
        self.toggle_compact_ledger()

    def _set_text(self, widget: scrolledtext.ScrolledText, text: str) -> None:
        widget.configure(state="normal")
        widget.delete("1.0", tk.END)
        widget.insert("1.0", text)
        widget.configure(state="disabled")

    def set_locale(self, locale: str) -> None:
        self.locale = locale
        self.player.preferred_locale = locale
        self.refresh_views()

    def _refresh_locale_text(self) -> None:
        self.root.title(ui_text(self.locale, "app_title"))
        self.header_title_label.configure(text=ui_text(self.locale, "header_title"))
        self.header_subtitle_label.configure(text=ui_text(self.locale, "header_subtitle"))
        self.compact_button.configure(text=ui_text(self.locale, "button_compact"))
        self.full_button.configure(text=ui_text(self.locale, "button_full"))
        self.field_button.configure(text=ui_text(self.locale, "button_field"))
        self.sim_button.configure(text=ui_text(self.locale, "button_sim"))
        self.endgame_button.configure(text=ui_text(self.locale, "button_endgame"))
        self.reset_button.configure(text=ui_text(self.locale, "button_reset"))
        self.locale_en_button.configure(text=ui_text(self.locale, "button_lang_en"))
        self.locale_ko_button.configure(text=ui_text(self.locale, "button_lang_ko"))

        self.full_notebook.tab(self.overview_tab, text=ui_text(self.locale, "overview_tab"))
        self.full_notebook.tab(self.trails_tab, text=ui_text(self.locale, "trails_tab"))
        self.full_notebook.tab(self.map_tab, text=ui_text(self.locale, "map_tab"))
        self.full_notebook.tab(self.homefront_tab, text=ui_text(self.locale, "homefront_tab"))
        self.full_notebook.tab(self.affinity_tab, text=ui_text(self.locale, "affinity_tab"))
        self.full_notebook.tab(self.farming_tab, text=ui_text(self.locale, "farming_tab"))
        self.full_notebook.tab(self.network_tab, text=ui_text(self.locale, "network_tab"))
        self.full_notebook.tab(self.world_state_tab, text=ui_text(self.locale, "world_tab"))
        self.full_notebook.tab(self.ending_tab, text=ui_text(self.locale, "ending_tab"))
        self.full_notebook.tab(self.stats_tab, text=ui_text(self.locale, "stats_tab"))
        self.full_notebook.tab(self.gear_tab, text=ui_text(self.locale, "gear_tab"))
        self.full_notebook.tab(self.armory_tab, text=ui_text(self.locale, "armory_tab"))
        self.full_notebook.tab(self.skills_tab, text=ui_text(self.locale, "skills_tab"))
        self.full_notebook.tab(self.perks_tab, text=ui_text(self.locale, "perks_tab"))
        self.full_notebook.tab(self.crafting_tab, text=ui_text(self.locale, "crafting_tab"))
        self.full_notebook.tab(self.dead_tab, text=ui_text(self.locale, "dead_tab"))
        self.full_notebook.tab(self.collection_tab, text=ui_text(self.locale, "collection_tab"))
        self.full_notebook.tab(self.upgrades_tab, text=ui_text(self.locale, "upgrades_tab"))
        self.full_notebook.tab(self.encyclopedia_tab, text=ui_text(self.locale, "encyclopedia_tab"))

        detail_text = ui_text(self.locale, "details")
        self.trails_tab.set_labels(ui_text(self.locale, "trails_tab"), detail_text)
        self.map_tab.set_labels(ui_text(self.locale, "map_tab"), detail_text)
        self.homefront_tab.set_labels(ui_text(self.locale, "homefront_tab"), detail_text)
        self.affinity_tab.set_labels(ui_text(self.locale, "affinity_tab"), detail_text)
        self.farming_tab.set_labels(ui_text(self.locale, "farming_tab"), detail_text)
        self.network_tab.set_labels(ui_text(self.locale, "network_tab"), detail_text)
        self.world_state_tab.set_labels(ui_text(self.locale, "world_tab"), detail_text)
        self.ending_tab.set_labels(ui_text(self.locale, "ending_tab"), detail_text)
        self.stats_tab.set_labels(ui_text(self.locale, "stats_tab"), detail_text)
        self.gear_tab.set_labels(ui_text(self.locale, "gear_tab"), detail_text)
        self.armory_tab.set_labels(ui_text(self.locale, "armory_tab"), detail_text)
        self.skills_tab.set_labels(ui_text(self.locale, "skills_tab"), detail_text)
        self.perks_tab.set_labels(ui_text(self.locale, "perks_tab"), detail_text)
        self.crafting_tab.set_labels(ui_text(self.locale, "crafting_tab"), detail_text)
        self.dead_tab.set_labels(ui_text(self.locale, "dead_tab"), detail_text)
        self.collection_tab.set_labels(ui_text(self.locale, "collection_tab"), detail_text)
        self.upgrades_tab.set_labels(ui_text(self.locale, "upgrades_tab"), detail_text)
        self.encyclopedia_tab.set_labels(ui_text(self.locale, "encyclopedia_tab"), detail_text)

    def run(self) -> None:
        self.root.mainloop()

    def destroy(self) -> None:
        if self.compact_window is not None and self.compact_window.winfo_exists():
            self.compact_window.destroy()
        self.root.destroy()


def launch_ui(simulated: bool = False, check: bool = False) -> str | None:
    player = simulate_first_ten_hours()[0] if simulated else create_baseline_player()
    app = FrontierLedgerApp(player=player)
    app.show_field_view()
    if check:
        app.root.update_idletasks()
        app.root.update()
        app.destroy()
        return "UI check passed"
    app.run()
    return None


