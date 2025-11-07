import unittest

from PIL import Image

from amulet.utils.lock import OrderedLock
from amulet.utils.event import Event

from amulet.nbt import StringTag

from amulet.core.version import VersionNumber

from amulet.level.bedrock import BedrockRawLevel, BedrockLevelDat

from amulet.minecraft_worlds import WorldTemp, BedrockLevels, BedrockLevelData


class BedrockRawLevelTest(unittest.TestCase):
    def test_load(self) -> None:
        for src_path in BedrockLevels:
            with WorldTemp(src_path) as world_data:
                raw_level = BedrockRawLevel.load(world_data.path)
                self.assertIsInstance(raw_level, BedrockRawLevel)

    @unittest.skip("TODO")
    def test_create(self) -> None:
        pass

    def test_metadata(self) -> None:
        for src_path in BedrockLevels:
            with self.subTest(src_path=src_path), WorldTemp(src_path) as world_data:
                level_data = world_data.level_data
                assert isinstance(level_data, BedrockLevelData)

                raw_level = BedrockRawLevel.load(world_data.path)
                self.assertIsInstance(raw_level, BedrockRawLevel)
                self.assertIsInstance(raw_level.lock, OrderedLock)
                self.assertTrue(raw_level.is_supported())
                self.assertEqual(
                    level_data.modified_time, raw_level.modified_time.timestamp()
                )
                self.assertEqual("bedrock", raw_level.platform)
                self.assertEqual(
                    VersionNumber(*level_data.max_version),
                    raw_level.last_opened_version,
                )
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

    def test_reload_metadata(self) -> None:
        for src_path in BedrockLevels:
            with self.subTest(src_path=src_path), WorldTemp(src_path) as world_data:
                raw_level_1 = BedrockRawLevel.load(world_data.path)
                raw_level_2 = BedrockRawLevel.load(world_data.path)
                raw_level_1.open()
                try:
                    raw_level_1.level_name = "HelloWorld"
                    self.assertEqual("HelloWorld", raw_level_1.level_name)
                    self.assertEqual(
                        world_data.level_data.level_name, raw_level_2.level_name
                    )
                    raw_level_2.reload_metadata()
                    self.assertEqual("HelloWorld", raw_level_2.level_name)
                finally:
                    raw_level_1.close()

    def test_close_open(self) -> None:
        for src_path in BedrockLevels:
            with self.subTest(src_path=src_path), WorldTemp(src_path) as world_data:
                raw_level = BedrockRawLevel.load(world_data.path)

                opened_count = 0
                closed_count = 0
                reloaded_count = 0

                def on_open() -> None:
                    nonlocal opened_count
                    opened_count += 1

                def on_close() -> None:
                    nonlocal closed_count
                    closed_count += 1

                def on_reload() -> None:
                    nonlocal reloaded_count
                    reloaded_count += 1

                opened_token = raw_level.opened.connect(on_open)
                closed_token = raw_level.closed.connect(on_close)
                reloaded_token = raw_level.reloaded.connect(on_reload)

                self.assertFalse(raw_level.is_open())

                raw_level.open()
                self.assertTrue(raw_level.is_open())
                self.assertEqual(1, opened_count)
                self.assertEqual(0, closed_count)
                self.assertEqual(0, reloaded_count)

                opened_count = 0
                raw_level.reload()
                self.assertTrue(raw_level.is_open())
                self.assertEqual(0, opened_count)
                self.assertEqual(0, closed_count)
                self.assertEqual(1, reloaded_count)

                with self.assertRaises(RuntimeError):
                    raw_level.reload_metadata()

                reloaded_count = 0
                raw_level.close()
                self.assertFalse(raw_level.is_open())
                self.assertEqual(0, opened_count)
                self.assertEqual(1, closed_count)
                self.assertEqual(0, reloaded_count)

    def test_level_dat(self) -> None:
        for src_path in BedrockLevels:
            with self.subTest(src_path=src_path), WorldTemp(src_path) as world_data:
                raw_level = BedrockRawLevel.load(world_data.path)
                raw_level.open()
                try:
                    level_dat = raw_level.level_dat
                    self.assertIsInstance(level_dat, BedrockLevelDat)
                    level_dat.named_tag.compound["HelloWorld"] = StringTag("HelloWorld")
                    raw_level.level_dat = level_dat
                finally:
                    raw_level.close()
                del raw_level

                raw_level_2 = BedrockRawLevel.load(world_data.path)
                self.assertEqual(
                    StringTag("HelloWorld"),
                    raw_level_2.level_dat.named_tag.compound["HelloWorld"],
                )
