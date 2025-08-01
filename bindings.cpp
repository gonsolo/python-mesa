#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "compiler/nir/nir.h"
#include "compiler/shader_enums.h"

namespace py = pybind11;

PYBIND11_MODULE(mesa3d, m) {
    m.doc() = "Python bindings for Mesa's NIR compiler.";

    m.def("ralloc_context", [](void* ctx_pointer) {
        void* new_ctx = ralloc_context(ctx_pointer);
        return py::capsule(new_ctx, "ralloc_context");
    }, "Creates a new ralloc memory context, optionally as a child of an existing one.",
         py::arg("ctx"));

    m.def("ralloc_free", [](py::capsule ctx_capsule) {
        if (ctx_capsule) {
            void* ctx_pointer = ctx_capsule.get_pointer();
            ralloc_free(ctx_pointer);
        }
    }, "Frees a ralloc memory context.", py::arg("ctx"));

    py::enum_<gl_shader_stage>(m, "gl_shader_stage")
        .value("COMPUTE", MESA_SHADER_COMPUTE)
        .export_values();

    py::class_<nir_shader_compiler_options>(m, "nir_shader_compiler_options")
        .def(py::init<>())
        .def_readwrite("lower_flrp32", &nir_shader_compiler_options::lower_flrp32,
                       "Lower 32-bit floating-point linear interpolate.")
        .def_readwrite("lower_flrp64", &nir_shader_compiler_options::lower_flrp64,
                       "Lower 64-bit floating-point linear interpolate.")
        .def_readwrite("lower_fpow", &nir_shader_compiler_options::lower_fpow,
                       "Lower float power to something more common.")
        .def_readwrite("lower_scmp", &nir_shader_compiler_options::lower_scmp,
                       "Lower 'fge' and 'flt' to 'fsl' and 'fsub'.")
        ;

    m.def("nir_shader_create", &nir_shader_create,
        "Creates a new NIR shader.",
        py::arg("mem_ctx"), py::arg("stage"), py::arg("options"), py::arg("si"));
}
