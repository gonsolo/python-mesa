#include <iostream>

#include <pybind11/pybind11.h>
#include <pybind11/native_enum.h>
#include <pybind11/stl.h>
#include "pybind11/detail/common.h"

#include "compiler/nir/nir.h"
#include "compiler/nir/nir_builder.h"
#include "compiler/shader_enums.h"

namespace py = pybind11;

//static void test_gonsolo() {
//  glsl_type_singleton_init_or_ref();
//  gl_shader_stage stage = MESA_SHADER_COMPUTE;
//  nir_shader_compiler_options options = {};
//  nir_builder builder = nir_builder_init_simple_shader(stage, &options, "simple");
//  nir_def* const_index = nir_imm_int(&builder, 3);
//  printf("test_gonsolo: bit_size: %u\n", const_index->bit_size);
//  ralloc_free(builder.shader);
//  glsl_type_singleton_decref();
//}

#include <stdio.h>
#include "compiler/nir/nir_builder.h"
#include "compiler/nir/nir.h"
#include "compiler/nir/nir_builder_opcodes.h"

#include <stdio.h>
#include "compiler/nir/nir.h"
#include "compiler/nir/nir_builder.h"

void test_gonsolo() {
    glsl_type_singleton_init_or_ref();
    gl_shader_stage stage = MESA_SHADER_COMPUTE;
    nir_shader_compiler_options options = {};
    nir_builder builder = nir_builder_init_simple_shader(stage, &options, "simple");

    nir_def* val1 = nir_imm_int(&builder, 3);
    assert(val1->num_components == 1);
    assert(val1->bit_size == 32);

    nir_def* val2 = nir_imm_int(&builder, 5);
    nir_def* sum = nir_iadd(&builder, val1, val2);

    // Um die Warnung "Variable wird nicht verwendet" zu beheben
    (void)sum;

    printf("--- Original Shader ---\n");
    nir_print_shader(builder.shader, stdout);
    printf("\n\n");

    // Korrekter Aufruf: builder.impl statt builder.shader
    nir_metadata_require(builder.impl, nir_metadata_block_index | nir_metadata_dominance);

    // Rufen Sie jeden Pass einzeln auf
    nir_opt_algebraic(builder.shader);
    nir_opt_constant_folding(builder.shader);
    nir_opt_dce(builder.shader);

    printf("--- Optimized Shader ---\n");
    nir_print_shader(builder.shader, stdout);

    ralloc_free(builder.shader);
}

static nir_builder
nir_builder_init_simple_shader_wrapper(gl_shader_stage stage,
                                      const nir_shader_compiler_options *options,
                                      const char *name)
{
    return nir_builder_init_simple_shader(stage, options, name);
}

py::object wrap_nir_load_ssbo(py::object builder, int num_components, int bit_size, py::object src0, py::object src1) {
    nir_builder* b = py::cast<nir_builder*>(builder);
    nir_def* s0 = py::cast<nir_def*>(src0);
    nir_def* s1 = py::cast<nir_def*>(src1);

    return py::cast(nir_load_ssbo(b, num_components, bit_size, s0, s1));
}

nir_def *
wrap_nir_imm_int(nir_builder *build, int x)
{
  assert(build);
  nir_def *result = nir_imm_int(build, x);
  assert(result);
  if (result == nullptr) {
    std::cout << "result is nullptr" << std::endl;
  } else {
    std::cout << "result bit size: " << result->bit_size << std::endl;
  }
  return result;
}


// This wrapper function takes a nir_def* and returns its bit_size
unsigned int get_nir_def_bit_size(nir_def* def) {
    if (def == nullptr) {
        return 0;
    }
    return def->bit_size;
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

    py::native_enum<gl_shader_stage>(m, "gl_shader_stage", "enum.IntEnum")
        .value("COMPUTE", MESA_SHADER_COMPUTE)
        .export_values()
        .finalize();

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

    py::class_<nir_function>(m, "nir_function")
        .def_readonly("impl", &nir_function::impl);

    m.def("nir_function_create", &nir_function_create,
          py::arg("shader"), py::arg("name"),
          py::return_value_policy::reference);

    py::class_<nir_function_impl>(m, "nir_function_impl")
        .def_readonly("body", &nir_function_impl::body);

    m.def("nir_function_impl_create", &nir_function_impl_create,
          py::arg("function"),
          py::return_value_policy::reference);

    py::class_<nir_builder>(m, "nir_builder")
        .def_readwrite("cursor", &nir_builder::cursor)
        .def_readwrite("impl", &nir_builder::impl)
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
        py::arg("name"));

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

    py::class_<nir_def> (m, "nir_def")
        .def_readonly("bit_size", &nir_def::bit_size);

    m.def("nir_imm_int", &wrap_nir_imm_int,
        "Wrapper for nir_imm_int",
        py::arg("builder"),
        py::arg("x"),
        py::return_value_policy::reference,
        py::keep_alive<1, 0>());

    m.def("nir_imm_intN_t", &nir_imm_intN_t,
        py::arg("builder"),
        py::arg("x"),
        py::arg("bit_size"),
        py::return_value_policy::reference);

    m.def("nir_imm_float", &nir_imm_float,
        py::arg("builder"),
        py::arg("x"),
        py::return_value_policy::reference);

    m.def("glsl_get_explicit_size", &glsl_get_explicit_size,
        py::arg("type"),
        py::arg("align_to_stride"));

    m.def("nir_imul_imm", &nir_imul_imm,
        py::arg("builder"),
        py::arg("x"),
        py::arg("y"),
        py::return_value_policy::reference);

    m.def("glsl_get_vector_elements", &glsl_get_vector_elements,
        py::arg("type"));

    m.def("glsl_get_bit_size", &glsl_get_bit_size,
        py::arg("type"));

     m.def("nir_load_ssbo", &wrap_nir_load_ssbo, "A C++ wrapper for the nir_load_ssbo macro.",
         py::arg("builder"),
         py::arg("num_components"),
         py::arg("bit_size"),
         py::arg("src0"),
         py::arg("src1"));

    m.def("nir_load_var", &nir_load_var,
        py::arg("builder"),
        py::arg("var"),
        py::return_value_policy::reference);

    m.def("nir_start_block", &nir_start_block,
        py::arg("impl"),
        py::return_value_policy::reference);

    m.def("nir_after_block", &nir_after_block,
        py::arg("block"));

    py::class_<exec_list> exec_list_class(m, "exec_list");

    py::class_<nir_block> nir_block_class(m, "nir_block");

    py::class_<nir_cursor> nir_cursor_class(m, "nir_cursor");

    m.def("nir_build_deref_var", &nir_build_deref_var,
        py::arg("builder"),
        py::arg("var"),
        py::return_value_policy::reference);

    py::class_<nir_deref_instr> nir_deref_instr_class(m, "nir_deref_instr");

    m.def("nir_build_deref_array", &nir_build_deref_array,
        py::arg("builder"),
        py::arg("parent"),
        py::arg("index"),
        py::return_value_policy::reference);

    m.def("nir_const_value_for_raw_uint", &nir_const_value_for_raw_uint,
        py::arg("x"),
        py::arg("bit_size"));

    m.def("get_nir_def_bit_size", &get_nir_def_bit_size,
      py::arg("def"),
      "Returns the bit_size of a nir_def object.");

    m.def("test_gonsolo", &test_gonsolo);
}
