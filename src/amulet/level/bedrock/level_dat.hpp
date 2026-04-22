#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>

#include <amulet/nbt/tag/named_tag.hpp>

#include <amulet/level/dll.hpp>

namespace Amulet {

class AMULET_LEVEL_EXPORT BedrockLevelDat {
private:
    std::uint32_t _version;
    std::shared_ptr<NBT::NamedTag> _named_tag;

public:
    BedrockLevelDat();

    // Construct with the level.dat version and named tag
    BedrockLevelDat(std::uint32_t version, std::shared_ptr<NBT::NamedTag> named_tag);
    BedrockLevelDat(std::uint32_t version, const NBT::NamedTag& named_tag);

    BedrockLevelDat(const BedrockLevelDat&);
    BedrockLevelDat(BedrockLevelDat&&);
    BedrockLevelDat& operator=(const BedrockLevelDat&);
    BedrockLevelDat& operator=(BedrockLevelDat&&);

    ~BedrockLevelDat();

    std::uint32_t get_version() const;
    void set_version(std::uint32_t);

    NBT::NamedTag& get_named_tag();
    const NBT::NamedTag& get_named_tag() const;
    std::shared_ptr<NBT::NamedTag> get_named_tag_ptr();
    
    void set_named_tag(std::shared_ptr<NBT::NamedTag>);
    void set_named_tag(const NBT::NamedTag&);

    // Construct from the binary data
    static BedrockLevelDat from_binary(std::string_view);

    // Construct from the give path.
    static BedrockLevelDat from_file(std::filesystem::path path);

    // Convert to binary.
    std::string to_binary() const;

    // Encode and write to the file.
    void save_to(std::filesystem::path path) const;

    BedrockLevelDat deep_copy() const;
};

} // namespace Amulet
