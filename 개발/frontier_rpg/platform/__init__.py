"""Prototype platform-layer package.

The Python prototype currently has minimal platform concerns, but the folder exists so
new platform-specific seams do not need to be introduced ad hoc later.
"""

from .localization import SUPPORTED_LOCALES, ui_text

__all__ = ["SUPPORTED_LOCALES", "ui_text"]
