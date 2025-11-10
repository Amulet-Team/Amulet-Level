#include <memory>
#include <stdexcept>

#include "chunk.hpp"
#include "raw_dimension.hpp"

using namespace Amulet::NBT;

namespace Amulet {

std::unique_ptr<BedrockChunk> BedrockRawDimension::decode_chunk(
    const BedrockRawChunk& raw_chunk,
    std::int32_t cx,
    std::int32_t cz)
{
    throw std::runtime_error("NotImplementedError");
}

} // namespace Amulet
