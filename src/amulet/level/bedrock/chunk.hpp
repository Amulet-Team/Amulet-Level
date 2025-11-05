#pragma once

#include <map>
//#include <optional>
//#include <stdexcept>
#include <string>
//#include <vector>

#include <amulet/nbt/tag/named_tag.hpp>

//#include <amulet/core/biome/biome.hpp>
//#include <amulet/core/block/block.hpp>
#include <amulet/core/chunk/chunk.hpp>
//#include <amulet/core/chunk/component/block_component.hpp>
//
//#include <amulet/level/dll.hpp>
//
namespace Amulet {
using BedrockRawChunk = std::map<std::string, Amulet::NBT::NamedTag>;

class BedrockChunk : public Chunk { };

namespace detail {
    // Get a null chunk instance for the given chunk id.
    std::unique_ptr<BedrockChunk> get_bedrock_null_chunk(const std::string& chunk_id);

    // Get the chunk's identifier.
    std::string get_bedrock_chunk_id(const BedrockChunk& chunk);
} // namespace detail

} // namespace Amulet
