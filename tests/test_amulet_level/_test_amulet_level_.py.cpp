#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_test_abc(py::module m_parent);

void init_test_amulet_level(py::module m){
    init_test_abc(m);
}
