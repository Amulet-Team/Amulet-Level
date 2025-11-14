#pragma once

#include <map>

#include <amulet/nbt/tag/named_tag.hpp>

#include <amulet/utils/bytes.hpp>

#include <amulet/level/dll.hpp>

namespace Amulet {

class BedrockRawChunk {
private:
    // LevelDB keys and values (keys have the dimension and coord stripped)
    std::map<Bytes, Bytes> _data;
    std::vector<std::shared_ptr<NBT::NamedTag>> _actors;

public:
    AMULET_LEVEL_EXPORT BedrockRawChunk();
    AMULET_LEVEL_EXPORT BedrockRawChunk(
        std::map<Bytes, Bytes>,
        std::vector<std::shared_ptr<NBT::NamedTag>>);

    AMULET_LEVEL_EXPORT BedrockRawChunk(const BedrockRawChunk&);
    AMULET_LEVEL_EXPORT BedrockRawChunk(BedrockRawChunk&&);
    AMULET_LEVEL_EXPORT BedrockRawChunk& operator=(const BedrockRawChunk&);
    AMULET_LEVEL_EXPORT BedrockRawChunk& operator=(BedrockRawChunk&&);

    AMULET_LEVEL_EXPORT ~BedrockRawChunk();

    AMULET_LEVEL_EXPORT std::map<Bytes, Bytes>& get_data();
    AMULET_LEVEL_EXPORT const std::map<Bytes, Bytes>& get_data() const;
    AMULET_LEVEL_EXPORT std::vector<std::shared_ptr<NBT::NamedTag>>& get_actors();
    AMULET_LEVEL_EXPORT const std::vector<std::shared_ptr<NBT::NamedTag>>& get_actors() const;
};

} // namespace Amulet
