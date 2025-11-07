import unittest

from amulet.utils.lock import OrderedLock

from amulet.core.block import BlockStack
from amulet.core.biome import Biome
from amulet.core.selection import SelectionBox

from amulet.level.bedrock import BedrockRawLevel, BedrockRawDimension

from amulet.minecraft_worlds import BedrockLevels, BedrockLevelData, WorldTemp


class BedrockRawDimensionTestCase(unittest.TestCase):
    def test_dimension(self) -> None:
        for src_path in BedrockLevels:
            with self.subTest(src_path=src_path), WorldTemp(src_path) as world_data:
                raw_level = BedrockRawLevel.load(world_data.path)
                with self.assertRaises(RuntimeError):
                    raw_level.dimension_ids
                with self.assertRaises(RuntimeError):
                    raw_level.get_dimension("minecraft:overworld")

                raw_level.open()
                try:
                    overworld = raw_level.get_dimension("minecraft:overworld")
                    nether = raw_level.get_dimension("minecraft:the_nether")

                    self.assertIs(overworld, raw_level.get_dimension(0))

                    for dimension_id in raw_level.dimension_ids:
                        dimension = raw_level.get_dimension(dimension_id)
                        self.assertIsInstance(dimension, BedrockRawDimension)
                        self.assertIsInstance(dimension.dimension_id, str)
                        self.assertEqual(dimension_id, dimension.dimension_id)
                        self.assertIsInstance(dimension.lock, OrderedLock)
                        self.assertIsInstance(dimension.default_block, BlockStack)
                        self.assertIsInstance(dimension.default_biome, Biome)
                        self.assertIsInstance(dimension.bounds, SelectionBox)

                    self.assertEqual(0, overworld.internal_dimension_id)
                    self.assertEqual("minecraft:overworld", overworld.dimension_id)
                    # self.assertEqual(1089, len(list(overworld.all_chunk_coords)))  # TODO
                    self.assertEqual(1, nether.internal_dimension_id)
                    self.assertEqual("minecraft:the_nether", nether.dimension_id)
                    # self.assertEqual(0, len(list(nether.all_chunk_coords)))  # TODO

                    self.assertFalse(overworld.is_destroyed())
                    self.assertFalse(nether.is_destroyed())
                finally:
                    raw_level.close()

                self.assertTrue(overworld.is_destroyed())
                self.assertTrue(nether.is_destroyed())

    @unittest.skip("TODO")
    def test_edit(self) -> None:
        pass
