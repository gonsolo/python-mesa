#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "compiler/nir/nir.h"

namespace py = pybind11;

PYBIND11_MODULE(mesanir, m) {
    m.doc() = "Python bindings for Mesa's NIR compiler.";

    m.def("nir_shader_create", &nir_shader_create,
        "Creates a new NIR shader.",
        py::arg("mem_ctx"), py::arg("stage"), py::arg("options"), py::arg("si"));
}
