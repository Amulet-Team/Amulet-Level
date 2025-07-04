from __future__ import annotations

from builtins import str as DimensionId

import amulet.core.biome
import amulet.core.block
import amulet.core.selection.box
import amulet.core.selection.group
import amulet.level.abc.chunk_handle

__all__ = ["Dimension", "DimensionId"]

class Dimension:
    def get_chunk_handle(
        self, cx: int, cz: int
    ) -> amulet.level.abc.chunk_handle.ChunkHandle:
        """
        Get the chunk handle for the given chunk in this dimension.
        Thread safe.

        :param cx: The chunk x coordinate to load.
        :param cz: The chunk z coordinate to load.
        """

    @property
    def bounds(
        self,
    ) -> (
        amulet.core.selection.box.SelectionBox
        | amulet.core.selection.group.SelectionGroup
    ):
        """
        The editable region of the dimension.
        Thread safe.
        """

    @property
    def default_biome(self) -> amulet.core.biome.Biome:
        """
        The default biome for this dimension
        Thread safe.
        """

    @property
    def default_block(self) -> amulet.core.block.BlockStack:
        """
        The default block for this dimension.
        Thread safe.
        """

    @property
    def dimension_id(self) -> str:
        """
        Get the dimension id for this dimension.
        Thread safe.
        """
