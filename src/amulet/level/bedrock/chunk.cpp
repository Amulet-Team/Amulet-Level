#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

// #include <amulet/core/chunk/chunk.hpp>
// #include <amulet/core/chunk/component/block_component.hpp>

#include "chunk.hpp"

namespace Amulet {

BedrockRawChunk::BedrockRawChunk() = default;
BedrockRawChunk::BedrockRawChunk(
    std::map<std::string, std::string> data,
    std::list<std::shared_ptr<NBT::NamedTag>> entity_actors,
    std::list<std::shared_ptr<NBT::NamedTag>> unknown_actors)
    : _data(data)
    , _entity_actors(entity_actors)
    , _unknown_actors(unknown_actors)
{
}

BedrockRawChunk::BedrockRawChunk(const BedrockRawChunk&) = default;
BedrockRawChunk::BedrockRawChunk(BedrockRawChunk&&) = default;
BedrockRawChunk& BedrockRawChunk::operator=(const BedrockRawChunk&) = default;
BedrockRawChunk& BedrockRawChunk::operator=(BedrockRawChunk&&) = default;

BedrockRawChunk::~BedrockRawChunk() = default;

std::map<std::string, std::string>& BedrockRawChunk::get_data()
{
    return _data;
}

std::list<std::shared_ptr<NBT::NamedTag>>& BedrockRawChunk::get_entity_actors()
{
    return _entity_actors;
}

std::list<std::shared_ptr<NBT::NamedTag>>& BedrockRawChunk::get_unknown_actors()
{
    return _unknown_actors;
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
