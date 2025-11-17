#include <memory>
#include <stdexcept>

#include "chunk.hpp"
#include "raw_chunk.hpp"
#include "raw_dimension.hpp"

using namespace Amulet::NBT;

namespace Amulet {

std::unique_ptr<BedrockChunk> BedrockRawDimension::decode_chunk(
    BedrockRawChunk raw_chunk,
    std::int32_t cx,
    std::int32_t cz)
{
    auto chunk = std::make_unique<BedrockChunkTemp>();
    chunk->set_raw_data(std::make_shared<BedrockRawChunk>(std::move(raw_chunk)));
    return chunk;
}

} // namespace Amulet
