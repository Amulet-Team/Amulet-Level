#include <pybind11/pybind11.h>

namespace py = pybind11;

py::module init_abc(py::module);
py::module init_loader(py::module);
py::module init_java(py::module);
py::module init_bedrock(py::module);

void init_amulet_level(py::module m)
{
    auto abc = init_abc(m);
    m.attr("Level") = abc.attr("Level");

    auto loader = init_loader(m);
    m.attr("get_level") = loader.attr("get_level");
    m.attr("NoValidLevelLoader") = loader.attr("NoValidLevelLoader");

    // Submodules
    init_java(m);
    init_bedrock(m);
}
