from __future__ import annotations

import amulet.core.chunk
import amulet.level.bedrock.chunk_components

__all__: list[str] = ["BedrockChunk", "BedrockChunkTemp"]

class BedrockChunk(amulet.core.chunk.Chunk):
    pass

class BedrockChunkTemp(
    BedrockChunk, amulet.level.bedrock.chunk_components.BedrockRawChunkComponent
):
    pass
