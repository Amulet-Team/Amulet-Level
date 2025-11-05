#include <memory>
#include <stdexcept>

#include "chunk.hpp"
#include "raw_dimension.hpp"

using namespace Amulet::NBT;

namespace Amulet {

std::unique_ptr<BedrockChunk> BedrockRawDimension::decode_chunk(
    BedrockRawChunk raw_chunk,
    std::int64_t cx,
    std::int64_t cz)
{
    throw std::runtime_error("NotImplementedError");
}

} // namespace Amulet
