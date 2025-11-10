#pragma once

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

namespace Amulet {

using BedrockInternalDimensionID = std::uint32_t;

class BedrockRawLevel;

class BedrockRawDimension {
private:
    OrderedMutex _public_mutex;
    std::shared_ptr<LevelDB> _db;
    BedrockInternalDimensionID _internal_dimension_id;
    DimensionId _dimension_id;
    SelectionBox _bounds;
    BlockStack _default_block;
    Biome _default_biome;
    bool _destroyed = false;

public:
    BedrockRawDimension(
        std::shared_ptr<LevelDB> db,
        BedrockInternalDimensionID internal_dimension_id,
        const DimensionId& dimension_id,
        const SelectionBox& bounds,
        const BlockStack& default_block,
        const Biome& default_biome);

    // Destructor.
    AMULET_LEVEL_EXPORT ~BedrockRawDimension();

    // The public mutex
    // Thread safe.
    AMULET_LEVEL_EXPORT OrderedMutex& get_mutex();

    // The identifier for this dimension. eg. "minecraft:overworld".
    // Thread safe.
    AMULET_LEVEL_EXPORT const DimensionId& get_dimension_id() const;

    // The internal identifier for this dimension. eg 0, 1 or 2
    // Thread safe.
    AMULET_LEVEL_EXPORT BedrockInternalDimensionID get_internal_dimension_id() const;

    // The selection box that fills the whole world.
    // Thread safe.
    AMULET_LEVEL_EXPORT const SelectionBox& get_bounds() const;

    // The default block for this dimension.
    // Thread safe.
    AMULET_LEVEL_EXPORT const BlockStack& get_default_block() const;

    // The default biome for this dimension.
    // Thread safe.
    AMULET_LEVEL_EXPORT const Biome& get_default_biome() const;

    // An iterator of all chunk coordinates in the dimension.
    // External Read:SharedReadWrite lock required.
    // External Read:SharedReadOnly lock optional.
    // AMULET_LEVEL_EXPORT AnvilChunkCoordIterator all_chunk_coords() const;

    // Does the chunk exist in this dimension.
    // External Read:SharedReadWrite lock required.
    // External Read:SharedReadOnly lock optional.
    AMULET_LEVEL_EXPORT bool has_chunk(std::int64_t cx, std::int64_t cz);

    // Delete the chunk from this dimension.
    // External ReadWrite:SharedReadWrite lock required.
    AMULET_LEVEL_EXPORT void delete_chunk(std::int64_t cx, std::int64_t cz);

    // Get the raw chunk from this dimension.
    // External Read:SharedReadWrite lock required.
    AMULET_LEVEL_EXPORT std::unique_ptr<BedrockRawChunk> get_raw_chunk(std::int64_t cx, std::int64_t cz);

    // Set the chunk in this dimension from raw data.
    // External ReadWrite:SharedReadWrite lock required.
    AMULET_LEVEL_EXPORT void set_raw_chunk(std::int64_t cx, std::int64_t cz, const BedrockRawChunk& chunk);

    // Decode a raw chunk to a chunk object.
    // This will mutate the chunk data.
    // TODO: thread safety
    AMULET_LEVEL_EXPORT std::unique_ptr<BedrockChunk> decode_chunk(const BedrockRawChunk& raw_chunk, std::int64_t cx, std::int64_t cz);

    // Encode a chunk object to its raw data.
    // This will mutate the chunk data.
    // TODO: thread safety
    AMULET_LEVEL_EXPORT std::unique_ptr<BedrockRawChunk> encode_chunk(BedrockChunk& chunk, std::int64_t cx, std::int64_t cz);

    // Get and decode the chunk.
    // TODO: thread safety
    AMULET_LEVEL_EXPORT std::unique_ptr<BedrockChunk> get_chunk(std::int64_t cx, std::int64_t cz);

    // Encode and set the chunk.
    // This will mutate the chunk data.
    // TODO: thread safety
    AMULET_LEVEL_EXPORT void set_chunk(std::int64_t cx, std::int64_t cz, BedrockChunk& chunk);

    // Destroy the instance.
    // Calls made after this will fail.
    // This may only be called by the owner of the instance.
    // External ReadWrite:Unique lock required.
    AMULET_LEVEL_EXPORT void destroy();

    // Has the instance been destroyed.
    // If this is false, other calls will fail.
    // External Read:SharedReadWrite lock required.
    AMULET_LEVEL_EXPORT bool is_destroyed();
};

} // namespace Amulet
