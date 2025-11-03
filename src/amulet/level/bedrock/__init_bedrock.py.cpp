#include <pybind11/pybind11.h>

#include <amulet/pybind11_extensions/py_module.hpp>

namespace py = pybind11;

void init_bedrock_level_dat(py::module);
py::module init_bedrock_raw_dimension(py::module);
py::module init_bedrock_raw_level(py::module);

py::module init_bedrock(py::module m_parent)
{
    auto m = Amulet::pybind11_extensions::def_subpackage(m_parent, "bedrock");

    init_bedrock_level_dat(m);

    auto raw_dimension = init_bedrock_raw_dimension(m);
    m.attr("BedrockRawDimension") = raw_dimension.attr("BedrockRawDimension");
    
    auto raw_level = init_bedrock_raw_level(m);
    m.attr("BedrockRawLevel") = raw_level.attr("BedrockRawLevel");

    return m;
}
