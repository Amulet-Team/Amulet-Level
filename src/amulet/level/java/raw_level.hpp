#pragma once

#include <chrono>
#include <filesystem>
#include <map>
#include <memory>
#include <shared_mutex>
#include <stdexcept>

#include <amulet/nbt/tag/named_tag.hpp>

#include <amulet/utils/image.hpp>

#include <amulet/core/version/version.hpp>

#include <amulet/utils/event.hpp>
#include <amulet/utils/lock_file.hpp>
#include <amulet/utils/mutex.hpp>

#include <amulet/level/abc/registry.hpp>
#include <amulet/level/dll.hpp>

#include "dimension.hpp"
#include "raw_dimension.hpp"

namespace Amulet {

class JavaCreateArgsV1 {
public:
    bool overwrite;
    std::filesystem::path path;
    VersionNumber version;
    std::string level_name;

    JavaCreateArgsV1(
        bool overwrite,
        const std::filesystem::path path,
        const VersionNumber& version,
        const std::string& level_name)
        : overwrite(overwrite)
        , path(path)
        , version(version)
        , level_name(level_name)
    {
    }
};

class JavaRawLevelOpenData {
public:
    std::unique_ptr<LockFile> session_lock;
    // TODO: data_pack
    std::shared_mutex dimensions_mutex;
    std::map<JavaInternalDimensionID, std::shared_ptr<JavaRawDimension>> dimensions;
    std::map<DimensionId, JavaInternalDimensionID> dimension_ids;
    std::shared_ptr<IdRegistry> block_id_override;
    std::shared_ptr<IdRegistry> biome_id_override;

    JavaRawLevelOpenData(
        std::unique_ptr<LockFile> session_lock)
        : session_lock(std::move(session_lock))
        , block_id_override(std::make_shared<IdRegistry>())
        , biome_id_override(std::make_shared<IdRegistry>())
    {
    }
};

class AMULET_LEVEL_EXPORT JavaRawLevel {
private:
    OrderedMutex _public_mutex;
    std::filesystem::path _path;
    Amulet::NBT::NamedTag _level_dat;
    VersionNumber _data_version;

    // Data that is only valid when the level is open.
    // The external unique lock must be held to change this pointer.
    std::unique_ptr<JavaRawLevelOpenData> _raw_open_data;

    // Construct a new instance. Path is the directory containing the level.dat file.
    JavaRawLevel(const std::filesystem::path path);

    // Validate _raw_open_data is valid and return a reference.
    // External Read:SharedReadWrite lock required.
    JavaRawLevelOpenData& _get_raw_open();

    JavaRawLevelOpenData& _find_dimensions();
    void _open(std::unique_ptr<LockFile> session_lock);
    std::unique_ptr<LockFile> _close();
    VersionNumber _get_data_version();

    SelectionBox _get_dimension_bounds(const DimensionId&);

    // Register a dimension
    // Must be called with the dimension lock in unique mode.
    void _register_dimension(JavaRawLevelOpenData&, const JavaInternalDimensionID&, const DimensionId&);

public:
    JavaRawLevel() = delete;
    JavaRawLevel(const JavaRawLevel&) = delete;
    JavaRawLevel& operator=(const JavaRawLevel&) = delete;
    JavaRawLevel(JavaRawLevel&&) = delete;
    JavaRawLevel& operator=(JavaRawLevel&&) = delete;
    ~JavaRawLevel();

    // Load an existing Java level from the given directory.
    // Thread safe.
    static std::unique_ptr<JavaRawLevel> load(const std::filesystem::path&);

    // Create a new Java level at the given directory.
    // Thread safe.
    static std::unique_ptr<JavaRawLevel> create(const JavaCreateArgsV1&);

    // External mutex
    // Thread safe.
    OrderedMutex& get_mutex();

    // Is the level open.
    // External Read:SharedReadWrite lock required.
    bool is_open() const;

    // Reload the metadata. This can only be called when the level is closed.
    // External ReadWrite:Unique lock required.
    void reload_metadata();

    // An event emitted when the level is opened.
    // Thread safe. Level locks may be held in directly connected handlers.
    Event<> opened;

    // Open the level.
    // opened event will be emitted when complete.
    // External ReadWrite:Unique lock required.
    void open();

    // An event emitted when the level is closed.
    // Thread safe. Level locks may be held in directly connected handlers.
    Event<> closed;

    // Close the level.
    // closed event will be emitted when complete.
    // External ReadWrite:Unique lock required.
    void close();

    // An event emitted when the level is reloaded.
    // Thread safe. Level locks may be held in directly connected handlers.
    Event<> reloaded;

    // Reload the level.
    // This is like closing and re-opening without releasing the session.lock file.
    // External ReadWrite:Unique lock required.
    void reload();

    // The path to the level directory.
    // Thread safe.
    const std::filesystem::path& get_path() const;

    // The NamedTag stored in the level.dat file. Returns a unique copy.
    // External Read:SharedReadWrite lock required.
    Amulet::NBT::NamedTag get_level_dat() const;

    // Set the level.dat NamedTag
    // This calls `reload` if the data version changed.
    // External ReadWrite:Unique lock required.
    void set_level_dat(const Amulet::NBT::NamedTag&);

    // The platform identifier. "java"
    // Thread safe.
    std::string get_platform() const;

    // The game data version that the level was last opened in.
    // External Read:SharedReadWrite lock required.
    VersionNumber get_data_version() const;

    // Set the maximum game version.
    // If the game version is different this will call `reload`.
    // External ReadWrite:Unique lock required.
    void set_data_version(const VersionNumber&);

    // Is this level a supported version.
    // This is true for all versions we support and false for snapshots and unsupported newer versions.
    // TODO: thread safety
    bool is_supported() const;

    // Get the thumbnail for the level.
    // This depends upon python so the GIL must be held.
    // Thread safe.
    PIL::Image::Image get_thumbnail() const;

    // The time when the level was lasted edited.
    // External Read:SharedReadWrite lock required.
    std::chrono::system_clock::time_point get_modified_time() const;

    // The name of the level.
    // External Read:SharedReadWrite lock required.
    std::string get_level_name() const;

    // Event emitted when the level name changes.
    // Thread safe. Level locks may be held in directly connected handlers.
    Event<std::string> level_name_changed;

    // Set the level name.
    // External ReadWrite:Unique lock required.
    void set_level_name(const std::string&);

    // The identifiers for all dimensions in this level.
    // External Read:SharedReadWrite lock required.
    // External Read:SharedReadOnly lock optional.
    std::vector<std::string> get_dimension_ids();

    // Get the raw dimension object for a specific dimension.
    // External Read:SharedReadWrite lock required.
    std::shared_ptr<JavaRawDimension> get_dimension(const DimensionId&);

    // Compact the level.
    // External Read:SharedReadWrite lock required.
    void compact();

    // Overridden block ids.
    // External Read:SharedReadWrite lock required.
    std::shared_ptr<IdRegistry> get_block_id_override();

    // Overridden biome ids.
    // External Read:SharedReadWrite lock required.
    std::shared_ptr<IdRegistry> get_biome_id_override();
};

} // namespace Amulet
