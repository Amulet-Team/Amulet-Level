#include <pybind11/pybind11.h>

#include <amulet/pybind11_extensions/py_module.hpp>

namespace py = pybind11;

void init_bedrock_level_dat(py::module);

py::module init_bedrock(py::module m_parent)
{
    auto m = Amulet::pybind11_extensions::def_subpackage(m_parent, "bedrock");

    init_bedrock_level_dat(m);

    return m;
}
