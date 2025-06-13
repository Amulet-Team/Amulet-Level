from __future__ import annotations

import faulthandler as faulthandler

from . import _test_amulet_level, test_abc

__all__ = ["compiler_config", "faulthandler", "test_abc"]

def _init() -> None: ...

compiler_config: dict
