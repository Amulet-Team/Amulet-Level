#include <cstdint>
#include <map>
#include <memory>
#include <stdexcept>

#include <amulet/nbt/tag/named_tag.hpp>

#include <amulet/core/chunk/chunk.hpp>

#include "chunk.hpp"
#include "raw_dimension.hpp"

namespace Amulet {

JavaRawChunk JavaRawDimension::encode_chunk(
    JavaChunk& chunk,
    std::int64_t cx,
    std::int64_t cz)
{
    throw std::runtime_error("");
    // floor_cy = bounds[0] >> 4
    // height_cy = (bounds[1] - bounds[0]) >> 4
    // ceil_cy = floor_cy + height_cy

    // data = chunk.misc.get("_java_chunk_data_layers", None)
    // if not isinstance(data, dict):
    //     data = {}
    // data = {
    //     key: value
    //     for key, value in data.items()
    //     if isinstance(key, str) and isinstance(value, NamedTag)
    // }

    // DataVersion = data_version

    // LongArrayDense = DataVersion < 2529

    // # Version tag
    // if 0 <= DataVersion:
    //     set_layer_obj(
    //         data,
    //         (
    //             "region",
    //             [("DataVersion", IntTag)],
    //             IntTag,
    //         ),
    //         IntTag(max_world_version[1])
    //     )
    // else:
    //     V = (
    //         "region",
    //         [("Level", CompoundTag), ("V", ByteTag)],
    //         ByteTag(1),
    //     )
    //     set_layer_obj(data, V, ByteTag(chunk.misc.get("V", 1)))

    // # Coords
    // if 2844 <= DataVersion:
    //     xPos = ("region", [("xPos", IntTag)], IntTag)
    //     zPos = ("region", [("zPos", IntTag)], IntTag)
    //     set_layer_obj(data, ("region", [("yPos", IntTag)], IntTag), IntTag(floor_cy))
    // else:
    //     xPos = (
    //         "region",
    //         [("Level", CompoundTag), ("xPos", IntTag)],
    //         IntTag,
    //     )
    //     zPos = (
    //         "region",
    //         [("Level", CompoundTag), ("zPos", IntTag)],
    //         IntTag,
    //     )
    // set_layer_obj(data, xPos, IntTag(chunk.cx))
    // set_layer_obj(data, zPos, IntTag(chunk.cz))

    // def _get_encode_sections() -> dict[int, CompoundTag]:
    //     """Get or create the section array populating all valid sections"""
    //     if 2844 <= DataVersion:
    //         Sections = ("region", [("sections", ListTag)], ListTag)
    //     else:
    //         Sections = (
    //             "region",
    //             [("Level", CompoundTag), ("Sections", ListTag)],
    //             ListTag,
    //         )
    //     sections: ListTag = set_layer_obj(data, Sections, setdefault=True)
    //     section_map: dict[int, CompoundTag] = {}
    //     section: CompoundTag
    //     for section_index in range(len(sections) - 1, -1, -1):
    //         section = sections[section_index]
    //         cy = section.get("Y", None)
    //         if isinstance(cy, ByteTag):
    //             section_map[cy.py_int] = section
    //         else:
    //             sections.pop(section_index)
    //     for cy in range(floor_cy, ceil_cy):
    //         if cy not in section_map:
    //             section = section_map[cy] = CompoundTag({"Y": ByteTag(cy)})
    //             sections.append(section)
    //     return section_map

    // sections = _get_encode_sections()

    // for cy in chunk.blocks.sub_chunks:
    //     if floor_cy <= cy < ceil_cy:
    //         if 1444 <= DataVersion:
    //             block_sub_array = numpy.transpose(
    //                 chunk.blocks.get_sub_chunk(cy), (1, 2, 0)
    //             ).ravel()
    //             sub_palette_, block_sub_array = numpy.unique(
    //                 block_sub_array, return_inverse=True
    //             )
    //             sub_palette = _encode_block_palette(block_palette[sub_palette_])

    //            if (
    //                DataVersion < 2844
    //                and len(sub_palette) == 1
    //                and sub_palette[0].get_string("Name").py_str == "minecraft:air"
    //            ):
    //                # TODO: do we need to save this in 2844+?
    //                continue

    //            section = sections.setdefault(cy, CompoundTag())

    //            if 2844 <= DataVersion:
    //                block_states = section["block_states"] = CompoundTag({"palette": sub_palette})
    //                if len(sub_palette) != 1:
    //                    block_states["data"] = LongArrayTag(
    //                        encode_long_array(
    //                            block_sub_array, dense=LongArrayDense, min_bits_per_entry=4
    //                        )
    //                    )
    //            elif 1444 <= DataVersion:
    //                section["BlockStates"] = LongArrayTag(
    //                    encode_long_array(
    //                        block_sub_array, dense=LongArrayDense, min_bits_per_entry=4
    //                    )
    //                )
    //                section["Palette"] = sub_palette
    //        else:
    //            block_sub_array = palette[
    //                numpy.transpose(
    //                    chunk.blocks.get_sub_chunk(cy), (1, 2, 0)
    //                ).ravel()  # XYZ -> YZX
    //            ]

    //            data_sub_array = block_sub_array[:, 1]
    //            block_sub_array = block_sub_array[:, 0]
    //            # if not numpy.any(block_sub_array) and not numpy.any(data_sub_array):
    //            #     return False
    //            sections[cy]["Blocks"] = ByteArrayTag(block_sub_array.astype("uint8"))
    //            sections[cy]["Data"] = ByteArrayTag(world_utils.to_nibble_array(data_sub_array))

    // if 2844 <= DataVersion:
    //     BlockEntities = ("region", [("block_entities", ListTag)], ListTag)
    // else:
    //     BlockEntities = (
    //         "region",
    //         [("Level", CompoundTag), ("TileEntities", ListTag)],
    //         ListTag,
    //     )
    // encoded_block_entities = []
    // for entity in chunk.block_entities:
    //     nbt = self._encode_block_entity(
    //         entity,
    //         EntityIDType.namespace_str_id,
    //         EntityCoordType.xyz_int,
    //     )
    //     if nbt is not None:
    //         encoded_block_entities.append(nbt.compound)
    // set_layer_obj(
    //     data,
    //     BlockEntities,
    //     ListTag(encoded_block_entities)
    //)

    // if amulet.entity_support:
    //     entities = chunk.entities
    // else:
    //     entities = chunk._native_entities

    // def _encode_entity_list(entities: Iterable["Entity"]) -> ListTag:
    //     entities_out = []
    //     for entity in entities:
    //         nbt = self._encode_entity(
    //             entity,
    //             EntityIDType.namespace_str_id,
    //             EntityCoordType.Pos_list_double,
    //         )
    //         if nbt is not None:
    //             entities_out.append(nbt.compound)

    //    return ListTag(entities_out)

    // encoded_entities = _encode_entity_list(entities)
    // if 2681 <= DataVersion:
    //     # TODO: it is possible the entity data version does not match the chunk data version
    //     Entities = (
    //         "entities",
    //         [("Entities", ListTag)],
    //         ListTag,
    //     )
    //     EntitiesDataVersion = (
    //         "entities",
    //         [("DataVersion", IntTag)],
    //         IntTag,
    //     )

    //    try:
    //        platform, version = chunk._native_version
    //    except:
    //        data.pop(EntitiesDataVersion[0], None)
    //    else:
    //        if platform == "java" and isinstance(version, int):
    //            set_layer_obj(
    //                data,
    //                Entities,
    //                encoded_entities,
    //            )
    //            set_layer_obj(data, EntitiesDataVersion, IntTag(version))
    //        else:
    //            data.pop(EntitiesDataVersion[0], None)
    // else:
    //    Entities = (
    //        "region",
    //        [("Level", CompoundTag), ("Entities", ListTag)],
    //        ListTag,
    //    )
    //    set_layer_obj(
    //        data,
    //        Entities,
    //        encoded_entities,
    //    )

    // if 2844 <= DataVersion:
    //     BlockTicks = ("region", [("block_ticks", ListTag)], ListTag)
    // else:
    //     BlockTicks = (
    //         "region",
    //         [("Level", CompoundTag), ("TileTicks", ListTag)],
    //         ListTag,
    //     )
    // set_layer_obj(
    //     data, BlockTicks, _encode_ticks(chunk.misc.get("block_ticks", {}))
    //)

    // if 1444 <= DataVersion < 2844:
    //     ToBeTicked = (
    //         "region",
    //         [("Level", CompoundTag), ("ToBeTicked", ListTag)],
    //         ListTag,
    //     )
    //     set_layer_obj(
    //         data,
    //         ToBeTicked,
    //         _encode_to_be_ticked(
    //             chunk.misc.get("to_be_ticked"), floor_cy, height_cy
    //         ),
    //     )

    // if 1444 <= DataVersion:
    //     if 2844 <= DataVersion:
    //         LiquidTicks = ("region", [("fluid_ticks", ListTag)], ListTag)
    //     else:
    //         LiquidTicks = (
    //             "region",
    //             [("Level", CompoundTag), ("LiquidTicks", ListTag)],
    //             ListTag,
    //         )
    //     set_layer_obj(
    //         data,
    //         LiquidTicks,
    //         _encode_ticks(chunk.misc.get("fluid_ticks", {})),
    //     )

    // if 1444 <= DataVersion < 2844:
    //     LiquidsToBeTicked = (
    //         "region",
    //         [("Level", CompoundTag), ("LiquidsToBeTicked", ListTag)],
    //         ListTag,
    //     )
    //     set_layer_obj(
    //         data,
    //         LiquidsToBeTicked,
    //         _encode_to_be_ticked(
    //             chunk.misc.get("liquids_to_be_ticked"), floor_cy, height_cy
    //         ),
    //     )

    // if 1444 <= DataVersion:
    //     if 2844 <= DataVersion:
    //         PostProcessing = ("region", [("PostProcessing", ListTag)], ListTag)
    //     else:
    //         PostProcessing = (
    //             "region",
    //             [("Level", CompoundTag), ("PostProcessing", ListTag)],
    //             ListTag,
    //         )
    //     set_layer_obj(
    //         data,
    //         PostProcessing,
    //         _encode_to_be_ticked(
    //             chunk.misc.get("post_processing"), floor_cy, height_cy
    //         ),
    //     )

    // if 2844 <= DataVersion:
    //     chunk.biomes.convert_to_3d()
    //     for cy in chunk.biomes.sections:
    //         if floor_cy <= cy < ceil_cy:
    //             biome_sub_array = numpy.transpose(
    //                 chunk.biomes.get_section(cy), (1, 2, 0)
    //             ).ravel()

    //            sub_palette_, biome_sub_array = numpy.unique(
    //                biome_sub_array, return_inverse=True
    //            )
    //            sub_palette = ListTag([StringTag(entry) for entry in chunk.biome_palette[sub_palette_]])
    //            biomes = sections[cy]["biomes"] = CompoundTag({"palette": sub_palette})
    //            if len(sub_palette) != 1:
    //                biomes["data"] = LongArrayTag(
    //                    encode_long_array(biome_sub_array, dense=LongArrayDense)
    //                )

    // elif 2203 <= DataVersion:
    //     Biomes = (
    //         "region",
    //         [("Level", CompoundTag), ("Biomes", IntArrayTag)],
    //         None,
    //     )
    //     if chunk.status.value > -0.7:
    //         chunk.biomes.convert_to_3d()
    //         set_layer_obj(
    //             data,
    //             Biomes,
    //             IntArrayTag(
    //                 numpy.transpose(
    //                     numpy.asarray(
    //                         chunk.biomes[
    //                             :, floor_cy * 4 : ceil_cy * 4, :
    //                         ]
    //                     ).astype(numpy.uint32),
    //                     (1, 2, 0),
    //                 ).ravel()  # YZX -> XYZ
    //             ),
    //         )
    // elif 1467 <= DataVersion:
    //     Biomes = (
    //         "region",
    //         [("Level", CompoundTag), ("Biomes", IntArrayTag)],
    //         None,
    //     )
    //     if chunk.status.value > -0.7:
    //         chunk.biomes.convert_to_2d()
    //         set_layer_obj(
    //             data,
    //             Biomes,
    //             IntArrayTag(chunk.biomes.astype(dtype=numpy.uint32).ravel()),
    //         )
    // else:
    //     Biomes = (
    //         "region",
    //         [("Level", CompoundTag), ("Biomes", ByteArrayTag)],
    //         None,
    //     )
    //     chunk.biomes.convert_to_2d()
    //     set_layer_obj(
    //         data,
    //         Biomes,
    //         ByteArrayTag(chunk.biomes.astype(dtype=numpy.uint8).ravel()),
    //     )

    // if 1934 <= DataVersion:
    //     if 2844 <= DataVersion:
    //         isLightOn = ("region", [("isLightOn", ByteTag)], ByteTag)
    //     else:
    //         isLightOn = ("region", [("Level", CompoundTag), ("isLightOn", ByteTag)], ByteTag)
    //     is_light_on = bool(chunk.misc.pop("isLightOn", None))
    //     set_layer_obj(data, isLightOn, ByteTag(is_light_on))

    // def _pack_light(
    //     feature_key: str,
    //     section_key: str,
    //):
    //     light_container = chunk.misc.get(feature_key, {})
    //     if not isinstance(light_container, dict):
    //         light_container = {}
    //     for cy, section in sections.items():
    //         light = light_container.get(cy, None)
    //         if (
    //             isinstance(light, numpy.ndarray)
    //             and numpy.issubdtype(light.dtype, numpy.integer)
    //             and light.shape == (16, 16, 16)
    //         ):
    //             light = light.ravel() % 16
    //             section[section_key] = ByteArrayTag(light[::2] + (light[1::2] << 4))
    //         elif DataVersion < 1934:
    //             # light is optional after 1934
    //             section[section_key] = ByteArrayTag(
    //                 numpy.full(2048, 255, dtype=numpy.uint8)
    //             )

    //_pack_light("block_light", "BlockLight")
    //_pack_light("sky_light", "SkyLight")

    // if 1466 <= DataVersion:
    //     if 2844 <= DataVersion:
    //         Heightmaps = ("region", [("Heightmaps", CompoundTag)], CompoundTag)
    //     else:
    //         Heightmaps = (
    //             "region",
    //             [("Level", CompoundTag), ("Heightmaps", CompoundTag)],
    //             CompoundTag,
    //         )
    //     maps = [
    //         "WORLD_SURFACE_WG",
    //         "OCEAN_FLOOR_WG",
    //         "MOTION_BLOCKING",
    //         "MOTION_BLOCKING_NO_LEAVES",
    //         "OCEAN_FLOOR",
    //     ]
    //     if 1908 <= DataVersion:
    //         maps.append("WORLD_SURFACE")
    //     elif 1503 <= DataVersion:
    //         maps.append("LIGHT_BLOCKING")
    //         maps.append("WORLD_SURFACE")
    //     elif 1484 <= DataVersion:
    //         maps.append("LIGHT_BLOCKING")
    //     else:
    //         maps = ("LIQUID", "SOLID", "LIGHT", "RAIN")
    //     heightmaps_temp: dict[str, numpy.ndarray] = chunk.misc.get("height_mapC", {})
    //     heightmaps = CompoundTag()
    //     for heightmap in maps:
    //         if (
    //                 heightmap in heightmaps_temp
    //                 and isinstance(heightmaps_temp[heightmap], numpy.ndarray)
    //                 and heightmaps_temp[heightmap].size == 256
    //         ):
    //             heightmaps[heightmap] = LongArrayTag(
    //                 encode_long_array(
    //                     heightmaps_temp[heightmap].ravel() - (floor_cy << 4),
    //                     (height_cy << 4).bit_length(),
    //                     LongArrayDense,
    //                 )
    //             )
    //     set_layer_obj(data, Heightmaps, heightmaps)
    // else:
    //     HeightMap = (
    //         "region",
    //         [("Level", CompoundTag), ("HeightMap", IntArrayTag)],
    //         IntArrayTag,
    //     )
    //     height = chunk.misc.get("height_map256IA", None)
    //     if (
    //         isinstance(height, numpy.ndarray)
    //         and numpy.issubdtype(height.dtype, numpy.integer)
    //         and height.shape == (16, 16)
    //     ):
    //         set_layer_obj(
    //             data,
    //             HeightMap,
    //             IntArrayTag(numpy.zeros(256, dtype=numpy.uint32)),
    //         )
    //     elif self._features["height_map"] == "256IARequired":
    //         set_layer_obj(data, HeightMap, IntArrayTag(height.ravel()))

    // if 2844 <= DataVersion:
    //     LastUpdate = ("region", [("LastUpdate", LongTag)], LongTag)
    // else:
    //     LastUpdate = (
    //         "region",
    //         [("Level", CompoundTag), ("LastUpdate", LongTag)],
    //         LongTag,
    //     )
    // set_layer_obj(
    //     data, LastUpdate, LongTag(chunk.misc.get("last_update", 0))
    //)

    // if 1444 <= DataVersion:
    //     if 2844 <= DataVersion:
    //         Status = ("region", [("Status", StringTag)], StringTag("full"))
    //     else:
    //         Status = (
    //             "region",
    //             [("Level", CompoundTag), ("Status", StringTag)],
    //             StringTag("full"),
    //         )
    //     # Order the float value based on the order they would be run. Newer replacements for the same come just after
    //     # to save back find the next lowest valid value.
    //     status = chunk.status.as_type(self._features["status"])
    //     set_layer_obj(data, Status, StringTag(status))
    // else:
    //     LightPopulated = (
    //         "region",
    //         [("Level", CompoundTag), ("LightPopulated", ByteTag)],
    //         ByteTag,
    //     )
    //     TerrainPopulated = (
    //         "region",
    //         [("Level", CompoundTag), ("TerrainPopulated", ByteTag)],
    //         ByteTag,
    //     )
    //     status = chunk.status.as_type(StatusFormats.Raw)
    //     set_layer_obj(data, TerrainPopulated, ByteTag(int(status > -0.3)))
    //     set_layer_obj(data, LightPopulated, ByteTag(int(status > -0.2)))

    // if 2844 <= DataVersion:
    //     InhabitedTime = ("region", [("InhabitedTime", LongTag)], LongTag)
    // else:
    //     InhabitedTime = (
    //         "region",
    //         [("Level", CompoundTag), ("InhabitedTime", LongTag)],
    //         LongTag,
    //     )
    // set_layer_obj(
    //     data, InhabitedTime, LongTag(chunk.misc.get("inhabited_time", 0))
    //)

    // if 1444 <= DataVersion:
    //     if 2844 <= DataVersion:
    //         Structures = ("region", [("structures", CompoundTag)], CompoundTag)
    //     else:
    //         Structures = (
    //             "region",
    //             [("Level", CompoundTag), ("Structures", CompoundTag)],
    //             CompoundTag,
    //         )
    //     set_layer_obj(
    //         data,
    //         Structures,
    //         chunk.misc.get(
    //             "structures",
    //             CompoundTag(
    //                 {
    //                     "References": CompoundTag(),
    //                     "Starts": CompoundTag(),
    //                 }
    //             ),
    //         ),
    //     )

    // # TODO: What is going on here?
    // #  This is implemented incorrectly
    // if DataVersion < 1901:
    //     Sections = (
    //         "region",
    //         [("Level", CompoundTag), ("Sections", ListTag)],
    //         ListTag,
    //     )
    //     if 1519 <= DataVersion:
    //         sections = get_layer_obj(data, Sections)
    //         for section_index in range(len(sections) - 1, -1, -1):
    //             if (
    //                     "BlockStates" not in sections[section_index]
    //                     or "Palette" not in sections[section_index]
    //             ):
    //                 del sections[section_index]
    //     else:
    //         # Strip out all empty sections
    //         sections = get_layer_obj(data, Sections)
    //         if sections:
    //             for i in range(len(sections) - 1, -1, -1):
    //                 section = sections[i]
    //                 if "Blocks" not in section or "Data" not in section:
    //                     # in 1.12 if a section exists, Blocks and Data must exist
    //                     sections.pop(i)
    //         if not sections:
    //             # if no sections remain we can remove the sections data
    //             get_layer_obj(data, Sections, pop_last=True)

    // if 2844 <= DataVersion:
    //     OldLevel = ("region", [("Level", CompoundTag)], CompoundTag)
    //     get_layer_obj(data, OldLevel, pop_last=True)

    // return data
}

} // namespace Amulet
