#include <map>
#include <memory>

#include <amulet/utils/bytes.hpp>

#include "raw_chunk.hpp"

namespace Amulet {

BedrockRawChunk::BedrockRawChunk() = default;
BedrockRawChunk::BedrockRawChunk(
    std::map<Bytes, Bytes> data,
    std::vector<std::shared_ptr<NBT::NamedTag>> entity_actors)
    : _data(data)
    , _actors(entity_actors)
{
}

BedrockRawChunk::BedrockRawChunk(const BedrockRawChunk&) = default;
BedrockRawChunk::BedrockRawChunk(BedrockRawChunk&&) = default;
BedrockRawChunk& BedrockRawChunk::operator=(const BedrockRawChunk&) = default;
BedrockRawChunk& BedrockRawChunk::operator=(BedrockRawChunk&&) = default;

BedrockRawChunk::~BedrockRawChunk() = default;

std::map<Bytes, Bytes>& BedrockRawChunk::get_data()
{
    return _data;
}

const std::map<Bytes, Bytes>& BedrockRawChunk::get_data() const
{
    return _data;
}

std::vector<std::shared_ptr<NBT::NamedTag>>& BedrockRawChunk::get_actors()
{
    return _actors;
}

const std::vector<std::shared_ptr<NBT::NamedTag>>& BedrockRawChunk::get_actors() const
{
    return _actors;
}

} // namespace Amulet
