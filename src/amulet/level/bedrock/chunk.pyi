from __future__ import annotations

import collections.abc

import amulet.core.chunk
import amulet.nbt

__all__: list[str] = ["BedrockChunk", "BedrockRawChunk"]

class BedrockChunk(amulet.core.chunk.Chunk):
    pass

class BedrockRawChunk:
    @property
    def actors(self) -> collections.abc.MutableSequence[amulet.nbt.NamedTag]: ...
    @property
    def data(self) -> collections.abc.MutableMapping[bytes, bytes]: ...
