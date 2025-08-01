#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "pybind11/detail/common.h"

#include "compiler/nir/nir.h"
#include "compiler/nir/nir_builder.h"
#include "compiler/shader_enums.h"

namespace py = pybind11;

static nir_builder
nir_builder_init_simple_shader_wrapper(gl_shader_stage stage,
                                      const nir_shader_compiler_options *options,
                                      const char *name)
{
    return nir_builder_init_simple_shader(stage, options, name);
}

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

    py::class_<shader_info>(m, "shader_info")
        .def(py::init<>())
        .def_readwrite("name", &shader_info::name)
        .def_property("stage",
            [](const shader_info &s) { return s.stage; },
            [](shader_info &s, gl_shader_stage val) { s.stage = val; },
            "The shader stage.")
        ;

    py::class_<nir_shader>(m, "nir_shader");

    m.def("nir_shader_create", &nir_shader_create,
        "Creates a new NIR shader.",
        py::arg("mem_ctx"), py::arg("stage"), py::arg("options"), py::arg("info"),
        py::return_value_policy::reference);

    py::class_<nir_function>(m, "nir_function");

    m.def("nir_function_create", &nir_function_create,
          py::arg("shader"), py::arg("name"),
          py::return_value_policy::reference);

    py::class_<nir_function_impl>(m, "nir_function_impl");

    m.def("nir_function_impl_create", &nir_function_impl_create,
          py::arg("function"),
          py::return_value_policy::reference);

    py::class_<nir_builder>(m, "nir_builder");

    m.def("nir_builder_create", &nir_builder_create,
        py::arg("impl"),
        py::return_value_policy::reference);

    m.def("nir_builder_init_simple_shader", &nir_builder_init_simple_shader_wrapper,
        py::arg("stage"),
        py::arg("options"),
        py::arg("name"),
        py::return_value_policy::reference);
}
