from __future__ import annotations

import typing

import amulet.level.abc.level
import amulet.level.bedrock.dimension
import amulet.level.bedrock.raw_level

__all__: list[str] = ["BedrockLevel"]

class BedrockLevel(
    amulet.level.abc.level.Level,
    amulet.level.abc.level.CompactibleLevel,
    amulet.level.abc.level.DiskLevel,
    amulet.level.abc.level.ReloadableLevel,
):
    def get_dimension(
        self, dimension_id: str | typing.SupportsInt
    ) -> amulet.level.bedrock.dimension.BedrockDimension:
        """
        Get a dimension.
        External Read:SharedReadWrite lock required.
        External ReadWrite:SharedReadWrite lock required when calling code in Dimension (and its children) that need write permission.
        """

    @property
    def raw_level(self) -> amulet.level.bedrock.raw_level.BedrockRawLevel:
        """
        Access the raw level instance.
        Before calling any mutating functions, the caller must call :meth:`purge` (optionally saving before)
        External ReadWrite:Unique lock required.
        """
