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

#include <amulet/level/dll.hpp>

namespace Amulet {

class BedrockRawChunk {
private:
    // LevelDB keys and values (keys have the dimension and coord stripped)
    std::map<std::string, std::string> _data;
    std::list<std::shared_ptr<NBT::NamedTag>> _entity_actors;
    std::list<std::shared_ptr<NBT::NamedTag>> _unknown_actors;

public:
    AMULET_LEVEL_EXPORT BedrockRawChunk();
    AMULET_LEVEL_EXPORT BedrockRawChunk(
        std::map<std::string, std::string>,
        std::list<std::shared_ptr<NBT::NamedTag>>,
        std::list<std::shared_ptr<NBT::NamedTag>>
    );

    AMULET_LEVEL_EXPORT BedrockRawChunk(const BedrockRawChunk&);
    AMULET_LEVEL_EXPORT BedrockRawChunk(BedrockRawChunk&&);
    AMULET_LEVEL_EXPORT BedrockRawChunk& operator=(const BedrockRawChunk&);
    AMULET_LEVEL_EXPORT BedrockRawChunk& operator=(BedrockRawChunk&&);

    AMULET_LEVEL_EXPORT ~BedrockRawChunk();

    AMULET_LEVEL_EXPORT std::map<std::string, std::string>& get_data();
    AMULET_LEVEL_EXPORT std::list<std::shared_ptr<NBT::NamedTag>>& get_entity_actors();
    AMULET_LEVEL_EXPORT std::list<std::shared_ptr<NBT::NamedTag>>& get_unknown_actors();
};

class BedrockChunk : public Chunk { };

namespace detail {
    // Get a null chunk instance for the given chunk id.
    std::unique_ptr<BedrockChunk> get_bedrock_null_chunk(const std::string& chunk_id);

    // Get the chunk's identifier.
    std::string get_bedrock_chunk_id(const BedrockChunk& chunk);
} // namespace detail

} // namespace Amulet
