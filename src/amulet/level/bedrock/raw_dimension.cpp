#include <bit>

#include "raw_dimension.hpp"

namespace {
class KeyWriter {
private:
    std::string& _data;

public:
    KeyWriter(std::string& data)
        : _data(data)
    {
    }

    // Fix the endianness of the numeric value and write it to the buffer.
    template <typename T>
    void write_numeric(const T& value)
    {
        if constexpr (std::endian::native == std::endian::little) {
            _data.append((char*)&value, sizeof(T));
        } else {
            const size_t data_size = _data.size() + sizeof(T);
            _data.resize(data_size);
            char* src = (char*)&value;
            char* dst = _data.data() + data_size;
            for (size_t i = 0; i < sizeof(T); i++) {
                *(dst - i - 1) = *(src + i);
            }
        }
    }
};
}

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

static std::string get_key_prefix(std::int32_t dimension, std::int32_t cx, std::int32_t cz)
{
    std::string key;
    KeyWriter writer(key);
    writer.write_numeric<std::int32_t>(cx);
    writer.write_numeric<std::int32_t>(cz);
    if (dimension != 0) {
        writer.write_numeric<std::int32_t>(dimension);
    }
    return key;
}

bool BedrockRawDimension::has_chunk(std::int32_t cx, std::int32_t cz)
{
    auto& db = _db->get_database();
    std::string value;
    auto key_prefix = get_key_prefix(_internal_dimension_id, cx, cz);
    return db.Get(_db->get_read_options(), key_prefix + ',', &value).ok() || db.Get(_db->get_read_options(), key_prefix + 'v', &value).ok();
}

void BedrockRawDimension::delete_chunk(std::int32_t cx, std::int32_t cz)
{
    throw std::runtime_error("NotImplementedError");
    //     OrderedLockGuard<Amulet::ThreadAccessMode::ReadWrite, Amulet::ThreadShareMode::SharedReadWrite> lock(_anvil_dimension.get_mutex());
    //     _anvil_dimension.delete_chunk(cx, cz);
}

BedrockRawChunk BedrockRawDimension::get_raw_chunk(std::int32_t cx, std::int32_t cz)
{
}

void BedrockRawDimension::set_raw_chunk(std::int32_t cx, std::int32_t cz, const BedrockRawChunk& chunk)
{
    throw std::runtime_error("NotImplementedError");
    //     OrderedLockGuard<Amulet::ThreadAccessMode::ReadWrite, Amulet::ThreadShareMode::SharedReadWrite> lock(_anvil_dimension.get_mutex());
    //     _anvil_dimension.set_chunk_data(cx, cz, chunk);
}

std::unique_ptr<BedrockChunk> BedrockRawDimension::get_chunk(std::int32_t cx, std::int32_t cz)
{
    return decode_chunk(get_raw_chunk(cx, cz), cz, cz);
}

void BedrockRawDimension::set_chunk(std::int32_t cx, std::int32_t cz, BedrockChunk& chunk)
{
    set_raw_chunk(cx, cz, encode_chunk(chunk, cx, cz));
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
