from typing import Any
from unittest import TestCase
from collections.abc import MutableMapping

from amulet.nbt import NamedTag, CompoundTag, ByteTag

from amulet.core.version import VersionNumber
from amulet.core.block import Block, BlockStack
from amulet.core.biome import Biome
from amulet.core.chunk import Chunk
from amulet.core.chunk.component import (
    BlockComponent,
    BlockComponentData,
    SectionArrayMap,
)
from amulet.core.palette.block_palette import BlockPalette

from amulet.level.java.chunk import (
    JavaChunkNA,
    JavaChunk0,
    JavaChunk1444,
    JavaChunk1466,
    JavaChunk2203,
)
from amulet.level.java.chunk_components import (
    JavaRawChunkComponent,
    DataVersionComponent,
)


def test_data_version_component(self: TestCase, chunk: Chunk) -> None:
    self.assertIsInstance(chunk, DataVersionComponent)
    assert isinstance(chunk, DataVersionComponent)
    self.assertIsInstance(chunk.data_version, int)
    with self.assertRaises(AttributeError):
        chunk.data_version = 0  # type: ignore


def test_java_raw_chunk_component(self: TestCase, chunk: Chunk) -> None:
    self.assertIsInstance(chunk, JavaRawChunkComponent)
    assert isinstance(chunk, JavaRawChunkComponent)
    self.assertIsInstance(chunk.raw_data, MutableMapping)
    raw_data = chunk.raw_data
    self.assertEqual(raw_data, raw_data)
    self.assertEqual(raw_data, chunk.raw_data)
    raw_data["test"] = NamedTag(CompoundTag(a=ByteTag(1)))
    raw_data["test2"] = NamedTag(ByteTag(2))

    self.assertEqual({"test", "test2"}, set(chunk.raw_data))
    self.assertEqual(
        {"test": NamedTag(CompoundTag(a=ByteTag(1))), "test2": NamedTag(ByteTag(2))},
        dict(chunk.raw_data),
    )

    chunk.raw_data = {}

    self.assertEqual({}, dict(chunk.raw_data))
    self.assertEqual(
        {"test": NamedTag(CompoundTag(a=ByteTag(1))), "test2": NamedTag(ByteTag(2))},
        dict(raw_data),
    )

    chunk.raw_data = raw_data
    self.assertEqual(
        {"test": NamedTag(CompoundTag(a=ByteTag(1))), "test2": NamedTag(ByteTag(2))},
        dict(chunk.raw_data),
    )

    chunk.raw_data = {
        "test": NamedTag(ByteTag(2)),
        "test2": NamedTag(CompoundTag(a=ByteTag(1))),
    }
    self.assertEqual(
        {"test": NamedTag(ByteTag(2)), "test2": NamedTag(CompoundTag(a=ByteTag(1)))},
        dict(chunk.raw_data),
    )


