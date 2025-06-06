#include <pybind11/pybind11.h>

#include <amulet/pybind11_extensions/compatibility.hpp>
#include <amulet/pybind11_extensions/py_module.hpp>

namespace py = pybind11;
namespace pyext = Amulet::pybind11_extensions;

py::module init_level_loader(py::module);
py::module init_level_abc(py::module);
py::module init_java(py::module);

void init_module(py::module m)
{
    pyext::init_compiler_config(m);
    pyext::check_compatibility(py::module::import("amulet.game"), m);

    init_level_abc(m);
    m.attr("Level") = py::module::import("amulet.level.abc").attr("Level");

    init_level_loader(m);
    m.attr("get_level") = py::module::import("amulet.level.loader").attr("get_level");
    m.attr("NoValidLevelLoader") = py::module::import("amulet.level.loader").attr("NoValidLevelLoader");

    // from .temporary_level import TemporaryLevel

    // Submodules
    auto java_module = init_java(m);
    // m.attr("JavaLevel") = java_module.attr("JavaLevel");

    // m.attr("BedrockLevel") = py::module::import("amulet.level.bedrock").attr("BedrockLevel");
}

PYBIND11_MODULE(_amulet_level, m)
{
    py::options options;
    options.disable_function_signatures();
    m.def("init", &init_module, py::doc("init(arg0: types.ModuleType) -> None"));
    options.enable_function_signatures();
}
