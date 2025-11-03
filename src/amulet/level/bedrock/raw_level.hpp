#pragma once

// #include <chrono>
#include <filesystem>
#include <map>
#include <memory>
// #include <shared_mutex>
// #include <stdexcept>

#include <amulet/nbt/tag/named_tag.hpp>

#include <amulet/utils/event.hpp>
#include <amulet/utils/image.hpp>
#include <amulet/utils/lock_file.hpp>
#include <amulet/utils/mutex.hpp>
//
// #include <amulet/core/selection/box.hpp>
#include <amulet/core/version/version.hpp>
//
#include <amulet/level/abc/dimension.hpp>
#include <amulet/level/abc/registry.hpp>
#include <amulet/level/dll.hpp>
//
// #include "dimension.hpp"
#include "level_dat.hpp"
#include "raw_dimension.hpp"

namespace Amulet {

// class BedrockCreateArgsV1 {
// public:
//     bool overwrite;
//     std::filesystem::path path;
//     VersionNumber version;
//     std::string level_name;
//
//     BedrockCreateArgsV1(
//         bool overwrite,
//         const std::filesystem::path path,
//         const VersionNumber& version,
//         const std::string& level_name)
//         : overwrite(overwrite)
//         , path(path)
//         , version(version)
//         , level_name(level_name)
//     {
//     }
// };

class BedrockRawLevelOpenData {
public:
    std::unique_ptr<LockFile> session_lock;
    // TODO: data_pack
    std::shared_mutex dimensions_mutex;
    //std::map<BedrockInternalDimensionID, std::shared_ptr<BedrockRawDimension>> dimensions;
    //std::map<DimensionId, BedrockInternalDimensionID> dimension_ids;
    std::shared_ptr<IdRegistry> block_id_override;
    std::shared_ptr<IdRegistry> biome_id_override;

    BedrockRawLevelOpenData(
        std::unique_ptr<LockFile> session_lock)
        : session_lock(std::move(session_lock))
        , block_id_override(std::make_shared<IdRegistry>())
        , biome_id_override(std::make_shared<IdRegistry>())
    {
    }
};

class BedrockRawLevel {
private:
    OrderedMutex _public_mutex;
    std::filesystem::path _path;
    BedrockLevelDat _level_dat;
    //VersionNumber _data_version;

    // Data that is only valid when the level is open.
    // The external unique lock must be held to change this pointer.
    std::unique_ptr<BedrockRawLevelOpenData> _raw_open_data;

    // Construct a new instance. Path is the directory containing the level.dat file.
    BedrockRawLevel(const std::filesystem::path path)
        : _path(path)
        , _level_dat()
        //, _data_version({})
    {
    }

    // Validate _raw_open_data is valid and return a reference.
    // External Read:SharedReadWrite lock required.
    BedrockRawLevelOpenData& _get_raw_open()
    {
        if (!_raw_open_data) {
            throw std::runtime_error("The level is not open.");
        }
        return *_raw_open_data;
    }

    // BedrockRawLevelOpenData& _find_dimensions();
    void _open(std::unique_ptr<LockFile> session_lock);
    std::unique_ptr<LockFile> _close();
    // VersionNumber _get_data_version();

    // SelectionBox _get_dimension_bounds(const DimensionId&);

    //// Register a dimension
    //// Must be called with the dimension lock in unique mode.
    // void _register_dimension(BedrockRawLevelOpenData&, const BedrockInternalDimensionID&, const DimensionId&);

public:
    BedrockRawLevel() = delete;
    BedrockRawLevel(const BedrockRawLevel&) = delete;
    BedrockRawLevel& operator=(const BedrockRawLevel&) = delete;
    BedrockRawLevel(BedrockRawLevel&&) = delete;
    BedrockRawLevel& operator=(BedrockRawLevel&&) = delete;
    AMULET_LEVEL_EXPORT ~BedrockRawLevel();

    // Load an existing Bedrock level from the given directory.
    // Thread safe.
    AMULET_LEVEL_EXPORT static std::unique_ptr<BedrockRawLevel> load(const std::filesystem::path&);