class JavaChunkTestCase(TestCase):
    def _test_component(self, component: Any) -> None:
        self.assertIsInstance(component.ComponentID, str)

    def _test_block_component(self, chunk: Chunk) -> None:
        self.assertIsInstance(chunk, BlockComponent)
        assert isinstance(chunk, BlockComponent)
        self.assertIsInstance(chunk.block, BlockComponentData)
        self.assertIsInstance(chunk.block.palette, BlockPalette)
        self.assertIsInstance(chunk.block.sections, SectionArrayMap)

    def test_java_raw_chunk_component(self) -> None:
        self._test_component(JavaRawChunkComponent)

    def test_data_version_component(self) -> None:
        self._test_component(DataVersionComponent)

    def test_java_chunk_na(self) -> None:
        chunk = JavaChunkNA(
            BlockStack(Block("java", VersionNumber(-1), "minecraft", "stone")),
            Biome("java", VersionNumber(-1), "minecraft", "plains"),
        )
        self.assertIsInstance(chunk, Chunk)
        test_java_raw_chunk_component(self, chunk)
        test_data_version_component(self, chunk)
        # self.assertIsInstance(chunk, LastUpdateComponent)
        # self.assertIsInstance(chunk, JavaLegacyVersionComponent)
        self._test_block_component(chunk)
        # self.assertIsInstance(chunk, BlockEntityComponent)
        # self.assertIsInstance(chunk, EntityComponent)
        # self.assertIsInstance(chunk, Biome2DComponent)
        # self.assertIsInstance(chunk, Height2DComponent)

    def test_java_chunk_0(self) -> None:
        chunk = JavaChunk0(
            1443,  # any int from 0-1443
            BlockStack(Block("java", VersionNumber(1443), "minecraft", "stone")),
            Biome("java", VersionNumber(1443), "minecraft", "plains"),
        )
        self.assertIsInstance(chunk, Chunk)
        test_java_raw_chunk_component(self, chunk)
        test_data_version_component(self, chunk)
        # self.assertIsInstance(chunk, LastUpdateComponent)
        # self.assertIsInstance(chunk, TerrainPopulatedComponent)
        # self.assertIsInstance(chunk, LightPopulatedComponent)
        self._test_block_component(chunk)
        # self.assertIsInstance(chunk, BlockEntityComponent)
        # self.assertIsInstance(chunk, EntityComponent)
        # self.assertIsInstance(chunk, Biome2DComponent)
        # self.assertIsInstance(chunk, Height2DComponent)

    def test_java_chunk_1444(self) -> None:
        chunk = JavaChunk1444(
            1465,  # any int from 1444-1465
            BlockStack(Block("java", VersionNumber(1465), "minecraft", "stone")),
            Biome("java", VersionNumber(1465), "minecraft", "plains"),
        )
        self.assertIsInstance(chunk, Chunk)
        test_java_raw_chunk_component(self, chunk)
        test_data_version_component(self, chunk)
        # self.assertIsInstance(chunk, LastUpdateComponent)
        # self.assertIsInstance(chunk, StatusStringComponent)
        self._test_block_component(chunk)
        # self.assertIsInstance(chunk, BlockEntityComponent)
        # self.assertIsInstance(chunk, EntityComponent)
        # self.assertIsInstance(chunk, Biome2DComponent)
        # self.assertIsInstance(chunk, Height2DComponent)

    def test_java_chunk_1466(self) -> None:
        chunk = JavaChunk1466(
            2202,  # any int from 1466-2202
            BlockStack(Block("java", VersionNumber(2202), "minecraft", "stone")),
            Biome("java", VersionNumber(2202), "minecraft", "plains"),
        )
        self.assertIsInstance(chunk, Chunk)
        test_java_raw_chunk_component(self, chunk)
        test_data_version_component(self, chunk)
        # self.assertIsInstance(chunk, LastUpdateComponent)
        # self.assertIsInstance(chunk, StatusStringComponent)
        self._test_block_component(chunk)
        # self.assertIsInstance(chunk, BlockEntityComponent)
        # self.assertIsInstance(chunk, EntityComponent)
        # self.assertIsInstance(chunk, Biome2DComponent)
        # self.assertIsInstance(chunk, NamedHeight2DComponent)

    def test_java_chunk_2203(self) -> None:
        chunk = JavaChunk2203(
            2203,  # any int from 1466-?
            BlockStack(Block("java", VersionNumber(2203), "minecraft", "stone")),
            Biome("java", VersionNumber(2203), "minecraft", "plains"),
        )
        self.assertIsInstance(chunk, Chunk)
        test_java_raw_chunk_component(self, chunk)
        test_data_version_component(self, chunk)
        # self.assertIsInstance(chunk, LastUpdateComponent)
        # self.assertIsInstance(chunk, StatusStringComponent)
        self._test_block_component(chunk)
        # self.assertIsInstance(chunk, BlockEntityComponent)
        # self.assertIsInstance(chunk, EntityComponent)
        # self.assertIsInstance(chunk, Biome3DComponent)
        # self.assertIsInstance(chunk, NamedHeight2DComponent)
