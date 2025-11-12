#include <bit>
#include <variant>

#include <amulet/leveldb.hpp>
#include <leveldb/write_batch.h>

#include <amulet/nbt/nbt_encoding/binary.hpp>

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

// Arbitrary tag type start.
// This allows us to skip over other dimensions without losing keys.
// Reduce this number if tags are added before this.
static const char MinTag = 0x10;

static void for_keys_in_chunk(LevelDB& _db, std::string key_prefix, std::function<void(const leveldb::Slice&)> callback)
{
    {
        auto it_ptr = _db.create_iterator();
        auto& it = it_ptr->get_iterator();

        auto key_start = key_prefix + MinTag;
        auto key_end = key_prefix + "\xFF\xFF";

        it.Seek(key_start);
        while (it.Valid()) {
            const auto& key = it.key();
            if (0 <= it.key().compare(key_end)) {
                break;
            }
            if (key_start.size() == key.size() || key_end.size() == key.size()) {
                callback(key);
            }
            it.Next();
        }
    }

    auto& db = _db.get_database();

    {
        auto& read_options = _db.get_read_options();
        std::string digp_key = "digp" + key_prefix;
        callback(digp_key);
        std::string digp;
        if (db.Get(read_options, digp_key, &digp).ok()) {
            size_t actor_count = (digp.size() / 8) * 8;
            for (size_t i = 0; i < actor_count; i += 8) {
                std::string actor_key;
                actor_key.reserve(19);
                actor_key = "actorprefix";
                actor_key += std::string_view(digp.data() + i, 8);

                callback(actor_key);
            }
        }
    }
}

void BedrockRawDimension::delete_chunk(std::int32_t cx, std::int32_t cz)
{
    auto key_prefix = get_key_prefix(_internal_dimension_id, cx, cz);

    leveldb::WriteBatch batch;
    for_keys_in_chunk(
        *_db,
        key_prefix,
        [&batch](const leveldb::Slice& key) {
            batch.Delete(key);
        });

    _db->get_database().Write(_db->get_write_options(), &batch);
}

BedrockRawChunk BedrockRawDimension::get_raw_chunk(std::int32_t cx, std::int32_t cz)
{
    auto key_prefix = get_key_prefix(_internal_dimension_id, cx, cz);
    std::map<Bytes, Bytes> data;

    {
        auto it_ptr = _db->create_iterator();
        auto& it = it_ptr->get_iterator();

        auto key_start = key_prefix + MinTag;
        auto key_end = key_prefix + "\xFF\xFF";

        it.Seek(key_start);
        while (it.Valid()) {
            const auto& key = it.key();
            if (0 <= it.key().compare(key_end)) {
                break;
            }
            if (key_start.size() == key.size() || key_end.size() == key.size()) {
                auto value = it.value();
                data.emplace(
                    Bytes(key.begin() + key_prefix.size(), key.size() - key_prefix.size()),
                    Bytes(value.data(), value.size()));
            }
            it.Next();
        }
    }

    std::vector<std::shared_ptr<NBT::NamedTag>> actors;

    {
        auto& db = _db->get_database();
        auto& read_options = _db->get_read_options();
        std::string digp_key = "digp" + key_prefix;
        std::string digp;
        if (db.Get(read_options, digp_key, &digp).ok()) {
            size_t actor_count = (digp.size() / 8) * 8;
            for (size_t i = 0; i < actor_count; i += 8) {
                std::string actor_key;
                actor_key.reserve(19);
                actor_key = "actorprefix";
                actor_key += std::string_view(digp.data() + i, 8);

                std::string actor_bytes;
                if (!db.Get(read_options, actor_key, &actor_bytes).ok()) {
                    error("Could not find actor " + actor_key + ". Skipping.");
                    continue;
                }

                std::shared_ptr<NBT::NamedTag> actor;
                try {
                    actor = std::make_shared<NBT::NamedTag>(
                        NBT::decode_nbt(actor_bytes, std::endian::little, NBT::utf8_to_utf8_escape));
                } catch (...) {
                    error("Failed to parse actor " + actor_key + ". Skipping.");
                    continue;
                }

                auto* actor_tag_ptr = std::get_if<NBT::CompoundTagPtr>(&actor->tag_node);
                if (!actor_tag_ptr) {
                    error("Actor " + actor_key + " is not a CompoundTag. Skipping.");
                    continue;
                }
                auto& actor_tag = **actor_tag_ptr;

                // Remove internal tags if they exist.
                actor_tag.erase("UniqueID");
                actor_tag.erase("internalComponents");

                actors.emplace_back(std::move(actor));
            }
        }
    }

    return BedrockRawChunk(std::move(data), std::move(actors));
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
