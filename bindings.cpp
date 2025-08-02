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

    py::class_<nir_shader> nir_shader_class(m, "nir_shader");

    m.def("nir_shader_create", &nir_shader_create,
        "Creates a new NIR shader.",
        py::arg("mem_ctx"), py::arg("stage"), py::arg("options"), py::arg("info"),
        py::return_value_policy::reference);

    py::class_<nir_function> nir_function_class(m, "nir_function");

    m.def("nir_function_create", &nir_function_create,
          py::arg("shader"), py::arg("name"),
          py::return_value_policy::reference);

    py::class_<nir_function_impl> nir_function_impl_class(m, "nir_function_impl");

    m.def("nir_function_impl_create", &nir_function_impl_create,
          py::arg("function"),
          py::return_value_policy::reference);

    py::class_<nir_builder>(m, "nir_builder")
        .def_property_readonly("shader", [](nir_builder *builder) {
            return builder->shader;
        },
        py::return_value_policy::reference);

    m.def("nir_builder_create", &nir_builder_create,
        py::arg("impl"),
        py::return_value_policy::reference);

    m.def("nir_builder_init_simple_shader", &nir_builder_init_simple_shader_wrapper,
        py::arg("stage"),
        py::arg("options"),
        py::arg("name"),
        py::return_value_policy::reference);

    m.def("nir_shader_get_function_for_name", &nir_shader_get_function_for_name,
        py::arg("shader"),
        py::arg("name"),
        py::return_value_policy::reference);

    m.def("nir_validate_shader", &nir_validate_shader,
        py::arg("shader"),
        py::arg("when"));

    py::class_<glsl_struct_field>(m, "glsl_struct_field")
        .def_readonly("name", &glsl_struct_field::name)
        .def_readonly("type", &glsl_struct_field::type, py::return_value_policy::reference_internal)
        .def_readonly("offset", &glsl_struct_field::offset);

    py::class_<glsl_type>(m, "glsl_type")
        .def_property_readonly("name",
            [](const glsl_type &self) {
                return glsl_get_type_name(&self);
            })
        .def_property_readonly("array",
            [](const glsl_type &self) {
                return self.fields.array;
            },
            py::return_value_policy::reference_internal)
        .def_property_readonly("structure",
            [](const glsl_type &self) {
                return self.fields.structure;
            },
            py::return_value_policy::reference_internal)
        .def("is_array", &glsl_type_is_array)
        .def("is_struct", &glsl_type_is_struct);

    m.def("glsl_int_type", &glsl_int_type,
        py::return_value_policy::reference);
    m.def("glsl_float_type", &glsl_float_type,
        py::return_value_policy::reference);

    m.def("glsl_array_type", &glsl_array_type,
        py::arg("element"),
        py::arg("array_size"),
        py::arg("explicit_stride"),
        py::return_value_policy::reference);

    m.def("glsl_type_singleton_init_or_ref", &glsl_type_singleton_init_or_ref);

    m.def("nir_variable_create", &nir_variable_create,
        py::arg("shader"),
        py::arg("mode"),
        py::arg("type"),
        py::arg("name"),
        py::return_value_policy::reference);

    py::enum_<nir_variable_mode>(m, "nir_variable_mode")
        .value("nir_var_mem_ssbo", nir_var_mem_ssbo)
        .export_values();

    py::class_<nir_variable::nir_variable_data>(m, "nir_variable_data")
        .def_readwrite("binding", &nir_variable::nir_variable_data::binding)
        .def_property("explicit_binding",
            [](const nir_variable::nir_variable_data& self) { return self.explicit_binding; },
            [](nir_variable::nir_variable_data& self, bool value) { self.explicit_binding = value; });

    py::class_<nir_variable, std::unique_ptr<nir_variable, py::nodelete>>(m, "nir_variable")
        .def_readwrite("data", &nir_variable::data)
        .def_readwrite("type", &nir_variable::type);

    py::enum_<nir_alu_type>(m, "nir_alu_type")
        .value("nir_type_int32", nir_type_int32)
        .value("nir_type_float32", nir_type_float32)
        .export_values();

    py::class_<nir_def> nir_def_class(m, "nir_def");

    m.def("nir_imm_int", &nir_imm_int,
        py::arg("builder"),
        py::arg("x"),
        py::return_value_policy::reference);

    m.def("nir_imm_float", &nir_imm_float,
        py::arg("builder"),
        py::arg("x"),
        py::return_value_policy::reference);

    m.def("glsl_get_explicit_size", &glsl_get_explicit_size,
        py::arg("type"),
        py::arg("align_to_stride"));
}
