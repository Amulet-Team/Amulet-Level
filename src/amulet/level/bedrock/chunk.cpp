#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

#include <amulet/utils/bytes.hpp>

// #include <amulet/core/chunk/chunk.hpp>
// #include <amulet/core/chunk/component/block_component.hpp>

#include "chunk.hpp"

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

static std::map<std::string, std::function<std::unique_ptr<BedrockChunk>()>> bedrock_chunk_constructors = {};

namespace detail {
    std::unique_ptr<BedrockChunk> get_bedrock_null_chunk(const std::string& chunk_id)
    {
        auto it = bedrock_chunk_constructors.find(chunk_id);
        if (it == bedrock_chunk_constructors.end()) {
            throw std::runtime_error("Unknown chunk_id " + chunk_id);
        }
        return it->second();
    }
    std::string get_bedrock_chunk_id(const BedrockChunk& chunk)
    {
        std::string chunk_id = chunk.get_chunk_id();
        if (!bedrock_chunk_constructors.contains(chunk_id)) {
            throw std::runtime_error("Unknown chunk_id " + chunk_id);
        }
        return chunk_id;
    }
} // namespace detail

} // namespace Amulet
