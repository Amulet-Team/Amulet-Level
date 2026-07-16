from unittest import TestCase
from tempfile import TemporaryDirectory
import os

from PIL import Image

from amulet.nbt import NamedTag, StringTag

from amulet.core.version import VersionNumber
from amulet.utils.lock import OrderedLock
from amulet.utils.event import Event
from amulet.level.abc import IdRegistry
from amulet.level.java import JavaRawLevel, JavaCreateArgsV1, JavaRawDimension

from amulet.minecraft_worlds import WorldTemp, JavaLevels, JavaLevelData


class JavaRawLevelTestCase(TestCase):
    def test_load(self) -> None:
        for src_path in JavaLevels:
            with self.subTest(src_path=src_path), WorldTemp(src_path) as world_data:
                raw_level = JavaRawLevel.load(world_data.path)
                self.assertIsInstance(raw_level, JavaRawLevel)
                self.assertFalse(raw_level.is_open())

    def test_create(self) -> None:
        with TemporaryDirectory() as temp_dir:
            raw_level = JavaRawLevel.create(
                JavaCreateArgsV1(
                    False,
                    os.path.join(temp_dir, "amulet_level"),
                    VersionNumber(1631),
                    "AmuletLevel",
                )
            )
            self.assertIsInstance(raw_level, JavaRawLevel)
            self.assertTrue(os.path.isdir(os.path.join(temp_dir, "amulet_level")))
            self.assertEqual("AmuletLevel", raw_level.level_name)

    def test_metadata(self) -> None:
        for src_path in JavaLevels:
            with self.subTest(src_path=src_path), WorldTemp(src_path) as world_data:
                level_data = world_data.level_data
                assert isinstance(level_data, JavaLevelData)

                raw_level = JavaRawLevel.load(world_data.path)
                self.assertIsInstance(raw_level, JavaRawLevel)
                self.assertIsInstance(raw_level.lock, OrderedLock)
                self.assertTrue(raw_level.is_supported())
                self.assertEqual(
                    level_data.modified_time, raw_level.modified_time.timestamp()
                )
                self.assertEqual("java", raw_level.platform)
                self.assertEqual(
                    VersionNumber(*level_data.max_version), raw_level.data_version
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
        for src_path in JavaLevels:
            with self.subTest(src_path=src_path), WorldTemp(src_path) as world_data:
                raw_level_1 = JavaRawLevel.load(world_data.path)
                raw_level_2 = JavaRawLevel.load(world_data.path)
                raw_level_1.open()
                try:
                    level_names = []

                    def on_level_name_changed():
                        level_names.append(raw_level_1.level_name)

                    level_name_change_token = raw_level_1.level_name_changed.connect(
                        on_level_name_changed
                    )
                    raw_level_1.level_name = "HelloWorld"
                    self.assertEqual("HelloWorld", raw_level_1.level_name)
                    self.assertEqual(
                        world_data.level_data.level_name, raw_level_2.level_name
                    )
                    raw_level_2.reload_metadata()
                    self.assertEqual("HelloWorld", raw_level_2.level_name)
                    self.assertEqual(["HelloWorld"], level_names)
                finally:
                    raw_level_1.close()

    def test_open_close(self) -> None:
        for src_path in JavaLevels:
            with self.subTest(src_path=src_path), WorldTemp(src_path) as world_data:
                raw_level = JavaRawLevel.load(world_data.path)

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
        for src_path in JavaLevels:
            with self.subTest(src_path=src_path), WorldTemp(src_path) as world_data:
                raw_level = JavaRawLevel.load(world_data.path)
                raw_level.open()
                try:
                    level_dat = raw_level.level_dat
                    self.assertIsInstance(level_dat, NamedTag)
                    level_dat.compound["HelloWorld"] = StringTag("HelloWorld")
                    raw_level.level_dat = level_dat
                finally:
                    raw_level.close()
                del raw_level

                raw_level_2 = JavaRawLevel.load(world_data.path)
                self.assertEqual(
                    StringTag("HelloWorld"),
                    raw_level_2.level_dat.compound["HelloWorld"],
                )

    def test_dimensions(self) -> None:
        for src_path in JavaLevels:
            with self.subTest(src_path=src_path), WorldTemp(src_path) as world_data:
                raw_level = JavaRawLevel.load(world_data.path)
                raw_level.open()
                try:
                    dimension_ids = raw_level.dimension_ids
                    self.assertIsInstance(dimension_ids, list)
                    self.assertEqual(
                        set(world_data.level_data.dim_height),
                        set(dimension_ids),
                    )
                    for dimension_id in dimension_ids:
                        dimension = raw_level.get_dimension(dimension_id)
                        self.assertIsInstance(dimension, JavaRawDimension)
                finally:
                    raw_level.close()

    def test_compact(self) -> None:
        for src_path in JavaLevels:
            with self.subTest(src_path=src_path), WorldTemp(src_path) as world_data:

                def get_region_size() -> int:
                    region_path = os.path.join(world_data.path, "region")
                    if not os.path.isdir(region_path):
                        return 0
                    return sum(
                        entry.stat().st_size
                        for entry in os.scandir(region_path)
                        if entry.is_file()
                    )

                start_size = get_region_size()
                raw_level = JavaRawLevel.load(world_data.path)
                with self.assertRaises(RuntimeError):
                    raw_level.compact()
                raw_level.open()
                try:
                    raw_level.compact()
                finally:
                    raw_level.close()
                end_size = get_region_size()
                self.assertLessEqual(end_size, start_size)

    def test_id_override(self) -> None:
        for src_path in JavaLevels:
            with self.subTest(src_path=src_path), WorldTemp(src_path) as world_data:
                raw_level = JavaRawLevel.load(world_data.path)
                with self.assertRaises(RuntimeError):
                    _ = raw_level.block_id_override
                with self.assertRaises(RuntimeError):
                    _ = raw_level.biome_id_override

                raw_level.open()
                try:
                    self.assertIsInstance(raw_level.block_id_override, IdRegistry)
                    self.assertIsInstance(raw_level.biome_id_override, IdRegistry)
                finally:
                    raw_level.close()