    //// Create a new Bedrock level at the given directory.
    //// Thread safe.
    // AMULET_LEVEL_EXPORT static std::unique_ptr<BedrockRawLevel> create(const BedrockCreateArgsV1&);

    // External mutex
    // Thread safe.
    AMULET_LEVEL_EXPORT OrderedMutex& get_mutex();

    // Is the level open.
    // External Read:SharedReadWrite lock required.
    AMULET_LEVEL_EXPORT bool is_open() const;

    // Reload the metadata. This can only be called when the level is closed.
    // External ReadWrite:Unique lock required.
    AMULET_LEVEL_EXPORT void reload_metadata();

    // An event emitted when the level is opened.
    Event<> opened;

    // Open the level.
    // opened event will be emitted when complete.
    // External ReadWrite:Unique lock required.
    AMULET_LEVEL_EXPORT void open();

    // An event emitted when the level is closed.
    Event<> closed;

    // Close the level.
    // closed event will be emitted when complete.
    // External ReadWrite:Unique lock required.
    AMULET_LEVEL_EXPORT void close();

    // An event emitted when the level is reloaded.
    Event<> reloaded;

    // Reload the level.
    // This is like closing and re-opening without releasing the session.lock file.
    // External ReadWrite:Unique lock required.
    AMULET_LEVEL_EXPORT void reload();

    // The path to the level directory.
    // Thread safe.
    AMULET_LEVEL_EXPORT const std::filesystem::path& get_path() const;

    // The NamedTag stored in the level.dat file. Returns a unique copy.
    // External Read:SharedReadWrite lock required.
    AMULET_LEVEL_EXPORT BedrockLevelDat get_level_dat() const;

    // Set the level.dat NamedTag
    // This calls `reload` if the data version changed.
    // External ReadWrite:Unique lock required.
    AMULET_LEVEL_EXPORT void set_level_dat(const BedrockLevelDat&);

    // The platform identifier. "bedrock"
    // Thread safe.
    AMULET_LEVEL_EXPORT std::string get_platform() const;

    //// The game data version that the level was last opened in.
    //// External Read:SharedReadWrite lock required.
    // AMULET_LEVEL_EXPORT VersionNumber get_data_version() const;

    //// Set the maximum game version.
    //// If the game version is different this will call `reload`.
    //// External ReadWrite:Unique lock required.
    // AMULET_LEVEL_EXPORT void set_data_version(const VersionNumber&);

    // Is this level a supported version.
    // This is true for all versions we support and false for snapshots and unsupported newer versions.
    // TODO: thread safety
    AMULET_LEVEL_EXPORT bool is_supported() const;

    // Get the thumbnail for the level.
    // This depends upon python so the GIL must be held.
    // Thread safe.
    AMULET_LEVEL_EXPORT PIL::Image::Image get_thumbnail() const;

    // The time when the level was lasted edited.
    // External Read:SharedReadWrite lock required.
    AMULET_LEVEL_EXPORT std::chrono::system_clock::time_point get_modified_time() const;

    // The name of the level.
    // External Read:SharedReadWrite lock required.
    AMULET_LEVEL_EXPORT std::string get_level_name() const;

    //// Set the level name.
    //// External ReadWrite:Unique lock required.
    // AMULET_LEVEL_EXPORT void set_level_name(const std::string&);

    //// The identifiers for all dimensions in this level.
    //// External Read:SharedReadWrite lock required.
    //// External Read:SharedReadOnly lock optional.
    // AMULET_LEVEL_EXPORT std::vector<std::string> get_dimension_ids();

    // Get the raw dimension object for a specific dimension.
    // External Read:SharedReadWrite lock required.
    //AMULET_LEVEL_EXPORT std::shared_ptr<BedrockRawDimension> get_dimension(const DimensionId&);

    // Compact the level.
    // External Read:SharedReadWrite lock required.
    AMULET_LEVEL_EXPORT void compact();

    // Overridden block ids.
    // External Read:SharedReadWrite lock required.
    AMULET_LEVEL_EXPORT std::shared_ptr<IdRegistry> get_block_id_override();

    // Overridden biome ids.
    // External Read:SharedReadWrite lock required.
    AMULET_LEVEL_EXPORT std::shared_ptr<IdRegistry> get_biome_id_override();
};

} // namespace Amulet
