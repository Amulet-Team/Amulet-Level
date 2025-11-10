// #include <mutex>
// #include <shared_mutex>

#include "raw_dimension.hpp"

namespace Amulet {

BedrockRawDimension::BedrockRawDimension(
    std::shared_ptr<LevelDB> db,
    BedrockInternalDimensionID internal_dimension_id,
    const DimensionId& dimension_id,
    const SelectionBox& bounds,
    const BlockStack& default_block,
    const Biome& default_biome)
    : _db(std::move(db))
    , _internal_dimension_id(internal_dimension_id)
    , _dimension_id(dimension_id)
    , _bounds(bounds)
    , _default_block(default_block)
    , _default_biome(default_biome)
{
}

BedrockRawDimension::~BedrockRawDimension()
{
    destroy();
}

OrderedMutex& BedrockRawDimension::get_mutex()
{
    return _public_mutex;
}

const DimensionId& BedrockRawDimension::get_dimension_id() const
{
    return _dimension_id;
}

BedrockInternalDimensionID BedrockRawDimension::get_internal_dimension_id() const
{
    return _internal_dimension_id;
}

const SelectionBox& BedrockRawDimension::get_bounds() const
{
    return _bounds;
}

const BlockStack& BedrockRawDimension::get_default_block() const
{
    return _default_block;
}

const Biome& BedrockRawDimension::get_default_biome() const
{
    return _default_biome;
}

// AnvilChunkCoordIterator BedrockRawDimension::all_chunk_coords() const
//{
//     return _anvil_dimension.all_chunk_coords();
// }
//

bool BedrockRawDimension::has_chunk(std::int64_t cx, std::int64_t cz)
{
    throw std::runtime_error("NotImplementedError");
    //     OrderedLockGuard<Amulet::ThreadAccessMode::Read, Amulet::ThreadShareMode::SharedReadWrite> lock(_anvil_dimension.get_mutex());
    //     return _anvil_dimension.has_chunk(cx, cz);
}

void BedrockRawDimension::delete_chunk(std::int64_t cx, std::int64_t cz)
{
    throw std::runtime_error("NotImplementedError");
    //     OrderedLockGuard<Amulet::ThreadAccessMode::ReadWrite, Amulet::ThreadShareMode::SharedReadWrite> lock(_anvil_dimension.get_mutex());
    //     _anvil_dimension.delete_chunk(cx, cz);
}

std::unique_ptr<BedrockRawChunk> BedrockRawDimension::get_raw_chunk(std::int64_t cx, std::int64_t cz)
{
}

void BedrockRawDimension::set_raw_chunk(std::int64_t cx, std::int64_t cz, const BedrockRawChunk& chunk)
{
    throw std::runtime_error("NotImplementedError");
    //     OrderedLockGuard<Amulet::ThreadAccessMode::ReadWrite, Amulet::ThreadShareMode::SharedReadWrite> lock(_anvil_dimension.get_mutex());
    //     _anvil_dimension.set_chunk_data(cx, cz, chunk);
}

std::unique_ptr<BedrockChunk> BedrockRawDimension::get_chunk(std::int64_t cx, std::int64_t cz)
{
    return decode_chunk(*get_raw_chunk(cx, cz), cz, cz);
}

void BedrockRawDimension::set_chunk(std::int64_t cx, std::int64_t cz, BedrockChunk& chunk)
{
    set_raw_chunk(cx, cz, *encode_chunk(chunk, cx, cz));
}

void BedrockRawDimension::destroy()
{
    _destroyed = true;
}

bool BedrockRawDimension::is_destroyed()
{
    return _destroyed;
}

} // namespace Amulet
