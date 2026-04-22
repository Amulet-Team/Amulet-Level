#pragma once

#include <map>

#include <amulet/io/binary_reader.hpp>
#include <amulet/io/binary_writer.hpp>

#include <amulet/nbt/tag/named_tag.hpp>

#include <amulet/utils/bytes.hpp>

#include <amulet/level/dll.hpp>

namespace Amulet {

class AMULET_LEVEL_EXPORT BedrockRawChunk {
private:
    // LevelDB keys and values (keys have the dimension and coord stripped)
    std::map<Bytes, Bytes> _data;
    std::vector<std::shared_ptr<NBT::NamedTag>> _actors;

public:
    // Constructors
    BedrockRawChunk();
    BedrockRawChunk(
        std::map<Bytes, Bytes>,
        std::vector<std::shared_ptr<NBT::NamedTag>>);

    // Copy
    BedrockRawChunk(const BedrockRawChunk&);
    BedrockRawChunk& operator=(const BedrockRawChunk&);

    // Move
    BedrockRawChunk(BedrockRawChunk&&);
    BedrockRawChunk& operator=(BedrockRawChunk&&);

    // Destructor
    ~BedrockRawChunk();

    void serialise(BaseBinaryWriter&) const;
    static BedrockRawChunk deserialise(BinaryReader&);

    std::map<Bytes, Bytes>& get_data();
    const std::map<Bytes, Bytes>& get_data() const;
    std::vector<std::shared_ptr<NBT::NamedTag>>& get_actors();
    const std::vector<std::shared_ptr<NBT::NamedTag>>& get_actors() const;
};

} // namespace Amulet
