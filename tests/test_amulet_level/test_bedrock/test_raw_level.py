import unittest

from PIL import Image

from amulet.utils.lock import OrderedLock
from amulet.utils.event import Event

from amulet.core.version import VersionNumber

from amulet.level.bedrock import BedrockRawLevel

from amulet.minecraft_worlds import WorldTemp, BedrockLevels, BedrockLevelData


class BedrockRawLevelTest(unittest.TestCase):
    def test_load(self) -> None:
        for src_path in BedrockLevels:
            with WorldTemp(src_path) as world_data:
                raw_level = BedrockRawLevel.load(world_data.path)
                self.assertIsInstance(raw_level, BedrockRawLevel)

    def test_metadata(self) -> None:
        for src_path in BedrockLevels:
            with WorldTemp(src_path) as world_data:
                level_data = world_data.level_data
                assert isinstance(level_data, BedrockLevelData)

                raw_level = BedrockRawLevel.load(world_data.path)
                self.assertIsInstance(raw_level, BedrockRawLevel)
                self.assertIsInstance(raw_level.lock, OrderedLock)
                self.assertTrue(raw_level.is_supported())
                self.assertEqual(level_data.modified_time, raw_level.modified_time.timestamp())
                self.assertEqual("bedrock", raw_level.platform)
                self.assertEqual(VersionNumber(*level_data.max_version), raw_level.last_opened_version)
                self.assertEqual(world_data.path, raw_level.path)
                self.assertEqual(level_data.level_name, raw_level.level_name)
                thumbnail = raw_level.thumbnail
                self.assertIsInstance(thumbnail, Image.Image)
                thumbnail.close()
                self.assertIsInstance(raw_level.opened, Event)
                self.assertIsInstance(raw_level.closed, Event)
                self.assertIsInstance(raw_level.reloaded, Event)
                with self.assertRaises(RuntimeError):
                    raw_level.reload()
