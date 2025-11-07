#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

#include "level_dat.hpp"

namespace py = pybind11;

void init_bedrock_level_dat(py::module m_parent)
{
    auto m = m_parent.def_submodule("level_dat");

    std::string module_name = m.attr("__name__").cast<std::string>();

    py::classh<Amulet::BedrockLevelDat>
        BedrockLevelDat(m, "BedrockLevelDat");

    BedrockLevelDat.def(
        py::init<std::uint32_t, const Amulet::NBT::NamedTag&>(),
        py::arg("version"),
        py::arg("named_tag"));

    BedrockLevelDat.def_static(
        "from_binary",
        [](py::bytes buffer) {
            return Amulet::BedrockLevelDat::from_binary(buffer.cast<std::string>());
        },
        py::arg("buffer"));

    BedrockLevelDat.def_static(
        "from_file",
        &Amulet::BedrockLevelDat::from_file,
        py::arg("path"));

    BedrockLevelDat.def(
        "to_binary",
        [](const Amulet::BedrockLevelDat& self) {
            return py::bytes(self.to_binary());
        });

    BedrockLevelDat.def(
        "save_to",
        &Amulet::BedrockLevelDat::save_to,
        py::arg("path"));

    BedrockLevelDat.def_readwrite(
        "version",
        &Amulet::BedrockLevelDat::version);

    BedrockLevelDat.def_readwrite(
        "named_tag",
        &Amulet::BedrockLevelDat::named_tag);

    BedrockLevelDat.def(
        "__repr__",
        [module_name](const Amulet::BedrockLevelDat& self) {
            return module_name + ".BedrockLevelDat("
                + std::to_string(self.version) + ", "
                + py::repr(py::cast(self.named_tag, py::return_value_policy::reference)).cast<std::string>() + ")";
        });

    BedrockLevelDat.def(
        "__copy__",
        [](const Amulet::BedrockLevelDat& self) {
            return self;
        });

    BedrockLevelDat.def(
        "__deepcopy__",
        [](const Amulet::BedrockLevelDat& self, py::dict) {
            return self.deep_copy();
        },
        py::arg("memo"));
}
