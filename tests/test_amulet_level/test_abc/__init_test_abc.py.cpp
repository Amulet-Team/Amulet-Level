#include <pybind11/pybind11.h>

#include <amulet/pybind11_extensions/py_module.hpp>

namespace py = pybind11;

void init_test_history(py::module m_parent);

void init_test_abc(py::module m_parent){
    auto m = Amulet::pybind11_extensions::def_subpackage(m_parent, "test_abc");
    init_test_history(m);
}
