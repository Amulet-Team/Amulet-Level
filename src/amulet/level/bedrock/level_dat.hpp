#pragma once

#include <filesystem>
#include <string>
#include <string_view>

#include <amulet/nbt/tag/named_tag.hpp>

#include <amulet/level/dll.hpp>

namespace Amulet {

class BedrockLevelDat {
public:
    std::uint32_t version;
    NBT::NamedTag named_tag;

    // Construct with the level.dat version and named tag
    AMULET_LEVEL_EXPORT BedrockLevelDat(std::uint32_t version, const NBT::NamedTag& named_tag);

    // Construct from the binary data
    AMULET_LEVEL_EXPORT static BedrockLevelDat from_binary(std::string_view);

    // Construct from the give path.
    AMULET_LEVEL_EXPORT static BedrockLevelDat from_file(std::filesystem::path path);

    // Convert to binary.
    AMULET_LEVEL_EXPORT std::string to_binary() const;

    // Encode and write to the file.
    AMULET_LEVEL_EXPORT void save_to(std::filesystem::path path) const;
};

} // namespace Amulet
