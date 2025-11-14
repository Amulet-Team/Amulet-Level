#include <memory>
#include <stdexcept>

#include "chunk.hpp"
#include "raw_dimension.hpp"

using namespace Amulet::NBT;

namespace Amulet {

BedrockRawChunk BedrockRawDimension::encode_chunk(
    BedrockChunk& chunk,
    std::int32_t cx,
    std::int32_t cz)
{
    throw std::runtime_error("NotImplementedError");
}

} // namespace Amulet
