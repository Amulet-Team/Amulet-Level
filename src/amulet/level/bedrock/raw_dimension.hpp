#pragma once

#include <atomic>
#include <memory>
#include <string>

#include <amulet/core/biome/biome.hpp>
#include <amulet/core/block/block.hpp>
#include <amulet/core/selection/box.hpp>

#include <amulet/leveldb.hpp>

#include <amulet/utils/mutex.hpp>

#include <amulet/level/abc/dimension.hpp>
#include <amulet/level/dll.hpp>

#include "chunk.hpp"
#include "raw_chunk.hpp"

namespace Amulet {

using BedrockInternalDimensionID = std::uint32_t;

class AMULET_LEVEL_EXPORT BedrockChunkCoordIterator {
private:
    std::unique_ptr<LevelDBIterator> _it_ptr;
    leveldb::Iterator& _it;
    std::string _dimension_id;

    bool _find_next_chunk();

public:
    // Constructor
    BedrockChunkCoordIterator(std::unique_ptr<LevelDBIterator> it, BedrockInternalDimensionID dimension_id);

    // Copy
    BedrockChunkCoordIterator(const BedrockChunkCoordIterator&) = delete;
    BedrockChunkCoordIterator& operator=(const BedrockChunkCoordIterator&) = delete;

    // Move
    BedrockChunkCoordIterator(BedrockChunkCoordIterator&&);
    BedrockChunkCoordIterator& operator=(BedrockChunkCoordIterator&&) = delete;

    // Delete
    ~BedrockChunkCoordIterator();

    // Is the iterator valid.
    bool is_vaild() const;

    // Seek to the first chunk.
    // Returns true if a chunk was found.
    // Requires is_valid() == true
    bool seek_to_first();

    // Go to the next coord.
    // Call seek_to_first() before calling this.
    // Returns true if the next chunk was found.
    // Requires is_valid() == true
    bool seek_to_next();

    // Get the current coord.
    // seek_to_first() or seek_to_next() must return true for this to be valid.
    const std::pair<std::int32_t, std::int32_t> get_coord() const;
};

class BedrockRawLevel;

class AMULET_LEVEL_EXPORT BedrockRawDimension {
private:
    OrderedMutex _public_mutex;
    std::shared_ptr<LevelDB> _db;
    BedrockInternalDimensionID _internal_dimension_id;
    DimensionId _dimension_id;
    SelectionBox _bounds;
    std::int16_t _legacy_floor;
    BlockStack _default_block;
    Biome _default_biome;
    std::uint32_t _actor_group;
    std::atomic_uint32_t _actor_index;
    VersionNumber _max_version;
    bool _destroyed = false;

public:
    BedrockRawDimension(
        std::shared_ptr<LevelDB> db,
        BedrockInternalDimensionID internal_dimension_id,
        const DimensionId& dimension_id,
        const SelectionBox& bounds,
        std::int16_t legacy_floor,
        const BlockStack& default_block,
        const Biome& default_biome,
        std::uint32_t actor_group,
        VersionNumber max_version);

    // Copy
    BedrockRawDimension(const BedrockRawDimension&) = delete;
    BedrockRawDimension& operator=(const BedrockRawDimension&) = delete;

    // Move
    BedrockRawDimension(BedrockRawDimension&&) = delete;
    BedrockRawDimension& operator=(BedrockRawDimension&&) = delete;

    // Destructor.
    ~BedrockRawDimension();

    // The public mutex
    // Thread safe.
    OrderedMutex& get_mutex();

    // The identifier for this dimension. eg. "minecraft:overworld".
    // Thread safe.
    const DimensionId& get_dimension_id() const;

    // The internal identifier for this dimension. eg 0, 1 or 2
    // Thread safe.
    BedrockInternalDimensionID get_internal_dimension_id() const;

    // The selection box that fills the whole world.
    // Thread safe.
    const SelectionBox& get_bounds() const;

    // The default block for this dimension.
    // Thread safe.
    const BlockStack& get_default_block() const;

    // The default biome for this dimension.
    // Thread safe.
    const Biome& get_default_biome() const;

    // An iterator of all chunk coordinates in the dimension.
    // External Read:SharedReadWrite lock required.
    // External Read:SharedReadOnly lock optional.
    BedrockChunkCoordIterator all_chunk_coords() const;

    // Does the chunk exist in this dimension.
    // External Read:SharedReadWrite lock required.
    // External Read:SharedReadOnly lock optional.
    bool has_chunk(std::int32_t cx, std::int32_t cz);

    // Delete the chunk from this dimension.
    // External ReadWrite:SharedReadWrite lock required.
    void delete_chunk(std::int32_t cx, std::int32_t cz);

    // Get the raw chunk from this dimension.
    // External Read:SharedReadWrite lock required.
    BedrockRawChunk get_raw_chunk(std::int32_t cx, std::int32_t cz);

    // Set the chunk in this dimension from raw data.
    // External ReadWrite:SharedReadWrite lock required.
    void set_raw_chunk(std::int32_t cx, std::int32_t cz, BedrockRawChunk& chunk);

    // Decode a raw chunk to a chunk object.
    // TODO: thread safety
    std::unique_ptr<BedrockChunk> decode_chunk(BedrockRawChunk raw_chunk, std::int32_t cx, std::int32_t cz);

    // Encode a chunk object to its raw data.
    // This will mutate the chunk data.
    // TODO: thread safety
    BedrockRawChunk encode_chunk(BedrockChunk& chunk, std::int32_t cx, std::int32_t cz);

    // Get and decode the chunk.
    // TODO: thread safety
    std::unique_ptr<BedrockChunk> get_chunk(std::int32_t cx, std::int32_t cz);

    // Encode and set the chunk.
    // This will mutate the chunk data.
    // TODO: thread safety
    void set_chunk(std::int32_t cx, std::int32_t cz, BedrockChunk& chunk);

    // Destroy the instance.
    // Calls made after this will fail.
    // This may only be called by the owner of the instance.
    // External ReadWrite:Unique lock required.
    void destroy();

    // Has the instance been destroyed.
    // If this is false, other calls will fail.
    // External Read:SharedReadWrite lock required.
    bool is_destroyed();
};

} // namespace Amulet
