from __future__ import annotations

import logging as logging

from amulet.level.abc.level import Level
from amulet.level.loader import NoValidLevelLoader, get_level

from . import _amulet_level, _version, abc, java, loader

__all__ = [
    "Level",
    "NoValidLevelLoader",
    "abc",
    "compiler_config",
    "get_level",
    "java",
    "loader",
    "logging",
]

def _init() -> None: ...

__version__: str
compiler_config: dict
