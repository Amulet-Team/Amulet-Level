import unittest
import copy

from amulet.nbt import NamedTag, CompoundTag, ByteTag

from amulet.level.bedrock import BedrockLevelDat


class BedrockLevelDatTestCase(unittest.TestCase):
    def test_default_constructor(self) -> None:
        dat = BedrockLevelDat()
        self.assertIsInstance(dat, BedrockLevelDat)
        self.assertIsInstance(dat.version, int)
        self.assertEqual(0, dat.version)
        self.assertIsInstance(dat.named_tag, NamedTag)
        self.assertIsInstance(dat.named_tag.tag, CompoundTag)
        self.assertFalse(dat.named_tag.compound)
        self.assertIs(dat.named_tag, dat.named_tag)

    def test_constructor(self) -> None:
        named_tag = NamedTag()
        dat = BedrockLevelDat(10, named_tag)
        self.assertIsInstance(dat, BedrockLevelDat)
        self.assertIsInstance(dat.version, int)
        self.assertEqual(10, dat.version)
        self.assertIsInstance(dat.named_tag, NamedTag)
        self.assertIs(named_tag, dat.named_tag)

    def test_edit(self) -> None:
        named_tag_1 = NamedTag()
        named_tag_2 = NamedTag()
        dat = BedrockLevelDat(10, named_tag_1)
        self.assertEqual(10, dat.version)
        self.assertIs(named_tag_1, dat.named_tag)
        dat.version = 9
        dat.named_tag = named_tag_2
        self.assertEqual(9, dat.version)
        self.assertIs(named_tag_2, dat.named_tag)

    def test_serialisation(self) -> None:
        dat = BedrockLevelDat(10, NamedTag(ByteTag(5), "test"))
        binary = dat.to_binary()
        self.assertEqual(b"\n\x00\x00\x00\x08\x00\x00\x00\x01\x04\x00test\x05", binary)
        dat2 = BedrockLevelDat.from_binary(binary)
        self.assertEqual(10, dat2.version)
        self.assertEqual(NamedTag(ByteTag(5), "test"), dat2.named_tag)

    def test_repr(self) -> None:
        dat = BedrockLevelDat(10, NamedTag(ByteTag(5), "test"))
        self.assertEqual("BedrockLevelDat(10, NamedTag(ByteTag(5), 'test'))", repr(dat))

    def test_copy(self) -> None:
        named_tag = NamedTag()
        dat = BedrockLevelDat(10, named_tag)
        dat2 = copy.copy(dat)
        self.assertEqual(10, dat.version)
        self.assertEqual(10, dat2.version)
        self.assertIs(named_tag, dat.named_tag)
        self.assertIs(named_tag, dat2.named_tag)

        named_tag2 = NamedTag()
        dat2.version = 9
        dat2.named_tag = named_tag2
        self.assertEqual(10, dat.version)
        self.assertEqual(9, dat2.version)
        self.assertIs(named_tag, dat.named_tag)
        self.assertIs(named_tag2, dat2.named_tag)

    def test_deepcopy(self) -> None:
        named_tag = NamedTag()
        dat = BedrockLevelDat(10, named_tag)
        dat2 = copy.deepcopy(dat)
        self.assertEqual(10, dat.version)
        self.assertEqual(10, dat2.version)
        self.assertIs(named_tag, dat.named_tag)
        self.assertIsNot(named_tag, dat2.named_tag)

        dat2.version = 9
        self.assertEqual(10, dat.version)
        self.assertEqual(9, dat2.version)
