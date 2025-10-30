#include <pybind11/pybind11.h>

#include <amulet/pybind11_extensions/py_module.hpp>

namespace py = pybind11;

py::module init_bedrock(py::module m_parent)
{
    auto m = Amulet::pybind11_extensions::def_subpackage(m_parent, "bedrock");

    return m;
}
