#include <memory>
#include <stdexcept>

#include "chunk.hpp"
#include "raw_dimension.hpp"

using namespace Amulet::NBT;

namespace Amulet {

std::unique_ptr<BedrockRawChunk> BedrockRawDimension::encode_chunk(
    BedrockChunk& chunk,
    std::int64_t cx,
    std::int64_t cz)
{
    throw std::runtime_error("NotImplementedError");
}

} // namespace Amulet
